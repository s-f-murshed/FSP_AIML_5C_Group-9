# 🔔 Smart Classroom Timetable & Automatic Bell System (IoT)

An IoT-driven automated period bell and digital door display system simulated on ESP32 using Wokwi. It synchronizes live time via Network Time Protocol (NTP), displays current lecture and faculty details on an SSD1306 OLED screen, and actuates an industrial bell/buzzer via relay at schedule transitions.

---

## Team Members
| Name | Roll No.| 
| :--- | :--- |
| Suroj Halder | 13030825164|
| Sufi Farhan Murshed | 13030825163|
| Sudip Majumdar| 13030825162|
| Suvadeep Mandal| 13030825165|
| Sayan Saha| 13030825166|
| Abhinav Kumar| 13030823002|


## 📌 Project Overview
Manual college bells are prone to timing drifts and human delays, while door schedules are typically printed on static paper. This project solves both issues:
- **Centralized Timekeeping:** Uses standard NTP (Pool.ntp.org) over Wi-Fi, eliminating manual clock drift.
- **Smart Classroom Signage:** Displays real-time subject, teacher, and room availability outside the lecture hall.
- **Automated Bell Actuation:** Triggers an optocoupler-isolated relay module at exact timetable milestones.
- **Offline Simulation:** Fully runnable and testable via Wokwi without physical hardware dependencies.

---

## 🚀 Live Simulation
Run the full simulation directly in your browser:
👉 **[Open in Wokwi Simulator](https://wokwi.com/projects/477768574062728193)**

---

## 🛠️ Hardware & Pin Configuration (Simulated)

| Component | ESP32 Pin | Logic Level | Description |
| :--- | :--- | :--- | :--- |
| **SSD1306 OLED (SDA)** | `GPIO 21` | 3.3V | I2C Data line |
| **SSD1306 OLED (SCL)** | `GPIO 22` | 3.3V | I2C Clock line |
| **Relay Module (IN)**  | `GPIO 18` | 5V / 3.3V | Active HIGH trigger |
| **Piezo Buzzer**       | Relay `NO` | 5V | Represents high-voltage bell |

---

## 📂 Repository Contents
- `code.ino`: Main ESP32 application logic, NTP synchronization, and slot parser.
- `diagram.json`: Circuit wiring layout and component positions for Wokwi.
- `library.txt`: Adafruit GFX and SSD1306 library declarations.

---
