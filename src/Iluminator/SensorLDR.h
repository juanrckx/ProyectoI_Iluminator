#ifndef SENSORLDR_H
#define SENSORLDR_H

#include "Componente.h"

class SensorLDR : public Componente {
  private:
    int pin;
    int umbralOscuro;                           // Valor por debajo de lo que se considera oscuro
    bool simulacion;                            // Modo simulación (por mientras no tenemos la    
    int valorSimulado;                          // conexión física hecha)

  public:
    SensorLDR(int pinSensor, int umbral = 500);
    void iniciar() override;
    void escribir(int valor) override;          // NO aplica para sensor
    int leer() override;                        // Devuelve el valor de la luz (0-1023)
    void setSimulacion(bool sim, int valorInicial = 512);
    bool esDeDia();                             // True si hay suficiente luz
};

#endif