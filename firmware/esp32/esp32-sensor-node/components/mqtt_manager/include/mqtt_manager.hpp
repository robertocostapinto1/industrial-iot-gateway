#pragma once

#include <string>
#include <vector>
#include <functional>
#include "mqtt_client.h"

namespace industrial {

class MqttManager {
public:
    MqttManager(const MqttManager&) = delete;
    MqttManager& operator=(const MqttManager&) = delete;

    // Constructor requires the Broker URI and the CA Certificate
    explicit MqttManager(const std::string& uri, const char* ca_cert);
    ~MqttManager();

    void start();
    void stop();
    bool publish(const std::string& topic, const std::vector<uint8_t>& payload);

    // Modern C++ Callback to notify main logic of connection status
    void set_on_connected_callback(std::function<void()> cb) { on_connected_ = cb; }

private:
    static void mqtt_event_handler(void* handler_args, esp_event_base_t base, 
                                 int32_t event_id, void* event_data);

    esp_mqtt_client_handle_t client_{nullptr};
    std::function<void()> on_connected_{nullptr};
    bool is_connected_{false};
};

} // namespace industrial
