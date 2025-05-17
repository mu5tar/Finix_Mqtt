#include "motor_control.h"

void MotorSetup() {
  pinMode(ENA_PIN, OUTPUT);
  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);

  analogWrite(ENA_PIN, 0);
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, LOW);
}

void controlMotor(MotorState desiredState, Direction desiredDirection, SpeedLevel desiredSpeed) {
  if (desiredState == OFF && currentState == OFF) {
    return; 
  }
  
  if (desiredState == OFF && currentState == ON) {
    while (currentSpeed > 0) {
      currentSpeed -= 5;
      if (currentSpeed < 0) currentSpeed = 0;
      analogWrite(ENA_PIN, currentSpeed);
      delay(20); 
    }
    digitalWrite(IN1_PIN, LOW);
    digitalWrite(IN2_PIN, LOW);
    currentState = OFF;
    return;
  }
  
  if (desiredState == ON) {
    bool directionChanged = (currentDirection != desiredDirection && currentState == ON);
    
    if (directionChanged) {
      while (currentSpeed > 0) {
        currentSpeed -= 5;
        if (currentSpeed < 0) currentSpeed = 0;
        analogWrite(ENA_PIN, currentSpeed);
        delay(20);
      }
      delay(100);
    }
    
    if (desiredDirection == FORWARD) {
      digitalWrite(IN1_PIN, HIGH);
      digitalWrite(IN2_PIN, LOW);
    } else { 
      digitalWrite(IN1_PIN, LOW);
      digitalWrite(IN2_PIN, HIGH);
    }
    currentDirection = desiredDirection;
    
    int targetSpeed = (int)desiredSpeed;
    if (currentSpeed < targetSpeed) {
      while (currentSpeed < targetSpeed) {
        currentSpeed += 5;
        if (currentSpeed > targetSpeed) currentSpeed = targetSpeed;
        analogWrite(ENA_PIN, currentSpeed);
        delay(20);
      }
    } else if (currentSpeed > targetSpeed) {
      while (currentSpeed > targetSpeed) {
        currentSpeed -= 5;
        if (currentSpeed < targetSpeed) currentSpeed = targetSpeed;
        analogWrite(ENA_PIN, currentSpeed);
        delay(20);
      }
    }
    
    currentState = ON;
  }
}

