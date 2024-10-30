#define BLYNK_TEMPLATE_NAME "Remote Car 2WD ESP32"
#define BLYNK_TEMPLATE_ID "xxxxxxxxxxx"
#define BLYNK_AUTH_TOKEN "xxxxxxxxxx" // Insira seu Token de Autenticação Blynk

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// Credenciais Wi-Fi
char auth[] = BLYNK_AUTH_TOKEN; //BLYNK_AUTH_TOKEN
char ssid[] = "Robson5G";         // Nome da rede Wi-Fi
char pass[] = "12345678";         // Senha da rede Wi-Fi

// Pinos do TB6612 para os motores
const int motorRightPWM = 13;
const int rightMotorPin1 = 25;
const int rightMotorPin2 = 33;
const int motorLeftPWM = 32;
const int leftMotorPin1 = 26;
const int leftMotorPin2 = 27;

// Canal PWM do ESP32
const int pwmChannelLeft = 0;
const int pwmChannelRight = 1;
const int pwmFreq = 5000;
const int pwmResolution = 8;

int baseMotorSpeedLeft = 100;
int baseMotorSpeedRight = 100;

void setup() {
  Serial.begin(115200);
  Serial.println("Iniciando conexão com o Wi-Fi...");
  
  Blynk.begin(auth, ssid, pass);
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Wi-Fi conectado!");
    Serial.println("Conectando ao Blynk...");
  }

  // Espera até conectar ao Blynk
  while (!Blynk.connected()) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado ao Blynk com sucesso!");

  // Configuração dos pinos dos motores
  pinMode(rightMotorPin1, OUTPUT);
  pinMode(rightMotorPin2, OUTPUT);
  pinMode(leftMotorPin1, OUTPUT);
  pinMode(leftMotorPin2, OUTPUT);

  ledcSetup(pwmChannelLeft, pwmFreq, pwmResolution);
  ledcAttachPin(motorLeftPWM, pwmChannelLeft);

  ledcSetup(pwmChannelRight, pwmFreq, pwmResolution);
  ledcAttachPin(motorRightPWM, pwmChannelRight);
}

void loop() {
  Blynk.run();
}

// Funções de controle
BLYNK_WRITE(V0) { 
  if (param.asInt() == 1) {
    moveForward();
  } else {
    stopMotors();
  }
}

BLYNK_WRITE(V1) { 
  if (param.asInt() == 1) {
    moveBackward();
  } else {
    stopMotors();
  }
}

BLYNK_WRITE(V2) { 
  if (param.asInt() == 1) {
    rotateLeft();
  } else {
    stopMotors();
  }
}

BLYNK_WRITE(V3) { 
  if (param.asInt() == 1) {
    rotateRight();
  } else {
    stopMotors();
  }
}

void moveForward() {
  Serial.println("Movendo para frente");
  digitalWrite(leftMotorPin1, HIGH);
  digitalWrite(leftMotorPin2, LOW);
  ledcWrite(pwmChannelLeft, baseMotorSpeedLeft);

  digitalWrite(rightMotorPin1, HIGH);
  digitalWrite(rightMotorPin2, LOW);
  ledcWrite(pwmChannelRight, baseMotorSpeedRight);
}

void moveBackward() {
  Serial.println("Movendo para trás");
  digitalWrite(leftMotorPin1, LOW);
  digitalWrite(leftMotorPin2, HIGH);
  ledcWrite(pwmChannelLeft, baseMotorSpeedLeft);

  digitalWrite(rightMotorPin1, LOW);
  digitalWrite(rightMotorPin2, HIGH);
  ledcWrite(pwmChannelRight, baseMotorSpeedRight);
}

void stopMotors() {
  Serial.println("Parando motores");
  ledcWrite(pwmChannelLeft, 0);
  ledcWrite(pwmChannelRight, 0);
}

void rotateLeft() {
  Serial.println("Girando para a esquerda");
  digitalWrite(leftMotorPin1, HIGH);
  digitalWrite(leftMotorPin2, LOW);
  ledcWrite(pwmChannelLeft, baseMotorSpeedLeft);

  digitalWrite(rightMotorPin1, LOW);
  digitalWrite(rightMotorPin2, HIGH);
  ledcWrite(pwmChannelRight, baseMotorSpeedRight);
}

void rotateRight() {
  Serial.println("Girando para a direita");
  digitalWrite(leftMotorPin1, LOW);
  digitalWrite(leftMotorPin2, HIGH);
  ledcWrite(pwmChannelLeft, baseMotorSpeedLeft);

  digitalWrite(rightMotorPin1, HIGH);
  digitalWrite(rightMotorPin2, LOW);
  ledcWrite(pwmChannelRight, baseMotorSpeedRight);
}
