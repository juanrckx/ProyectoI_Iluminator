#include "Boton.h"
#include <Arduino.h>

Boton::Boton(int pinBoton, int debounce)
  : pin(pinBoton), estadoAnterior(LOW), ultimoCambio(0),
    debounceDelay(debounce), estadoPresionadoAnterior(false) {}

void Boton::iniciar() {
    pinMode(pin, INPUT_PULLUP);                         // Normalmente HIGH, presionado = LOW
}

void Boton::escribir(int valor) {}

int Boton::leer() {
  // Lectura real con debounce
  int lectura = digitalRead(pin);

  if (lectura != estadoAnterior) {
    ultimoCambio = millis();
  }

  if ((millis() - ultimoCambio) > debounceDelay) {
    if (lectura != estadoAnterior) {
      estadoAnterior = lectura;
    }
  }
  return estadoAnterior;
}

bool Boton::fuePresionado() {
  int estadoActual = leer();
  bool presionado = (estadoActual == LOW);                  // LOW = presionado en PULLUP

  bool resultado = (presionado && !estadoPresionadoAnterior);
  estadoPresionadoAnterior = presionado;
  
  return resultado;
}