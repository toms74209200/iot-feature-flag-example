/**
 * @file wifi_client.h
 * @brief Wi-Fi client component.
 */
#pragma once

#include <cstring>
#include <string>
#include <string_view>

#include "esp_log.h"
#include "esp_mac.h"
#include "esp_wifi.h"
#include "nvs_flash.h"

namespace wifi_client {

constexpr std::string_view kTag = "wifi_client";

/**
 * @brief Wi-Fi client.
 */
class WifiClient {
 public:
  /**
   * @brief Connect to Wi-Fi.
   * @param ssid[in] SSID.
   * @param password[in] Password.
   */
  void ConnectWifi(const std::string_view ssid, const std::string_view password) noexcept;

  /**
   * @brief Check if connected to Wi-Fi.
   * @return True if connected to Wi-Fi.
   */
  bool Connected() const noexcept;

  /**
   * @brief Get MAC address.
   * @return MAC address.
   */
  static std::string GetMac() noexcept;

 private:
  static constexpr int_fast32_t kMacSize = 6;
  static constexpr int_fast32_t kMacStrSize = kMacSize * 2 + 1;

  static void OnEventWifi(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) noexcept;

  static void OnEventIp(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) noexcept;
};

}  // namespace wifi_client