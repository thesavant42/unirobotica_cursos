Here is the translation of the README:

# 2WD Robot Car with ESP32 and MPU6050

## Description

This project uses an ESP32 microcontroller and an MPU6050 sensor to control a two-wheel drive (2WD) robot car via the Dabble app using Bluetooth. The robot is capable of following a straight path or making incremental rotations based on the received commands. The path control is performed using a PID (Proportional, Integral, Derivative) algorithm that corrects the direction based on the gyroscope data from the MPU6050, ensuring that the robot maintains its course, even when there is slippage.

## Required Components

- **Hardware:**
  - 1 x ESP32
  - 1 x MPU6050
  - 1 x TB6612 Motor Driver
  - 2 x DC Motors
  - 1 x Chassis for the robot
  - Connecting wires

- **Software:**
  - Arduino IDE
  - MPU6050 library (available in the Arduino library manager)
  - Dabble library (available in the Arduino library manager)

## Installation

### 1. Set Up the Development Environment

1. Download and install the [Arduino IDE](https://www.arduino.cc/en/software).
2. Open the IDE and go to **File** > **Preferences**.
3. In the "Additional Board Manager URLs" field, add:
   ```
   https://dl.espressif.com/dl/package_esp32_index.json
   ```
4. Go to **Tools** > **Board** > **Boards Manager** and search for "ESP32". Install the ESP32 platform.

### 2. Install Required Libraries

1. Open the IDE and go to **Sketch** > **Include Library** > **Manage Libraries**.
2. Search for and install the following libraries:
   - **MPU6050** (look for "MPU6050 by Electronic Cats").
   - **Dabble** (look for "Dabble ESP32").

### 3. Circuit Assembly

Connect the components as per the following schematic:

- **MPU6050:**
  - VCC -> 3.3V on ESP32
  - GND -> GND on ESP32
  - SDA -> GPIO 21 (or the SDA pin on ESP32)
  - SCL -> GPIO 22 (or the SCL pin on ESP32)

- **TB6612 Motor Driver:**
  - Control pins connected to the pins defined in the code:
    - Right Motor: PWM -> GPIO 13, IN1 -> GPIO 25, IN2 -> GPIO 33
    - Left Motor: PWM -> GPIO 32, IN1 -> GPIO 26, IN2 -> GPIO 27
  - Power the driver with the appropriate voltage for the motors.

### 4. Upload the Code

1. Copy the provided code (see below) and paste it into the Arduino IDE.
2. Ensure the ESP32 board is selected in **Tools** > **Board**.
3. Connect the ESP32 to the computer via USB.
4. Select the correct port in **Tools** > **Port**.
5. Click **Upload** to load the code onto the ESP32.

## Code

[PID_MPU6050_ESP32_Dabble.ino](https://github.com/UniRobotica/Cursos/blob/main/Robotica/PID_MPU6050_ESP32_Dabble/PID_MPU6050_ESP32_Dabble.ino)

## Using the Dabble App

1. Download the Dabble app from the Google Play Store or Apple App Store.
2. Open the app and connect to the Bluetooth device named "ESP32-Robo".
3. Use the buttons in the app to control the robot:
   - **Up Arrow**: move forward
   - **Down Arrow**: move backward
   - **Left Arrow**: turn left
   - **Right Arrow**: turn right

## Adjustments and Calibrations

- If the robot does not follow a straight path, adjust the PID constants in the code.
- Experiment with changing the base speeds of the motors to find the best configuration.

## Contributions

Feel free to make improvements or report issues. Contributions are welcome!

## License

This project is open source. Feel free to use and modify it as needed.
