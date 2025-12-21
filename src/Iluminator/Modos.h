#include <Arduino.h>
#ifndef MODOS_H
#define MODOS_H


enum ModoIluminacion {
  MODO_AUTO = 0,
  MODO_MANUAL = 1,
  MODO_NOCHE = 2,
  MODO_LECTURA = 3,
  MODO_FIESTA = 4,
  MODO_RELAJACION = 5,
  MODO_APAGADO = 6
};

inline String nombreModo(ModoIluminacion modo) {
  switch(modo) {
    case MODO_AUTO: return "Auto";
    case MODO_MANUAL: return "Manual";
    case MODO_NOCHE: return "Noche";
    case MODO_LECTURA: return "Lectura";
    case MODO_FIESTA: return "Fiesta";
    case MODO_RELAJACION: return "Relax";
    case MODO_APAGADO: return "Apagado";
    default: return "Desconocido";
  }
}

#endif