#include <memory>

#include "greeter.h"
#include "greeter_dummy.h"
#include "greeter_impl.h"
#include "message.h"
#include "mqtt.h"
#include "storage_controller.h"
#include "wifi_client.h"

constexpr std::string_view kSsid = CONFIG_ESP_WIFI_SSID;          ///< SSID. It is defined from sdkconfig.
constexpr std::string_view kPassword = CONFIG_ESP_WIFI_PASSWORD;  ///< Password. It is defined from sdkconfig.

constexpr std::string_view kMqttBrokerDomain = CONFIG_BROKER_DOMAIN;  ///< MQTT broker domain. It is defined from sdkconfig.
constexpr int_fast32_t kMqttBrokerPort = CONFIG_BROKER_PORT;          ///< MQTT broker port. It is defined from sdkconfig.

constexpr std::string_view kGreeterTopic = "greeter";
constexpr std::string_view kFeatureTopic = "feature";

constexpr std::string_view kStorageNamespace = "features";
constexpr std::string_view kStorageKeyGreeter = "greeter";

constexpr uint8_t kGreeterFlagMask = 0x01;

wifi_client::WifiClient wifi;
std::shared_ptr<mqtt_client::Mqtt> mqtt = std::make_shared<mqtt_client::Mqtt>(wifi_client::WifiClient::GetMac());
storage_controller::StorageController feature_storage(kStorageNamespace);

std::unique_ptr<greeter::Greeter> greeter_i = std::make_unique<greeter::GreeterDummy>();

void setup() {
  wifi.ConnectWifi(kSsid, kPassword);
  while (!wifi.Connected()) {
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
  mqtt->Connect(kMqttBrokerDomain, kMqttBrokerPort);
  while (!mqtt->Connected()) {
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }

  auto greeter_flag = feature_storage.ReadByte(kStorageKeyGreeter).value_or(0);
  bool greeter_enabled = greeter_flag & kGreeterFlagMask;

  mqtt->Subscribe(kFeatureTopic);
  // Notify the feature flag value from this client, since server has no persistent storage.
  mqtt->Publish(kFeatureTopic, message::FeatureMessage(wifi_client::WifiClient::GetMac(), greeter_enabled).ToJson());

  if (greeter_enabled) {
    greeter_i = std::make_unique<greeter::GreeterImpl>(mqtt, wifi_client::WifiClient::GetMac(), kGreeterTopic);
  }
}

void FeatureFlagCheck() {
  auto messages = mqtt->PopMessages(kFeatureTopic);
  if (messages.empty()) {
    return;
  }
  for (const auto& msg : messages) {
    auto m = message::MessageFrom(msg);
    if (m == nullptr) {
      continue;
    }
    if (typeid(*m) != typeid(message::FeatureMessage)) {
      continue;
    }
    std::unique_ptr<message::FeatureMessage> feature_message = std::make_unique<message::FeatureMessage>(dynamic_cast<message::FeatureMessage*>(m.get()));
    if (feature_message->client_id == wifi_client::WifiClient::GetMac()) {
      continue;
    }
    feature_storage.WriteByte(kStorageKeyGreeter, feature_message->greeter_is_enabled & kGreeterFlagMask);
    if (feature_message->greeter_is_enabled) {
      greeter_i = std::make_unique<greeter::GreeterImpl>(mqtt, wifi_client::WifiClient::GetMac(), kGreeterTopic);
    } else {
      greeter_i = std::make_unique<greeter::GreeterDummy>();
    }
  }
}

void loop() {
  FeatureFlagCheck();
  greeter_i->Greet();
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
