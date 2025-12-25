#pragma once

#include <string>
#include "esp_wifi.h"
#include "esp_event.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

namespace industrial {

class WifiManager {
public:
    // Delete copy constructor and assignment (Non-copyable hardware resource)
    WifiManager(const WifiManager&) = delete;
    WifiManager& operator=(const WifiManager&) = delete;

    explicit WifiManager(const std::string& ssid, const std::string& password);
    ~WifiManager(); // RAII Destructor: Ensures radio is powered down

    bool connect();

private:
    static void event_handler(void* arg, esp_event_base_t event_base,
                            int32_t event_id, void* event_data);

    std::string ssid_;
    std::string password_;
    static inline EventGroupHandle_t wifi_event_group_;
    static constexpr int CONNECTED_BIT = BIT0;
    static constexpr int FAIL_BIT      = BIT1;
};

} // namespace industrial
