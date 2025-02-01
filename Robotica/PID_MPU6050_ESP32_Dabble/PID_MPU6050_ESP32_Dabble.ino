/*
 * Project: 2WD Robot Car Control with ESP32 and MPU6050
 * 
 * Description:
 * This code controls a two-wheel drive (2WD) robot car using an ESP32 microcontroller
 * and an MPU6050 sensor. The robot can be commanded via the Dabble app using Bluetooth.
 * The goal is to allow the robot to follow a straight path or make incremental rotations 
 * according to the received commands. The path control is performed using a PID 
 * (Proportional, Integral, Derivative) algorithm that corrects the direction based on the 
 * gyroscope data from the MPU6050, ensuring that the robot maintains its course, even when there is slippage.
 * 
 * Required Components:
 * - ESP32
 * - MPU6050
 * - TB6612 Motor Driver
 * - DC Motors (2 units)
 * 
 * Operation:
 * - The robot moves forward and backward according to the app's "forward" and "backward" commands.
 * - The robot rotates left and right based on the corresponding commands.
 * - PID is used to correct the path in real-time, adjusting each wheel's speed to maintain a straight trajectory.
 * 
 * Version: 1.1
 * Author: Prof. Dr. Robson Marinho
 * Date: 16/10/2024
 */

#include <Wire.h>
#include <MPU6050.h>
#include <DabbleESP32.h>

// MPU6050 Configuration
MPU6050 mpu;
int16_t ax, ay, az; // Acceleration
int16_t gx, gy, gz; // Gyroscope
float currentYaw = 0;
float targetYaw = 0;
bool rotating = false;

// Define TB6612 pins for motors
// Right motor
const int motorRightPWM = 13;  
const int rightMotorPin1 = 25;
const int rightMotorPin2 = 33;

// Left motor
const int motorLeftPWM = 32;
const int leftMotorPin1 = 26;
const int leftMotorPin2 = 27;

// Base speeds adjusted for the wheels
int baseMotorSpeedLeft = 50;  // Initial base speed for the left wheel
int baseMotorSpeedRight = 50;  // Initial base speed for the right wheel
int rotationSpeed = 50;        // Reduced speed for rotation

// PID constants adjusted for the left wheel
float KpLeft = 3.0, KiLeft = 0.1, KdLeft = 1.5;  // Constants adjusted to improve the left wheel
float prevErrorYawLeft = 0;
float integralYawLeft = 0;

// PID constants adjusted for the right wheel
float KpRight = 3.0, KiRight = 0.1, KdRight = 1.5;  // Constants adjusted for the right wheel
float prevErrorYawRight = 0;
float integralYawRight = 0;

// Sampling time for PID control
unsigned long lastTime = 0;
float dt = 0.1;  // Time interval in seconds (100ms)

void setup() {
  // Initialize Serial for debugging
  Serial.begin(115200);

  // Initialize Dabble
  Dabble.begin("ESP32-Robo");  // Robot name visible via Bluetooth app
  Serial.println("Dabble Bluetooth started!");

  // Initialize MPU6050
  Wire.begin();
  mpu.initialize();
  
  if (!mpu.testConnection()) {
    Serial.println("Failed to connect to MPU6050!");
    while (1);
  } else {
    Serial.println("MPU6050 connected!");
  }

  // Initialize motor pins as output
  pinMode(rightMotorPin1, OUTPUT);
  pinMode(rightMotorPin2, OUTPUT);
  pinMode(motorRightPWM, OUTPUT);
  
  pinMode(leftMotorPin1, OUTPUT);
  pinMode(leftMotorPin2, OUTPUT);
  pinMode(motorLeftPWM, OUTPUT);
}

void loop() {
  Dabble.processInput();  // Process Dabble commands

  // Read acceleration and gyroscope values from MPU6050
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Update the current yaw (rotation using the gyroscope)
  currentYaw += (gz / 131.0) * dt;  // 131 is the standard gyroscope sensitivity for degrees/second

  // Movement based on Dabble control
  if (GamePad.isUpPressed()) {
    moveForward();
    targetYaw = currentYaw;  // Set target yaw during movement
    correctTrajectoryPID();  // Correct trajectory during movement
  } else if (GamePad.isDownPressed()) {
    moveBackward();
    targetYaw = currentYaw;  // Set target yaw during movement
    correctTrajectoryPID();  // Correct trajectory during movement
  } else if (GamePad.isLeftPressed()) {
    rotateLeft();
    targetYaw -= 5;  // Set desired rotation value when turning left
    correctRotationPID();  // Correct rotation using PID for each wheel
  } else if (GamePad.isRightPressed()) {
    rotateRight();
    targetYaw += 5;  // Set desired rotation value when turning right
    correctRotationPID();  // Correct rotation using PID for each wheel
  } else {
    stopMotors();
  }
}

// Forward movement
void moveForward() {
  // Motors spinning forward
  digitalWrite(leftMotorPin1, HIGH);
  digitalWrite(leftMotorPin2, LOW);
  analogWrite(motorLeftPWM, baseMotorSpeedLeft);

  digitalWrite(rightMotorPin1, HIGH);
  digitalWrite(rightMotorPin2, LOW);
  analogWrite(motorRightPWM, baseMotorSpeedRight);
}

// Backward movement
void moveBackward() {
  // Motors spinning backward
  digitalWrite(leftMotorPin1, LOW);
  digitalWrite(leftMotorPin2, HIGH);
  analogWrite(motorLeftPWM, baseMotorSpeedLeft);

  digitalWrite(rightMotorPin1, LOW);
  digitalWrite(rightMotorPin2, HIGH);
  analogWrite(motorRightPWM, baseMotorSpeedRight);
}

// Function to stop the motors
void stopMotors() {
  analogWrite(motorLeftPWM, 0);
  analogWrite(motorRightPWM, 0);
}

// Incremental right rotation
void rotateRight() {
  digitalWrite(leftMotorPin1, LOW);  // Left motor moves backward
  digitalWrite(leftMotorPin2, HIGH);
  analogWrite(motorLeftPWM, rotationSpeed);

  digitalWrite(rightMotorPin1, HIGH);  // Right motor moves forward
  digitalWrite(rightMotorPin2, LOW);
  analogWrite(motorRightPWM, rotationSpeed);
}

// Incremental left rotation
void rotateLeft() {
  digitalWrite(leftMotorPin1, HIGH);  // Left motor moves forward
  digitalWrite(leftMotorPin2, LOW);
  analogWrite(motorLeftPWM, rotationSpeed);

  digitalWrite(rightMotorPin1, LOW);  // Right motor moves backward
  digitalWrite(rightMotorPin2, HIGH);
  analogWrite(motorRightPWM, rotationSpeed);
}

// Function to correct the trajectory using PID with MPU6050 data
void correctTrajectoryPID() {
  // Calculate time since last execution
  unsigned long currentTime = millis();
  dt = (currentTime - lastTime) / 1000.0;  // Convert to seconds
  lastTime = currentTime;

  // Calculate yaw error (rotation)
  float errorYaw = targetYaw - currentYaw;

  // Proportional term for the left wheel
  float proportionalYawLeft = KpLeft * errorYaw;

  // Integral term for the left wheel
  integralYawLeft += errorYaw * dt;
  float integralTermYawLeft = KiLeft * integralYawLeft;

  // Derivative term for the left wheel
  float derivativeYawLeft = (errorYaw - prevErrorYawLeft) / dt;
  float derivativeTermYawLeft = KdLeft * derivativeYawLeft;

  // Proportional term for the right wheel
  float proportionalYawRight = KpRight * errorYaw;

  // Integral term for the right wheel
  integralYawRight += errorYaw * dt;
  float integralTermYawRight = KiRight * integralYawRight;

  // Derivative term for the right wheel
  float derivativeYawRight = (errorYaw - prevErrorYawRight) / dt;
  float derivativeTermYawRight = KdRight * derivativeYawRight;

  // Calculate final correction for each wheel
  float correctionYawLeft = proportionalYawLeft + integralTermYawLeft + derivativeTermYawLeft;
  float correctionYawRight = proportionalYawRight + integralTermYawRight + derivativeTermYawRight;

  // Limit corrections to avoid extreme values
  correctionYawLeft = constrain(correctionYawLeft, -30, 30);  // Extended correction for the left wheel
  correctionYawRight = constrain(correctionYawRight, -30, 30);  // Maintained correction for the right wheel

  // Apply corrections to the motors
  int leftSpeed = baseMotorSpeedLeft - correctionYawLeft;   // Correct the left motor
  int rightSpeed = baseMotorSpeedRight + correctionYawRight; // Correct the right motor

  // Ensure speeds stay within limits (0 to 255)
  leftSpeed = constrain(leftSpeed, 0, 255);
  rightSpeed = constrain(rightSpeed, 0, 255);

  // Apply correction to motors
  analogWrite(motorLeftPWM, leftSpeed);
  analogWrite(motorRightPWM, rightSpeed);

  // Store error for use in the next iteration
  prevErrorYawLeft = errorYaw;
  prevErrorYawRight = errorYaw;
}

// Function to correct rotation using separate PID for right and left wheels
void correctRotationPID() {
  // Calculate time since last execution
  unsigned long currentTime = millis();
  dt = (currentTime - lastTime) / 1000.0;  // Convert to seconds
  lastTime = currentTime;

  // Calculate yaw error (difference between current and desired angle)
  float errorYaw = targetYaw - currentYaw;

  // PID for the left wheel
  float proportionalYawLeft = KpLeft * errorYaw;
  integralYawLeft += errorYaw * dt;
  float integralTermYawLeft = KiLeft * integralYawLeft;
  float derivativeYawLeft = (errorYaw - prevErrorYawLeft) / dt;
  float derivativeTermYawLeft = KdLeft * derivativeYawLeft;

  // PID for the right wheel
  float proportionalYawRight = KpRight * errorYaw;
  integralYawRight += errorYaw * dt;
  float integralTermYawRight = KiRight * integralYawRight;
  float derivativeYawRight = (errorYaw - prevErrorYawRight) / dt;
  float derivativeTermYawRight = KdRight * derivativeYawRight;

  // Calculate final corrections for each wheel
  float correctionYawLeft = proportionalYawLeft + integralTermYawLeft + derivativeTermYawLeft;
  float correctionYawRight = proportionalYawRight + integralTermYawRight + derivativeTermYawRight;

  // Limit corrections to avoid extreme values
  correctionYawLeft = constrain(correctionYawLeft, -30, 30);  // Extended correction for the left wheel
  correctionYawRight = constrain(correctionYawRight, -30, 30);  // Maintained correction for the right wheel

  // Apply corrections to the motors during rotation
  int leftSpeed = rotationSpeed - correctionYawLeft;   // Correct the left motor
  int rightSpeed = rotationSpeed + correctionYawRight; // Correct the right motor

  // Ensure speeds stay within limits (0 to 255)
  leftSpeed = constrain(leftSpeed, 0, 255);
  rightSpeed = constrain(rightSpeed, 0, 255);

  // Apply correction to motors
  analogWrite(motorLeftPWM, leftSpeed);
  analogWrite(motorRightPWM, rightSpeed);

  // Store error for use in the next iteration
  prevErrorYawLeft = errorYaw;
  prevErrorYawRight = errorYaw;
}
