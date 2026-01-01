#include "SensorLDR.h"
#include <Arduino.h>

SensorLDR::SensorLDR(int pinSensor, int umbral, int umbralClaro)
  : pin(pinSensor), umbralOscuro(umbral), umbralClaro(umbralClaro),
    valorMin(512), valorMax(0) {} 


void SensorLDR::iniciar() {
    pinMode(pin, INPUT);
}

float SensorLDR::obtenerNivelLuz() {
  int valor = leer();

  if(valor <= umbralOscuro) return 0.0;
  if(valor >= umbralClaro) return 1.0;

  // Mapear linealmente entre los umbrales
  return (float)(valor - umbralOscuro) / (float)(umbralClaro - umbralOscuro);
}

int SensorLDR::calcularIntensidadLED(int maxIntensidad) {
  float nivelLuz = obtenerNivelLuz();

  // Curva exponencial: más sensible en valores bajos de luz
  float factorNoLineal = pow(nivelLuz, 1.5);

  float intensidadNormalizada = 1.0 - factorNoLineal;

  if(intensidadNormalizada < 0) intensidadNormalizada = 0;
  if(intensidadNormalizada > 1) intensidadNormalizada = 1;

  // Convertir a rango PWM
  return (int)(intensidadNormalizada * maxIntensidad);
}

void SensorLDR::calibrar() {
  valorMin = 512;
  valorMax = 0;

  // Leer múltiples veces para estabilizar
  for (int i = 0; i < 10; i++) {
      int valor = leer();
      if (valor < valorMin) valorMin = valor;
      if (valor > valorMax) valorMax = valor;
      delay(50);
  }
  
}

//Aunque los sensores no escriben, hay que implementar el método
void SensorLDR::escribir(int valor) {}

int SensorLDR::leer() {
    return analogRead(pin);
}


bool SensorLDR::esDeDia() {
  return leer() > umbralOscuro;
}