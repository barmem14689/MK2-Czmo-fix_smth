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
- [Wi‑Fi: домашняя сеть и точка доступа](#wi-fi-домашняя-сеть-и-точка-доступа)
- [Прошивка и запуск](#прошивка-и-первый-запуск)
- [Использование](#как-пользоваться)
- [Язык веб-интерфейса](#язык-веб-интерфейса)
- [Serial Monitor](#отладка-через-serial-monitor)
- [Список изменений](#список-внесённых-изменений)
- [Неисправности](#устранение-неполадок)

---

## Возможности

- Управление с телефона или ПК через браузер
- Кнопки «удерживай, чтобы ехать»
- Анимации на OLED (`Emotions.h`, 110 кадров)
- Подключение к домашней Wi‑Fi **или** собственная точка доступа (AP), если домашняя сеть недоступна
- Переключатель языка веб-страницы **RU / EN**
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

---

## Wi‑Fi: домашняя сеть и точка доступа

### Режим STA (основной)

В `Cozmo-MK2.ino`:

```cpp
const char* ssid = "ИМЯ_ВАШЕЙ_WIFI";
const char* password = "ВАШ_ПАРОЛЬ";
```

Робот пытается подключиться **20 секунд**. Телефон/ПК — в **той же сети**.

### Режим AP (запасной)

Если STA не удался, робот поднимает свою точку доступа:

| Параметр | По умолчанию |
|----------|----------------|
| Имя сети (SSID) | `Cozmo-MK2` |
| Пароль | `cozmo1234` |
| IP робота | обычно `192.168.4.1` |

Настройка в скетче:

```cpp
const char* apSsid = "Cozmo-MK2";
const char* apPassword = "cozmo1234";  // минимум 8 символов
const unsigned long WIFI_CONNECT_TIMEOUT_MS = 20000;
```

**Как подключиться в режиме AP:**

1. На телефоне/ПК откройте список Wi‑Fi.
2. Подключитесь к **`Cozmo-MK2`**, пароль **`cozmo1234`**.
3. В браузере откройте **`http://192.168.4.1/`** (или URL с OLED).
4. Нажмите **Start**, управляйте как обычно.

На OLED в режиме AP: первая строка — имя AP, вторая — URL.

В Serial Monitor:

```
STA failed — starting AP fallback
AP SSID: Cozmo-MK2
AP pass: cozmo1234
AP IP: 192.168.4.1
```

---

## Прошивка и первый запуск

1. Откройте `Cozmo-MK2.ino`, задайте Wi‑Fi (и при желании AP).
2. Загрузите скетч.
3. Монитор порта **115200**.

**Успешный STA:**

```
Connecting to EgorNet
.....
STA OK, IP: 192.168.x.x
```

**Переход в AP:**

```
Connecting to EgorNet
....................
STA failed — starting AP fallback
AP SSID: Cozmo-MK2
...
```

---

## Как пользоваться

1. Дождитесь IP на OLED или сообщения AP в Serial.
2. Откройте URL в браузере (`http://192.168.x.x/` или `http://192.168.4.1/`).
3. При необходимости выберите **RU** или **EN** вверху страницы.
4. Нажмите **Start** / **Старт**.
5. **Удерживайте** кнопки движения.

| Состояние | OLED | Серво |
|-----------|------|-------|
| До Start | URL / имя AP | `updateCar()` работает |
| После Start | Анимация | Во время анимации |

---

## Язык веб-интерфейса

- Кнопки **RU** и **EN** вверху страницы управления.
- Выбор сохраняется в браузере (`localStorage`, ключ `cozmoLang`).
- При первом открытии: русский, если язык системы начинается с `ru`, иначе английский.
- В режиме AP показывается подсказка с именем и паролем сети на выбранном языке.

Язык OLED (Connect to / AP) — только английский (краткие строки на маленьком экране).

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
| Потеря сети (STA) | `Wi-Fi lost` |

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
8. **Точка доступа (AP)** — если STA не подключился за 20 с, включается `WiFi.softAP()`.
9. **Язык веб-UI** — RU/EN, `localStorage`, баннер режима AP.
10. **Документация** — `README.md`, `README.ru.md`, `README.en.md`.

---

## Устранение неполадок

| Проблема | Решение |
|----------|---------|
| `tcp_alloc` / reboot | ESP32Async библиотеки, удалить me-no-dev |
| Пустой Serial | 115200, USB CDC Enabled |
| Не открывается страница в AP | Подключены к `Cozmo-MK2`, не к домашней сети |
| Кнопки не работают | Start, удерживать кнопку, обновить страницу |
| Всегда режим AP | Проверьте `ssid`/`password`, роутер 2.4 GHz |
| Моторы молчат | Start, питание серво, GPIO 6/7 |

---

## Файлы

| Файл | Назначение |
|------|------------|
| `Cozmo-MK2.ino` | Скетч |
| `Emotions.h` | Анимации |
| `README.md` | Краткий указатель языков |
| `README.ru.md` | Эта документация |
| `README.en.md` | English |

---

Оригинал: [nturmose21/MK2-Czmo](https://github.com/nturmose21/MK2-Czmo).
