#include <Arduino.h>

const int ledPin = 8;

void setup()
{
  pinMode(ledPin, OUTPUT);

  Serial.begin(9600);
}

void loop()
{
  digitalWrite(ledPin, HIGH);
  delay(1500);
  digitalWrite(ledPin, LOW);

  delay(1500);
}