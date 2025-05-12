#include <Arduino.h>
#include "status_led.h"

#define STATUS_LED 2

void initStatusLED() {
  pinMode(STATUS_LED, OUTPUT);
  digitalWrite(STATUS_LED, LOW);
}

void indicateStatus(bool connected) {
  digitalWrite(STATUS_LED, connected ? HIGH : LOW);
}
