#include <Arduino.h>

#define ledPin 3 // Pino do LED

int intervalo; // Intervalo em milissegundos para o piscar do LED

void setup()
{
  pinMode(ledPin, OUTPUT); // Configura o pino do LED como saída
}

void loop()
{
  int leituraPotenciometro = analogRead(A0); // Lê o valor do potenciômetro

  // Mapeia a leitura do potenciômetro para um intervalo de 10 ms a 1000 ms
  intervalo = map(leituraPotenciometro, 0, 1023, 10, 1000);

  // Alterna o estado do LED com base no intervalo
  digitalWrite(ledPin, HIGH);
  delay(intervalo); // Espera o intervalo em milissegundos
  digitalWrite(ledPin, LOW);
  delay(intervalo); // Espera o intervalo em milissegundos
}