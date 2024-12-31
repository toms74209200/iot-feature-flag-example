/**
 * @file storage_controller.h
 * @brief Storage component.
 */
#pragma once

#include <string>
#include <string_view>

#include "nvs.h"
#include "nvs_flash.h"
#include "nvs_handle.hpp"

namespace storage_controller {
/**
 * @brief Storage.
 */
class StorageController {
 public:
  /**
   * @brief Constructor.
   * @param storage_namespace[in] Storage namespace.
   */
  StorageController(const std::string_view storage_namespace) noexcept : storage_namespace_(storage_namespace) {
    nvs_flash_init();
  };
  /**
   * @brief Write key-value pair.
   * @param key[in] Key.
   * @param value[in] Value.
   */
  void Write(const std::string_view key, const std::string_view value) noexcept;
  /**
   * @brief Read value by key. If key does not exist, return empty string.
   * @param key[in] Key.
   * @return Value.
   */
  std::string Read(const std::string_view key) noexcept;

 private:
  const std::string_view storage_namespace_;  ///< Storage namespace.
};
}  // namespace storage_controller