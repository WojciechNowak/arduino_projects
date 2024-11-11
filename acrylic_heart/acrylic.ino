#include <pins_arduino.h>
#include <Adafruit_NeoPixel.h>

const unsigned int cNumLed = 8;
const unsigned int cLedPin = D1;
Adafruit_NeoPixel strip(cNumLed, cLedPin, NEO_GRB + NEO_KHZ800);

const uint8_t cStartSat = 200;
const uint8_t cStartBrightness = 80;
const uint32_t cStartCol = 0;

const uint8_t cBrightnessStep = 2;
const uint8_t cBrightnessChangeCount = cStartBrightness / cBrightnessStep;

const uint16_t cColSingleStep = 10922;
const uint8_t cColChangeCount = 6;

const unsigned int cColorStayTime = 60 * 1000;
const unsigned int cBrightnessDelayTime = 100;

void setup() {
  strip.begin();
  strip.show();
  strip.setBrightness(cStartBrightness);
}

void loop() {
  unsigned int currentColor = cStartCol;
  for(uint8_t i = 0; i < cColChangeCount; i++) {
    currentColor = changeColor(currentColor);
    delay(cColorStayTime);
    changeBrightness(currentColor);
  }
}

uint32_t changeColor(uint32_t color) {
  color += cColSingleStep;
  uint32_t rgbcolor = strip.ColorHSV(color, cStartSat, cStartBrightness);
  strip.fill(rgbcolor, 0 , cNumLed);
  strip.show();
  return color;
}

void changeBrightness(unsigned int color) {
  uint8_t brightness = cStartBrightness;
  for(uint8_t i = 0; i < cBrightnessChangeCount; i++)
  {
    uint32_t rgbcolor = strip.ColorHSV(color, cStartSat, brightness);   
    strip.fill(rgbcolor, 0 , cNumLed);
    strip.show();
    delay(cBrightnessDelayTime);
    brightness = brightness - cBrightnessStep;
  }
}
