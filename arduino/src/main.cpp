#include <Arduino.h>
#include <Servo.h> // Inclu i a biblioteca Servo para controlar servos

Servo meuServo; // Cria um objeto Servo para controlar o servo motor
int pos;        // Declara uma variável para controlar a posição do servo motor

void setup()
{
  meuServo.attach(6); // Associa o servo motor ao pino digital 6 do Arduino
  meuServo.write(0);  // Define a posição inicial do servo motor para 0 graus
  Serial.begin(9600);
}

void loop()
{
  // O potenciômetro agora controla o "BPM" da dança
  int leitura = analogRead(A0);
  int velocidade = map(leitura, 0, 1023, 5, 50);

  Serial.print("Ritmo da Dança: ");
  Serial.println(velocidade);

  // --- PASSO 1: A "Onda" Elegante ---
  for (pos = 0; pos <= 180; pos += 5)
  {
    meuServo.write(pos);
    delay(velocidade); // O pot controla a fluidez da onda
  }
  for (pos = 180; pos >= 0; pos -= 5)
  {
    meuServo.write(pos);
    delay(velocidade);
  }

  for (int i = 0; i < 10; i++)
  {
    meuServo.write(85);
    delay(50);
    meuServo.write(95);
    delay(50);
  }

  // --- PASSO 2: O "Tremidinho" (Seu favorito!) ---
  // Ele vai para o meio e dá aquela vibrada
  meuServo.write(90);
  delay(200);
  for (int i = 0; i < 12; i++)
  {
    meuServo.write(85);
    delay(velocidade / 2); // Tremor mais rápido que a onda
    meuServo.write(95);
    delay(velocidade / 2);
  }

  for (int i = 0; i < 15; i++)
  {
    meuServo.write(80);  // Vai um pouco para a esquerda
    delay(30);           // Quase sem pausa
    meuServo.write(100); // Vai um pouco para a direita
    delay(30);
  }

  int passos[] = {0, 45, 90, 135, 180, 90, 0}; // Lista de posições
  for (int j = 0; j < 7; j++)
  {
    meuServo.write(passos[j]);
    delay(150); // Pausa brusca para dar o efeito de "travado"
  }
  // --- PASSO 3: O "Deslize" de Lado a Lado ---
  meuServo.write(30);
  delay(300);
  meuServo.write(150);
  delay(300);
  meuServo.write(90);
  delay(300);

  // --- PASSO 4: Final de Estrela (O Giro Rápido) ---
  for (int i = 0; i < 3; i++)
  {
    meuServo.write(0);
    delay(150);
    meuServo.write(180);
    delay(150);
  }

  // 3. Efeito Mola (Caminhada suave e volta tremida)
  for (pos = 0; pos <= 180; pos += 5)
  {
    meuServo.write(pos);
    delay(20);
  }
  // Volta tremendo como uma mola solta
  for (int k = 0; k < 10; k++)
  {
    meuServo.write(10);
    delay(40);
    meuServo.write(0);
    delay(40);
  }
}