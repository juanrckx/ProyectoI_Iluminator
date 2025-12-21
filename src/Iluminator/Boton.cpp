#include "Boton.h"
#include <Arduino.h>

Boton::Boton(int pinBoton, int debounce)
  : pin(pinBoton), estadoAnterior(LOW), ultimoCambio(0),
    debounceDelay(debounce), simulacion(false), estadoSimulado(false) {}

void Boton::iniciar() {
  if (!simulacion) {
    pinMode(pin, INPUT_PULLUP);                         // Normalmente HIGH, presionado = LOW
  }
}

void Boton::escribir(int valor) {
  if (valor == 1) {
    simulacion = true;
    estadoSimulado = true;
  }
  else if (valor == 0) {
    estadoSimulado = false;
  }
}

int Boton::leer() {
  if (simulacion) {
    // Para prueba: simula pulsación cada 5 segundos
    static unsigned long ultimaPulsacion = 0;
    if (millis() - ultimaPulsacion > 5000) {
      estadoSimulado = true;
      ultimaPulsacion = millis();
    }
    else if (millis() - ultimaPulsacion > 200) {
      estadoSimulado = false;
    }
    return estadoSimulado ? 0 : 1;                        //Invertido por PULLUP
  }

  // Lectura real con debounce
  int lectura = digitalRead(pin);
  if (lectura != estadoAnterior) {
    ultimoCambio = millis();
  }
  if ((millis() - ultimoCambio) > debounceDelay) {
    estadoAnterior = lectura;
  }
  return estadoAnterior;
}

bool Boton::fuePresionado() {
  int estadoActual = leer();
  bool presionado = (estadoActual == LOW);                  // LOW = presionado en PULLUP
  static bool anterior = false;

  bool resultado = (presionado && !anterior);
  anterior = presionado;
  return resultado;
}