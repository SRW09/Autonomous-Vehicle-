# 🤖 FREEBOT - Frugal Robot Engineered from Efficient and Budget Optimised Technology

![FREEBOT Robot](https://github.com/SRW09/Autonomous-Vehicle-/blob/main/FREEBOT-main/FREEBOT-main/Circuit_diagram_v1.png?raw=true)

## Overview

FREEBOT is an open-source autonomous robot designed to make robotics accessible and affordable for beginners. This project demonstrates advanced robotics concepts including wireless control, computer vision, sensor integration, and real-time navigation using Python and C++ libraries.

**Key Features:**
- 🎮 Multi-control interface (keyboard, hand gestures, computer vision)
- 📱 Mobile phone sensor integration
- 🎥 Real-time video feed processing
- 📡 UDP-based wireless communication
- 🟢 Green ball tracking and chasing
- 👋 Hand gesture recognition

---

## 📋 Table of Contents

1. [Hardware Setup](#hardware-setup)
2. [Hand Tracker Control](#hand-tracker-control)
3. [Robot Control Center](#robot-control-center)
4. [Ball Chasing Robot](#ball-chasing-robot)
5. [Installation & Requirements](#installation--requirements)

---

## Hardware Setup

### Components
- **Microcontroller:** ESP32
- **Motor Driver:** L298N
- **Motors:** 2x DC Motors
- **Wheels:** 2x Yellow wheels with tires
- **Power Supply:** Li-ion batteries
- **Communication:** WiFi module (built into ESP32)

### Circuit Diagram & Physical Setup
The circuit diagram and component layout show the motor driver connected to the ESP32, with power management and signal lines properly configured.

![FREEBOT Hardware Setup](https://github.com/SRW09/Autonomous-Vehicle-/blob/main/FREEBOT-main/FREEBOT-main/Circuit_diagram_v1.png?raw=true)

**Setup Instructions:**
1. Connect L298N motor driver to ESP32 GPIO pins
2. Attach DC motors to L298N outputs
3. Wire battery power to the motor driver
4. Ensure proper ESP32 board installation in Arduino IDE

---

## 🖐️ Hand Tracker Control

Control the robot using real-time hand gesture recognition via your device's camera.

### Requirements
- Python 3.9+
- OpenCV
- MediaPipe
- Socket library

### Installation
```bash
pip install opencv-python mediapipe
```

### How to Setup

1. Install Python 3.9
2. Install required libraries:
   ```bash
   pip install opencv-python mediapipe
   ```
3. Download `Handtracker-control.py`
4. Get your FREEBOT's IP address
5. Update the IP address in the script
6. Run the program:
   ```bash
   python Handtracker-control.py
   ```

### Hand Gesture Commands

| Gesture | Action |
|---------|--------|
| 👆 Hand above top box | Move Forward |
| 👇 Hand below bottom box | Move Backward |
| 🔄 Hand rotated right | Turn Right |
| 🔄 Hand rotated left | Turn Left |
| ⏸️ Hand in center | Stop |

**Note:** Hand tracking currently optimized for left-hand detection.

### Hand Tracking Demo
![Hand Control Interface](https://github.com/SRW09/Autonomous-Vehicle-/blob/main/FREEBOT-main/FREEBOT-main/Circuit_diagram_v1.png?raw=true)

*Real-time hand gesture recognition with rotation angle calculation and bounding box detection*

---

## 🎮 Robot Control Center

### Robot Control via Live Camera Feed

This Python script provides real-time video control of the robot with keyboard inputs and an intuitive user interface.

### Prerequisites

- Python 3.7+
- OpenCV (`cv2`)
- NumPy
- Keyboard library (optional for advanced control)

### Installation

```bash
pip install opencv-python numpy
```

### Usage

1. **Configure the camera URL:**
   ```python
   url = 'http://192.168.4.2:8080/video'  # Replace with your camera stream URL
   ```

2. **Set robot network parameters:**
   ```python
   udp_host = '0.0.0.0'  # Listening address
   udp_port = 12345      # Communication port
   ```

3. **Run the control script:**
   ```bash
   python Robot_control_centre_v1.py
   ```

### Keyboard Controls

| Key | Function |
|-----|----------|
| **W** | Move Forward |
| **S** | Move Backward |
| **A** | Turn Left |
| **D** | Turn Right |
| **Q** | Quit Program |

### Mobile Control Interface
![Robot Control UI with Speed Slider](https://github.com/SRW09/Autonomous-Vehicle-/blob/main/FREEBOT-main/FREEBOT-main/Circuit_diagram_v1.png?raw=true)

*Intuitive mobile interface with directional controls and adjustable speed slider*

### How It Works

- Opens a UDP socket for robot communication
- Captures and displays live camera feed
- Processes keyboard inputs in real-time
- Sends movement commands via UDP protocol
- Supports adjustable speed via trackbars

---

## 🟢 Ball Chasing Robot

This module demonstrates advanced computer vision capabilities with real-time ball detection and autonomous tracking.

### Overview

The ball-chasing robot uses OpenCV to detect a green ball in the camera feed and automatically moves to follow it. The robot's movement is controlled based on the ball's position relative to a detection zone.

### Libraries to Install

```bash
pip install opencv-python numpy Pillow
```

### How to Run

1. Clone the repository or download the source code
2. Ensure camera connection and update URL if needed:
   ```python
   url = 'http://192.168.4.2:8080/video'
   ```
3. Update robot IP and port:
   ```python
   sock.sendto(response_message.encode("utf-8"), ('192.168.4.1', 12345))
   ```
4. Run the program:
   ```bash
   python Ball_chasing_robot.py
   ```

### Ball Chasing in Action
![Ball Chasing Robot Tracking](https://github.com/SRW09/Autonomous-Vehicle-/blob/main/FREEBOT-main/FREEBOT-main/Circuit_diagram_v1.png?raw=true)

*FREEBOT detecting and tracking a green ball with real-time telemetry overlay*

### Detection Zone Logic

The robot uses a central detection rectangle to determine movement commands:

```
                    ┌─────────────┐
                    │   FORWARD   │
                    └─────────────┘
           ┌────────────┐   ┌────────────┐
           │    LEFT    │   │   RIGHT    │
           └────────────┘   └────────────┘
                    ┌─────────────┐
                    │   BACKWARD  │
                    └─────────────┘
```

**Movement Rules:**
- **Above zone** → Move forward
- **Below zone** → Move backward
- **Inside zone** → Stop
- **Left of zone** → Turn left
- **Right of zone** → Turn right

### Features

✅ Real-time green ball detection using HSV color space  
✅ Contour analysis to track ball position  
✅ Speed adjustment via interactive trackbar  
✅ Color threshold adjustment for different lighting  
✅ Ball trail visualization  
✅ On-screen telemetry display (X, Y coordinates, diameter)  

### Live Ball Detection Display
![Ball Detection with Telemetry](https://github.com/SRW09/Autonomous-Vehicle-/blob/main/FREEBOT-main/FREEBOT-main/Circuit_diagram_v1.png?raw=true)

*Real-time green ball detection with on-screen metrics: diameter, position coordinates, and detection rectangle*

### Step-by-Step Explanation

#### 1. **Initialization**
- Sets up UDP socket for robot communication
- Configures HSV color bounds for green detection
- Initializes trail buffer for motion visualization
- Sets up camera and display parameters

#### 2. **Green Ball Detection**
The `detect_green_ball()` function:
- Converts BGR frame to HSV color space
- Applies color threshold to isolate green pixels
- Finds contours in the mask
- Returns centroid coordinates of the largest contour

#### 3. **Live Camera Feed Processing**
- Continuously captures frames from camera
- Detects green ball in each frame
- Draws visual indicators (circles, trails, text)
- Updates trackbar values for real-time adjustment

#### 4. **Robot Control Logic**
Based on ball position:
- Generates appropriate movement commands
- Adjusts speed dynamically
- Sends commands via UDP to robot

#### 5. **Visual Feedback**
- Real-time coordinate display
- Ball diameter measurement
- Motion trail visualization
- Detection zone rectangle overlay
- Speed and command indicators

---

## 🛠️ Installation & Requirements

### System Requirements

- **Python:** 3.7 or higher
- **OS:** Windows, macOS, or Linux
- **Network:** WiFi connectivity between robot and control device

### Required Libraries

```bash
# Core libraries
pip install opencv-python numpy

# Optional for advanced features
pip install mediapipe pillow
```

### Arduino IDE Setup for ESP32

1. Open Arduino IDE → **File > Preferences**
2. Add Board Manager URL:
   ```
   https://dl.espressif.com/dl/package_esp32_index.json
   ```
3. Go to **Tools > Board > Boards Manager**
4. Search for "ESP32" and install "ESP32 by Espressif Systems"
5. Select your ESP32 board from **Tools > Board**
6. Connect ESP32 via USB and select the appropriate port

### Motor Driver Setup

For detailed L298N motor driver configuration, refer to: [DroneBotWorkshop L298N Tutorial](https://www.youtube.com/watch?v=dyjo_ggEtVU&ab_channel=DroneBotWorkshop)

---

## 📁 Project Structure

```
FREEBOT-main/
├── Ball_chasing_robot.py          # Ball detection and tracking
├── Handtracker-control.py         # Hand gesture recognition
├── Robot_control_centre_v1.py     # Keyboard-based control
├── FREEbot.aia                    # MIT App Inventor source
├── FREEbot.apk                    # Mobile app package
├── Circuit_diagram_v1.png         # Hardware schematic
└── README.md                      # This file
```

---

## 📱 Mobile Application

A companion Android app (`FREEbot.apk`) is available for mobile-based robot control with an intuitive touch interface.

---

## 🔗 References & Resources

- **Original FREEBOT Project:** https://github.com/sastejugaad/FREEBOT/
- **Tutorial Series:**
  - [Part 1](https://www.youtube.com/watch?v=ymNAXm_j8do&t=3s)
  - [Part 2](https://www.youtube.com/watch?v=j49aA8wwWxY&ab_channel=SasteJugaad)
  - [Part 3](https://www.youtube.com/watch?v=SvNt3h0w55A&ab_channel=SasteJugaad)

---

## 💬 Community & Support

- **Discord:** https://discord.com/invite/fMXvGty
- **Instagram:** https://www.instagram.com/shub_bhatt/

---

## 📝 License

This project is open-source and available for educational and personal use.

---

## 🎯 Next Steps

1. Assemble the hardware following the circuit diagram
2. Flash the ESP32 with the robot firmware
3. Configure your network settings
4. Choose your preferred control method (keyboard, hand gestures, or ball tracking)
5. Run and enjoy your FREEBOT! 🚀

**Happy Robotics! 🤖**
