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

[MeOptico.ino](https://github.com/UniRobotica/Cursos/blob/main/Robotica/Medi%C3%A7%C3%A3o%20%C3%B3ptica/MedOptico.ino)

---

