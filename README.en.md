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
- [Wi-Fi: home network and access point](#wi-fi-home-network-and-access-point)
- [Flash and first run](#flash-and-first-run)
- [Usage](#usage)
- [Web UI language](#web-ui-language)
- [Serial Monitor](#serial-monitor-debug)
- [Changelog](#changelog)
- [Troubleshooting](#troubleshooting)

---

## Features

- Control from phone or PC in the browser
- Hold-to-drive buttons
- OLED emotion animations (`Emotions.h`, 110 frames)
- Connect to home Wi-Fi **or** built-in **access point (AP)** if home Wi-Fi fails
- Web page language switch **RU / EN**
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

---

## Wi-Fi: home network and access point

### STA mode (primary)

In `Cozmo-MK2.ino`:

```cpp
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";
```

The robot tries to connect for **20 seconds**. Your phone/PC must be on the **same network**.

### AP mode (fallback)

If STA fails, the robot starts its own access point:

| Setting | Default |
|---------|---------|
| Network name (SSID) | `Cozmo-MK2` |
| Password | `cozmo1234` |
| Robot IP | usually `192.168.4.1` |

Configure in the sketch:

```cpp
const char* apSsid = "Cozmo-MK2";
const char* apPassword = "cozmo1234";  // WPA2: min. 8 characters
const unsigned long WIFI_CONNECT_TIMEOUT_MS = 20000;
```

**How to use AP mode:**

1. On your phone/PC, open Wi-Fi settings.
2. Connect to **`Cozmo-MK2`**, password **`cozmo1234`**.
3. In the browser open **`http://192.168.4.1/`** (or the URL on the OLED).
4. Press **Start** and drive as usual.

On the OLED in AP mode: line 1 = AP name, line 2 = URL.

Serial Monitor example:

```
STA failed — starting AP fallback
AP SSID: Cozmo-MK2
AP pass: cozmo1234
AP IP: 192.168.4.1
```

---

## Flash and first run

1. Open `Cozmo-MK2.ino`, set Wi-Fi (and AP if needed).
2. Upload the sketch.
3. Open Serial Monitor at **115200**.

**Successful STA:**

```
Connecting to MyNetwork
.....
STA OK, IP: 192.168.x.x
```

**Fallback to AP:**

```
Connecting to MyNetwork
....................
STA failed — starting AP fallback
AP SSID: Cozmo-MK2
...
```

---

## Usage

1. Wait for the IP on the OLED or AP info in Serial.
2. Open the URL in a browser (`http://192.168.x.x/` or `http://192.168.4.1/`).
3. Optionally choose **RU** or **EN** at the top of the page.
4. Press **Start**.
5. **Hold** the drive buttons (do not just tap).

| State | OLED | Servos |
|-------|------|--------|
| Before Start | URL / AP name | `updateCar()` active |
| After Start | Animation | Updated during animation |

---

## Web UI language

- **RU** and **EN** buttons at the top of the control page.
- Choice is saved in the browser (`localStorage`, key `cozmoLang`).
- First visit: Russian if the system language starts with `ru`, otherwise English.
- In AP mode, a banner shows the AP name and password in the selected language.

OLED status text stays short English (small display).

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
| Network lost (STA) | `Wi-Fi lost` |

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
8. **Access point (AP)** — `WiFi.softAP()` if STA fails within 20 s.
9. **Web UI i18n** — RU/EN, `localStorage`, AP mode banner.
10. **Docs** — `README.md`, `README.ru.md`, `README.en.md`.

---

## Troubleshooting

| Issue | Fix |
|-------|-----|
| `tcp_alloc` reboot loop | Install ESP32Async libs, remove me-no-dev |
| Empty Serial | 115200 baud, USB CDC Enabled |
| Page won’t open in AP | Join `Cozmo-MK2`, not your home Wi-Fi |
| Buttons don’t work | Press Start, hold buttons, refresh page |
| Always AP mode | Check `ssid`/`password`, use 2.4 GHz router |
| Servos don’t move | Press Start, check power and GPIO 6/7 |

---

## Files

| File | Description |
|------|-------------|
| `Cozmo-MK2.ino` | Main sketch |
| `Emotions.h` | Animations |
| `README.md` | Language index |
| `README.ru.md` | Russian docs |
| `README.en.md` | This file |

---

Original project: [nturmose21/MK2-Czmo](https://github.com/nturmose21/MK2-Czmo).
