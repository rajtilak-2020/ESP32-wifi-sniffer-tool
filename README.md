# 📡 ESP32 Wi-Fi Sniffer Tool

A real-time, web-based Wi-Fi packet sniffer built using an ESP32. This project captures and displays nearby Wi-Fi packets (in promiscuous mode) along with MAC addresses, signal strength (RSSI), and packet types. The captured data is displayed on an auto-refreshing dashboard served via a built-in web server on the ESP32.

> ⚠️ **Disclaimer**: This tool is intended strictly for educational and ethical testing purposes on networks you own or have permission to analyze.

---


## 🧠 Features

- 📶 Sniffs Wi-Fi packets in **promiscuous mode**
- 🧾 Captures **MAC addresses**, **packet types**, and **RSSI**
- 🌐 Hosts a **web dashboard** from the ESP32
- 🔄 Auto-refreshes every 3 seconds
- 🧼 Responsive and clean **UI using HTML + CSS**
- ⚙️ Written in Arduino C++ with **no external backend**

---

## 🎯 Use Cases

- Network analysis for educational purposes
- Signal strength (RSSI) testing for devices
- Wi-Fi presence detection
- Learning embedded systems and web integration with ESP32

---

## 📊 Architecture Overview

```mermaid
flowchart TD
    A[ESP32 Microcontroller] --> B[Wi-Fi Promiscuous Mode]
    B --> C{Sniffer Callback}
    C --> D[Extract MAC, RSSI, Packet Type]
    D --> E[Format as HTML Table]
    E --> F[WebServer-Port 80]
    F --> G[User Browser]
    G --> H[Auto-refresh Every 3s]
````

---

## ⚙️ Hardware & Software Requirements

### Hardware

* ✅ ESP32 Dev Board (NodeMCU, DOIT, etc.)
* ✅ USB cable
* ✅ Wi-Fi network

### Software

* Arduino IDE with ESP32 board support
* Required libraries:

  * `WiFi.h`
  * `esp_wifi.h`
  * `WebServer.h`

---

## 🚀 Getting Started

### 1. 🔧 Setup Arduino IDE

* Install [Arduino IDE](https://www.arduino.cc/en/software)
* Install the ESP32 board via Board Manager
  `Tools → Board → Boards Manager → esp32`

### 2. 📥 Upload the Code

* Open the provided `.ino` file
* Replace Wi-Fi credentials:

```cpp
const char* ssid = "Your_Wi-Fi_SSID";
const char* password = "Your_Wi-Fi_Password";
```

* Select your ESP32 board and port
* Upload the sketch

### 3. 📡 Connect & Monitor

* Open Serial Monitor at `115200`
* After connection, note the ESP32's IP:

  ```
  ESP32 IP: 192.168.X.X
  ```
* Visit this IP in your browser to view the dashboard.

---

## 📁 Project Structure

```plaintext
ESP32-WiFi-Sniffer/
├── WiFiSniffer.ino         # Main Arduino sketch
├── README.md               # This documentation
└── /data/                  # (Optional) Store logs or saved packets
```

---

## 🔍 What It Detects

| Field       | Description                                                 |
| ----------- | ----------------------------------------------------------- |
| MAC Address | Device's hardware network address (source from Wi-Fi frame) |
| Packet Type | Type of 802.11 frame: Management, Control, or Data          |
| RSSI (dBm)  | Signal strength; lower is weaker (e.g., -90 dBm = weak)     |

> Note: No personal data is captured. The project only logs metadata.

---

## 🧪 Example Output

```html
<tr>
  <td>DE:AD:BE:EF:00:11</td>
  <td>Data</td>
  <td>-67 dBm</td>
</tr>
<tr>
  <td>12:34:56:78:9A:BC</td>
  <td>Management</td>
  <td>-73 dBm</td>
</tr>
```

---

## 🛡️ Legal & Ethical Considerations

Using promiscuous mode means **capturing wireless packets not intended for you**, which may:

* Be illegal in your country without proper authorization
* Violate terms of service or ethical use policies

Please ensure:

* You **only test on networks you own or manage**
* You **use this for research, security education, or academic learning**

---

## 🧑‍💻 Author

**K Rajtilak**

📍Student | CSE-Cyber Security

🔗[GitHub: @rajtilak-2020](https://github.com/rajtilak-2020)

---

## 📜 License

This project is licensed under the [MIT License](LICENSE).
Feel free to fork, modify, and share under the terms of open source.

---
