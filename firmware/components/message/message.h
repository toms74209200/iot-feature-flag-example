/**
 * @file message.h
 * @brief Message component.
 */
#pragma once

#include <memory>
#include <string>
#include <string_view>

#include "cJSON.h"

namespace message {

constexpr std::string_view kClientIdKey = "client_id";
constexpr std::string_view kNameKey = "name";
constexpr std::string_view kFeatureKey = "feature";
constexpr std::string_view kGreeterKey = "greeter";

struct Message {
  virtual ~Message() = default;
  virtual std::string ToJson() const noexcept = 0;
};

/**
 * @brief Greeter message.
 * @note JSON schema is as follows:
 * @code
 * {
 *   "client_id": "foo",
 *   "name": "foo"
 * }
 * @endcode
 */
struct GreeterMessage : public Message {
  GreeterMessage(const std::string& client_id, const std::string& name) : client_id(client_id), name(name){};
  GreeterMessage(const GreeterMessage* message) : client_id(message->client_id), name(message->name){};
  const std::string client_id;
  const std::string name;
  std::string ToJson() const noexcept override {
    return "{\"" + std::string{kClientIdKey} + "\":\"" + client_id + "\",\"" + std::string{kNameKey} + "\":\"" + name + "\"}";
  }
};

struct GreeterReplyMessage : public Message {
  GreeterReplyMessage(const std::string& client_id, const std::string& message) : client_id(client_id), message(message){};
  const std::string client_id;
  const std::string message;
  std::string ToJson() const noexcept override {
    return "{\"" + std::string{kClientIdKey} + "\":\"" + client_id + "\",\"message\":\"" + message + "\"}";
  }
};

/**
 * @brief Feature message.
 * @note JSON schema is as follows:
 * @code
 * {
 *   "client_id": "foo",
 *   "feature": {
 *     "greeter": true
 *   }
 * }
 * @endcode
 */
struct FeatureMessage : public Message {
  FeatureMessage(const std::string& client_id) : client_id(client_id){};
  FeatureMessage(const std::string& client_id, const bool greeter_is_enabled) : client_id(client_id), greeter_is_enabled(greeter_is_enabled){};
  const std::string client_id;
  const bool greeter_is_enabled = false;
  std::string ToJson() const noexcept override {
    return "{\"}" + std::string{kClientIdKey} + "\":\"" + client_id + "\",\"" + std::string{kFeatureKey} +
           "\":{\"greeter\":" + (greeter_is_enabled ? "true" : "false") + "}}";
  }
};

/**
 * @brief Create a message from a string.
 * @param message[in] Message.
 * @return Message.
 */
std::unique_ptr<Message> MessageFrom(const std::string_view msg) noexcept;

}  // namespace message