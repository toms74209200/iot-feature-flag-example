#include "timestamp_format.h"

timestamp::TimestampFormat timestamp::TimestampFormatFrom(const std::string_view format) noexcept {
  if (format == "iso8601") {
    return TimestampFormat::kIso8601;
  } else if (format == "epoch") {
    return TimestampFormat::kIso8601;
  }
  return TimestampFormat::kIso8601;
}

timestamp::TimestampFormat timestamp::TimestampFormatFrom(const uint8_t format) noexcept {
  switch (format) {
    case 0:
      return TimestampFormat::kIso8601;
    case 1:
      return TimestampFormat::kEpoch;
    default:
      return TimestampFormat::kIso8601;
  }
}

std::string timestamp::TimestampFormatToString(const TimestampFormat format) noexcept {
  switch (format) {
    case TimestampFormat::kIso8601:
      return "iso8601";
    case TimestampFormat::kEpoch:
      return "epoch";
    default:
      return "iso8601";
  }
}

uint8_t timestamp::TimestampFormatToUint8(const TimestampFormat format) noexcept {
  switch (format) {
    case TimestampFormat::kIso8601:
      return 0;
    case TimestampFormat::kEpoch:
      return 1;
    default:
      return 0;
  }
}