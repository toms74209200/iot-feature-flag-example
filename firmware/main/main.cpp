#include "freertos/FreeRTOS.h"

void setup() {
}

void loop() {}

extern "C" {
void app_main(void) {
  setup();
  while (1) {
    loop();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
}
