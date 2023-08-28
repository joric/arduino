#include <Arduino.h>
#include <TM1637Display.h>

const int pinDisplayCLK = 16;
const int pinDisplayDIO = 10;
const int pinSpeaker = 3;
const int pinButton = 9;

TM1637Display display(pinDisplayCLK, pinDisplayDIO);

void setup() {
  pinMode(pinSpeaker, OUTPUT);
  display.setBrightness(15);
  display.showNumberDec(0, true);
  pinMode(pinButton, INPUT);
  digitalWrite(pinButton, HIGH);
}

unsigned long timerStartTime = 0;
unsigned long timerStopTime = 0;
unsigned long timerDuration = 10 * 1000;

bool timerPaused = true;
bool wasPressed = false;

void loop() {
  
  bool pressed = digitalRead(pinButton)==LOW;
  
  if (pressed!=wasPressed) {
    wasPressed = pressed;    
    if (pressed) {
      timerPaused = !timerPaused;
      if (timerPaused) {
        timerStopTime = millis();
      } else {
        timerStartTime = millis() - (timerStopTime - timerStartTime);
      }
    }
    delay(25); // antichatter
  }

  unsigned long t = timerPaused ? timerStopTime-timerStartTime : millis() - timerStartTime;

  t = timerDuration - (t % timerDuration);
  
  int sec = (t/1000) % 60;
  int cent = (t/10) % 100;
  
  display.showNumberDec(sec*100+cent, true);
  display.setColon(timerPaused ? true : cent<50);
  
  digitalWrite(pinSpeaker, !timerPaused && cent>90 ? HIGH : LOW);
}

