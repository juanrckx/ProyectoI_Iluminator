#ifndef BOTON_H
#define BOTON_H

#include "Componente.h"

class Boton : public Componente {
  private:
    int pin;
    bool estadoAnterior;
    unsigned long ultimoCambio;
    int debounceDelay;

  public:
    Boton(int pinBoton, int debounce = 50);
    void iniciar() override;
    void escribir(int valor) override;      // 
    int leer() override;                    // 1 si presionado, 0 si no
    bool fuePresionado();                   // Detecta la acción
};

#endif