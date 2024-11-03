# Controle de Carro Robô 2WD com ESP32 e MPU6050

## Descrição
Este projeto controla um carro robô de duas rodas (2WD) utilizando um microcontrolador ESP32 e um sensor MPU6050. O robô pode ser comandado através do aplicativo Dabble via Bluetooth. O controle de trajetória é realizado utilizando um algoritmo PID para corrigir a direção e a calibração do yaw.

## Componentes Necessários
- ESP32
- MPU6050
- Driver de motor TB6612
- Motores DC
- Bateria
- Fios de conexão

## Conexões
- **MPU6050:**
  - VCC -> 3.3V
  - GND -> GND
  - SDA -> GPIO 21
  - SCL -> GPIO 22

- **TB6612:**
  - Motor Direito:
    - PWM -> GPIO 13
    - AIN1 -> GPIO 25
    - AIN2 -> GPIO 33
  - Motor Esquerdo:
    - PWM -> GPIO 32
    - BIN1 -> GPIO 26
    - BIN2 -> GPIO 27

## Funcionalidades
- **Movimentação:**
  - Mover para frente
  - Mover para trás
  - Rotacionar para a esquerda
  - Rotacionar para a direita
  - Parar

- **Calibração:**
  - Calibração do yaw pressionando o botão **X** no aplicativo Dabble.

## Instruções de Uso
1. **Configuração do Ambiente:**
   - Instale a biblioteca `MPU6050` e `DabbleESP32` na IDE do Arduino.

2. **Upload do Código:**
   - Conecte o ESP32 ao computador e faça o upload do código fornecido.

3. **Conexão Bluetooth:**
   - Abra o aplicativo Dabble e conecte-se ao ESP32.

4. **Controles:**
   - Utilize os botões quadrado, triângulo, círculo e **X** no aplicativo Dabble para controlar o robô.

## Ajustes e Calibrações
- É possível ajustar as constantes do PID diretamente no código para melhorar a precisão da trajetória.
- O valor inicial do offset de yaw é ajustado na inicialização e você pode conferial pelo monitor serial da IDE.

## Licença
Este projeto é de código aberto. Sinta-se à vontade para modificar e usar como desejar.

## Contribuições
Contribuições são bem-vindas! Sinta-se à vontade para abrir um pull request.

## Contato
Para dúvidas ou sugestões, entre em contato com o autor:
- Prof. Dr. Robson Marinho
