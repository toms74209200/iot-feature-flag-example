#include "wifi_client.h"

constexpr std::string_view kSsid = CONFIG_ESP_WIFI_SSID;          ///< SSID. It is defined from sdkconfig.
constexpr std::string_view kPassword = CONFIG_ESP_WIFI_PASSWORD;  ///< Password. It is defined from sdkconfig.

wifi_client::WifiClient wifi;

void setup() {
  wifi.ConnectWifi(kSsid, kPassword);
  while (!wifi.Connected()) {
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void loop() {}

extern "C" {
void app_main(void) {
  setup();
  while (1) {
    loop();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
}
