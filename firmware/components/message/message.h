/**
 * @file message.h
 * @brief Message component.
 */
#pragma once

#include <memory>
#include <string>
#include <string_view>

#include "cJSON.h"

#include "timestamp_format.h"

namespace message {

constexpr std::string_view kClientIdKey = "client_id";
constexpr std::string_view kNameKey = "name";
constexpr std::string_view kFeatureKey = "feature";
constexpr std::string_view kTimestampKey = "timestamp";
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
  GreeterMessage(const std::string& client_id, const std::string& name) : client_id(client_id), name(name) {};
  GreeterMessage(const GreeterMessage* message) : client_id(message->client_id), name(message->name) {};
  const std::string client_id;
  const std::string name;
  std::string ToJson() const noexcept override {
    return "{\"" + std::string{kClientIdKey} + "\":\"" + client_id + "\",\"" + std::string{kNameKey} + "\":\"" + name + "\"}";
  }
};

struct GreeterReplyMessage : public Message {
  GreeterReplyMessage(const std::string& client_id, const std::string& message) : client_id(client_id), message(message) {};
  const std::string client_id;
  const std::string message;
  std::string ToJson() const noexcept override {
    return "{\"" + std::string{kClientIdKey} + "\":\"" + client_id + "\",\"message\":\"" + message + "\"}";
  }
};

/**
 * @brief Request timestamp message.
 * @note JSON schema is as follows:
 * @code
 * {
 *   "client_id": "foo"
 * }
 * @endcode
 */
struct TimestampMessage : public Message {
  TimestampMessage(const std::string& client_id) : client_id(client_id) {};
  const std::string client_id;
  std::string ToJson() const noexcept override {
    return "{\"" + std::string{kClientIdKey} + "\":\"" + client_id + "\"}";
  }
};

/**
 * @brief Timestamp reply message.
 * @note JSON schema is as follows:
 * @code
 * {
 *   "client_id": "foo",
 *   "timestamp": "2021-01-01T00:00:00Z"
 * }
 * @endcode
 */
struct TimestampReplyMessage : public Message {
  TimestampReplyMessage(const std::string& client_id, const std::string& timestamp) : client_id(client_id), timestamp(timestamp) {};
  const std::string client_id;
  const std::string timestamp;
  std::string ToJson() const noexcept override {
    return "{\"" + std::string{kClientIdKey} + "\":\"" + client_id + "\",\"timestamp\":\"" + timestamp + "\"}";
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
  FeatureMessage(const std::string& client_id) : client_id(client_id) {};
  FeatureMessage(const std::string& client_id, const bool greeter_is_enabled, const timestamp::TimestampFormat timestamp_format)
      : client_id(client_id), greeter_is_enabled(greeter_is_enabled), timestamp_format(timestamp_format) {};
  FeatureMessage(const FeatureMessage* message)
      : client_id(message->client_id), greeter_is_enabled(message->greeter_is_enabled), timestamp_format(message->timestamp_format) {};
  const std::string client_id;
  const bool greeter_is_enabled = false;
  const timestamp::TimestampFormat timestamp_format = timestamp::TimestampFormat::kIso8601;
  std::string ToJson() const noexcept override {
    return "{\"}" + std::string{kClientIdKey} + "\":\"" + client_id + "\",\"" + std::string{kFeatureKey} +
           "\":{\"greeter\":" + (greeter_is_enabled ? "true" : "false") + ",\"timestamp\":\"" + timestamp::TimestampFormatToString(timestamp_format) + "\"}}";
  }
};

/**
 * @brief Create a message from a string.
 * @param message[in] Message.
 * @return Message.
 */
std::unique_ptr<Message> MessageFrom(const std::string_view msg) noexcept;

}  // namespace message