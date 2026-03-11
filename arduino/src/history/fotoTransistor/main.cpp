#include <Arduino.h>

int leituraSensor;      // variavel de leitura do sensor
int led = 8;            // led indicador = D8 do Arduino
int fotoTransistor = 7; // coletor do fototransistor = D7 do Arduino
void setup()
{
  pinMode(led, OUTPUT);           // pino do led indicador = saida
  pinMode(fotoTransistor, INPUT); // pino do coletor do fototransistor = entrada
}
void loop()
{
  leituraSensor = digitalRead(fotoTransistor); // leitura do módulo
  if (leituraSensor == 0)                      // se sensor detectar a luz
    digitalWrite(led, HIGH);                   // acende LED indicador
  else                                         // senão
    digitalWrite(led, LOW);                    // apaga LED indicador
  delay(500);                                  // atraso de 0,5 segundos
}