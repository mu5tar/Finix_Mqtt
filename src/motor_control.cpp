// src/motor_control.cpp
#include "motor_control.h"

void setupMotorPins() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(EN, OUTPUT);
  analogWrite(EN, 0);
}

void controlFan(bool power, int speedLevel, String direction) {
  int pwmValue = 0;
  if (speedLevel == 1) pwmValue = 85;
  else if (speedLevel == 2) pwmValue = 170;
  else if (speedLevel == 3) pwmValue = 255;

  if (!power) {
    analogWrite(EN, 0);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    Serial.println("Fan stopped");
    return;
  }

  if (direction == "ltr") {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  } else if (direction == "rtl") {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
  }

  analogWrite(EN, pwmValue);
  Serial.printf("Fan ON, speed: %d, direction: %s\n", speedLevel, direction.c_str());
}
