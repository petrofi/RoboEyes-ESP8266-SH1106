# Troubleshooting

## 1. The sketch compiles but OLED stays black

Verify wiring:

- OLED GND → ESP8266 GND
- OLED VCC → ESP8266 3.3V
- OLED SDA → GPIO4 / D2
- OLED SCL → GPIO5 / D1

Then try changing:

```cpp
#define OLED_ADDRESS 0x3C
```

to:

```cpp
#define OLED_ADDRESS 0x3D
```

## 2. `D1`, `D2`, or `D5` is not declared

Use the included sketch. It uses raw GPIO numbers:

- GPIO4 instead of D2
- GPIO5 instead of D1
- GPIO14 instead of D5

## 3. Upload error

Try this checklist:

1. Select `NodeMCU 1.0 (ESP-12E Module)`.
2. Select the correct COM port.
3. Close Serial Monitor.
4. Set upload speed to 115200.
5. Use a known-good USB data cable.
6. Try another USB port.

## 4. Reset loop / boot problems

Do not move the button to ESP8266 boot-strap pins unless you understand the boot requirements.

This project deliberately uses GPIO14 / D5 for the button.

## 5. RoboEyes library missing

Install `FluxGarage RoboEyes` through Arduino Library Manager.

Official source:

https://github.com/FluxGarage/RoboEyes

## 6. SH1106 library missing

Install:

- Adafruit GFX Library
- Adafruit SH110X

## 7. Button changes mode more than once per press

The included sketch contains a 50 ms debounce. If your switch is unusually noisy, increase:

```cpp
const unsigned long debounceDelay = 50;
```

to e.g.:

```cpp
const unsigned long debounceDelay = 80;
```
