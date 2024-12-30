#include "wifi_client.h"

void wifi_client::WifiClient::ConnectWifi(const std::string_view ssid, const std::string_view password) noexcept {
  ESP_ERROR_CHECK(nvs_flash_init());
  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());
  esp_netif_create_default_wifi_sta();
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));

  ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_client::WifiClient::OnEventWifi, nullptr));
  ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, ESP_EVENT_ANY_ID, &wifi_client::WifiClient::OnEventIp, nullptr));

  wifi_config_t wifi_config = {};
  std::strncpy(reinterpret_cast<char*>(wifi_config.sta.ssid), ssid.data(), password.size());
  std::strncpy(reinterpret_cast<char*>(wifi_config.sta.password), password.data(), password.size());
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  ESP_ERROR_CHECK(esp_wifi_set_config(static_cast<wifi_interface_t>(ESP_IF_WIFI_STA), &wifi_config));
  ESP_ERROR_CHECK(esp_wifi_start());
}

bool wifi_client::WifiClient::Connected() const noexcept {
  esp_netif_ip_info_t ip;
  esp_netif_t* esp_netif;
  esp_netif = esp_netif_next_unsafe(NULL);
  esp_netif_get_ip_info(esp_netif, &ip);
  return ip.ip.addr != 0;
}

void wifi_client::WifiClient::OnEventWifi(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
  if (event_base == WIFI_EVENT) {
    ESP_LOGI(kTag.data(), "WIFI_EVENT: %ld", event_id);
    switch (event_id) {
      case WIFI_EVENT_STA_START:
        esp_wifi_connect();
        break;
      default:
        break;
    }
  }
};

void wifi_client::WifiClient::OnEventIp(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
  if (event_base == IP_EVENT) {
    ESP_LOGI(kTag.data(), "IP_EVENT: %ld", event_id);
  }
};