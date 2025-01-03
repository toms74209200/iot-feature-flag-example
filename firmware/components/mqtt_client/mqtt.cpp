#include "mqtt.h"

void mqtt_client::Mqtt::Connect(const std::string_view domain, const int_fast32_t port) noexcept {
  esp_mqtt_client_config_t mqtt_cfg = {};
  mqtt_cfg.broker.address.hostname = std::string{domain}.c_str();
  mqtt_cfg.broker.address.port = port;
  mqtt_cfg.broker.address.transport = MQTT_TRANSPORT_OVER_TCP;

  client_ = esp_mqtt_client_init(&mqtt_cfg);
  esp_mqtt_client_register_event(client_, static_cast<esp_mqtt_event_id_t>(ESP_EVENT_ANY_ID), OnEvent, this);
  esp_mqtt_client_start(client_);
}

void mqtt_client::Mqtt::Publish(const std::string_view topic, const std::string_view message) noexcept {
  if (!connected_) {
    return;
  }
  esp_mqtt_client_publish(client_, std::string{topic}.c_str(), std::string{message}.c_str(), 0, 0, 0);
}

void mqtt_client::Mqtt::Subscribe(const std::string_view topic) noexcept {
  if (!connected_) {
    return;
  }
  esp_mqtt_client_subscribe(client_, std::string{topic}.c_str(), 0);
}

void mqtt_client::Mqtt::Unsubscribe(const std::string_view topic) noexcept {
  esp_mqtt_client_unsubscribe(client_, std::string{topic}.c_str());
}

void mqtt_client::Mqtt::Disconnect() noexcept {
  esp_mqtt_client_stop(client_);
}

std::vector<std::string> mqtt_client::Mqtt::PopMessages(const std::string_view topic) noexcept {
  if (messages_.find(std::string{topic}) == messages_.end()) {
    return {};
  }
  auto messages = messages_[std::string{topic}];
  messages_.erase(std::string{topic});
  return {messages.begin(), messages.end()};
}

void mqtt_client::Mqtt::OnEvent(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) noexcept {
  ESP_LOGI(kTag.data(), "MQTT_EVENT: %ld", event_id);
  esp_mqtt_event_handle_t event = static_cast<esp_mqtt_event_handle_t>(event_data);
  switch (event->event_id) {
    case MQTT_EVENT_CONNECTED:
      static_cast<Mqtt*>(arg)->connected_ = true;
      static_cast<Mqtt*>(arg)->messages_.clear();
      break;
    case MQTT_EVENT_DISCONNECTED:
      static_cast<Mqtt*>(arg)->connected_ = false;
      break;
    case MQTT_EVENT_SUBSCRIBED: {
      break;
    }
    case MQTT_EVENT_UNSUBSCRIBED: {
      std::string topic(event->topic, event->topic_len);
      static_cast<Mqtt*>(arg)->messages_.erase(topic);
      break;
    }
    case MQTT_EVENT_PUBLISHED:
      break;
    case MQTT_EVENT_DATA: {
      std::string topic(event->topic, event->topic_len);
      std::string message(event->data, event->data_len);
      if (message.find(static_cast<Mqtt*>(arg)->client_id_) != std::string::npos) {
        break;
      }
      if (static_cast<Mqtt*>(arg)->messages_.find(topic) == static_cast<Mqtt*>(arg)->messages_.end()) {
        static_cast<Mqtt*>(arg)->messages_.emplace(topic, std::deque<std::string>{message});
      } else {
        static_cast<Mqtt*>(arg)->messages_[topic].push_back(message);
      }
      break;
    }
    default:
      break;
  }
}