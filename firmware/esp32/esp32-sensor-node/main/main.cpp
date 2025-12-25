#include <string>
#include "wifi_manager.hpp"
#include "protobuf_wrapper.hpp"
#include "mqtt_manager.hpp"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Access the embedded certificate (This MUST match the filename in CMake)
extern const uint8_t ca_cert_pem_start[] asm("_binary_ca_cert_pem_start");

extern "C" void app_main(void) {
    ESP_LOGI("MAIN", "Industrial Edge Node Booting (Secure Tier)...");

    // 1. Initialize WiFi (RAII)
    industrial::WifiManager wifi("Foco", "Foco2020-116363");
    
    // 2. MQTTS RAII (Pointing to Gateway - real IP handled in Phase 3)
    industrial::MqttManager mqtt("mqtts://192.168.1.100:8883", (const char*)ca_cert_pem_start);

    if (wifi.connect()) {
        mqtt.start();

        uint32_t counter = 0;
        while (true) {
            // 3. Serialize Data
            auto payload = industrial::ProtobufSerializer::serialize_sensor_data(
                "ESP32-NODE-01", 25.4f, 48.2f, counter++
            );

            // 4. Secure Publish
            if (mqtt.publish("industrial/sensors/data", payload)) {
                ESP_LOGI("MAIN", "Telemetry securely published! (%zu bytes)", payload.size());
            } else {
                ESP_LOGW("MAIN", "MQTTS Offline - Attempting Reconnect...");
            }

            vTaskDelay(pdMS_TO_TICKS(10000));
        }
    }
}
