#ifndef LED_H
#define LED_H

#include "Componente.h"

class LED : public Componente 
{
  private:
    int pin;
    bool encendido;
    int intensidad;                       // PARA PWM (0-255)

  public:
    LED(int pinLED);
    void iniciar() override;
    void escribir(int valor) override;    // 0=apagar, 1=encender, 2-255=PWM
    int leer() override;                  // Devuelve 1 si encendido, 0 si apagado
    void setIntensidad(int valor);        // Para modos especiales
};

#endif