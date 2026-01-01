#ifndef SENSORLDR_H
#define SENSORLDR_H

#include "Componente.h"

class SensorLDR : public Componente {
  private:
    int pin;
    int umbralOscuro;                           // Valor por debajo de lo que se considera oscuro
    int umbralClaro;
    int valorMin;
    int valorMax;

  public:
    SensorLDR(int pinSensor, int umbral = 70, int umbralClaro = 150);
    void iniciar() override;
    void escribir(int valor) override;          // NO aplica para sensor
    int leer() override;                        // Devuelve el valor de la luz (0-1023)
    
    bool esDeDia();                             // True si hay suficiente luz

    float obtenerNivelLuz(); // Devuelve de 0.0 (oscuro) a 1.0 (claro)
    int calcularIntensidadLED(int maxIntensidad = 255); // Intensidad proporcional
    void calibrar(); 
};

#endif