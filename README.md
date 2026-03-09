# ESP Data Dashboard

A lightweight, real-time data visualization and hardware control dashboard for the ESP8266 (NodeMCU/Wemos D1 Mini). Designed for educational experiments, this project allows students to monitor sensors and trigger actuators via a responsive web interface.

## Features

- **Real-time Monitoring**: Live time-series charts for analog and digital sensors.
- **Hardware Control**: Automatically trigger LEDs, motors, or fans when an experiment starts.
- **Data Export**: Download recorded experiment data as a `.CSV` file for analysis in Excel or Google Sheets.
- **Dynamic UI**: The dashboard automatically adjusts its layout based on the sensors and outputs defined in the Arduino code.
- **NTP Time Sync**: Accurate timestamps for all data points using Network Time Protocol.

## Hardware Requirements

- **Microcontroller**: ESP8266 (e.g., NodeMCU, Wemos D1 Mini).
- **Sensors**: Any analog (A0) or digital sensors (e.g., Temperature, Light, Soil Moisture).
- **Actuators**: LEDs, Relays, or Motors (connected via Transistors/Motor Drivers).

## Software Setup

### 1. Prerequisites
- [Arduino IDE](https://www.arduino.cc/en/software)
- ESP8266 Board Support (Add `http://arduino.esp8266.com/stable/package_esp8266com_index.json` to Boards Manager).

### 2. Configuration
Open `ESPDataDashboard.ino` and complete the following **STUDENT TASKS**:

- **WiFi**: Enter your SSID and Password.
- **Sensors**: Define your sensor reading functions and register them in the `sensors[]` array.
- **Outputs**: Register your actuator pins (LEDs, Motors) in the `outputs[]` array.

### 3. Upload
Upload the code to your ESP8266. Once connected, the Serial Monitor will display the IP address (e.g., `192.168.1.50`). Open this address in any web browser on the same network.

## Dashboard Usage

1. **Live View**: View instantaneous sensor readings in the cards at the top.
2. **Recording**: Click **Start Recording** to begin logging data and activate connected hardware.
3. **Analyze**: The charts will scale automatically as data is collected.
4. **Export**: Click **Download CSV** to save your results.

## Project Structure

- `ESPDataDashboard.ino`: Main logic, WiFi, and Web API.
- `index_html.h`: HTML structure of the dashboard.
- `style_css.h`: Visual styling and responsive layout.
- `script_js.h`: Frontend logic, charting engine, and data polling.

## API Endpoints

The ESP8266 serves the following REST API:
- `GET /api/config`: Returns the list of registered sensors and outputs.
- `GET /api/data`: Returns current sensor values, time, and recording status.
- `GET /api/start`: Starts recording and activates hardware.
- `GET /api/stop`: Stops recording and deactivates hardware.

---
*Developed for PRISMS Student Projects.*
