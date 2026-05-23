# Cozmo-MK2

Small ESP32-C3 robot with OLED face, dual servos, and browser control over Wi-Fi.

Based on [nturmose21/MK2-Czmo](https://github.com/nturmose21/MK2-Czmo) — patched sketch for ESP32 Core 3.x and web UI fixes.

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
4. Open Serial Monitor **115200** — note the IP address.
5. Browser: open the URL from the OLED, press **Start**, hold drive buttons.

### Wi-Fi won’t connect (correct SSID and password)

Use the **`WiFi-Fix`** sketch — see [README.ru.md — Сброс Wi‑Fi](README.ru.md#сброс-wi-fi-wifi-fix) or [README.en.md — Wi-Fi reset](README.en.md#wi-fi-reset-wifi-fix).

---

## Project files

| File / folder | Description |
|---------------|-------------|
| `Cozmo-MK2.ino` | Main robot sketch |
| `WiFi-Fix/WiFi-Fix.ino` | NVS erase utility when Wi-Fi stops working |
| `Emotions.h` | Face animations (110 frames) |
| `README.md` | This index |
| `README.en.md` | Full English docs |
| `README.ru.md` | Full Russian docs |
