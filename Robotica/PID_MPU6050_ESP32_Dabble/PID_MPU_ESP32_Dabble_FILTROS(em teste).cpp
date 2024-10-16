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
 * Versão: 1.1
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

// Defina os pinos do TB6612 para os motores
// Motor direito
const int motorRightPWM = 13;  
const int rightMotorPin1 = 25;
const int rightMotorPin2 = 33;

// Motor esquerdo
const int motorLeftPWM = 32;
const int leftMotorPin1 = 26;
const int leftMotorPin2 = 27;

// Velocidades base ajustadas para as rodas
int baseMotorSpeedLeft = 50;  // Velocidade base inicial para a roda esquerda
int baseMotorSpeedRight = 50;  // Velocidade base inicial para a roda direita
int rotationSpeed = 50;        // Velocidade reduzida para rotação

// Constantes PID ajustadas para a roda esquerda
float KpLeft = 3.0, KiLeft = 0, KdLeft = 1.5;  // Constantes ajustadas para melhorar a roda esquerda
float prevErrorYawLeft = 0;
float integralYawLeft = 0;

// Constantes PID ajustadas para a roda direita
float KpRight = 3.0, KiRight = 0, KdRight = 1.5;  // Constantes ajustadas para a roda direita
float prevErrorYawRight = 0;
float integralYawRight = 0;

// Tempo de amostragem para o controle PID
unsigned long lastTime = 0;
float dt = 0.1;  // Intervalo de tempo em segundos (100ms)

// Filtro passa-baixa de primeira ordem
float alpha = 0.9; // Fator de suavização
float filteredError = 0;

// Anti-windup
float integralLimit = 10.0; // Limite máximo para o integral

void setup() {
  // Inicializa o Serial para debug
  Serial.begin(115200);

  // Inicializa o Dabble
  Dabble.begin("ESP32-Robo");  // Nome do robô no Bluetooth visível pelo app
  Serial.println("Dabble Bluetooth iniciado!");

  // Inicializa o MPU6050
  Wire.begin();
  mpu.initialize();
  
  if (!mpu.testConnection()) {
    Serial.println("Falha ao conectar ao MPU6050!");
    while (1);
  } else {
    Serial.println("MPU6050 conectado!");
  }

  // Inicializa os pinos do motor como saída
  pinMode(rightMotorPin1, OUTPUT);
  pinMode(rightMotorPin2, OUTPUT);
  pinMode(motorRightPWM, OUTPUT);
  
  pinMode(leftMotorPin1, OUTPUT);
  pinMode(leftMotorPin2, OUTPUT);
  pinMode(motorLeftPWM, OUTPUT);
}

void loop() {
  Dabble.processInput();  // Processa os comandos do Dabble

  // Lê os valores de aceleração e giroscópio do MPU6050
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Atualiza a leitura atual de yaw (rotação usando o giroscópio)
  currentYaw += (gz / 131.0) * dt;  // 131 é a sensibilidade padrão do giroscópio para grau/segundo

  // Movimentação baseada no controle via Dabble
  if (GamePad.isUpPressed()) {
    moveForward();
    targetYaw = currentYaw;  // Define o yaw de destino durante o movimento
    correctTrajectoryPID();  // Corrige a trajetória durante a movimentação
  } else if (GamePad.isDownPressed()) {
    moveBackward();
    targetYaw = currentYaw;  // Define o yaw de destino durante o movimento
    correctTrajectoryPID();  // Corrige a trajetória durante a movimentação
  } else if (GamePad.isLeftPressed()) {
     rotateLeft();
     targetYaw -= 5;  // Define um valor de rotação desejada ao girar para a esquerda
     correctRotationPID();  // Corrige a rotação usando PID para cada roda
  } else if (GamePad.isRightPressed()) {
    rotateRight();
    targetYaw += 5;  // Define um valor de rotação desejada ao girar para a direita
    correctRotationPID();  // Corrige a rotação usando PID para cada roda
  } else {
    stopMotors();
  }
}

// Movimentação para frente
void moveForward() {
  // Motores girando para frente
  digitalWrite(leftMotorPin1, HIGH);
  digitalWrite(leftMotorPin2, LOW);
  analogWrite(motorLeftPWM, baseMotorSpeedLeft);

  digitalWrite(rightMotorPin1, HIGH);
  digitalWrite(rightMotorPin2, LOW);
  analogWrite(motorRightPWM, baseMotorSpeedRight);
}

// Movimentação para trás
void moveBackward() {
  // Motores girando para trás
  digitalWrite(leftMotorPin1, LOW);
  digitalWrite(leftMotorPin2, HIGH);
  analogWrite(motorLeftPWM, baseMotorSpeedLeft);

  digitalWrite(rightMotorPin1, LOW);
  digitalWrite(rightMotorPin2, HIGH);
  analogWrite(motorRightPWM, baseMotorSpeedRight);
}

// Função para parar os motores
void stopMotors() {
  analogWrite(motorLeftPWM, 0);
  analogWrite(motorRightPWM, 0);
}

// Rotação incremental para a direita
void rotateRight() {
  digitalWrite(leftMotorPin1, LOW);  // Esquerdo vai para trás
  digitalWrite(leftMotorPin2, HIGH);
  analogWrite(motorLeftPWM, rotationSpeed);

  digitalWrite(rightMotorPin1, HIGH);  // Direito vai para frente
  digitalWrite(rightMotorPin2, LOW);
  analogWrite(motorRightPWM, rotationSpeed);
}

// Rotação incremental para a esquerda
void rotateLeft() {
  digitalWrite(leftMotorPin1, HIGH);  // Esquerdo vai para frente
  digitalWrite(leftMotorPin2, LOW);
  analogWrite(motorLeftPWM, rotationSpeed);

  digitalWrite(rightMotorPin1, LOW);  // Direito vai para trás
  digitalWrite(rightMotorPin2, HIGH);
  analogWrite(motorRightPWM, rotationSpeed);
}

// Função para corrigir a trajetória usando PID com os dados do MPU6050
void correctTrajectoryPID() {
  // Calcula o tempo desde a última execução
  unsigned long currentTime = millis();
  dt = (currentTime - lastTime) / 1000.0;  // Converte para segundos
  lastTime = currentTime;

  // Calcular erro de yaw (rotação)
  float errorYaw = targetYaw - currentYaw;

  // Filtro passa-baixa para suavizar o erro
  filteredError = alpha * filteredError + (1 - alpha) * errorYaw;

  // Termo Proporcional para a roda esquerda
  float proportionalYawLeft = KpLeft * filteredError;

  // Termo Integral para a roda esquerda
  integralYawLeft += filteredError * dt;
  integralYawLeft = constrain(integralYawLeft, -integralLimit, integralLimit); // Anti-windup
  float integralTermYawLeft = KiLeft * integralYawLeft;

  // Termo Derivativo para a roda esquerda
  float derivativeYawLeft = (filteredError - prevErrorYawLeft) / dt;
  float derivativeTermYawLeft = KdLeft * derivativeYawLeft;

  // Termo Proporcional para a roda direita
  float proportionalYawRight = KpRight * filteredError;

  // Termo Integral para a roda direita
  integralYawRight += filteredError * dt;
  integralYawRight = constrain(integralYawRight, -integralLimit, integralLimit); // Anti-windup
  float integralTermYawRight = KiRight * integralYawRight;

  // Termo Derivativo para a roda direita
  float derivativeYawRight = (filteredError - prevErrorYawRight) / dt;
  float derivativeTermYawRight = KdRight * derivativeYawRight;

  // Atualiza a velocidade dos motores com a correção do PID
  int correctedSpeedLeft = baseMotorSpeedLeft + proportionalYawLeft + integralTermYawLeft + derivativeTermYawLeft;
  int correctedSpeedRight = baseMotorSpeedRight - proportionalYawRight - integralTermYawRight - derivativeTermYawRight;  // Inverte para corrigir o yaw

  // Aplica a correção à velocidade dos motores
  analogWrite(motorLeftPWM, constrain(correctedSpeedLeft, 0, 255));
  analogWrite(motorRightPWM, constrain(correctedSpeedRight, 0, 255));

  // Atualiza o erro anterior para o próximo loop
  prevErrorYawLeft = filteredError;
  prevErrorYawRight = filteredError;
}

// Função para corrigir a rotação usando PID para cada roda
void correctRotationPID() {
  // Calcula o tempo desde a última execução
  unsigned long currentTime = millis();
  dt = (currentTime - lastTime) / 1000.0;  // Converte para segundos
  lastTime = currentTime;

  // Calcula o erro de yaw (rotação) durante a rotação
  float errorYaw = targetYaw - currentYaw;

  // Filtro passa-baixa para suavizar o erro
  filteredError = alpha * filteredError + (1 - alpha) * errorYaw;

  // Termo Proporcional para a roda esquerda
  float proportionalYawLeft = KpLeft * filteredError;

  // Termo Integral para a roda esquerda
  integralYawLeft += filteredError * dt;
  integralYawLeft = constrain(integralYawLeft, -integralLimit, integralLimit); // Anti-windup
  float integralTermYawLeft = KiLeft * integralYawLeft;

  // Termo Derivativo para a roda esquerda
  float derivativeYawLeft = (filteredError - prevErrorYawLeft) / dt;
  float derivativeTermYawLeft = KdLeft * derivativeYawLeft;

  // Termo Proporcional para a roda direita
  float proportionalYawRight = KpRight * filteredError;

  // Termo Integral para a roda direita
  integralYawRight += filteredError * dt;
  integralYawRight = constrain(integralYawRight, -integralLimit, integralLimit); // Anti-windup
  float integralTermYawRight = KiRight * integralYawRight;

  // Termo Derivativo para a roda direita
  float derivativeYawRight = (filteredError - prevErrorYawRight) / dt;
  float derivativeTermYawRight = KdRight * derivativeYawRight;

  // Atualiza a velocidade dos motores com a correção do PID
  int correctedSpeedLeft = baseMotorSpeedLeft + proportionalYawLeft + integralTermYawLeft + derivativeTermYawLeft;
  int correctedSpeedRight = baseMotorSpeedRight - proportionalYawRight - integralTermYawRight - derivativeTermYawRight;  // Inverte para corrigir o yaw

  // Aplica a correção à velocidade dos motores
  analogWrite(motorLeftPWM, constrain(correctedSpeedLeft, 0, 255));
  analogWrite(motorRightPWM, constrain(correctedSpeedRight, 0, 255));

  // Atualiza o erro anterior para o próximo loop
  prevErrorYawLeft = filteredError;
  prevErrorYawRight = filteredError;
}
