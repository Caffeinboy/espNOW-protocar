# ESP32 ESP-NOW Wireless RC Car

A low-latency wireless robotic car system built using **ESP32 + ESP-NOW**, featuring **real-time joystick control** and **hardware PWM motor driving**.

This project eliminates the need for Wi-Fi routers or Bluetooth pairing, using **direct ESP32-to-ESP32 communication** for fast and reliable control.

---

#  System Overview

The system consists of two ESP32 devices:

### Transmitter (Controller)

* Reads joystick X/Y values
* Sends data via ESP-NOW

###  Receiver (Car)

* Receives joystick data
* Processes direction logic
* Drives motors using PWM

---

# Core Features

* ESP-NOW peer-to-peer communication
* Real-time directional control
* Hardware PWM using ESP32 LEDC
* Fixed 75% motor speed
* Clean directional logic (no blocking delays)
* Lightweight and efficient

---

# Control Logic

| Condition | Action   |
| --------- | -------- |
| Y > 3000  | Forward  |
| Y < 1000  | Backward |
| X > 3000  | Right    |
| X < 1000  | Left     |
| Otherwise | Stop     |

---

# Receiver Pin Mapping

## ESP32 → Motor Driver (H-Bridge)

| ESP32 GPIO | Driver Pin | Description     |
| ---------- | ---------- | --------------- |
| 25         | IN1        | Motor A input 1 |
| 26         | IN2        | Motor A input 2 |
| 27         | IN3        | Motor B input 1 |
| 14         | IN4        | Motor B input 2 |

---

# Motor Driver Configuration

This code assumes:

* Using a driver like L298N / L293D
* **ENA and ENB are permanently enabled (jumpered HIGH)**

 This means:

* Speed control is applied directly on IN pins via PWM
* No separate enable pin control is used

---

# PWM Configuration

* PWM Frequency: **1000 Hz**
* Resolution: **8-bit**
* Duty Cycle: **191 (≈75%)**

PWM is generated using ESP32’s **LEDC peripheral**.

---

#  ESP-NOW Communication

## Data Structure

```cpp
typedef struct struct_message {
  int xValue;
  int yValue;
};
```

* X → Left/Right control
* Y → Forward/Backward control

---

## Receiver Workflow

1. ESP-NOW packet received
2. Data copied into struct
3. Values evaluated
4. Corresponding motor function executed

---

# Setup Instructions

## 1. Install ESP32 Board Package

* Open Arduino IDE
* Go to Preferences
* Add:

  ```
  https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
  ```
* Install ESP32 via Boards Manager

---

## 2. Select Board

```
Tools → Board → ESP32 Dev Module
```

---

## 3. Upload Code

* Upload transmitter code to controller ESP32
* Upload receiver code to car ESP32

---

## 4. Pair Devices

* Open Serial Monitor on receiver
* Note MAC address
* Add it in transmitter code

---

# Hardware Requirements

* 2 × ESP32 boards
* Motor driver (L298N recommended)
* 2 DC motors
* Joystick module
* Battery (7.4V–12V)
* Chassis + wheels

---

# Design Notes

* PWM is applied on direction pins (simplified wiring)
* Suitable for basic RC car projects
* For higher efficiency:

  * Move PWM to ENA/ENB pins
  * Use IN pins only for direction logic

---

# Limitations

* Fixed speed (no proportional control)
* No acceleration smoothing
* No braking (coast stop)
* No feedback system

---

# 🚀 Future Improvements

* Variable speed based on joystick magnitude
* PID-based motion smoothing
* Soft start / ramp control
* Obstacle avoidance integration
* Telemetry (battery, signal strength)

---

# Project Structure (Suggested)

```
/esp32-rc-car
 ├── transmitter/
 ├── receiver/
 ├── README.md
 ├── circuit_diagram.png
```

---

# 📜 License

Open-source project for educational and hobby use.

---

