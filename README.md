# EMOTO LED Strip ESP32 Web Server

## Introduction
This project is developed for **EMOTO**, a university student organization focused on motorbike innovation. It features an **ESP32-based web server** to control LED strips and play music on motorbikes via Wi-Fi. Users can adjust brightness, color, effects, and music playback through a web interface.

## Features
- **ESP32 Web Server:** Control LEDs and music via a browser
- **Wi-Fi Connectivity:** No external app needed
- **Color Customization:** Choose any color from a color picker
- **Predefined Effects:** Blink, Pulse, Rainbow, Moving Light, and more
- **Brightness Control:** Adjust LED intensity
- **DFPlayer Mini Support:** Play music from an SD card
- **Responsive UI:** Works on mobile and desktop

## Hardware Requirements
- ESP32 Development Board
- WS2812B or similar addressable LED strip
- DFRobot DFPlayer Mini MP3 module
- MicroSD card with music files
- Power Supply (5V, sufficient amperage for LEDs and DFPlayer)
- Resistors (330Ω recommended for data line)
- Capacitors (1000µF recommended for stability)

## Software Requirements
- Arduino IDE or PlatformIO
- ESPAsyncWebServer Library
- Adafruit NeoPixel or FastLED Library
- DFRobotDFPlayerMini Library
- LittleFS or SPIFFS for storing web assets

## Installation & Setup
### 1. Install ESP32 Board in Arduino IDE
Follow the official ESP32 setup guide for [Arduino](https://github.com/espressif/arduino-esp32).

### 2. Install Required Libraries
In Arduino IDE, install the following libraries via the Library Manager:
- **ESPAsyncWebServer**
- **AsyncTCP**
- **FastLED or Adafruit NeoPixel**
- **DFRobotDFPlayerMini**
- **ArduinoJson**

### 3. Wiring Diagram
```
ESP32        LED Strip (WS2812B)     DFPlayer Mini
------------------------------------------------
GND    --->  GND                     GND
5V     --->  VCC                     VCC
D4     --->  Data In (through 330Ω resistor)
RX2    --->  TX
TX2    --->  RX
```  

### 4. Flashing the Code
1. Clone this repository:  
   ```bash
   git clone https://github.com/your-repo/ledstrip-esp32-webserver.git
   ```
2. Open the project in Arduino IDE or PlatformIO.
3. Upload the code to your ESP32 board.

### 5. Connecting to the Web Interface
1. Connect to the ESP32 Wi-Fi network (`ESP32` with password `espemoto123espemoto`).
2. Open a web browser and enter the ESP32's IP address.
3. Use the interface to control your LED strip and play music.

## Usage
- **Lighting Control:** Choose from Blink, Pulse, Rainbow, Moving Light, or static colors.
- **Music Control:** Play, stop, and switch between five different tracks stored on an SD card.
- **Adjust Settings:** Save your preferences for automatic restoration on boot.
