#ifndef CASA_H
#define CASA_H

#include "LED.h"
#include "SensorLDR.h"
#include "Boton.h"
#include "PantallaLCD.h"
#include "Modos.h"

class Casa {
  private:
    // Espacios (6 en total), en caso de no poder se descartarán algunos
    LED* sala;
    LED* cocina;
    LED* cuarto1;
    LED* patioInterno;
    LED* patioFrontal;
    LED* patioTrasero;

    // Sensores, (pueden ser 1 por espacio, pero para pruebas se probará con uno global)
    SensorLDR* sensor;

    // Botones: 6 para manual + 1 para auto + 1 para modos
    Boton* botonSala;
    Boton* botonCocina;
    Boton* botonCuarto1;
    Boton* botonPatioInt;
    Boton* botonPatioFront;
    Boton* botonPatioTras;
    Boton* botonAuto;
    Boton* botonModo;
    
    // Pantalla
    PantallaLCD* pantalla;

    // Estado del sistema
    ModoIluminacion modoActual;
    bool autoActivo;
    int ledsEncendidos;
    unsigned long ultimaActualizacionLCD;

  public:
    Casa(); //Constructor con pines por defecto (simulación)
    void iniciar();
    void actualizar();
    void cambiarModo();
    int contarLEDsEncendidos();
    void aplicarModoGlobal(ModoIluminacion modo);

  private:
    void actualizarLCD();
    void verificarBotonesManuales();
    void controlAutomatico();
};

#endif