#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Inicializar display LCD no endereço I2C 0x27 com tamanho 16x2
LiquidCrystal_I2C lcd(0x27, 16, 2); 

// Definição dos pinos
const int motorPin1 = 9;     // Pino de controle de velocidade (PWM)
const int motorPin2 = 10;    // Pino de controle de direção
const int ldrPin = A0;       // Pino do LDR
const int potentiometerPin = A1; // Pino do potenciômetro para controle de PWM
const int encoderPin = 2;    // Pino do sensor de efeito Hall ou encoder

// Variáveis de controle
volatile int rotationCount = 0;    // Contador de rotações (usado com interrupção)
int targetRotations = 10;          // Número desejado de rotações
int threshold = 500;               // Limite para detectar o laser refletido
int pwmValue = 150;                // Valor inicial do PWM (0-255)

void setup() {
  // Configurar os pinos do motor como saída
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);

  // Configurar o pino do encoder como entrada
  pinMode(encoderPin, INPUT);

  // Iniciar a comunicação serial e o display LCD
  Serial.begin(9600);
  lcd.begin(16, 2); 
  lcd.backlight();
  lcd.print("Iniciando...");

  // Configurar interrupção para contar rotações (chama função `countRotation` a cada pulso do encoder)
  attachInterrupt(digitalPinToInterrupt(encoderPin), countRotation, RISING);

  // Iniciar o motor DC em uma direção
  analogWrite(motorPin1, pwmValue);  // Controla a velocidade do motor com PWM
  digitalWrite(motorPin2, LOW); // Define a direção do motor

  delay(1000); // Espera inicial para exibir mensagem
  lcd.clear();
}

void loop() {
  // Ler o valor do LDR e do potenciômetro
  int ldrValue = analogRead(ldrPin);
  int potValue = analogRead(potentiometerPin);

  // Calcular novo valor PWM com base no potenciômetro
  pwmValue = map(potValue, 0, 1023, 0, 255);
  analogWrite(motorPin1, pwmValue); // Ajusta a velocidade do motor com o novo valor de PWM

  // Exibir informações no display LCD
  lcd.setCursor(0, 0);
  lcd.print("LDR: ");
  lcd.print(ldrValue);
  lcd.setCursor(0, 1);
  lcd.print("PWM: ");
  lcd.print(pwmValue);

  // Exibir informações no monitor serial
  Serial.print("LDR Value: ");
  Serial.println(ldrValue);
  Serial.print("PWM Value: ");
  Serial.println(pwmValue);

  // Verificar se o laser foi refletido (acima do limite)
  if (ldrValue > threshold) {
    Serial.println("Laser detectado!");
    lcd.setCursor(8, 0);
    lcd.print("Laser detect.");
  } else {
    lcd.setCursor(8, 0);
    lcd.print("Nao detectado"); // Exibe "Nao detectado" quando o laser não é refletido
  }

  // Verificar se o número de rotações foi atingido
  if (rotationCount >= targetRotations) {
    // Parar o motor
    analogWrite(motorPin1, 0);
    Serial.println("Número de rotações atingido!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Rotac atingida!");
    while (1); // Para o loop, pois o número de rotações foi atingido
  }

  // Pausa curta para evitar muitas leituras consecutivas
  delay(100);
}

// Função de interrupção para contar rotações
void countRotation() {
  rotationCount++; // Incrementa o contador a cada pulso do sensor
}
