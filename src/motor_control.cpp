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

void parseMessage(String message) {
  int firstSlash = message.indexOf('/');
  int secondSlash = message.lastIndexOf('/');

  String stateStr = message.substring(0, firstSlash);
  String speedStr = message.substring(firstSlash + 1, secondSlash);
  String directionStr = message.substring(secondSlash + 1);

  MotorState desiredState;
  if (stateStr == "on") {
    desiredState = ON;
  } else if (stateStr == "off") {
    desiredState = OFF;
  } else {
    Serial.println("Invalid state!");
    return;
  }

  SpeedLevel desiredSpeed;
  if (speedStr == "1") {
    desiredSpeed = SLOW;
  } else if (speedStr == "2") {
    desiredSpeed = MEDIUM;
  } else if (speedStr == "3") {
    desiredSpeed = HIGH;
  } else {
    Serial.println("Invalid speed!");
    return;
  }

  Direction desiredDirection;
  if (directionStr == "rtl") {
    desiredDirection = FORWARD;
  } else if (directionStr == "ltr") {
    desiredDirection = REVERSE;
  } else {
    Serial.println("Invalid direction!");
    return;
  }
  controlMotor(desiredState, desiredDirection, desiredSpeed);
}
