/**
 * @file storage_controller.h
 * @brief Storage component.
 */
#pragma once

#include <optional>
#include <string>
#include <string_view>

#include "esp_log.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "nvs_handle.hpp"

namespace storage_controller {

constexpr std::string_view kTag = "storage_controller";

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

  /**
   * @brief Write byte value.
   * @param key[in] Key.
   * @param value[in] Value.
   */
  void WriteByte(const std::string_view key, const uint8_t value) noexcept;
  /**
   * @brief Read byte value by key. If key does not exist, return 0.
   * @param key[in] Key.
   * @return Value. If key does not exist, return std::nullopt.
   */
  std::optional<uint8_t> ReadByte(const std::string_view key) noexcept;

 private:
  const std::string_view storage_namespace_;  ///< Storage namespace.
};
}  // namespace storage_controller