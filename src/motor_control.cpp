#include "motor_control.h"

// Ramping variables
int currentPwm = 0;             // Current PWM value
int targetPwm = 0;              // Target PWM value
unsigned long rampStartTime = 0;// Time when ramping started
bool isRamping = false;         // Ramping in progress
bool rampingEnabled = true;     // Enable/disable ramping (default: true)
MotorState currentState = STOP; // Current motor state
MotorState targetState = STOP;  // Target motor state

void setupMotor() {
  // Set pins as outputs
  pinMode(HIN1, OUTPUT);
  pinMode(LIN1, OUTPUT);
  pinMode(HIN2, OUTPUT);
  pinMode(LIN2, OUTPUT);
  
  // Initialize all pins to LOW (motor off)
  digitalWrite(HIN1, LOW);
  digitalWrite(LIN1, LOW);
  digitalWrite(HIN2, LOW);
  digitalWrite(LIN2, LOW);
  
  // Configure ESP32 PWM for LIN1 and LIN2
  ledcSetup(pwmChannelLIN1, pwmFreq, pwmResolution);
  ledcSetup(pwmChannelLIN2, pwmFreq, pwmResolution);
  ledcAttachPin(LIN1, pwmChannelLIN1);
  ledcAttachPin(LIN2, pwmChannelLIN2);
  
  // Initialize PWM to 0
  ledcWrite(pwmChannelLIN1, 0);
  ledcWrite(pwmChannelLIN2, 0);
}

void setMotorForward(int pwmValue) {
  targetPwm = constrain(pwmValue, 0, 255);
  targetState = FORWARD;
  if (currentState != FORWARD) {
    // Stop motor and apply dead time
    stopMotorImmediate();
    delayMicroseconds(deadTime);
  }
  startRamping();
}

void setMotorReverse(int pwmValue) {
  targetPwm = constrain(pwmValue, 0, 255);
  targetState = REVERSE;
  if (currentState != REVERSE) {
    // Stop motor and apply dead time
    stopMotorImmediate();
    delayMicroseconds(deadTime);
  }
  startRamping();
}

void stopMotor() {
  targetPwm = 0;
  targetState = STOP;
  startRamping();
}

void stopMotorImmediate() {
  // Turn off all MOSFETs immediately
  digitalWrite(HIN1, LOW);
  digitalWrite(LIN1, LOW);
  digitalWrite(HIN2, LOW);
  digitalWrite(LIN2, LOW);
  ledcWrite(pwmChannelLIN1, 0);
  ledcWrite(pwmChannelLIN2, 0);
  currentPwm = 0;
  currentState = STOP;
}

void startRamping() {
  if (!rampingEnabled) {
    // Skip ramping if disabled
    currentPwm = targetPwm;
    isRamping = false;
  } else {
    rampStartTime = millis();
    isRamping = true;
  }
}

void updateRamping() {
  if (!rampingEnabled) {
    // Apply PWM instantly if ramping is disabled
    currentPwm = targetPwm;
    isRamping = false;
  } else if (!isRamping) {
    return;
  } else {
    unsigned long elapsed = millis() - rampStartTime;
    if (elapsed >= rampTime) {
      // Ramping complete
      currentPwm = targetPwm;
      isRamping = false;
    } else {
      // Linear interpolation for smooth ramping
      currentPwm = currentPwm + (targetPwm - currentPwm) * (elapsed / (float)rampTime);
    }
  }
  
  // Apply motor state and PWM
  switch (targetState) {
    case FORWARD:
      digitalWrite(HIN1, HIGH);   // Q1 on
      ledcWrite(pwmChannelLIN1, 0); // Q2 off
      digitalWrite(HIN2, LOW);    // Q3 off
      ledcWrite(pwmChannelLIN2, currentPwm); // PWM on Q4
      currentState = FORWARD;
      break;
    case REVERSE:
      digitalWrite(HIN1, LOW);    // Q1 off
      ledcWrite(pwmChannelLIN1, currentPwm); // PWM on Q2
      digitalWrite(HIN2, HIGH);   // Q3 on
      ledcWrite(pwmChannelLIN2, 0); // Q4 off
      currentState = REVERSE;
      break;
    case STOP:
      stopMotorImmediate();
      break;
  }
}

void setRampingEnabled(bool enabled) {
  rampingEnabled = enabled;
  Serial.printf("Ramping %s\n", enabled ? "enabled" : "disabled");
  if (!enabled && isRamping) {
    // Instantly apply target PWM if ramping is disabled mid-ramp
    currentPwm = targetPwm;
    isRamping = false;
    updateRamping(); // Apply immediately
  }
}

void controlMotor(String power, int speed, String direction) {
  // Convert inputs to lowercase for case-insensitive comparison
  power.toLowerCase();
  direction.toLowerCase();
  
  // Validate power
  if (power != "on" && power != "off") {
    Serial.println("Error: power must be 'on' or 'off'");
    stopMotor();
    return;
  }
  
  // Handle power off
  if (power == "off") {
    Serial.println("Motor Off");
    stopMotor();
    return;
  }
  
  // Validate speed
  if (speed < 1 || speed > 3) {
    Serial.println("Error: speed must be 1, 2, or 3");
    stopMotor();
    return;
  }
  
  // Map speed to PWM (1: ~33%, 2: ~67%, 3: 100%)
  int pwmValue;
  switch (speed) {
    case 1: pwmValue = 85; break;
    case 2: pwmValue = 170; break;
    case 3: pwmValue = 255; break;
    default: pwmValue = 0; // Should not occur due to validation
  }
  
  // Validate and apply direction
  if (direction == "ltr") {
    Serial.printf("Motor Forward, Speed: %d, PWM: %d\n", speed, pwmValue);
    setMotorForward(pwmValue);
  } else if (direction == "rtl") {
    Serial.printf("Motor Reverse, Speed: %d, PWM: %d\n", speed, pwmValue);
    setMotorReverse(pwmValue);
  } else {
    Serial.println("Error: direction must be 'ltr' or 'rtl'");
    stopMotor();
  }
}
