// src/motor_control.h
#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include <Arduino.h>

#define IN1 26
#define IN2 27
#define EN  25

void setupMotorPins();
void controlFan(bool power, int speedLevel, String direction);

#endif
