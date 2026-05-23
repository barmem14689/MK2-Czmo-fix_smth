# Cozmo-MK2 (исправленная версия)

[English](README.en.md) · [README (кратко)](README.md)

Небольшой робот на **ESP32-C3 Super Mini** с OLED-«лицом», двумя сервоприводами и управлением через браузер по Wi‑Fi.

Основано на: [nturmose21/MK2-Czmo](https://github.com/nturmose21/MK2-Czmo).

---

## Содержание

- [Возможности](#возможности)
- [Аппаратная часть](#аппаратная-часть)
- [Arduino IDE](#установка-и-настройка-arduino-ide)
- [Библиотеки](#библиотеки-важно)
- [Настройка Wi‑Fi](#настройка-wi-fi)
- [Сброс Wi‑Fi (WiFi-Fix)](#сброс-wi-fi-wifi-fix)
- [Прошивка и запуск](#прошивка-и-первый-запуск)
- [Использование](#как-пользоваться)
- [Serial Monitor](#отладка-через-serial-monitor)
- [Список изменений](#список-внесённых-изменений)
- [Неисправности](#устранение-неполадок)

---

## Возможности

- Управление с телефона или ПК через браузер
- Кнопки «удерживай, чтобы ехать»
- Анимации на OLED (`Emotions.h`, 110 кадров)
- Подключение к домашней Wi‑Fi (режим STA)
- Утилита **WiFi-Fix** для сброса сохранённых Wi‑Fi-настроек
- Отладка в Serial Monitor

---

## Аппаратная часть

| Компонент | Описание |
|-----------|----------|
| ESP32-C3 Super Mini | Контроллер и Wi‑Fi |
| SSD1306 0.96" OLED | I²C, адрес `0x3C` |
| 2× SG90 / MG90S | Приводы |
| Аккумулятор 3.7 V | Li-ion / Li-Po |

| Пин | Назначение |
|-----|------------|
| GPIO 6 | Левый серво |
| GPIO 7 | Правый серво |
| SDA / SCL | OLED (часто GPIO8 / GPIO9 на C3 Super Mini) |

---

## Установка и настройка Arduino IDE

1. [Arduino IDE](https://www.arduino.cc/en/software) 2.x
2. URL плат: `https://espressif.github.io/arduino-esp32/package_esp32_index.json`
3. Менеджер плат → **esp32** (Espressif)

| Параметр | Значение |
|----------|----------|
| Board | **ESP32C3 Dev Module** |
| USB CDC On Boot | **Enabled** |
| Flash Size | 4MB (по плате) |
| Serial | **115200** |

---

## Библиотеки (важно)

| Библиотека | Установка |
|------------|-----------|
| ESP32Servo | Менеджер библиотек |
| Adafruit GFX, Adafruit SSD1306 | Менеджер библиотек |
| [ESP32Async/AsyncTCP](https://github.com/ESP32Async/AsyncTCP) ≥ 3.3.2 | ZIP |
| [ESP32Async/ESPAsyncWebServer](https://github.com/ESP32Async/ESPAsyncWebServer) ≥ 3.6.2 | ZIP |

Удалите старые **me-no-dev** `AsyncTCP` и `ESPAsyncWebServer` — иначе ошибка:

```
assert failed: tcp_alloc ... Required to lock TCPIP core functionality!
```

Скетч **WiFi-Fix** дополнительных библиотек не требует (только ядро ESP32).

---

## Настройка Wi‑Fi

В `Cozmo-MK2.ino`:

```cpp
const char* ssid = "ИМЯ_ВАШЕЙ_WIFI";
const char* password = "ВАШ_ПАРОЛЬ";
```

Робот подключается к домашней сети (STA). Телефон или ПК для управления должны быть в **той же сети**.

Рекомендуется сеть **2.4 GHz** (ESP32-C3 не работает с 5 GHz-only Wi‑Fi).

---

## Сброс Wi‑Fi (WiFi-Fix)

Иногда ESP32 перестаёт подключаться к Wi‑Fi, даже если `ssid` и `password` в скетче **верные**. Частая причина — повреждённые или устаревшие данные во flash-памяти (NVS), куда ESP сохраняет настройки Wi‑Fi.

Для этого в проекте есть отдельный скетч: **`WiFi-Fix/WiFi-Fix.ino`**.

Он полностью стирает NVS (`nvs_flash_erase()`), после чего плата «забывает» старые Wi‑Fi-настройки.

### Когда использовать

- В Serial Monitor бесконечно идут точки `.....`, IP не появляется
- Раньше подключался, потом перестал при тех же `ssid` / `password`
- Меняли сеть или пароль, а плата всё равно не коннектится

### Порядок действий

1. Откройте в Arduino IDE папку **`WiFi-Fix`** и файл **`WiFi-Fix.ino`**.
2. Выберите ту же плату: **ESP32C3 Dev Module**, порт, **USB CDC On Boot → Enabled**.
3. **Загрузите** скетч WiFi-Fix на плату.
4. Откройте **Монитор порта** (**115200**). Должно появиться:
   ```
   Suscess!
   ```
5. Нажмите кнопку **RST** (Reset) на плате.
6. Снова откройте **`Cozmo-MK2.ino`**, проверьте `ssid` и `password`, **загрузите** основной скетч.
7. Дождитесь IP в Serial и на OLED, откройте URL в браузере.

> **Важно:** WiFi-Fix — только временная прошивка для сброса. После RST и успешного сообщения в Serial снова прошейте **Cozmo-MK2.ino**, иначе робот не будет управляться.

> Сброс NVS удаляет сохранённые Wi‑Fi-данные на чипе; ваши строки `ssid` / `password` в скетче не стираются.

---

## Прошивка и первый запуск

1. Откройте `Cozmo-MK2.ino`, задайте Wi‑Fi.
2. Загрузите скетч.
3. Монитор порта **115200**.

**Успешное подключение:**

```
.....
192.168.x.x
http://192.168.x.x/
```

На OLED до **Start**:

```
Connect to:
http://192.168.x.x/
```

---

## Как пользоваться

1. Дождитесь IP на OLED или в Serial.
2. Откройте URL в браузере, например `http://192.168.1.50/`
3. Нажмите **Start**.
4. **Удерживайте** кнопки движения (на ПК — мышью).

| Состояние | OLED | Серво |
|-----------|------|-------|
| До Start | URL для подключения | `updateCar()` работает |
| После Start | Анимация | Во время анимации |

---

## Отладка через Serial Monitor

| Действие | Сообщение |
|----------|-----------|
| Start | `WEB: /start` |
| Forward | `WEB: forward ON` / `OFF` |
| Backward | `WEB: backward ON` / `OFF` |
| Left | `WEB: left ON` / `OFF` |
| Right | `WEB: right ON` / `OFF` |
| Wiggle | `WEB: wiggle ON` / `OFF` |
| Потеря Wi‑Fi | `Wi-Fi lost` |

---

## Список внесённых изменений

Относительно [оригинала](https://github.com/nturmose21/MK2-Czmo/blob/main/Cozmo-MK2.ino).

1. **ESP32 Core 3.x** — библиотеки ESP32Async (см. выше).
2. **Ожидание Wi‑Fi** перед `server.begin()`.
3. **Безопасный URL на OLED** — `showConnectedUrl()` без битого `sprintf`.
4. **Forward / Backward** — исправлены перепутанные маршруты в HTML.
5. **Touch + mouse** — `bindHold()` для телефона и ПК.
6. **`loop()`** — понятные ветки, `updateCar()` до Start.
7. **Serial** во всех HTTP-обработчиках, включая left/right OFF.
8. **WiFi-Fix** — утилита сброса NVS при проблемах с Wi‑Fi.
9. **Документация** — `README.md`, `README.ru.md`, `README.en.md`.

---

## Устранение неполадок

| Проблема | Решение |
|----------|---------|
| `tcp_alloc` / reboot | ESP32Async библиотеки, удалить me-no-dev |
| Пустой Serial | 115200, USB CDC Enabled |
| Не подключается к Wi‑Fi (верный SSID) | Скетч **WiFi-Fix** → RST → снова **Cozmo-MK2.ino** |
| Кнопки не работают | Start, удерживать кнопку, обновить страницу |
| Моторы молчат | Start, питание серво, GPIO 6/7 |
| OLED не работает | Адрес `0x3C`, пины SDA/SCL |

---

## Файлы

| Файл | Назначение |
|------|------------|
| `Cozmo-MK2.ino` | Основной скетч робота |
| `WiFi-Fix/WiFi-Fix.ino` | Сброс NVS / Wi‑Fi |
| `Emotions.h` | Анимации |
| `README.md` | Краткий указатель |
| `README.ru.md` | Эта документация |
| `README.en.md` | English |

---

Оригинал: [nturmose21/MK2-Czmo](https://github.com/nturmose21/MK2-Czmo).
