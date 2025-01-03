#include "greeter_dummy.h"

void greeter::GreeterDummy::Greet() noexcept {
  ESP_LOGI(std::string{kTag}.c_str(), "Hello, world!");
}