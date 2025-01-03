/**
 * @file greeter.h
 * @brief Greeter component.
 */
#pragma once

#include <string_view>

namespace greeter {
/**
 * @brief Greeter interface.
 */
class Greeter {
 public:
  virtual ~Greeter() = default;
  /**
   * @brief Greet. The output destination depends on the implementation.
   */
  virtual void Greet() noexcept = 0;
};
}  // namespace greeter