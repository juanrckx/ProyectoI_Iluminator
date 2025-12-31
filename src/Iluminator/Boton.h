#ifndef BOTON_H
#define BOTON_H

#include "Componente.h"

class Boton : public Componente {
  private:
    int pin;
    int debounceDelay;
    bool estadoAnterior;
    bool estadoPresionadoAnterior;
    unsigned long ultimoCambio;

  public:
    Boton(int pinBoton, int debounce = 50);
    void iniciar() override;
    void escribir(int valor) override;      // 
    int leer() override;                    // 1 si presionado, 0 si no
    bool fuePresionado();                   // Detecta la acción
};

#endif