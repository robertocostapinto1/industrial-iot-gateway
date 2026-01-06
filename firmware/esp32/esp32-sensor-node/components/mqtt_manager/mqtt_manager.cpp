#include "mqtt_manager.hpp"
#include "esp_log.h"

static const char* TAG = "MQTT_MGR";

namespace industrial {

MqttManager::MqttManager(const std::string& uri, const char* ca_cert) {
    esp_mqtt_client_config_t mqtt_cfg = {};
    mqtt_cfg.broker.address.uri = uri.c_str();

    // THE SECURITY CORE:
    // This tells the MQTTS client to use the CA certificate to verify the server.
    mqtt_cfg.broker.verification.certificate = ca_cert;

    mqtt_cfg.broker.verification.skip_cert_common_name_check = true;

    // Requirement for TLS: Ensure the client task has enough stack for the handshake
    mqtt_cfg.task.stack_size = 8192; 

    client_ = esp_mqtt_client_init(&mqtt_cfg);

    esp_mqtt_client_register_event(client_, (esp_mqtt_event_id_t)ESP_EVENT_ANY_ID, 
                                 MqttManager::mqtt_event_handler, this);
}

MqttManager::~MqttManager() {
    ESP_LOGW(TAG, "RAII: Stopping MQTTS Client");
    esp_mqtt_client_stop(client_);
    esp_mqtt_client_destroy(client_);
}

void MqttManager::start() {
    esp_mqtt_client_start(client_);
}

bool MqttManager::publish(const std::string& topic, const std::vector<uint8_t>& payload) {
    if (!is_connected_) return false;

    int msg_id = esp_mqtt_client_publish(client_, topic.c_str(), 
                                        reinterpret_cast<const char*>(payload.data()), 
                                        payload.size(), 1, 0);
    return msg_id != -1;
}

void MqttManager::mqtt_event_handler(void* handler_args, esp_event_base_t base, 
                                   int32_t event_id, void* event_data) {
    auto* self = static_cast<MqttManager*>(handler_args);
    auto* event = static_cast<esp_mqtt_event_handle_t>(event_data); (void)event;

    switch ((esp_mqtt_event_id_t)event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTTS Secure Handshake Successful");
            self->is_connected_ = true;
            if (self->on_connected_) self->on_connected_();
            break;
        case MQTT_EVENT_DISCONNECTED:
            self->is_connected_ = false;
            ESP_LOGW(TAG, "MQTTS Offline");
            break;
        case MQTT_EVENT_ERROR:
            // This is where you would see "Verification Failed" if certs didn't match
            ESP_LOGE(TAG, "MQTTS TLS/SSL Error");
            break;
        default:
            break;
    }
}

} // namespace industrial
