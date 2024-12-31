#include "mqtt.h"
#include "wifi_client.h"

constexpr std::string_view kSsid = CONFIG_ESP_WIFI_SSID;          ///< SSID. It is defined from sdkconfig.
constexpr std::string_view kPassword = CONFIG_ESP_WIFI_PASSWORD;  ///< Password. It is defined from sdkconfig.

constexpr std::string_view kMqttBrokerDomain = CONFIG_BROKER_DOMAIN;  ///< MQTT broker domain. It is defined from sdkconfig.
constexpr int_fast32_t kMqttBrokerPort = CONFIG_BROKER_PORT;          ///< MQTT broker port. It is defined from sdkconfig.

wifi_client::WifiClient wifi;
mqtt_client::Mqtt mqtt;

void setup() {
  wifi.ConnectWifi(kSsid, kPassword);
  while (!wifi.Connected()) {
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
  mqtt.Connect(kMqttBrokerDomain, kMqttBrokerPort);
  while (!mqtt.Connected()) {
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }

  mqtt.Subscribe("test");
}

void loop() {
  mqtt.Publish("test", "Hello, world!");
}

extern "C" {
void app_main(void) {
  setup();
  while (1) {
    loop();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
}
