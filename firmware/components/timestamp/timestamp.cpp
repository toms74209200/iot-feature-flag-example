#include "timestamp.h"

void timestamp::Timestamp::Notify(const timestamp::TimestampFormat format) noexcept {
  if (!subscribed_) {
    mqtt_->Subscribe(topic_);
    subscribed_ = true;
  }
  auto messages = mqtt_->PopMessages(topic_);
  if (messages.empty()) {
    return;
  }
  for (const auto& msg : messages) {
    auto m = message::MessageFrom(msg);
    if (m == nullptr) {
      continue;
    }
    if (typeid(*m) != typeid(message::TimestampMessage)) {
      continue;
    }
    std::chrono::time_point now = std::chrono::system_clock::now();
    std::string timestamp;
    switch (format) {
      case TimestampFormat::kIso8601:
        timestamp = std::format("{:%FT%R:%OS%Ez}", now);
        break;
      case TimestampFormat::kEpoch:
        timestamp = std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count());
        break;
    }
    mqtt_->Publish(topic_, message::TimestampReplyMessage(client_id_, timestamp).ToJson());
  }
}