/*

// Informações do template Blynk
#define BLYNK_TEMPLATE_NAME "Remote Car 2WD Esp32"
#define BLYNK_TEMPLATE_ID "xxxxxxxxxx"
#define BLYNK_AUTH_TOKEN "xxxxxxxxxx"

#include <WiFi.h>
#include <Wire.h>
#include <MPU6050.h>
#include <BlynkSimpleEsp32.h>

// Credenciais do Wi-Fi
char auth[] = BLYNK_AUTH_TOKEN; // Insira seu Token de Autenticação Blynk
char ssid[] = "Robson5G";         // Insira o nome da sua rede Wi-Fi
char pass[] = "12345678";      // Insira a senha da sua rede Wi-Fi

*/
/*
 * Projeto: Controle de Carro Robô 2WD com ESP32 e MPU6050
 * 
 * Descrição:
 * Este código controla um carro robô de duas rodas (2WD) utilizando um microcontrolador ESP32
 * e um sensor MPU6050. O robô pode ser comandado através do aplicativo Blynk via Wi-Fi.
 * O controle de trajetória é realizado utilizando um algoritmo PID para corrigir a direção.
 * 
 * Versão: 1.6
 * Autor: Prof. Dr. Robson Marinho
 * Data: 30/10/2024
 */

/*
 * Projeto: Controle de Carro Robô 2WD com ESP32 e MPU6050
 * 
 * Descrição:
 * Este código controla um carro robô de duas rodas (2WD) utilizando um microcontrolador ESP32
 * e um sensor MPU6050. O robô pode ser comandado através do aplicativo Blynk via Wi-Fi.
 * O controle de trajetória é realizado utilizando um algoritmo PID para corrigir a direção.
 * 
 * Versão: 1.6
 * Autor: Prof. Dr. Robson Marinho
 * Data: 30/10/2024
 */

// Informações do template Blynk
#define BLYNK_TEMPLATE_NAME "Remote Car 2WD Esp32"
#define BLYNK_TEMPLATE_ID "xxxxxxxxxxxxxxxx"
#define BLYNK_AUTH_TOKEN "xxxxxxxxxxxxxxxx"
 // Token de Autenticação Blynk 

#include <WiFi.h>
#include <Wire.h>
#include <MPU6050.h>
#include <BlynkSimpleEsp32.h>

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

// Pinos do TB6612 para os motores
const int motorRightPWM = 13;
const int rightMotorPin1 = 25;
const int rightMotorPin2 = 33;
const int motorLeftPWM = 32;
const int leftMotorPin1 = 26;
const int leftMotorPin2 = 27;

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

// Credenciais do Wi-Fi
char auth[] = BLYNK_AUTH_TOKEN; // Insira seu Token de Autenticação Blynk
char ssid[] = "Robson5G";         // Insira o nome da sua rede Wi-Fi
char pass[] = "12345678";         // Insira a senha da sua rede Wi-Fi

void setup() {
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass); // Insira seu SSID e senha
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
  Blynk.run();
  
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  float rawYaw = (gz / 131.0) * dt - yawOffset;
  filteredYaw = alpha * filteredYaw + (1 - alpha) * rawYaw;
  currentYaw += filteredYaw;

  // Correção da trajetória PID
  targetYaw = currentYaw;
  correctTrajectoryPID();
}

// Funções para controle do robô usando Blynk
BLYNK_WRITE(V0) { // Botão "Avançar" - V0
  if (param.asInt() == 1) {
    moveForward();
  } else {
    stopMotors();
  }
}

BLYNK_WRITE(V1) { // Botão "Recuar" - V1
  if (param.asInt() == 1) {
    moveBackward();
  } else {
    stopMotors();
  }
}

BLYNK_WRITE(V2) { // Botão "Esquerda" - V2
  if (param.asInt() == 1) {
    rotateLeft();
  } else {
    stopMotors();
  }
}

BLYNK_WRITE(V3) { // Botão "Direita" - V3
  if (param.asInt() == 1) {
    rotateRight();
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
  digitalWrite(leftMotorPin1, LOW);
  digitalWrite(leftMotorPin2, LOW);
  analogWrite(motorLeftPWM, 0);
  
  digitalWrite(rightMotorPin1, LOW);
  digitalWrite(rightMotorPin2, LOW);
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
  
  // Adicione o código de correção PID aqui, se necessário
}
