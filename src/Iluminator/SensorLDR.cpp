#include "SensorLDR.h"
#include <Arduino.h>

SensorLDR::SensorLDR(int pinSensor, int umbral)
  : pin(pinSensor), umbralOscuro(umbral) {}


void SensorLDR::iniciar() {
    pinMode(pin, INPUT);
}

//Aunque los sensores no escriben, hay que implementar el método
void SensorLDR::escribir(int valor) {}

int SensorLDR::leer() {
    return analogRead(pin);
}


bool SensorLDR::esDeDia() {
  return leer() > umbralOscuro;
}