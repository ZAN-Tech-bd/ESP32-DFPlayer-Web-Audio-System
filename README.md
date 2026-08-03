# ESP32 DFPlayer Web Audio System

An ESP32-based MP3 audio playback system developed by ZAN Tech.

The system can play MP3 files from a microSD card using a DFPlayer Mini.
Audio can be controlled through:

- ESP32 Local Web Server
- Serial Monitor
- Wi-Fi Access Point

## Features

- MP3 playback from microSD card
- ESP32 Wi-Fi hotspot
- Local web control interface
- Serial Monitor control
- Play / Pause / Resume / Stop
- Volume control
- Multiple audio track selection
- LM386 audio amplification

## Hardware Components

| Component | Quantity |
|---|---:|
| ESP32 DevKit V1 | 1 |
| DFPlayer Mini | 1 |
| LM386 Audio Amplifier Module | 1 |
| 3W 4Ω Speaker | 1 |
| MicroSD Card | 1 |
| 1kΩ Resistor | 1 |
| External 5V Power Supply | 1 |
| Jumper Wires | As required |

## Pin Connection

### ESP32 → DFPlayer Mini

| ESP32 | DFPlayer |
|---|---|
| GPIO 17 (TX2) | RX |
| GPIO 16 (RX2) | TX |
| GND | GND |
| External 5V | VCC |

### DFPlayer → LM386

| DFPlayer | LM386 |
|---|---|
| DAC_L | Audio IN |
| GND | GND |

### LM386 → Speaker

| LM386 | Speaker |
|---|---|
| OUT+ | Speaker + |
| OUT- | Speaker - |

> ESP32, DFPlayer, LM386 and external power supply must share a common ground.

## SD Card Setup

Format the microSD card as FAT32.

Create:

MP3/

Add files:

0001.mp3  
0002.mp3  
0003.mp3

## Wi-Fi Control

After powering the ESP32, connect your phone or computer to:

SSID: ESP32-MP3  
Password: 12345678

Then open:

http://192.168.4.1

The web interface provides:

- Play Track
- Pause
- Resume
- Stop
- Volume Up
- Volume Down

## Serial Monitor Control

Set Serial Monitor baud rate to:

115200

Commands:

| Command | Function |
|---|---|
| 1 | Play 0001.mp3 |
| 2 | Play 0002.mp3 |
| 3 | Play 0003.mp3 |
| p | Pause |
| r | Resume |
| s | Stop |
| + | Volume Up |
| - | Volume Down |

## Required Arduino Library

Install:

DFRobotDFPlayerMini

ESP32 WiFi and WebServer libraries are included with the ESP32 Arduino core.

## Applications

This system can be used for:

- Talking robots
- Educational robots
- Museum information systems
- Smart announcement systems
- Interactive projects
- Voice feedback systems
- IoT audio systems

## Developed By

**ZAN Tech**

Robotics & Electronics Education  
Bangladesh
