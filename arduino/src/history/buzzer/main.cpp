#include <Arduino.h>

#define BUTTON 3
#define BUZZER 4

void setup()
{
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);
  Serial.begin(9600);
  delay(100);
}

void loop()
{
  if (digitalRead(BUTTON) == LOW)
  {
    Serial.println("os guri tao no tone");
    tone(BUZZER, 127);
  }
  else
  {
    noTone(BUZZER);
  }
  delay(100);
}