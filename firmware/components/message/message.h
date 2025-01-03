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
  const std::string client_id;
  const std::string name;
  std::string ToJson() const noexcept override {
    return R"({"client_id":")" + client_id + R"(","name":")" + name + R"("})";
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
    return R"({"client_id":")" + client_id + R"(","feature":{"greeter":)" + (greeter_is_enabled ? "true" : "false") + R"(}})";
  }
};

/**
 * @brief Create a message from a string.
 * @param message[in] Message.
 * @return Message.
 */
std::unique_ptr<Message> MessageFrom(const std::string_view message) noexcept {
  std::unique_ptr<cJSON, decltype(&cJSON_Delete)> root(cJSON_Parse(message.data()), cJSON_Delete);
  if (root == nullptr) {
    return nullptr;
  }
  cJSON* client_id_value = cJSON_GetObjectItem(root.get(), kClientIdKey.data());
  if (message.find(kNameKey) != std::string::npos) {
    cJSON* name_value = cJSON_GetObjectItem(root.get(), kNameKey.data());
    if (name_value == nullptr || !cJSON_IsString(name_value)) {
      return nullptr;
    }
    if (client_id_value == nullptr || !cJSON_IsString(client_id_value)) {
      return std::make_unique<GreeterMessage>("", name_value->valuestring);
    } else {
      return std::make_unique<GreeterMessage>(client_id_value->valuestring, name_value->valuestring);
    }
  }
  if (message.find(kFeatureKey) != std::string::npos) {
    cJSON* feature_value = cJSON_GetObjectItem(root.get(), kFeatureKey.data());
    if (feature_value == nullptr) {
      return nullptr;
    }
    cJSON* greeter_is_enable_value = cJSON_GetObjectItem(feature_value, kGreeterKey.data());
    if (client_id_value == nullptr || !cJSON_IsString(client_id_value)) {
      return std::make_unique<FeatureMessage>("", greeter_is_enable_value->type == cJSON_True);
    } else {
      return std::make_unique<FeatureMessage>(client_id_value->valuestring, greeter_is_enable_value->type == cJSON_True);
    }
  }
  return nullptr;
}

}  // namespace message