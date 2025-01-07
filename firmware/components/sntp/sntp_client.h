/**
 * @file sntp_client.h
 * @brief SNTP component.
 */
#pragma once

#include <string>
#include <string_view>

#include "esp_netif_sntp.h"
#include "esp_sntp.h"

namespace sntp_client {
class SntpClient {
 public:
  /**
   * @brief Connect to SNTP server.
   * @param sntp_server[in] SNTP server.
   * @param timezone[in] Timezone. POSIX format.
   */
  void ConnectNtp(const std::string_view sntp_server, const std::string_view timezone) noexcept;

  /**
   * @brief Check if connected to SNTP server.
   * @return True if connected to SNTP server.
   */
  bool Connected() const noexcept;
};
}  // namespace sntp_client