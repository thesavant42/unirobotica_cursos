/*
 * Projeto: Controle de Carro Robô 2WD com ESP32 e MPU6050
 * Controle por giroscópio e acelerômetro do celular (Dabble)
 */

#include <Wire.h>
#include <MPU6050.h>
#include <DabbleESP32.h>

// Configuração do MPU6050 e variáveis de yaw
MPU6050 mpu;
int16_t ax, ay, az;
int16_t gx, gy, gz;
float currentYaw = 0;
float targetYaw = 0;

// Configuração do filtro
float alpha = 0.8;
float filteredYaw = 0;

// Configuração dos pinos do motor
const int motorRightPWM = 13;
const int rightMotorPin1 = 25;
const int rightMotorPin2 = 33;
const int motorLeftPWM = 32;
const int leftMotorPin1 = 26;
const int leftMotorPin2 = 27;

// Configuração da velocidade
int baseMotorSpeedLeft = 50;
int baseMotorSpeedRight = 50;
int rotationSpeed = 30; // Variável para controlar a velocidade de rotação

// Variáveis PID
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
  Dabble.begin("ESP32-Robo");  // Inicializa o Dabble com o nome do dispositivo
  
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
  Dabble.processInput();  // Processa dados do Dabble
  getSensorData();  // Função para coletar dados do sensor

  // Comando de movimento baseado nos dados do sensor
  int tiltX = Sensor.getGyroscopeXaxis();  // Inclinação no eixo X
  int tiltY = Sensor.getGyroscopeYaxis();  // Inclinação no eixo Y

  if (tiltY > 10) {  // Inclinação para frente
    moveForward();
    targetYaw = currentYaw;  // Define o targetYaw como o currentYaw
    correctTrajectoryPID();
  } else if (tiltY < -10) {  // Inclinação para trás
    moveBackward();
    targetYaw = currentYaw;
    correctTrajectoryPID();
  } else if (tiltX > 10) {  // Inclinação para a direita
    rotateRight();
    targetYaw += 5;  // Ajusta o targetYaw para a direita
    correctRotationPID();
  } else if (tiltX < -10) {  // Inclinação para a esquerda
    rotateLeft();
    targetYaw -= 5;  // Ajusta o targetYaw para a esquerda
    correctRotationPID();
  } else {
    stopMotors();  // Para os motores se não houver inclinação
  }
}

void getSensorData() {
  // Lê dados do MPU6050 e atualiza o yaw
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  
  // Atualiza e filtra a leitura de yaw
  float rawYaw = (gz / 131.0) * dt; // Raw yaw baseado na taxa de giroscópio
  filteredYaw = alpha * filteredYaw + (1 - alpha) * rawYaw; // Filtro passa-baixa
  currentYaw += filteredYaw;
  
  // Impressão dos dados do acelerômetro usando a biblioteca Dabble
  Serial.print("Acelerômetro - X: "); Serial.print(Sensor.getAccelerometerXaxis());
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
  analogWrite(motorLeftPWM, rotationSpeed);  // Usa rotationSpeed para a rotação

  digitalWrite(rightMotorPin1, HIGH);
  digitalWrite(rightMotorPin2, LOW);
  analogWrite(motorRightPWM, rotationSpeed);
}

void rotateLeft() {
  digitalWrite(leftMotorPin1, HIGH);
  digitalWrite(leftMotorPin2, LOW);
  analogWrite(motorLeftPWM, rotationSpeed);  // Usa rotationSpeed para a rotação

  digitalWrite(rightMotorPin1, LOW);
  digitalWrite(rightMotorPin2, HIGH);
  analogWrite(motorRightPWM, rotationSpeed);
}

// Função para limitar o termo integral do PID (windup)
void limitIntegral(float &integralYaw) {
  integralYaw = constrain(integralYaw, -windupLimit, windupLimit);
}

// Função para corrigir a trajetória com PID
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

// Função para corrigir a rotação com PID
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
    // Ajusta a velocidade do motor esquerdo
    analogWrite(motorLeftPWM, baseMotorSpeedLeft - speedAdjustment);
    // Motor direito permanece com a velocidade base
    analogWrite(motorRightPWM, baseMotorSpeedRight);
  } else {
    // Motor esquerdo permanece com a velocidade base
    analogWrite(motorLeftPWM, baseMotorSpeedLeft);
    // Ajusta a velocidade do motor direito
    analogWrite(motorRightPWM, baseMotorSpeedRight + speedAdjustment);
  }

  prevErrorYawRight = errorYaw;
}
