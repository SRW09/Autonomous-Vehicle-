# FREEBOT — Frugal Robot Engineered for Efficient, Affordable Autonomous Navigation

![FREEBOT Hardware](/FREEBOT-main/FREEBOT-main/WhatsApp%20Image%202025-11-07%20at%2013.55.11_5dcacd1c.jpg)

A compact, open-source autonomous robotics platform built around the ESP32 microcontroller and vision-based control. FREEBOT demonstrates how to combine lightweight embedded firmware, real-time computer vision, and simple UDP networking to create reliable, low-cost autonomous behaviors such as teleoperation, hand-gesture control, and visual object tracking.

---

## Project summary

FREEBOT is designed with three core goals:

- Accessibility: Use common, inexpensive hardware (ESP32, L298N, DC motors, smartphone camera) and easy-to-install software.
- Modularity: Separate firmware, networking, and perception so you can replace or extend any component independently.
- Real-time perception + control: Demonstrate closed-loop behavior using camera-based sensing (hand gestures, green-ball tracking) with low-latency UDP commands to the robot.

This repository contains Python control clients, example firmware integration patterns, mobile app artifacts, and schematics for the hardware.

---

## Architecture (high level)

1. Perception (Control Host)
   - A laptop or smartphone runs a camera stream + computer vision pipeline (OpenCV, MediaPipe).
   - Visual modules detect gestures or green objects and convert them into discrete motion commands.

2. Communication Layer
   - Commands are serialized as short UDP messages and sent over the local Wi-Fi network to the robot's IP/port.
   - Chosen for simplicity and low overhead; UDP provides minimal latency for interactive teleoperation.

3. Actuation (Robot)
   - An ESP32 receives UDP packets, parses movement commands, and drives the L298N motor controller.
   - Power and motor connections use a simple H-bridge (L298N) and battery management suitable for small DC motors.

---

## What's new in this README

- Clear architecture and dataflow explanation
- Deployment and quick-start instructions for both perception host and robot firmware
- Descriptions of the core algorithms used for gesture and ball tracking
- Example UDP command format and recommended safety considerations

---

## Key features

- Multi-modal control: Keyboard, hand-gesture (MediaPipe), and vision-based autonomous tracking
- Live camera streaming support for remote monitoring
- UDP-based low-latency control channel
- Tunable vision parameters (HSV thresholds, contour filtering) exposed via GUI trackbars
- Minimal hardware BOM — designed for low-cost experimentation

---

## Quick start — perception host (Python)

Prerequisites

- Python 3.9+ recommended
- Libraries: OpenCV, NumPy. Optional: MediaPipe for hand tracking.

Install

```bash
pip install opencv-python numpy
# Optional for gesture detection
pip install mediapipe
```

Run examples

1. Hand-tracker (gesture -> UDP command)

- Edit the IP/PORT at the top of `Handtracker-control.py` to your robot's IP and UDP port.

```python
ROBOT_IP = "192.168.4.1"
ROBOT_PORT = 12345
```

- Run:

```bash
python Handtracker-control.py
```

Hand gestures are mapped to concise action tokens that the firmware can parse (e.g. `FWD`, `BACK`, `LEFT`, `RIGHT`, `STOP`).

2. Robot control centre (keyboard + live camera)

- Update the camera stream URL and robot network settings inside `Robot_control_centre_v1.py`.
- Example camera stream URL:

```python
url = 'http://192.168.4.2:8080/video'  # replace with your camera stream
```

- Run:

```bash
python Robot_control_centre_v1.py
```

Keyboard layout: W/A/S/D for motion, Q to quit. The script opens a UDP socket and sends compact commands to the robot.

3. Ball chasing (autonomous vision)

- Update camera URL and robot IP/port in `Ball_chasing_robot.py` and run:

```bash
python Ball_chasing_robot.py
```

The program performs HSV-based color segmentation, contour extraction, and a simple position-based controller to steer the robot toward the detected ball.

---

## Vision algorithms — practical details

1. Green-ball detection (HSV segmentation + contour tracking)

- Convert BGR -> HSV and apply tunable thresholds (H, S, V). Use GUI trackbars to adjust in-field.
- Apply morphological opening/closing to reduce noise, then find contours.
- Filter contours by area and circularity; choose the largest valid contour and compute its centroid and enclosing circle.
- Control logic maps centroid position relative to a central "dead-zone" rectangle to discrete commands:
  - x left → TURN LEFT
  - x right → TURN RIGHT
  - y above → MOVE FORWARD
  - y below → MOVE BACK
  - inside dead-zone → STOP

2. Hand-gesture control (MediaPipe landmarks)

- Detect hand landmarks and compute gesture heuristics (e.g., centroid location in vertical bands, rotation angle from wrist-to-index vector).
- Map discrete gestures to commands (same tokens used by ball-chasing/keyboard code).
- This separation keeps the command layer consistent across controllers.

3. Optional: Add a lightweight PID on angle or distance to smooth commands and reduce oscillation. Keep the command rate bounded (e.g., 10–20 Hz) to avoid flooding the ESP32.

---

## UDP command format (recommended)

- Use simple ASCII tokens, 1–8 bytes long, to simplify parsing on the microcontroller.
- Example messages:

```
FWD:100   # Move forward at speed 100 (0-255)
BACK:120  # Move backward
LEFT:080  # Turn left at speed 80
RIGHT:080 # Turn right at speed 80
STOP
```

- Keep a short heartbeat or safety timeout on the ESP32: if no valid command arrives for >300–500 ms, stop motors.

---

## Firmware notes (ESP32)

- The ESP32 firmware should: (1) bring up Wi-Fi in STA or AP mode, (2) open a UDP socket and listen for commands, (3) parse tokens and map to PWM outputs for the L298N.
- Use hardware PWM channels where possible and protect motors with current-limited battery packs.

---

## Hardware bill of materials (typical)

- ESP32 development board (any common dev board)
- L298N motor driver (or any compatible H-bridge)
- 2x DC motors + wheels
- Li-ion battery pack (voltage matched to motors)
- Jumper wires, chassis

Refer to `FREEBOT-main/Circuit_diagram_v1.png` for wiring and pin suggestions.

---

## Project layout

```
FREEBOT-main/
├── Ball_chasing_robot.py
├── Handtracker-control.py
├── Robot_control_centre_v1.py
├── FREEbot.aia
├── FREEbot.apk
├── Circuit_diagram_v1.png
└── README.md
```

---

## Troubleshooting & tips

- If the camera stream is noisy, tune HSV thresholds and increase morphological kernel sizes.
- If commands are missed, check Wi‑Fi connectivity and increase packet send interval; enable a small ACK/heartbeat if needed.
- Always test motors at low speeds first and verify wiring before applying full battery voltage.

---

## References

- Original inspiration & upstream: https://github.com/sastejugaad/FREEBOT/
- DroneBot L298N tutorial: https://www.youtube.com/watch?v=dyjo_ggEtVU&ab_channel=DroneBotWorkshop

---

## License

Open-source for educational and personal use. Please credit and link back to the original authors when reusing material.

---

## Next steps

- Add a small firmware example folder with a UDP command parser and motor PWM abstraction.
- Add unit-tested vision modules (separate detection and control components).
- Optional: add a simple web dashboard for remote monitoring and parameter tuning.
