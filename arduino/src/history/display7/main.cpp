#include <Arduino.h>

int conta = 0;

// --- CONFIGURAÇÃO DA LÓGICA DO BOTÃO ---
// Se continuar invertido, troque: PRESSIONADO = HIGH e SOLTO = LOW
const int PINO_BOTAO = 10;
const int ESTADO_PRESSIONADO = LOW;
const int ESTADO_SOLTO = HIGH;

int estadoAnterior = ESTADO_SOLTO;
unsigned long tempoPressionado = 0;
const unsigned long tempoLongPress = 1000;
bool longPressFoiAtivado = false;

// Matriz Ânodo Comum (0 liga, 1 desliga)
byte displaySeteSeg[10][7] = {
    {0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 1, 1, 1, 1},
    {0, 0, 1, 0, 0, 1, 0},
    {0, 0, 0, 0, 1, 1, 0},
    {1, 0, 0, 1, 1, 0, 0},
    {0, 1, 0, 0, 1, 0, 0},
    {0, 1, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 1, 1, 1},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0}};

void ligaSegmentosDisplay(byte digito)
{
  byte pino = 2;
  for (byte i = 0; i < 7; ++i)
  {
    digitalWrite(pino, displaySeteSeg[digito][i]);
    ++pino;
  }
}

void normalPress()
{
  conta++;

  if (conta > 9)
  {
    conta = 0;
    ligaSegmentosDisplay(conta);
  }
  else
  {
    ligaSegmentosDisplay(conta);
  }
}

void longPress()
{
  conta--;

  if (conta < 0)
  {
    conta = 9;
    ligaSegmentosDisplay(conta);
  }
  else
  {
    ligaSegmentosDisplay(conta);
  }

  Serial.print("Aplicando Long Press, valor vai para: ");
  Serial.println(conta);
}

void setup()
{
  for (int i = 2; i <= 8; i++)
  {
    pinMode(i, OUTPUT);
  }

  pinMode(PINO_BOTAO, INPUT_PULLUP);
  ligaSegmentosDisplay(conta);
  Serial.begin(9600);
}

void loop()
{
  if (digitalRead(10) == HIGH)
  {
    unsigned long tempoAgora = millis();

    if (tempoPressionado == 0)
    {
      tempoPressionado = tempoAgora;
      Serial.print("Usuario comecou a pressionar aos ");
      Serial.print(tempoPressionado);
      Serial.println("ms");
    }
    else if (tempoAgora > tempoPressionado && tempoAgora - tempoPressionado > 500)
    {
      Serial.print("Diferenca de ");
      Serial.print(tempoAgora - tempoPressionado);
      Serial.print(" tempoAgora eh: ");
      Serial.print(tempoAgora);
      Serial.print(" E o tempoPressionado: ");
      Serial.print(tempoPressionado);
      Serial.print(" ");

      longPress();
      longPressFoiAtivado = true;
      // diminui por 250 pros 500s passarem no modo gato a jato ne paezao
      tempoPressionado = tempoAgora - 250;
    }
  }
  else if (tempoPressionado > 0)
  {
    Serial.print("Usuario pressionou por: ");
    Serial.print(millis() - tempoPressionado);
    Serial.println("ms");

    if (!longPressFoiAtivado && millis() - tempoPressionado < 500)
    {
      Serial.println("Aplicando Normal Press!");
      normalPress();
    }

    longPressFoiAtivado = false;
    tempoPressionado = 0;
  }

  delay(100);
}
