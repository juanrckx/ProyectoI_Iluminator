#ifndef BOTON_H
#define BOTON_H

#include "Componente.h"

class Boton : public Componente {
  private:
    int pin;
    int debounceDelay;
    int ultimaEstable;           // Estado estable después de debounce
    int estadoPrevio;            // Estado anterior para detección de cambios
    unsigned long ultimoCambio;  // Tiempo del último cambio
    unsigned long ultimoEvento;  // Tiempo del último evento de presión

  public:
    Boton(int pinBoton, int debounce = 50);
    void iniciar() override;
    void escribir(int valor) override;
    int leer() override;
    bool fuePresionado();        // Devuelve true solo en el flanco descendente
};

#endif