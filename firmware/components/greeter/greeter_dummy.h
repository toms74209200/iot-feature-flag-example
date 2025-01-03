/**
 * @file greeter_dummy.h
 * @brief Greeter component dummy.
 */
#pragma once

#include <string>
#include <string_view>

#include "esp_log.h"

#include "greeter.h"

namespace greeter {

constexpr std::string_view kTag = "greeter dummy";

class GreeterDummy : public Greeter {
 public:
  /**
   * @brief Greet. Dummy implementation that logs the greeting message.
   */
  void Greet() noexcept override;
};
}  // namespace greeter