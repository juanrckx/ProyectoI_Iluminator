#ifndef SENSORLDR_H
#define SENSORLDR_H

#include "Componente.h"

class SensorLDR : public Componente {
  private:
    int pin;
    int umbralOscuro;                           // Valor por debajo de lo que se considera oscuro

  public:
    SensorLDR(int pinSensor, int umbral = 250);
    void iniciar() override;
    void escribir(int valor) override;          // NO aplica para sensor
    int leer() override;                        // Devuelve el valor de la luz (0-1023)
    bool esDeDia();                             // True si hay suficiente luz
};

#endif