Here is the translation of the comments in the code to English:

```c++
/*
 * Project: 2WD Robot Car Control with ESP32 and MPU6050
 * Control via gyroscope and accelerometer of the mobile phone (Dabble)
 */

#include <Wire.h>
#include <MPU6050.h>
#include <DabbleESP32.h>

// MPU6050 configuration and yaw variables
MPU6050 mpu;
int16_t ax, ay, az;
int16_t gx, gy, gz;
float currentYaw = 0;
float targetYaw = 0;

// Filter configuration
float alpha = 0.8;
float filteredYaw = 0;

// Motor pin configuration
const int motorRightPWM = 13;
const int rightMotorPin1 = 25;
const int rightMotorPin2 = 33;
const int motorLeftPWM = 32;
const int leftMotorPin1 = 26;
const int leftMotorPin2 = 27;

// Speed configuration
int baseMotorSpeedLeft = 50;
int baseMotorSpeedRight = 50;
int rotationSpeed = 30; // Variable to control rotation speed

// PID variables
float KpLeft = 3.0, KiLeft = 0.1, KdLeft = 1.5;
float KpRight = 3.0, KiRight = 0.1, KdRight = 1.5;
float prevErrorYawLeft = 0;
float integralYawLeft = 0;
float prevErrorYawRight = 0;
float integralYawRight = 0;
const float windupLimit = 50.0;

unsigned long lastTime = 0;
float dt = 0.1;

void setup() {
  Serial.begin(115200);
  Dabble.begin("ESP32-Robo");  // Initialize Dabble with the device name
  
  Wire.begin();
  mpu.initialize();
  
  if (!mpu.testConnection()) {
    Serial.println("Failed to connect to MPU6050!");
    while (1);
  } else {
    Serial.println("MPU6050 connected!");
  }

  pinMode(rightMotorPin1, OUTPUT);
  pinMode(rightMotorPin2, OUTPUT);
  pinMode(motorRightPWM, OUTPUT);
  
  pinMode(leftMotorPin1, OUTPUT);
  pinMode(leftMotorPin2, OUTPUT);
  pinMode(motorLeftPWM, OUTPUT);
}

void loop() {
  Dabble.processInput();  // Process Dabble data
  getSensorData();  // Function to collect sensor data

  // Movement command based on sensor data
  int tiltX = Sensor.getGyroscopeXaxis();  // Tilt on the X-axis
  int tiltY = Sensor.getGyroscopeYaxis();  // Tilt on the Y-axis

  if (tiltY > 10) {  // Tilt forward
    moveForward();
    targetYaw = currentYaw;  // Set targetYaw as currentYaw
    correctTrajectoryPID();
  } else if (tiltY < -10) {  // Tilt backward
    moveBackward();
    targetYaw = currentYaw;
    correctTrajectoryPID();
  } else if (tiltX > 10) {  // Tilt to the right
    rotateRight();
    targetYaw += 5;  // Adjust targetYaw to the right
    correctRotationPID();
  } else if (tiltX < -10) {  // Tilt to the left
    rotateLeft();
    targetYaw -= 5;  // Adjust targetYaw to the left
    correctRotationPID();
  } else {
    stopMotors();  // Stop motors if there is no tilt
  }
}

void getSensorData() {
  // Read data from MPU6050 and update yaw
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  
  // Update and filter yaw reading
  float rawYaw = (gz / 131.0) * dt; // Raw yaw based on gyroscope rate
  filteredYaw = alpha * filteredYaw + (1 - alpha) * rawYaw; // Low-pass filter
  currentYaw += filteredYaw;
  
  // Print accelerometer data using the Dabble library
  Serial.print("Accelerometer - X: "); Serial.print(Sensor.getAccelerometerXaxis());
  Serial.print(", Y: "); Serial.print(Sensor.getAccelerometerYaxis());
  Serial.print(", Z: "); Serial.println(Sensor.getAccelerometerZaxis());
}

void moveForward() {
  digitalWrite(leftMotorPin1, HIGH);
  digitalWrite(leftMotorPin2, LOW);
  analogWrite(motorLeftPWM, baseMotorSpeedLeft);

  digitalWrite(rightMotorPin1, HIGH);
  digitalWrite(rightMotorPin2, LOW);
  analogWrite(motorRightPWM, baseMotorSpeedRight);
}

void moveBackward() {
  digitalWrite(leftMotorPin1, LOW);
  digitalWrite(leftMotorPin2, HIGH);
  analogWrite(motorLeftPWM, baseMotorSpeedLeft);

  digitalWrite(rightMotorPin1, LOW);
  digitalWrite(rightMotorPin2, HIGH);
  analogWrite(motorRightPWM, baseMotorSpeedRight);
}

void stopMotors() {
  analogWrite(motorLeftPWM, 0);
  analogWrite(motorRightPWM, 0);
}

void rotateRight() {
  digitalWrite(leftMotorPin1, LOW);
  digitalWrite(leftMotorPin2, HIGH);
  analogWrite(motorLeftPWM, rotationSpeed);  // Use rotationSpeed for rotation

  digitalWrite(rightMotorPin1, HIGH);
  digitalWrite(rightMotorPin2, LOW);
  analogWrite(motorRightPWM, rotationSpeed);
}

void rotateLeft() {
  digitalWrite(leftMotorPin1, HIGH);
  digitalWrite(leftMotorPin2, LOW);
  analogWrite(motorLeftPWM, rotationSpeed);  // Use rotationSpeed for rotation

  digitalWrite(rightMotorPin1, LOW);
  digitalWrite(rightMotorPin2, HIGH);
  analogWrite(motorRightPWM, rotationSpeed);
}

// Function to limit the integral term of PID (windup)
void limitIntegral(float &integralYaw) {
  integralYaw = constrain(integralYaw, -windupLimit, windupLimit);
}

// Function to correct the trajectory with PID
void correctTrajectoryPID() {
  unsigned long currentTime = millis();
  dt = (currentTime - lastTime) / 1000.0;
  lastTime = currentTime;

  float errorYaw = targetYaw - currentYaw;

  float KpAdjustLeft = KpLeft * 0.5;
  float proportionalYawLeft = KpAdjustLeft * errorYaw;

  float KpAdjustRight = KpRight * 0.5;
  float proportionalYawRight = KpAdjustRight * errorYaw;

  integralYawLeft += errorYaw * dt;
  limitIntegral(integralYawLeft);
  float integralTermYawLeft = KiLeft * integralYawLeft;

  float derivativeYawLeft = (errorYaw - prevErrorYawLeft) / dt;
  float derivativeTermYawLeft = KdLeft * derivativeYawLeft;

  integralYawRight += errorYaw * dt;
  limitIntegral(integralYawRight);
  float integralTermYawRight = KiRight * integralYawRight;

  float derivativeYawRight = (errorYaw - prevErrorYawRight) / dt;
  float derivativeTermYawRight = KdRight * derivativeYawRight;

  float correctionYawLeft = proportionalYawLeft + integralTermYawLeft + derivativeTermYawLeft;
  float correctionYawRight = proportionalYawRight + integralTermYawRight + derivativeTermYawRight;

  int leftSpeed = baseMotorSpeedLeft - correctionYawLeft;
  int rightSpeed = baseMotorSpeedRight + correctionYawRight;

  leftSpeed = constrain(leftSpeed, 0, 100);
  rightSpeed = constrain(rightSpeed, 0, 100);

  analogWrite(motorLeftPWM, leftSpeed);
  analogWrite(motorRightPWM, rightSpeed);

  prevErrorYawLeft = errorYaw;
  prevErrorYawRight = errorYaw;
}

// Function to correct rotation with PID
void correctRotationPID() {
  unsigned long currentTime = millis();
  dt = (currentTime - lastTime) / 1000.0;
  lastTime = currentTime;

  float errorYaw = targetYaw - currentYaw;

  float proportionalYaw = KpRight * errorYaw;

  integralYawRight += errorYaw * dt;
  limitIntegral(integralYawRight);
  float integralTermYawRight = KiRight * integralYawRight;

  float derivativeYaw = (errorYaw - prevErrorYawRight) / dt;
  float derivativeTermYawRight = KdRight * derivativeYaw;

  float correctionYaw = proportionalYaw + integralTermYawRight + derivativeTermYawRight;

  int speedAdjustment = constrain(correctionYaw, -100, 100);

  if (speedAdjustment > 0) {
    // Adjust left motor speed
    analogWrite(motorLeftPWM, baseMotorSpeedLeft - speedAdjustment);
    // Right motor remains at base speed
    analogWrite(motorRightPWM, baseMotorSpeedRight);
  } else {
    // Left motor remains at base speed
    analogWrite(motorLeftPWM, baseMotorSpeedLeft);
    // Adjust right motor speed
    analogWrite(motorRightPWM, baseMotorSpeedRight + speedAdjustment);
  }

  prevErrorYawRight = errorYaw;
}
```

Let me know if you need any further assistance.
