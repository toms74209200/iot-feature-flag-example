#include "greeter_impl.h"

void greeter::GreeterImpl::Greet() noexcept {
  auto messages = mqtt_->PopMessages(topic_);
  if (messages.empty()) {
    return;
  }
  for (const auto& msg : messages) {
    auto m = message::MessageFrom(msg);
    if (m == nullptr) {
      continue;
    }
    if (typeid(*m) != typeid(message::GreeterMessage)) {
      continue;
    }
    std::unique_ptr<message::GreeterMessage> greeter_message = std::make_unique<message::GreeterMessage>(dynamic_cast<message::GreeterMessage*>(m.get()));
    mqtt_->Publish(topic_, message::GreeterReplyMessage(client_id_, "Hello, " + greeter_message->name + "!").ToJson());
  }
}