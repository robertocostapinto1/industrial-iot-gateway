#include "wifi_manager.hpp"
#include "esp_log.h"
#include "nvs_flash.h"
#include <cstring>
#include <algorithm>

static const char* TAG = "WIFI_MGR";

namespace industrial {

WifiManager::WifiManager(const std::string& ssid, const std::string& password)
    : ssid_(ssid), password_(password) {
    
    ESP_LOGI(TAG, "Initializing System Resources (NVS)...");
    
    // 1. Initialize NVS (Requirement for WiFi stack)
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated or had a version mismatch; erase and retry
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // 2. Initialize Network Interface
    ESP_ERROR_CHECK(esp_netif_init());
    
    esp_err_t err = esp_event_loop_create_default();
    if (err != ESP_OK && err != ESP_ERR_INVALID_STATE) {
        ESP_ERROR_CHECK(err);
    }
    
    esp_netif_create_default_wifi_sta();

    // 3. Initialize WiFi with default config
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    wifi_event_group_ = xEventGroupCreate();

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, 
                    &WifiManager::event_handler, nullptr, nullptr));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, 
                    &WifiManager::event_handler, nullptr, nullptr));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
}

WifiManager::~WifiManager() {
    ESP_LOGW(TAG, "RAII: Destroying WiFi Resource.");
    esp_wifi_stop();
    esp_wifi_deinit();
    vEventGroupDelete(wifi_event_group_);
}

bool WifiManager::connect() {
    wifi_config_t wifi_config = {};
    std::memset(&wifi_config, 0, sizeof(wifi_config_t));

    size_t ssid_len = std::min(ssid_.size(), sizeof(wifi_config.sta.ssid) - 1);
    std::copy_n(ssid_.begin(), ssid_len, wifi_config.sta.ssid);

    size_t pass_len = std::min(password_.size(), sizeof(wifi_config.sta.password) - 1);
    std::copy_n(password_.begin(), pass_len, wifi_config.sta.password);

    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "Waiting for AP connection...");
    EventBits_t bits = xEventGroupWaitBits(wifi_event_group_, CONNECTED_BIT | FAIL_BIT,
                                           pdFALSE, pdFALSE, portMAX_DELAY);
    
    return (bits & CONNECTED_BIT);
}

void WifiManager::event_handler(void* arg, esp_event_base_t event_base,
                               int32_t event_id, void* event_data) {
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        esp_wifi_connect();
        ESP_LOGW(TAG, "Retrying connection...");
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        xEventGroupSetBits(wifi_event_group_, CONNECTED_BIT);
    }
}

} // namespace industrial
