#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#define ENA_PIN 9    
#define IN1_PIN 7    
#define IN2_PIN 8    

enum MotorState { OFF, ON };
enum Direction { FORWARD, REVERSE };
enum SpeedLevel { SLOW = 85, MEDIUM = 170, HIGH = 255 }; 

MotorState currentState = OFF;
Direction currentDirection = FORWARD;
int currentSpeed = 0; // السرعة الحالية (PWM: 0-255)
void MotorSetup() ;
void controlMotor(MotorState desiredState, Direction desiredDirection, SpeedLevel desiredSpeed);
void parseMessage(String message);
#endif
