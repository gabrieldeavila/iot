#include <Arduino.h>

#define Vin 5.0          // define a tensão de entrada igual a 5.0
#define T0 298.15        // define constante igual a 298.15 Kelvin
#define Rt 10000         // Resistor do divisor de tensão
#define R0 10000         // Valor da resistência inicial do NTC
#define T1 273.15        // [K] in datasheet 0º C
#define T2 373.15        // [K] in datasheet 100° C
#define RT1 35563        // [ohms] Resistência in T1
#define RT2 549          // [ohms] Resistência in T2
float beta = 0.0;        // parâmetros iniciais [K]
float Rinf = 0.0;        // parâmetros iniciais [ohm]
float TempKelvin = 0.0;  // variable output
float TempCelsius = 0.0; // variable output
float Vout = 0.0;        // Vout in A0
float Rout = 0.0;        // Rout in A0
void setup()
{
  Serial.begin(9600);                              // monitor serial - velocidade 9600 Bps
  beta = (log(RT1 / RT2)) / ((1 / T1) - (1 / T2)); // cálculo de beta
  Rinf = R0 * exp(-beta / T0);                     // cálculo de Rinf
  delay(100);                                      // atraso de 100 milissegundos
}

void loop()
{
  int leitura = analogRead(0);
  Vout = Vin * ((float)leitura / 1023.0); // 1023 é o valor máximo do ADC

  // FÓRMULA CORRIGIDA PARA NTC NO 5V:
  Rout = (Rt * Vout) / (Vin - Vout);

  TempKelvin = (beta / log(Rout / Rinf));
  TempCelsius = TempKelvin - 273.15;

  Serial.print("Leitura ADC: ");
  Serial.print(leitura);
  Serial.print(" | Celsius: ");
  Serial.println(TempCelsius);

  delay(500);
}