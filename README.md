# Cozmo-MK2

Small ESP32-C3 robot with OLED face, dual servos, and browser control over Wi-Fi.

Based on [nturmose21/MK2-Czmo](https://github.com/nturmose21/MK2-Czmo) — this folder contains a patched sketch (ESP32 Core 3.x, web UI, AP fallback).

---

## Documentation / Документация

| Language | File |
|----------|------|
| **English** | [README.en.md](README.en.md) |
| **Русский** | [README.ru.md](README.ru.md) |

---

## Quick start

1. Install libraries (see full README) — **ESP32Async** AsyncTCP + ESPAsyncWebServer.
2. Set `ssid` / `password` in `Cozmo-MK2.ino`.
3. Flash **ESP32C3 Dev Module**, USB CDC On Boot **Enabled**.
4. Open Serial Monitor **115200** — note IP or AP details.
5. Browser: open the URL from the OLED, press **Start**, hold drive buttons.

### Wi-Fi modes

| Mode | When | How to connect |
|------|------|----------------|
| **STA** (default) | Home Wi-Fi OK within ~20 s | Same network as robot → `http://<STA-IP>/` |

enter your ssid and pass in const char* ssid and const char* password


## Project files

| File | Description |
|------|-------------|
| `Cozmo-MK2.ino` | Main sketch |
| `Emotions.h` | Face animations (110 frames) |
| `README.md` | This index |
| `README.en.md` | Full English docs |
| `README.ru.md` | Full Russian docs |
