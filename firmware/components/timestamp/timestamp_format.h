/**
 * @file timestamp_format.h
 * @brief Timestamp format component.
 */

#pragma once

#include <cstdint>
#include <string>
#include <string_view>

namespace timestamp {
enum class TimestampFormat {
  kIso8601 = 0,
  kEpoch,
};

/**
 * @brief Convert string to timestamp format.
 * @param format[in] Format.
 * @return Timestamp format. If the format is not recognized, kIso8601 is returned by default.
 */
TimestampFormat TimestampFormatFrom(const std::string_view format) noexcept;

/**
 * @brief Convert uint8_t to timestamp format.
 * @param format[in] Format.
 * @return Timestamp format. If the format is not recognized, kIso8601 is returned by default.
 */
TimestampFormat TimestampFormatFrom(const uint8_t format) noexcept;

/**
 * @brief Convert timestamp format to string.
 * @param format[in] Format.
 * @return String representation of the timestamp format.
 */
std::string TimestampFormatToString(const TimestampFormat format) noexcept;

/**
 * @brief Convert timestamp format to uint8_t.
 * @param format[in] Format.
 * @return uint8_t representation of the timestamp format.
 */
uint8_t TimestampFormatToUint8(const TimestampFormat format) noexcept;
}  // namespace timestamp
