#include "SensorLDR.h"
#include <Arduino.h>

SensorLDR::SensorLDR(int pinSensor, int umbral)
  : pin(pinSensor), umbralOscuro(umbral), simulacion(false), valorSimulado(512) {}

void SensorLDR::iniciar() {
  if (!simulacion) {
    pinMode(pin, INPUT);
  }
}

//Aunque los sensores no escriben, hay que implementar el método, así que usamos 
//este método para cambiar el modo simulación por ahora.
void SensorLDR::escribir(int valor) {
  if (valor == 999) {
    simulacion = !simulacion;
  }
}

int SensorLDR::leer() {
  if (simulacion) {
    // Para prueba: alterna entre claro y oscuro cada 10 segundos
    if (millis() % 20000 < 10000) {
      valorSimulado = 800;  // Claro
    }
    else {
      valorSimulado = 300; // Oscuro
    }
    return valorSimulado;
  }
  else {
    return analogRead(pin);
  }
}

bool SensorLDR::esDeDia() {
  return leer() > umbralOscuro;
}

void SensorLDR::setSimulacion(bool sim, int valorInicial) {
  simulacion = sim;
  valorSimulado = valorInicial;
}