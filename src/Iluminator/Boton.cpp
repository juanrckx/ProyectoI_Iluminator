#include "Boton.h"
#include <Arduino.h>

Boton::Boton(int pinBoton, int debounce)
  : pin(pinBoton), debounceDelay(debounce), 
    ultimaEstable(LOW), ultimoCambio(0), 
    ultimoEvento(0), estadoPrevio(HIGH) {}

void Boton::iniciar() {
    pinMode(pin, INPUT_PULLUP);
    // Leer estado inicial después de configurar el pin
    ultimaEstable = digitalRead(pin);
    estadoPrevio = ultimaEstable;
}

void Boton::escribir(int valor) {
    // No aplica para botones
}

int Boton::leer() {
    int lectura = digitalRead(pin);
    unsigned long ahora = millis();
    
    // Si el estado cambió, reiniciar el timer
    if (lectura != estadoPrevio) {
        ultimoCambio = ahora;
        estadoPrevio = lectura;
    }
    
    // Si ha pasado el tiempo de debounce, actualizar estado estable
    if ((ahora - ultimoCambio) > debounceDelay) {
        if (lectura != ultimaEstable) {
            ultimaEstable = lectura;
        }
    }
    
    return ultimaEstable;
}

bool Boton::fuePresionado() {
    int estadoActual = leer();
    
    // Con INPUT_PULLUP: LOW = presionado, HIGH = no presionado
    bool presionado = (estadoActual == LOW);
    
    // Solo generar evento si pasó tiempo suficiente desde el último
    unsigned long ahora = millis();
    bool resultado = false;
    
    if (presionado) {
        if (ultimoEvento == 0) {
            resultado = true;
            ultimoEvento = ahora;
        }
        else if ((ahora - ultimoEvento) > debounceDelay * 3) {
            ultimoEvento = 0;
        }
    }
    else {
        ultimoEvento = 0;
    }
    return resultado;
}