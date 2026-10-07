/*
  RoboEyes ESP8266 + SH1106 128x64
  --------------------------------
  Hardware:
    - ESP8266 NodeMCU / ESP-12E or compatible
    - 1.3" SH1106 128x64 I2C OLED
    - 1x momentary push button

  Wiring:
    OLED GND -> GND
    OLED VCC -> 3.3V
    OLED SDA -> GPIO4  (NodeMCU D2)
    OLED SCL -> GPIO5  (NodeMCU D1)
    BUTTON   -> GPIO14 (NodeMCU D5) and GND

  Libraries:
    - Adafruit GFX Library
    - Adafruit SH110X
    - FluxGarage RoboEyes

  Notes:
    - Uses GPIO numbers instead of D1/D2/D5 aliases to avoid board-definition issues.
    - OLED address defaults to 0x3C. If your display is not detected, try 0x3D.
*/

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

// ESP8266 core may define DEFAULT. Remove it before RoboEyes is included.
#ifdef DEFAULT
#undef DEFAULT
#endif

#include <FluxGarage_RoboEyes.h>

#define SCREEN_WIDTH   128
#define SCREEN_HEIGHT   64
#define OLED_RESET      -1
#define OLED_ADDRESS    0x3C

#define OLED_SDA         4   // GPIO4  = NodeMCU D2
#define OLED_SCL         5   // GPIO5  = NodeMCU D1
#define BUTTON_PIN      14   // GPIO14 = NodeMCU D5

Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
RoboEyes<Adafruit_SH1106G> roboEyes(display);

bool buttonStableState = HIGH;
bool lastButtonReading = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

uint8_t faceMode = 0;
const uint8_t FACE_COUNT = 5;

void changeFace(uint8_t mode);
void readButton();

void setup() {
  Serial.begin(115200);
  delay(100);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Wire.begin(OLED_SDA, OLED_SCL);
  Wire.setClock(400000);

  if (!display.begin(OLED_ADDRESS, true)) {
    Serial.println();
    Serial.println("ERROR: SH1106 OLED not found.");
    Serial.println("Check wiring and OLED I2C address (0x3C / 0x3D).");
    while (true) {
      delay(100);
      yield();
    }
  }

  display.clearDisplay();
  display.display();
  delay(250);

  roboEyes.begin(SCREEN_WIDTH, SCREEN_HEIGHT, 50);
  roboEyes.setWidth(36, 36);
  roboEyes.setHeight(36, 36);
  roboEyes.setBorderradius(8, 8);
  roboEyes.setSpacebetween(10);

  roboEyes.setMood(DEFAULT);
  roboEyes.setPosition(DEFAULT);
  roboEyes.setCuriosity(OFF);
  roboEyes.setHFlicker(OFF, 0);
  roboEyes.setVFlicker(OFF, 0);

  roboEyes.setAutoblinker(ON, 3, 2);
  roboEyes.setIdleMode(ON, 2, 2);
  roboEyes.open();

  Serial.println();
  Serial.println("RoboEyes ESP8266 ready.");
}

void loop() {
  roboEyes.update();
  readButton();
  yield();
}

void readButton() {
  const bool reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonReading) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonStableState) {
      buttonStableState = reading;

      if (buttonStableState == LOW) {
        faceMode++;
        if (faceMode >= FACE_COUNT) {
          faceMode = 0;
        }

        changeFace(faceMode);

        Serial.print("Face mode: ");
        Serial.println(faceMode);
      }
    }
  }

  lastButtonReading = reading;
}

void changeFace(uint8_t mode) {
  roboEyes.setCuriosity(OFF);
  roboEyes.setHFlicker(OFF, 0);
  roboEyes.setVFlicker(OFF, 0);

  switch (mode) {
    case 0:
      roboEyes.setMood(DEFAULT);
      roboEyes.setPosition(DEFAULT);
      break;

    case 1:
      roboEyes.setMood(HAPPY);
      roboEyes.setPosition(DEFAULT);
      roboEyes.anim_laugh();
      break;

    case 2:
      roboEyes.setMood(DEFAULT);
      roboEyes.setCuriosity(ON);
      roboEyes.setPosition(E);
      break;

    case 3:
      roboEyes.setMood(ANGRY);
      roboEyes.setPosition(DEFAULT);
      roboEyes.anim_confused();
      break;

    case 4:
      roboEyes.setMood(TIRED);
      roboEyes.setPosition(DEFAULT);
      break;
  }
}
