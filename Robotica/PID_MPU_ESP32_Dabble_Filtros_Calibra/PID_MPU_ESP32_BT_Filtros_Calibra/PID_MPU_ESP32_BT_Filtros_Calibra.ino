/*
 * Projeto: Controle de Carro Robô 2WD com ESP32 e MPU6050
 * 
 * Descrição:
 * Este código controla um carro robô de duas rodas (2WD) utilizando um microcontrolador ESP32
 * e um sensor MPU6050. O robô pode ser comandado através do aplicativo Dabble via Bluetooth.
 * O controle de trajetória é realizado utilizando um algoritmo PID para corrigir a direção.
 * 
 * Versão: 1.5
 * Autor: Prof. Dr. Robson Marinho
 * Data: 29/10/2024
 */

#include <Wire.h>
#include <MPU6050.h>
#include <DabbleESP32.h>

// Configurações do MPU6050
MPU6050 mpu;
int16_t ax, ay, az;
int16_t gx, gy, gz;
float currentYaw = 0;
float targetYaw = 0;
float yawOffset = 0;

// Filtro passa-baixa
float alpha = 0.8;
float filteredYaw = 0;

// Pinos do TB6612 para os motores (ajustados)
const int motorRightPWM = 25;  // PWM do motor direito
const int rightMotorPin1 = 33; // Direção do motor direito
const int rightMotorPin2 = 26;
const int motorLeftPWM = 32;   // PWM do motor esquerdo
const int leftMotorPin1 = 27;  // Direção do motor esquerdo
const int leftMotorPin2 = 14;  // Novo pino para direção

// Velocidades base ajustadas para as rodas
int baseMotorSpeedLeft = 65;  
int baseMotorSpeedRight = 60;
int rotationSpeedLeft = 55;  
int rotationSpeedRight = 65;

// Constantes PID ajustadas para as rodas
float KpLeft = 1.2, KiLeft = 0, KdLeft = 0.05;
float prevErrorYawLeft = 0;
float integralYawLeft = 0;
float KpRight = 1.0, KiRight = 0, KdRight = 0.05;
float prevErrorYawRight = 0;
float integralYawRight = 0;

// Limites para windup
const float windupLimit = 120.0;

unsigned long lastTime = 0;
float dt = 0.1;

void setup() {
  Serial.begin(115200);
  Dabble.begin("ESP32-Sonic");
  Wire.begin();
  mpu.initialize();

  if (!mpu.testConnection()) {
    Serial.println("Falha ao conectar ao MPU6050!");
    while (1);
  } else {
    Serial.println("MPU6050 conectado!");
  }

  for (int i = 0; i < 100; i++) {
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    yawOffset += (gz / 131.0);
    delay(10);
  }
  yawOffset /= 100;

  pinMode(rightMotorPin1, OUTPUT);
  pinMode(rightMotorPin2, OUTPUT);
  pinMode(motorRightPWM, OUTPUT);
  pinMode(leftMotorPin1, OUTPUT);
  pinMode(leftMotorPin2, OUTPUT);
  pinMode(motorLeftPWM, OUTPUT);
}

void loop() {
  Dabble.processInput();
  
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  float rawYaw = (gz / 131.0) * dt - yawOffset;
  filteredYaw = alpha * filteredYaw + (1 - alpha) * rawYaw;
  currentYaw += filteredYaw;

  if (GamePad.isUpPressed()) {
    moveForward();
    targetYaw = currentYaw;
    correctTrajectoryPID();
  } else if (GamePad.isDownPressed()) {
    moveBackward();
    targetYaw = currentYaw;
    correctTrajectoryPID();
  } else if (GamePad.isLeftPressed()) {
    rotateLeft();
    targetYaw = currentYaw;
    correctRotationPID();
  } else if (GamePad.isRightPressed()) {
    rotateRight();
    targetYaw = currentYaw;
    correctRotationPID();
  } else {
    stopMotors();
  }
}

void moveForward() {
  digitalWrite(leftMotorPin1, HIGH);
  digitalWrite(leftMotorPin2, LOW);
  analogWrite(motorLeftPWM, baseMotorSpeedLeft + 5);  // Compensação para tendência à direita

  digitalWrite(rightMotorPin1, HIGH);
  digitalWrite(rightMotorPin2, LOW);
  analogWrite(motorRightPWM, baseMotorSpeedRight);
}

void moveBackward() {
  digitalWrite(leftMotorPin1, LOW);
  digitalWrite(leftMotorPin2, HIGH);
  analogWrite(motorLeftPWM, baseMotorSpeedLeft + 5);  // Compensação para tendência anti-horária

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
  analogWrite(motorLeftPWM, rotationSpeedLeft);

  digitalWrite(rightMotorPin1, HIGH);
  digitalWrite(rightMotorPin2, LOW);
  analogWrite(motorRightPWM, rotationSpeedRight);
}

void rotateLeft() {
  digitalWrite(leftMotorPin1, HIGH);
  digitalWrite(leftMotorPin2, LOW);
  analogWrite(motorLeftPWM, rotationSpeedLeft);

  digitalWrite(rightMotorPin1, LOW);
  digitalWrite(rightMotorPin2, HIGH);
  analogWrite(motorRightPWM, rotationSpeedRight);
}

void limitIntegral(float &integralYaw) {
  integralYaw = constrain(integralYaw, -windupLimit, windupLimit);
}

void correctTrajectoryPID() {
  unsigned long currentTime = millis();
  dt = (currentTime - lastTime) / 1000.0;
  lastTime = currentTime;

  float errorYaw = targetYaw - currentYaw;

  float KpAdjustLeft = KpLeft * 0.7;  
  float KpAdjustRight = KpRight * 0.7;

  float proportionalYawLeft = KpAdjustLeft * errorYaw;
  integralYawLeft += errorYaw * dt;
  limitIntegral(integralYawLeft);
  float integralTermYawLeft = KiLeft * integralYawLeft;
  float derivativeYawLeft = (errorYaw - prevErrorYawLeft) / dt;
  float derivativeTermYawLeft = KdLeft * derivativeYawLeft;

  float proportionalYawRight = KpAdjustRight * errorYaw;
  integralYawRight += errorYaw * dt;
  limitIntegral(integralYawRight);
  float integralTermYawRight = KiRight * integralYawRight;
  float derivativeYawRight = (errorYaw - prevErrorYawRight) / dt;
  float derivativeTermYawRight = KdRight * derivativeYawRight;

  float correctionYawLeft = proportionalYawLeft + integralTermYawLeft + derivativeTermYawLeft;
  float correctionYawRight = proportionalYawRight + integralTermYawRight + derivativeTermYawRight;

  int leftSpeed = baseMotorSpeedLeft + correctionYawLeft;
  int rightSpeed = baseMotorSpeedRight - correctionYawRight;

  leftSpeed = constrain(leftSpeed, -windupLimit, windupLimit);
  rightSpeed = constrain(rightSpeed, -windupLimit, windupLimit);

  analogWrite(motorLeftPWM, leftSpeed);
  analogWrite(motorRightPWM, rightSpeed);

  prevErrorYawLeft = errorYaw;
  prevErrorYawRight = errorYaw;
}

void correctRotationPID() {
  unsigned long currentTime = millis();
  dt = (currentTime - lastTime) / 1000.0;
  lastTime = currentTime;

  float errorYaw = targetYaw - currentYaw;

  float proportionalYawLeft = KpLeft * errorYaw;
  integralYawLeft += errorYaw * dt;
  float integralTermYawLeft = KiLeft * integralYawLeft;
  float derivativeYawLeft = (errorYaw - prevErrorYawLeft) / dt;
  float derivativeTermYawLeft = KdLeft * derivativeYawLeft;

  float proportionalYawRight = KpRight * errorYaw;
  integralYawRight += errorYaw * dt;
  float integralTermYawRight = KiRight * integralYawRight;
  float derivativeYawRight = (errorYaw - prevErrorYawRight) / dt;
  float derivativeTermYawRight = KdRight * derivativeYawRight;

  float correctionYawLeft = proportionalYawLeft + integralTermYawLeft + derivativeTermYawLeft;
  float correctionYawRight = proportionalYawRight + integralTermYawRight + derivativeTermYawRight;

  correctionYawLeft = constrain(correctionYawLeft, -windupLimit, windupLimit);
  correctionYawRight = constrain(correctionYawRight, -windupLimit, windupLimit);

  analogWrite(motorLeftPWM, correctionYawLeft);
  analogWrite(motorRightPWM, correctionYawRight);

  prevErrorYawLeft = errorYaw;
  prevErrorYawRight = errorYaw;
}
