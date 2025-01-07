#include "sntp_client.h"

void sntp_client::SntpClient::ConnectNtp(const std::string_view sntp_server, const std::string_view timezone) noexcept {
  esp_sntp_config_t config = ESP_NETIF_SNTP_DEFAULT_CONFIG(sntp_server.data());
  esp_netif_sntp_init(&config);
  setenv("TZ", timezone.data(), 1);
  tzset();
}

bool sntp_client::SntpClient::Connected() const noexcept {
  return sntp_get_sync_status() == SNTP_SYNC_STATUS_COMPLETED;
}