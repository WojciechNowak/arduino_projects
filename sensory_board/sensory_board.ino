#include <SD.h>
#include <SPI.h>
#include <TMRpcm.h>
#include <Adafruit_NeoPixel.h>

#define SPI_CS_PIN 10

#define LED_CONTROL_PIN 2
#define LED_RED_BUTTON 3
#define LED_GREEN_BUTTON 4
#define LED_BLUE_BUTTON 5
#define LED_LIGHTS_BUTTON 6
#define LED_LIGHTS_SWITCH A0
#define NUMPIXELS 45

#define AUDIO_SPEAKER_PIN 9 //5, 6, 11, 46 on mega
#define AUDIO_BLUE_BUTTON 8
#define AUDIO_RED_BUTTON 7

TMRpcm tmrpcm;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, LED_CONTROL_PIN, NEO_GRB + NEO_KHZ800);
static const uint32_t offColor = pixels.Color(0, 0, 0);
uint32_t currentColor = offColor;
int oldLedButtonState = HIGH;

void setup() {
  Serial.begin(9600);

  // light
  pixels.begin(); // This initializes the NeoPixel library.
  pixels.setBrightness(2); // 100/255 brightness (about 40%)
  pixels.show();             // Initialize all pixels to 'off'
  pinMode(LED_RED_BUTTON, INPUT_PULLUP);
  pinMode(LED_GREEN_BUTTON, INPUT_PULLUP);
  pinMode(LED_BLUE_BUTTON, INPUT_PULLUP);

  // led light strip
  pinMode(LED_LIGHTS_BUTTON, INPUT_PULLUP);
  pinMode(LED_LIGHTS_SWITCH, OUTPUT);
  digitalWrite(LED_LIGHTS_SWITCH, HIGH);
  
  // mp3
  pinMode(AUDIO_BLUE_BUTTON, INPUT_PULLUP);
  pinMode(AUDIO_RED_BUTTON, INPUT_PULLUP);
  tmrpcm.speakerPin = AUDIO_SPEAKER_PIN;
  tmrpcm.setVolume(5);

  // SD CARD
  pinMode(SPI_CS_PIN, OUTPUT);
  if (!SD.begin(SPI_CS_PIN))
  {
    Serial.println("Failed to open SD card");
    return;
  }
}

void loop() {
  Serial.println("START");
  AudioHandler();
  LedHandler();
  LedLightsHandler();
  delay(100);
  Serial.println("DONE");
}

void AudioHandler()
{
  Serial.println("AUDIO START");
  if (!tmrpcm.isPlaying())
  {
    if (digitalRead(AUDIO_BLUE_BUTTON) == LOW)
    {
      Serial.println("AUDIO PLAY BLUE");
      tmrpcm.play("blue.wav");
    }
    else if (digitalRead(AUDIO_RED_BUTTON) == LOW)
    {
      Serial.println("AUDIO PLAY RED");
      tmrpcm.play("red.wav");
    }
  }
  Serial.println("AUDIO FINISH");
}

void LedHandler()
{
  Serial.println("LED START");
  int redState = digitalRead(LED_RED_BUTTON);
  int greenState = digitalRead(LED_GREEN_BUTTON);
  int blueState = digitalRead(LED_BLUE_BUTTON);

  if (redState == HIGH && greenState == HIGH && blueState == HIGH && currentColor != offColor)
  {
    Serial.println("LED OFF");
    currentColor = offColor;
    pixels.clear();
    pixels.show();
    Serial.println("LED FINISH");
    return;
  }

  uint8_t redPart = 0;
  uint8_t greenPart = 0;
  uint8_t bluePart = 0;
  if (redState == LOW)
    redPart = 255;
  if (greenState == LOW)
    greenPart = 255;
  if (blueState == LOW)
    bluePart = 255;

  char strBuf[255];
  sprintf(strBuf, "LED FILL R:%u G:%u B:%u", redPart, greenPart, bluePart);
  Serial.println(strBuf);

  uint32_t newColor = pixels.Color(redPart, greenPart, bluePart);
  if (currentColor != newColor)
  {
    Serial.println("LED CHANGE COLOR");
    currentColor = newColor;
    pixels.fill(currentColor);
    pixels.show();
  }
  Serial.println("LED FINISH");
}

void LedLightsHandler()
{
  Serial.println("LED LIGHT START");
  int currentLedState = digitalRead(LED_LIGHTS_BUTTON);
  if (currentLedState == HIGH && oldLedButtonState == LOW)
  {
    Serial.println("LED LIGHT SWITCH TOGGLE");
    digitalWrite(LED_LIGHTS_SWITCH, !digitalRead(LED_LIGHTS_SWITCH));
  }
  oldLedButtonState = currentLedState;
  Serial.println("LED LIGHT STOP");
}
