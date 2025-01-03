#include "storage_controller.h"

void storage_controller::StorageController::Write(const std::string_view key, const std::string_view value) noexcept {
  std::unique_ptr<nvs::NVSHandle> handle = nvs::open_nvs_handle(std::string{storage_namespace_}.c_str(), NVS_READWRITE, nullptr);
  esp_err_t err = handle->set_string(std::string{key}.c_str(), std::string{value}.c_str());
  if (err != ESP_OK) {
    ESP_LOGE(std::string{kTag}.c_str(), "Failed to set item");
  }
}

std::string storage_controller::StorageController::Read(const std::string_view key) noexcept {
  std::unique_ptr<nvs::NVSHandle> handle = nvs::open_nvs_handle(std::string{storage_namespace_}.c_str(), NVS_READONLY, nullptr);
  if (handle == nullptr) {
    ESP_LOGE(std::string{kTag}.c_str(), "Failed to open NVS handle");
    return "";
  }
  size_t required_size = 0;
  esp_err_t get_size_err = handle->get_item_size(nvs::ItemType::SZ, std::string{key}.c_str(), required_size);
  if (get_size_err != ESP_OK) {
    ESP_LOGE(std::string{kTag}.c_str(), "Failed to get item size");
    return "";
  }

  char* value = new char[required_size];
  esp_err_t get_err = handle->get_string(std::string{key}.c_str(), value, required_size);
  if (get_err != ESP_OK) {
    ESP_LOGE(std::string{kTag}.c_str(), "Failed to get item");
    delete[] value;
    return "";
  }

  auto result = std::string{value};
  delete[] value;
  return result;
}

void storage_controller::StorageController::WriteByte(const std::string_view key, const uint8_t value) noexcept {
  std::unique_ptr<nvs::NVSHandle> handle = nvs::open_nvs_handle(std::string{storage_namespace_}.c_str(), NVS_READWRITE, nullptr);
  esp_err_t err = handle->set_item(std::string{key}.c_str(), value);
  if (err != ESP_OK) {
    ESP_LOGE(std::string{kTag}.c_str(), "Failed to set item");
  }
}

std::optional<uint8_t> storage_controller::StorageController::ReadByte(const std::string_view key) noexcept {
  std::unique_ptr<nvs::NVSHandle> handle = nvs::open_nvs_handle(std::string{storage_namespace_}.c_str(), NVS_READONLY, nullptr);
  if (handle == nullptr) {
    ESP_LOGE(std::string{kTag}.c_str(), "Failed to open NVS handle");
    return std::nullopt;
  }
  uint8_t value = 0;
  esp_err_t err = handle->get_item(std::string{key}.c_str(), value);
  if (err != ESP_OK) {
    ESP_LOGE(std::string{kTag}.c_str(), "Failed to get item");
    return std::nullopt;
  }

  return value;
}