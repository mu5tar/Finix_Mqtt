// src/motor_control.h
#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include <Arduino.h>

// Pin definitions for IR2102 inputs (ESP32 GPIO pins)
const int HIN1 = 26;  // High-side input for IR2102 #1 (Q1)
const int LIN1 = 27;  // Low-side input for IR2102 #1 (Q2, PWM)
const int HIN2 = 14;  // High-side input for IR2102 #2 (Q3)
const int LIN2 = 12;  // Low-side input for IR2102 #2 (Q4, PWM)

// PWM settings for ESP32 LEDC
const int pwmFreq = 15000;      // 15 kHz PWM frequency
const int pwmResolution = 8;    // 8-bit resolution (0–255)
const int pwmChannelLIN1 = 0;   // LEDC channel for LIN1
const int pwmChannelLIN2 = 1;   // LEDC channel for LIN2

// Motor control parameters
const int deadTime = 1000;      // Dead time in microseconds
const int rampTime = 500;       // Ramp time in milliseconds for accel/decel
extern bool rampingEnabled;     // Enable/disable ramping (default: true)

// Motor state enum
enum MotorState { STOP, FORWARD, REVERSE };

// Function prototypes
void setupMotor();
void controlMotor(String power, int speed, String direction);
void setMotorForward(int pwmValue);
void setMotorReverse(int pwmValue);
void stopMotor();
void stopMotorImmediate();
void startRamping();
void updateRamping();
void setRampingEnabled(bool enabled);

#endif
