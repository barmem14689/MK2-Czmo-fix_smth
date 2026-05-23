# Cozmo-MK2 (patched build)

[Русский](README.ru.md) · [README (short)](README.md)

A small **ESP32-C3 Super Mini** robot with an OLED “face”, two servos, and browser control over Wi-Fi.

Based on: [nturmose21/MK2-Czmo](https://github.com/nturmose21/MK2-Czmo).

---

## Contents

- [Features](#features)
- [Hardware](#hardware)
- [Arduino IDE](#arduino-ide-setup)
- [Libraries](#libraries-important)
- [Wi-Fi setup](#wi-fi-setup)
- [Wi-Fi reset (WiFi-Fix)](#wi-fi-reset-wifi-fix)
- [Flash and first run](#flash-and-first-run)
- [Usage](#usage)
- [Serial Monitor](#serial-monitor-debug)
- [Changelog](#changelog)
- [Troubleshooting](#troubleshooting)

---

## Features

- Control from phone or PC in the browser
- Hold-to-drive buttons
- OLED emotion animations (`Emotions.h`, 110 frames)
- Home Wi-Fi (STA mode)
- **WiFi-Fix** utility to clear stored Wi-Fi settings
- Serial Monitor debug output

---

## Hardware

| Part | Notes |
|------|-------|
| ESP32-C3 Super Mini | MCU + Wi-Fi |
| SSD1306 0.96" OLED | I²C, address `0x3C` |
| 2× SG90 / MG90S | Drive servos |
| 3.7 V battery | Li-ion / Li-Po |

| Pin | Function |
|-----|----------|
| GPIO 6 | Left servo signal |
| GPIO 7 | Right servo signal |
| SDA / SCL | OLED (often GPIO8 / GPIO9 on C3 Super Mini) |

---

## Arduino IDE setup

1. Install [Arduino IDE](https://www.arduino.cc/en/software) 2.x
2. Board manager URL: `https://espressif.github.io/arduino-esp32/package_esp32_index.json`
3. Install **esp32** by Espressif

| Setting | Value |
|---------|-------|
| Board | **ESP32C3 Dev Module** |
| USB CDC On Boot | **Enabled** |
| Flash Size | 4MB (as on your board) |
| Serial speed | **115200** |

---

## Libraries (important)

| Library | Install |
|---------|---------|
| ESP32Servo | Library Manager |
| Adafruit GFX, Adafruit SSD1306 | Library Manager |
| [ESP32Async/AsyncTCP](https://github.com/ESP32Async/AsyncTCP) ≥ 3.3.2 | ZIP |
| [ESP32Async/ESPAsyncWebServer](https://github.com/ESP32Async/ESPAsyncWebServer) ≥ 3.6.2 | ZIP |

Remove old **me-no-dev** `AsyncTCP` and `ESPAsyncWebServer` copies. Otherwise you may get:

```
assert failed: tcp_alloc ... Required to lock TCPIP core functionality!
```

The **WiFi-Fix** sketch needs no extra libraries (ESP32 core only).

---

## Wi-Fi setup

In `Cozmo-MK2.ino`:

```cpp
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";
```

The robot joins your home network (STA). Your phone/PC must be on the **same network**.

Use a **2.4 GHz** network (ESP32-C3 does not support 5 GHz-only Wi-Fi).

---

## Wi-Fi reset (WiFi-Fix)

Sometimes the ESP32 stops connecting even when `ssid` and `password` in the sketch are **correct**. A common cause is corrupted or outdated data in flash (NVS), where the ESP stores Wi-Fi settings.

The project includes a separate sketch for this: **`WiFi-Fix/WiFi-Fix.ino`**.

It fully erases NVS (`nvs_flash_erase()`), so the board forgets old Wi-Fi configuration.

### When to use it

- Serial Monitor shows endless dots `.....` and no IP
- It used to connect, then stopped with the same `ssid` / `password`
- You changed network or password but the board still won’t connect

### Steps

1. In Arduino IDE, open the **`WiFi-Fix`** folder and **`WiFi-Fix.ino`**.
2. Select the same board: **ESP32C3 Dev Module**, port, **USB CDC On Boot → Enabled**.
3. **Upload** the WiFi-Fix sketch.
4. Open **Serial Monitor** at **115200**. You should see:
   ```
   Suscess!
   ```
5. Press the **RST** (Reset) button on the board.
6. Open **`Cozmo-MK2.ino`** again, check `ssid` and `password`, and **upload** the main sketch.
7. Wait for the IP in Serial and on the OLED, then open the URL in your browser.

> **Important:** WiFi-Fix is only a temporary firmware for reset. After RST and the Serial message, flash **Cozmo-MK2.ino** again — otherwise the robot will not run.

> NVS erase clears Wi-Fi data stored on the chip; your `ssid` / `password` lines in the sketch are not deleted.

---

## Flash and first run

1. Open `Cozmo-MK2.ino`, set Wi-Fi credentials.
2. Upload the sketch.
3. Open Serial Monitor at **115200**.

**Successful connection:**

```
.....
192.168.x.x
http://192.168.x.x/
```

On the OLED before **Start**:

```
Connect to:
http://192.168.x.x/
```

---

## Usage

1. Wait for the IP on the OLED or in Serial.
2. Open the URL in a browser, e.g. `http://192.168.1.50/`
3. Press **Start**.
4. **Hold** the drive buttons (on PC — with the mouse).

| State | OLED | Servos |
|-------|------|--------|
| Before Start | Connection URL | `updateCar()` active |
| After Start | Animation | Updated during animation |

---

## Serial Monitor debug

| Action | Message |
|--------|---------|
| Start | `WEB: /start` |
| Forward | `WEB: forward ON` / `OFF` |
| Backward | `WEB: backward ON` / `OFF` |
| Left | `WEB: left ON` / `OFF` |
| Right | `WEB: right ON` / `OFF` |
| Wiggle | `WEB: wiggle ON` / `OFF` |
| Wi-Fi lost | `Wi-Fi lost` |

---

## Changelog

Compared to the [original sketch](https://github.com/nturmose21/MK2-Czmo/blob/main/Cozmo-MK2.ino).

1. **ESP32 Core 3.x** — ESP32Async libraries (see above).
2. **Wi-Fi wait** before `server.begin()`.
3. **Safe OLED URL** — `showConnectedUrl()` without broken `sprintf`.
4. **Forward / Backward** — fixed swapped HTML routes.
5. **Touch + mouse** — `bindHold()` for phone and PC.
6. **`loop()`** — clear branches, `updateCar()` before Start.
7. **Serial logging** on all HTTP handlers, including left/right OFF.
8. **WiFi-Fix** — NVS erase utility for Wi-Fi connection issues.
9. **Docs** — `README.md`, `README.ru.md`, `README.en.md`.

---

## Troubleshooting

| Issue | Fix |
|-------|-----|
| `tcp_alloc` reboot loop | Install ESP32Async libs, remove me-no-dev |
| Empty Serial | 115200 baud, USB CDC Enabled |
| Won’t connect (correct SSID) | Flash **WiFi-Fix** → press **RST** → flash **Cozmo-MK2.ino** |
| Buttons don’t work | Press Start, hold buttons, refresh page |
| Servos don’t move | Press Start, check power and GPIO 6/7 |
| OLED blank | Address `0x3C`, check SDA/SCL pins |

---

## Files

| File | Description |
|------|-------------|
| `Cozmo-MK2.ino` | Main robot sketch |
| `WiFi-Fix/WiFi-Fix.ino` | NVS / Wi-Fi reset utility |
| `Emotions.h` | Animations |
| `README.md` | Language index |
| `README.ru.md` | Russian docs |
| `README.en.md` | This file |

---

Original project: [nturmose21/MK2-Czmo](https://github.com/nturmose21/MK2-Czmo).
