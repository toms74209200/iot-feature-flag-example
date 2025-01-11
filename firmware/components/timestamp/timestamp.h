#pragma once

#include <chrono>
#include <format>
#include <memory>
#include <string>

#include "message.h"
#include "mqtt.h"
#include "timestamp_format.h"

namespace timestamp {

class Timestamp {
 public:
  Timestamp(const std::shared_ptr<mqtt_client::Mqtt> mqtt, const std::string_view client_id, const std::string_view topic) noexcept
      : mqtt_(mqtt), client_id_(client_id), topic_(topic){};
  void Notify(const TimestampFormat format) noexcept;

 private:
  const std::shared_ptr<mqtt_client::Mqtt> mqtt_;
  const std::string client_id_;
  const std::string topic_;
  bool subscribed_ = false;
};

}  // namespace timestamp