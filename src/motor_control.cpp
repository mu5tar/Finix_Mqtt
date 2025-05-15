#include "motor_control.h"

const int accelDelay = 50;   
const int accelStep = 2;     

int currentPWM = 0;
String currentDirection = "";
bool motorIsOn = false;

void setupMotorPins() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(EN, OUTPUT);
  analogWrite(EN, 0);
}

void setMotorDirection(String direction) {
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
}

void rampToPWM(int targetPWM) {
  while (currentPWM != targetPWM) {
    if (currentPWM < targetPWM) currentPWM += accelStep;
    else if (currentPWM > targetPWM) currentPWM -= accelStep;

    currentPWM = constrain(currentPWM, 0, 255);
    analogWrite(EN, currentPWM);
    delay(accelDelay);
  }
}

void stopMotor() {
  rampToPWM(0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  motorIsOn = false;
  currentDirection = "";
  Serial.println("Fan stopped.");
}

void controlFan(bool power, int speedLevel, String direction) {
  int targetPWM = 0;

  switch (speedLevel) {
    case 1: targetPWM = 85; break;
    case 2: targetPWM = 170; break;
    case 3: targetPWM = 255; break;
    default: targetPWM = 0; break;
  }

  if (!power) {
    stopMotor();
    return;
  }

  if (motorIsOn && direction != currentDirection) {
    rampToPWM(0);  
    setMotorDirection(direction);  
    currentDirection = direction;
    rampToPWM(targetPWM); 
  }
  
  else {
    if (!motorIsOn || currentDirection == "") {
      setMotorDirection(direction);
      currentDirection = direction;
    }
    rampToPWM(targetPWM);
  }

  motorIsOn = true;
  Serial.printf("Fan ON, speed: %d, direction: %s\n", speedLevel, direction.c_str());
}
