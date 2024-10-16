
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

[PID_MPU6050_ESP32_Dabble.ino](https://github.com/UniRobotica/Cursos/blob/main/Robotica/PID_MPU6050_ESP32_Dabble/PID_MPU6050_ESP32_Dabble.ino)

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
