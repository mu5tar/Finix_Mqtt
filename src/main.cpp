#include <Arduino.h>
#include "config.h"
#include "net_handler.h"
#include "mqtt_handler.h"
#include "status_led.h"
#include "motor_control.h"


void setup() {
  Serial.begin(115200);
  delay(1000);
  initStatusLED();
  initNetwork();
  setupMQTT();
  setupMotorPins();
}

void loop() {
  checkNetworkConnection();
  handleMQTT();
}