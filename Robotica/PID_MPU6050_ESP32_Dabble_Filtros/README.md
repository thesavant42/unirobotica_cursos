# Controle de Carro Robô 2WD com ESP32 e MPU6050

Este projeto implementa o controle de um carro robô 2WD (duas rodas) utilizando um ESP32 e um sensor MPU6050. O sistema é controlado via Bluetooth usando o aplicativo Dabble. Além disso, é utilizado um controle PID (Proporcional, Integral, Derivativo) para corrigir a trajetória do robô e manter uma linha reta, ou realizar rotações controladas.

## Componentes
- ESP32
- Sensor MPU6050
- Driver de motor TB6612
- 2 motores DC
- App Dabble (via Bluetooth)

## Funcionalidades

- O robô pode se mover para frente e para trás, girar à esquerda ou direita, conforme os comandos do app Dabble.
- O controle PID é aplicado para corrigir o movimento do robô, mantendo-o em uma linha reta.
- Um sistema de filtro passa-baixa é utilizado para suavizar os dados do giroscópio.

---

## Explicação do Código

### Inicialização do Sistema

Na função `setup()`, os componentes são inicializados:

```cpp
void setup() {
    Serial.begin(115200);
    Dabble.begin("ESP32-Robo");
    Wire.begin();
    mpu.initialize();
    pinMode(rightMotorPin1, OUTPUT);
    pinMode(rightMotorPin2, OUTPUT);
    pinMode(motorRightPWM, OUTPUT);
    pinMode(leftMotorPin1, OUTPUT);
    pinMode(leftMotorPin2, OUTPUT);
    pinMode(motorLeftPWM, OUTPUT);
}
```

## Inicialização do Sistema

Aqui, o ESP32 é preparado para comunicação serial, o Bluetooth é ativado para se comunicar com o app Dabble, e o sensor MPU6050 é inicializado para leitura dos dados de movimento.

---

## Loop Principal

A função `loop()` processa as entradas do controle remoto (Dabble), atualiza a leitura do sensor MPU6050 e aplica o controle de movimentação baseado nos comandos recebidos:

```cpp
void loop() {
    Dabble.processInput();
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    currentYaw += (gz / 131.0) * dt;

    if (GamePad.isUpPressed()) {
        moveForward();
        targetYaw = currentYaw;
        correctTrajectoryPID();
    } else if (GamePad.isDownPressed()) {
        moveBackward();
        targetYaw = currentYaw;
        correctTrajectoryPID();
    } else if (GamePad.isLeftPressed()) {
        rotateLeft();
        targetYaw -= 5;
        correctRotationPID();
    } else if (GamePad.isRightPressed()) {
        rotateRight();
        targetYaw += 5;
        correctRotationPID();
    } else {
        stopMotors();
    }
}
```

## Controle de Direção

O GamePad do Dabble é utilizado para determinar a direção (frente, trás, esquerda, direita). O valor de yaw (orientação) é atualizado com base nos dados do giroscópio do MPU6050. Dependendo do comando do usuário, o robô move-se para frente, para trás ou realiza rotações incrementais.

---

## Funções de Movimentação

Funções como `moveForward()`, `moveBackward()`, `rotateLeft()`, `rotateRight()` e `stopMotors()` controlam diretamente o comportamento dos motores:

```cpp
void moveForward() {
    digitalWrite(leftMotorPin1, HIGH);
    digitalWrite(leftMotorPin2, LOW);
    analogWrite(motorLeftPWM, baseMotorSpeedLeft);
    digitalWrite(rightMotorPin1, HIGH);
    digitalWrite(rightMotorPin2, LOW);
    analogWrite(motorRightPWM, baseMotorSpeedRight);
}
```

## Funções de Movimentação

A função `moveForward()` liga os motores na direção para frente com base nas velocidades configuradas para cada roda. De maneira similar, `moveBackward()` inverte a direção dos motores para mover o robô para trás.

---

## Controle PID para Correção de Trajetória

Para manter o robô seguindo em linha reta, o controle PID é aplicado. A função `correctTrajectoryPID()` ajusta a velocidade de cada motor com base no erro de yaw (a diferença entre o yaw atual e o yaw desejado):

```cpp

void correctTrajectoryPID() {
    float errorYaw = targetYaw - currentYaw;
    
    float proportionalYawLeft = KpLeft * errorYaw;
    integralYawLeft += errorYaw * dt;
    float derivativeYawLeft = (errorYaw - prevErrorYawLeft) / dt;
    float correctionYawLeft = proportionalYawLeft + (KiLeft * integralYawLeft) + (KdLeft * derivativeYawLeft);

    float proportionalYawRight = KpRight * errorYaw;
    integralYawRight += errorYaw * dt;
    float derivativeYawRight = (errorYaw - prevErrorYawRight) / dt;
    float correctionYawRight = proportionalYawRight + (KiRight * integralYawRight) + (KdRight * derivativeYawRight);

    int leftSpeed = baseMotorSpeedLeft - correctionYawLeft;
    int rightSpeed = baseMotorSpeedRight + correctionYawRight;

    analogWrite(motorLeftPWM, constrain(leftSpeed, 0, 255));
    analogWrite(motorRightPWM, constrain(rightSpeed, 0, 255));

    prevErrorYawLeft = errorYaw;
    prevErrorYawRight = errorYaw;
}
```

## Controle PID para Rotação

- **Erro de Yaw**: É a diferença entre a orientação desejada e a orientação atual.
- **Correção Proporcional**: Calculada como `Kp * erro`, fornece uma correção diretamente proporcional ao erro.
- **Correção Integral**: Acumula o erro ao longo do tempo e ajuda a eliminar desvios sistemáticos.
- **Correção Derivativa**: Baseada na taxa de mudança do erro, ajuda a suavizar as correções, evitando oscilações.

---

A função `correctRotationPID()` aplica o controle PID para corrigir as rotações incrementais à esquerda ou à direita:

```cpp

void correctRotationPID() {
    float errorYaw = targetYaw - currentYaw;
    
    float proportionalYawLeft = KpLeft * errorYaw;
    integralYawLeft += errorYaw * dt;
    float derivativeYawLeft = (errorYaw - prevErrorYawLeft) / dt;
    float correctionYawLeft = proportionalYawLeft + (KiLeft * integralYawLeft) + (KdLeft * derivativeYawLeft);

    float proportionalYawRight = KpRight * errorYaw;
    integralYawRight += errorYaw * dt;
    float derivativeYawRight = (errorYaw - prevErrorYawRight) / dt;
    float correctionYawRight = proportionalYawRight + (KiRight * integralYawRight) + (KdRight * derivativeYawRight);

    int leftSpeed = rotationSpeed - correctionYawLeft;
    int rightSpeed = rotationSpeed + correctionYawRight;

    analogWrite(motorLeftPWM, constrain(leftSpeed, 0, 255));
    analogWrite(motorRightPWM, constrain(rightSpeed, 0, 255));

    prevErrorYawLeft = errorYaw;
    prevErrorYawRight = errorYaw;
}
```

Semelhante à correção da trajetória, esta função ajusta as velocidades dos motores durante as rotações, mantendo a precisão na movimentação angular.

## Diagrama de Conexão

Aqui está um diagrama básico de conexão do ESP32 com o MPU6050 e os motores:

- **ESP32 GPIOs**:
  - Motores conectados aos pinos: 13, 25, 33, 32, 26, 27 (via driver TB6612).
  - MPU6050 conectado aos pinos SDA e SCL (I2C).

> O diagrama pode ser gerado utilizando ferramentas como o Fritzing, conectando o ESP32, o MPU6050 e os motores controlados pelo driver TB6612.

## Conclusão

Este projeto implementa um controle eficiente para um robô de duas rodas utilizando controle PID para trajetórias retas e rotações precisas. A combinação do ESP32 com o sensor MPU6050 permite monitorar a orientação do robô em tempo real e ajustar sua movimentação para alcançar a trajetória desejada com precisão.
