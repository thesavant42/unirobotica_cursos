# Projeto de Controle de Motor DC com Leitura de LDR

Este projeto utiliza um Arduino Uno para controlar um motor DC, ajustar a velocidade através de um potenciômetro, e exibir informações em um display LCD. Um sensor LDR detecta a presença de um laser refletido em um obstáculo, e o sistema interrompe o motor após um número pré-definido de rotações.

## Objetivos
- Controlar a velocidade de um motor DC com um potenciômetro.
- Detectar um feixe de laser refletido com um sensor LDR.
- Contar rotações do motor usando um encoder ou sensor de efeito Hall.
- Exibir leituras no LCD para monitoramento em tempo real.

---

## Componentes Necessários

- **Arduino Uno** - 1 unidade
- **LDR (Light Dependent Resistor)** - 1 unidade
- **Motor DC** - 1 unidade
- **Driver de motor** (L298N ou L293D) - 1 unidade
- **Display LCD** (16x2 com interface I2C) - 1 unidade
- **Potenciômetro** (10k ohms recomendado) - 1 unidade
- **Sensor de efeito Hall ou encoder** - 1 unidade
- **Fonte de Alimentação** (9-12V para o motor, 7-9V para o Arduino)
- **Diodo Flyback** (1N4007) - 1 unidade
- **Fusível** (conforme a corrente nominal do motor) - 1 unidade
- **Capacitores de Desacoplamento** (100nF e 470µF) - 1 unidade de cada

---

## Diagrama do Circuito

Abaixo, você encontra o diagrama de ligação para o projeto.

![Diagrama do Circuito](diagrama_motor_dc_ldr.png)

---

## Explicação do Código

### 1. Configuração e Controle do Motor DC
   - **Motor DC**: Conectado ao driver de motor para controle seguro da corrente e tensão.
   - **Velocidade do Motor**: Ajustada por PWM utilizando um potenciômetro conectado ao pino analógico `A1` do Arduino. O valor do potenciômetro é mapeado de 0 a 255 e aplicado no pino de controle de velocidade do motor (`motorPin1`).

### 2. Sensor LDR e Detecção do Laser
   - **LDR**: Conectado ao pino `A0` para ler a intensidade da luz. Quando a leitura ultrapassa o valor limite (`threshold`), o código detecta que o laser foi refletido.
   - **Ação na Detecção**: O código pode registrar a detecção ou realizar ações adicionais, como parar o motor.

### 3. Display LCD para Monitoramento
   - **Display LCD**: Exibe a leitura do LDR e o valor PWM ajustado pelo potenciômetro, permitindo monitorar o sistema em tempo real.

### 4. Contagem de Rotações do Motor
   - **Sensor de efeito Hall ou Encoder**: Conectado ao pino de interrupção do Arduino (`encoderPin`) para contar rotações. Cada pulso incrementa o contador, e o motor para quando o número desejado de rotações é alcançado.

### 5. Estrutura do Código

O código está dividido em seções:
- **Configuração inicial (`setup`)**: Configura pinos e inicializa o LCD.
- **Loop principal (`loop`)**: Lê o valor do LDR, ajusta a velocidade do motor, verifica a presença do laser e exibe valores no LCD.
- **Interrupção para contagem de rotações**: A função `countRotation` é acionada por interrupção, contando cada rotação detectada.

---

## Código Completo

```cpp
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
    lcd.print("             "); // Limpa mensagem de detecção do laser
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
```

---

Esse código completa o projeto com o display e o potenciômetro para ajuste de velocidade PWM.