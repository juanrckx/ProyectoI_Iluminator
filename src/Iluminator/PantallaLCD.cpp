#include "PantallaLCD.h"
#include <Arduino.h>

PantallaLCD::PantallaLCD(int addr, int cols, int rows)
  : direccionI2C(addr), columnas(cols), filas(rows) {
    lcd = new LiquidCrystal_I2C(direccionI2C, columnas, filas);
  }


PantallaLCD::~PantallaLCD() {
  delete lcd;
}

void PantallaLCD::iniciar() {
    lcd->init();
    lcd->backlight();
    lcd->clear();
    lcd->print("Sistema Listo");
    delay(1000);
}

void PantallaLCD::escribir(int valor) {
  // No aplica directamente
}

int PantallaLCD::leer() {
  // Tampoco aplica
  return 0;
}

void PantallaLCD::mostrarInfo(int valorLuz, String modo, int ledsEncendidos) {
  limpiar();
  lcd->setCursor(0, 0);
  lcd->print("Luz: ");
  lcd->print(valorLuz);
  lcd->print(" Modo:");

  lcd->setCursor(0, 1);
  lcd->print("LEDs ON:");
  lcd->print(ledsEncendidos);
  lcd->print("/6");
}

void PantallaLCD::limpiar() {
    lcd->clear();
}

void PantallaLCD::imprimir(String texto, int fila, int columna) {
  lcd->setCursor(columna, fila);
  lcd->print(texto);
}