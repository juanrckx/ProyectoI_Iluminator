#include "LED.h"
#include <Arduino.h>

LED::LED(int pinLED) : pin(pinLED), encendido(false), intensidad(255) {}

void LED::iniciar() {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}

void LED::escribir(int valor) {
  if (valor == 0) {
    digitalWrite(pin, LOW);
    encendido = false;
  }

  else if (valor == 1) {
    digitalWrite(pin, HIGH);
    encendido = true;
  }

  else if (valor >= 2 && valor <= 255) {
    analogWrite(pin, valor);
    encendido = (valor > 0);
    intensidad = valor;
  }
}

int LED::leer() {
  return encendido ? 1 : 0;
}

void LED::setIntensidad(int valor) {
  if (valor >= 0 && valor <= 255) {
    intensidad = valor;
    if (encendido) {
      analogWrite(pin, intensidad);
    }
  }
}