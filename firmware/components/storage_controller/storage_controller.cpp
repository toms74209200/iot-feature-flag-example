#include "storage_controller.h"

void storage_controller::StorageController::Write(const std::string_view key, const std::string_view value) noexcept {
  std::unique_ptr<nvs::NVSHandle> handle = nvs::open_nvs_handle(std::string{storage_namespace_}.c_str(), NVS_READWRITE, nullptr);
  handle->set_string(std::string{key}.c_str(), std::string{value}.c_str());
}

std::string storage_controller::StorageController::Read(const std::string_view key) noexcept {
  std::unique_ptr<nvs::NVSHandle> handle = nvs::open_nvs_handle(std::string{storage_namespace_}.c_str(), NVS_READONLY, nullptr);
  size_t required_size = 0;
  handle->get_item_size(nvs::ItemType::SZ, std::string{key}.c_str(), required_size);

  char* value = new char[required_size];
  handle->get_string(std::string{key}.c_str(), value, required_size);

  auto result = std::string{value};
  delete[] value;
  return result;
}