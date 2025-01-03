#include "message.h"

std::unique_ptr<message::Message> message::MessageFrom(const std::string_view msg) noexcept {
  std::unique_ptr<cJSON, decltype(&cJSON_Delete)> root(cJSON_Parse(msg.data()), cJSON_Delete);
  if (root == nullptr) {
    return nullptr;
  }
  cJSON* client_id_value = cJSON_GetObjectItem(root.get(), kClientIdKey.data());
  if (msg.find(kNameKey) != std::string::npos) {
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
  if (msg.find(kFeatureKey) != std::string::npos) {
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