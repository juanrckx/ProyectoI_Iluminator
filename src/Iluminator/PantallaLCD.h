#ifndef PANTALLALCD_H
#define PANTALLALCD_H

#include "Componente.h"
#include <LiquidCrystal_I2C.h> 

class PantallaLCD : public Componente {
  private:
    LiquidCrystal_I2C * lcd;
    int direccionI2C;
    int columnas;
    int filas;
    bool simulacion;

  public:
    PantallaLCD(int addr = 0x27, int cols = 16, int rows = 2);
    ~PantallaLCD();
    void iniciar() override;
    void escribir(int valor) override; // No se usa directamente
    int leer() override;              // No aplica
    void mostrarInfo(int valorLuz,  String modo, int ledsEncendidos);
    void limpiar();
    void imprimir(String texto, int fila = 0, int columna = 0);
};

#endif