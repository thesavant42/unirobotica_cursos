/*
 * Projeto: Controle de Carro Robô 2WD com ESP32 e MPU6050
 * 
 * Descrição:
 * Este código controla um carro robô de duas rodas (2WD) utilizando um microcontrolador ESP32
 * e um sensor MPU6050. O robô pode ser comandado através do aplicativo Dabble via Bluetooth.
 * O objetivo é permitir que o robô siga uma trajetória reta ou faça rotações incrementais 
 * conforme os comandos recebidos. O controle de trajetória é realizado utilizando um algoritmo 
 * PID (Proporcional, Integral, Derivativo) que corrige a direção com base nos dados do giroscópio 
 * do MPU6050, garantindo que o robô mantenha seu curso, mesmo quando há escorregamento.
 * 
 * Componentes Necessários:
 * - ESP32
 * - MPU6050
 * - Driver de Motor TB6612
 * - Motores DC (2 unidades)
 * 
 * Funcionamento:
 * - O robô avança e recua conforme os comandos "para frente" e "para trás" do aplicativo.
 * - O robô rotaciona à esquerda e à direita com base nos comandos correspondentes.
 * - O PID é utilizado para corrigir a trajetória em tempo real, ajustando a velocidade de cada roda para manter uma trajetória reta.
 * 
 * Versão: 1.2
 * Autor: Prof. Dr. Robson Marinho
 * Data: 16/10/2024
 */


#include <Wire.h>
#include <MPU6050.h>
#include <DabbleESP32.h>

// Configurações do MPU6050
MPU6050 mpu;
int16_t ax, ay, az; // Aceleração
int16_t gx, gy, gz; // Giroscópio
float currentYaw = 0;
float targetYaw = 0;
bool rotating = false;

// Filtro passa-baixa para suavizar a leitura do giroscópio
float alpha = 0.8;  // Constante do filtro passa-baixa
float filteredYaw = 0;

// Defina os pinos do TB6612 para os motores
const int motorRightPWM = 13;  
const int rightMotorPin1 = 25;
const int rightMotorPin2 = 33;

const int motorLeftPWM = 32;
const int leftMotorPin1 = 26;
const int leftMotorPin2 = 27;

// Velocidades base ajustadas para as rodas
int baseMotorSpeedLeft = 50;  
int baseMotorSpeedRight = 50;  
int rotationSpeed = 50;        

// Constantes PID ajustadas para a roda esquerda
float KpLeft = 3.0, KiLeft = 0.1, KdLeft = 1.5;
float prevErrorYawLeft = 0;
float integralYawLeft = 0;

// Constantes PID ajustadas para a roda direita
float KpRight = 3.0, KiRight = 0.1, KdRight = 1.5;
float prevErrorYawRight = 0;
float integralYawRight = 0;

// Limites para o windup do PID (para evitar saturação do termo integral)
const float windupLimit = 100.0;  // Ajuste conforme necessário

unsigned long lastTime = 0;
float dt = 0.1;

void setup() {
  Serial.begin(115200);

  Dabble.begin("ESP32-Robo");  
  Wire.begin();
  mpu.initialize();
  
  if (!mpu.testConnection()) {
    Serial.println("Falha ao conectar ao MPU6050!");
    while (1);
  } else {
    Serial.println("MPU6050 conectado!");
  }

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

  // Atualiza e filtra a leitura de yaw
  float rawYaw = (gz / 131.0) * dt;
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
    targetYaw -= 5;  
    correctRotationPID();  
  } else if (GamePad.isRightPressed()) {
    rotateRight();
    targetYaw += 5;  
    correctRotationPID();  
  } else {
    stopMotors();
  }
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
  analogWrite(motorLeftPWM, rotationSpeed);

  digitalWrite(rightMotorPin1, HIGH);  
  digitalWrite(rightMotorPin2, LOW);
  analogWrite(motorRightPWM, rotationSpeed);
}

void rotateLeft() {
  digitalWrite(leftMotorPin1, HIGH);  
  digitalWrite(leftMotorPin2, LOW);
  analogWrite(motorLeftPWM, rotationSpeed);

  digitalWrite(rightMotorPin1, LOW);  
  digitalWrite(rightMotorPin2, HIGH);
  analogWrite(motorRightPWM, rotationSpeed);
}

// Função para corrigir a trajetória usando PID com filtro passa-baixa e windup
void correctTrajectoryPID() {
  unsigned long currentTime = millis();
  dt = (currentTime - lastTime) / 1000.0;
  lastTime = currentTime;

  float errorYaw = targetYaw - currentYaw;

  float proportionalYawLeft = KpLeft * errorYaw;

  integralYawLeft += errorYaw * dt;
  integralYawLeft = constrain(integralYawLeft, -windupLimit, windupLimit);  // Controle de windup
  float integralTermYawLeft = KiLeft * integralYawLeft;

  float derivativeYawLeft = (errorYaw - prevErrorYawLeft) / dt;
  float derivativeTermYawLeft = KdLeft * derivativeYawLeft;

  float proportionalYawRight = KpRight * errorYaw;

  integralYawRight += errorYaw * dt;
  integralYawRight = constrain(integralYawRight, -windupLimit, windupLimit);  // Controle de windup
  float integralTermYawRight = KiRight * integralYawRight;

  float derivativeYawRight = (errorYaw - prevErrorYawRight) / dt;
  float derivativeTermYawRight = KdRight * derivativeYawRight;

  float correctionYawLeft = proportionalYawLeft + integralTermYawLeft + derivativeTermYawLeft;
  float correctionYawRight = proportionalYawRight + integralTermYawRight + derivativeTermYawRight;

  correctionYawLeft = constrain(correctionYawLeft, -30, 30);
  correctionYawRight = constrain(correctionYawRight, -30, 30);

  int leftSpeed = baseMotorSpeedLeft - correctionYawLeft;
  int rightSpeed = baseMotorSpeedRight + correctionYawRight;

  leftSpeed = constrain(leftSpeed, 0, 255);
  rightSpeed = constrain(rightSpeed, 0, 255);

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
  integralYawLeft = constrain(integralYawLeft, -windupLimit, windupLimit);
  float integralTermYawLeft = KiLeft * integralYawLeft;
  float derivativeYawLeft = (errorYaw - prevErrorYawLeft) / dt;
  float derivativeTermYawLeft = KdLeft * derivativeYawLeft;

  float proportionalYawRight = KpRight * errorYaw;
  integralYawRight += errorYaw * dt;
  integralYawRight = constrain(integralYawRight, -windupLimit, windupLimit);
  float integralTermYawRight = KiRight * integralYawRight;
  float derivativeYawRight = (errorYaw - prevErrorYawRight) / dt;
  float derivativeTermYawRight = KdRight * derivativeYawRight;

  float correctionYawLeft = proportionalYawLeft + integralTermYawLeft + derivativeTermYawLeft;
  float correctionYawRight = proportionalYawRight + integralTermYawRight + derivativeTermYawRight;

  correctionYawLeft = constrain(correctionYawLeft, -30, 30);
  correctionYawRight = constrain(correctionYawRight, -30, 30);

  int leftSpeed = rotationSpeed - correctionYawLeft;
  int rightSpeed = rotationSpeed + correctionYawRight;

  leftSpeed = constrain(leftSpeed, 0, 255);
  rightSpeed = constrain(rightSpeed, 0, 255);

  analogWrite(motorLeftPWM, leftSpeed);
  analogWrite(motorRightPWM, rightSpeed);

  prevErrorYawLeft = errorYaw;
  prevErrorYawRight = errorYaw;
}
