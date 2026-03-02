#include <Arduino.h>

// Definindo os pinos
const int trigPin = 9;
const int echoPin = 10;
const int ledPin = 8; // <-- NOVO: Pino onde o LED está conectado

// Variáveis para armazenar o tempo e a distância
long duracao;
int distancia;

void setup()
{
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);

  Serial.begin(9600);
}

void loop()
{
  // 1. Gera o pulso sonoro
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // 2. Lê o tempo de retorno
  duracao = pulseIn(echoPin, HIGH);

  // 3. Calcula a distância
  distancia = duracao * 0.034 / 2;

  // 4. Mostra no Monitor Serial
  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.println(" cm");

  if (distancia > 0 && distancia < 10)
  {
    digitalWrite(ledPin, HIGH); // Acende o LED
    Serial.println("Objeto muito próximo! Acendendo o LED de alerta.");
  }
  else
  {
    digitalWrite(ledPin, LOW); // Apaga o LED
  }

  // Espera um pouco antes de ler novamente
  delay(1500);
}