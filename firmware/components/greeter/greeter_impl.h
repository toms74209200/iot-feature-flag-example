/**
 * @file greeter_impl.h
 * @brief Greeter component implementation.
 */
#pragma once

#include <memory>
#include <string>

#include "esp_log.h"

#include "greeter.h"
#include "message.h"
#include "mqtt.h"

namespace greeter {
class GreeterImpl : public Greeter {
 public:
  GreeterImpl(const std::shared_ptr<mqtt_client::Mqtt> mqtt, const std::string_view client_id, const std::string_view topic) noexcept
      : mqtt_(mqtt), client_id_(client_id), topic_(topic) {
    mqtt_->Subscribe(topic_);
  };
  /**
   * @brief Greet.
   * Implementation that sends the greeting message to MQTT broker.
   */
  void Greet() noexcept override;

 private:
  const std::shared_ptr<mqtt_client::Mqtt> mqtt_;
  const std::string client_id_;
  const std::string topic_;
};
}  // namespace greeter