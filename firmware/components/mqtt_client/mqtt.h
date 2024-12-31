/**
 * @file mqtt.h
 * @brief MQTT client component.
 */
#pragma once

#include <deque>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "esp_log.h"
#include "mqtt_client.h"

namespace mqtt_client {

constexpr std::string_view kTag = "mqtt";

/**
 * @brief MQTT client.
 */
class Mqtt {
 public:
  /**
   * @brief Connect to MQTT broker.
   * @param domain[in] Domain.
   * @param port[in] Port.
   */
  void Connect(const std::string_view domain, const int_fast32_t port) noexcept;

  /**
   * @brief Publish message.
   * @param topic[in] Topic.
   * @param message[in] Message.
   */
  void Publish(const std::string_view topic, const std::string_view message) noexcept;

  /**
   * @brief Subscribe to topic.
   * @param topic[in] Topic.
   */
  void Subscribe(const std::string_view topic) noexcept;

  /**
   * @brief Unsubscribe from topic.
   * @param topic[in] Topic.
   */
  void Unsubscribe(const std::string_view topic) noexcept;

  /**
   * @brief Disconnect from MQTT broker.
   */
  void Disconnect() noexcept;

  /**
   * @brief Check if connected to MQTT broker.
   * @return True if connected, false otherwise.
   */
  bool Connected() const noexcept {
    return connected_;
  };

  /**
   * @brief Pop messages from queue. When a message is popped, it is removed from the queue.
   * If topic does not exist, an empty vector is returned.
   * @param topic[in] Topic.
   * @return Messages.
   */
  std::vector<std::string> PopMessages(const std::string_view topic) noexcept;

 private:
  static void OnEvent(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) noexcept;

  esp_mqtt_client_handle_t client_;
  bool connected_ = false;
  std::unordered_map<std::string_view, std::deque<std::string>> messages_;
};
}  // namespace mqtt