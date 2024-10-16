
# Carro Robô 2WD com ESP32 e MPU6050

## Descrição

Este projeto utiliza um microcontrolador ESP32 e um sensor MPU6050 para controlar um carro robô de duas rodas (2WD) através do aplicativo Dabble via Bluetooth. O robô é capaz de seguir uma trajetória reta e fazer rotações incrementais, corrigindo sua direção em tempo real usando um algoritmo PID (Proporcional, Integral, Derivativo).

## Componentes Necessários

- **Hardware:**
  - 1 x ESP32
  - 1 x MPU6050
  - 1 x Driver de Motor TB6612
  - 2 x Motores DC
  - 1 x Chassi para o robô
  - Fios de conexão

- **Software:**
  - Arduino IDE
  - Biblioteca MPU6050 (disponível no gerenciador de bibliotecas do Arduino)
  - Biblioteca Dabble (disponível no gerenciador de bibliotecas do Arduino)

## Instalação

### 1. Configurar o Ambiente de Desenvolvimento

1. Baixe e instale a [Arduino IDE](https://www.arduino.cc/en/software).
2. Abra a IDE e vá em **File** > **Preferences**.
3. No campo "Additional Board Manager URLs", adicione: 
   ```
   https://dl.espressif.com/dl/package_esp32_index.json
   ```
4. Vá em **Tools** > **Board** > **Boards Manager** e busque por "ESP32". Instale a plataforma ESP32.

### 2. Instalar Bibliotecas Necessárias

1. Abra a IDE e vá em **Sketch** > **Include Library** > **Manage Libraries**.
2. Busque e instale as seguintes bibliotecas:
   - **MPU6050** (procure por "MPU6050 by Electronic Cats").
   - **Dabble** (procure por "Dabble ESP32").

### 3. Montagem do Circuito

Conecte os componentes conforme o seguinte esquema:

- **MPU6050:**
  - VCC -> 3.3V do ESP32
  - GND -> GND do ESP32
  - SDA -> GPIO 21 (ou o pino SDA do ESP32)
  - SCL -> GPIO 22 (ou o pino SCL do ESP32)

- **Driver de Motor TB6612:**
  - Pinos de controle conectados aos pinos definidos no código:
    - Motor Direito: PWM -> GPIO 13, IN1 -> GPIO 25, IN2 -> GPIO 33
    - Motor Esquerdo: PWM -> GPIO 32, IN1 -> GPIO 26, IN2 -> GPIO 27
  - Alimente o driver com a tensão apropriada para os motores.

### 4. Carregar o Código

1. Copie o código fornecido (veja abaixo) e cole na Arduino IDE.
2. Verifique se a placa ESP32 está selecionada em **Tools** > **Board**.
3. Conecte o ESP32 ao computador via USB.
4. Selecione a porta correta em **Tools** > **Port**.
5. Clique em **Upload** para carregar o código no ESP32.

## Código

```cpp
/*
 * Controle de Carro Robô 2WD com ESP32 e MPU6050
 * 
 * Descrição:
 * Este código controla um carro robô de duas rodas (2WD) utilizando um microcontrolador ESP32
 * e um sensor MPU6050. O robô pode ser comandado através do aplicativo Dabble via Bluetooth.
 * O objetivo é permitir que o robô siga uma trajetória reta ou faça rotações incrementais 
 * conforme os comandos recebidos. O controle de trajetória é realizado utilizando um algoritmo 
 * PID (Proporcional, Integral, Derivativo) que corrige a direção com base nos dados do giroscópio 
 * do MPU6050, garantindo que o robô mantenha seu curso, mesmo quando há escorregamento.
 * 
 * Versão: 1.0
 * Autor: Prof. Dr. Robson Marinho
 * Data: 15/10/2024
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
int baseMotorSpeedLeft = 100;  // Velocidade base inicial para a roda esquerda
int baseMotorSpeedRight = 80;  // Velocidade base inicial para a roda direita
int rotationSpeed = 50;        // Velocidade reduzida para rotação

// Constantes PID ajustadas para a roda esquerda
float KpLeft = 4.5, KiLeft = 0.15, KdLeft = 2.8;  // Constantes ajustadas para melhorar a roda esquerda
float prevErrorYawLeft = 0;
float integralYawLeft = 0;

// Constantes PID ajustadas para a roda direita
float KpRight = 3.0, KiRight = 0.05, KdRight = 2.5;  // Constantes ajustadas para a roda direita
float prevErrorYawRight = 0;
float integralYawRight = 0;

// Tempo de amostragem para o controle PID
unsigned long lastTime = 0;
float dt = 0.1;  // Intervalo de tempo em segundos (100ms)

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
    rotateLeft();  // Rotação incremental para a esquerda
  } else if (GamePad.isRightPressed()) {
    rotateRight();  // Rotação incremental para a direita
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

// Rotação incremental para a esquerda
void rotateLeft() {
  digitalWrite(leftMotorPin1, LOW);  // Esquerdo vai para trás
  digitalWrite(leftMotorPin2, HIGH);
  analogWrite(motorLeftPWM, rotationSpeed);

  digitalWrite(rightMotorPin1, HIGH);  // Direito vai para frente
  digitalWrite(rightMotorPin2, LOW);
  analogWrite(motorRightPWM, rotationSpeed);
}

// Rotação incremental para a direita
void rotateRight() {
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

  // Termo Proporcional para a roda esquerda
  float proportionalYawLeft = KpLeft * errorYaw;

  // Termo Integral para a roda esquerda
  integralYawLeft += errorYaw * dt;
  float integralTermYawLeft = KiLeft * integralYawLeft;

  // Termo Derivativo para a roda esquerda
  float derivativeYawLeft = (errorYaw - prevErrorYawLeft) / dt;
  float derivativeTermYawLeft = KdLeft * derivativeYawLeft;

  // Termo Proporcional para a roda direita
  float proportionalYawRight = KpRight * errorYaw;

  // Termo Integral para a roda direita
  integralYawRight += errorYaw * dt;
  float integralTermYawRight = KiRight * integralYawRight;

  // Termo Derivativo para a roda direita
  float derivativeYawRight = (errorYaw - prevErrorYawRight) / dt;
  float derivativeTermYawRight = KdRight * derivativeYawRight;

  // Calcular a correção final para cada roda
  float correctionYawLeft = proportionalYawLeft + integralTermYawLeft + derivativeTermYawLeft;
  float correctionYawRight = proportionalYawRight + integralTermYawRight + derivativeTermYawRight;

  // Limita as correções para evitar valores extremos
  correctionYawLeft = constrain(correctionYawLeft, -20, 20);  // Correção ampliada para a roda esquerda
  correctionYawRight = constrain(correctionYawRight, -10, 10);  // Correção mantida para a roda direita

  // Aplica as correções nos motores
  int leftSpeed = baseMotorSpeedLeft - correctionYawLeft;   // Corrige o motor esquerdo
  int rightSpeed = baseMotorSpeedRight + correctionYawRight; // Corrige o motor direito

  // Garante que as velocidades fiquem dentro dos limites (0 a 255)
  leftSpeed = constrain(leftSpeed, 0, 255);
  rightSpeed = constrain(rightSpeed, 0, 255);

  // Aplica a correção aos motores
  analogWrite(motorLeftPWM, leftSpeed);
  analogWrite(motorRightPWM, rightSpeed);

  // Armazena o erro para uso na próxima iteração
  prevErrorYawLeft = errorYaw;
  prevErrorYawRight = errorYaw;
}
```

## Uso do Aplicativo Dabble

1. Baixe o aplicativo Dabble na Google Play Store ou Apple App Store.
2. Abra o aplicativo e conecte-se ao dispositivo Bluetooth nomeado "ESP32-Robo".
3. Use os botões no aplicativo para controlar o robô:
   - **Seta para cima**: mover para frente
   - **Seta para baixo**: mover para trás
   - **Seta para a esquerda**: girar à esquerda
   - **Seta para a direita**: girar à direita

## Ajustes e Calibrações

- Se o robô não seguir uma trajetória reta, ajuste as constantes PID no código.
- Experimente alterar as velocidades base dos motores para encontrar a melhor configuração.

## Contribuições

Sinta-se à vontade para fazer melhorias ou relatar problemas. Contribuições são bem-vindas!

## Licença

Este projeto é de código aberto. Sinta-se à vontade para usá-lo e modificá-lo conforme necessário.
