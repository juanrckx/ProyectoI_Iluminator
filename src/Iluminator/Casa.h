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
    LED* cuarto2;
    LED* patioInterno;
    LED* patioFrontal;
    LED* patioTrasero;

    // Sensores, (pueden ser 1 por espacio, pero para pruebas se probará con uno global)
    SensorLDR* sensor;

    // Botones: 7 para manual
    Boton* botonSala;
    Boton* botonCocina;
    Boton* botonCuarto1;
    Boton* botonCuarto2;
    Boton* botonPatioInt;
    Boton* botonPatioFront;
    Boton* botonPatioTras;

    //Potenciometro para modos
    static const int PIN_POTENCIOMETRO = A1;
    
    // Pantalla
    PantallaLCD* pantalla;

    // Estado del sistema
    ModoIluminacion modoActual;
    ModoIluminacion modoAnterior;
    int ledsEncendidos;
    unsigned long ultimaActualizacionPot;
    unsigned long ultimaActualizacionLCD;

    bool estadoManual[7]; // Estado manual para cada LED
    bool overrideManual[7];

    // Intensidades para LEDs
    int intensidadAutoArray[7];
    static const int UMBRAL_OSCURO = 50;
    static const int UMBRAL_CLARO = 200;

  public:
    Casa(); //Constructor con pines por defecto (simulación)
    void iniciar();
    void actualizar();
    void cambiarModo(ModoIluminacion nuevoModo);
    int contarLEDsEncendidos();
    void aplicarModoGlobal(ModoIluminacion modo);

    void toggleLEDManual(int index);
    void aplicarControlCombinado(); // Aplica OR entre automático y manual

  private:
    void actualizarLCD();
    void verificarBotones();

    void controlAutomatico();
    void leerPotenciometro();
    ModoIluminacion obtenerModoDesdePot(int valorPot); //Método interno

    enum Espacios {SALA, COCINA, CUARTO1, CUARTO2, PATIO_INT, PATIO_FRONT, PATIO_TRAS};
};

#endif