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
  
  // Línea 1: Modo y estado
  lcd->setCursor(0, 0);
  lcd->print("Modo:");

  if (modo.length() <= 11) {
    lcd->setCursor(6, 0);
    lcd->print(modo);
  }
  else {
    lcd->setCursor(6, 0);
    lcd->print(modo.substring(0, 10));
  }

  // Línea 2: Información detallada
  lcd->setCursor(0, 1);
  lcd->print("S:");
  if (valorLuz < 10) {
    lcd->print("00");
    lcd->print(valorLuz);
  }
  else if (valorLuz < 100) {
    lcd->print("0");
    lcd->print(valorLuz);
  }
  else {
    lcd->print(valorLuz);
  }

  //Mostrar LEDs encendidos
  lcd->print(" L:");
  lcd->print(ledsEncendidos);
  lcd->print("/6");

  //Indicador visual de modo
  lcd->setCursor(15, 1);
  if (modo == "Auto") {
    lcd->print("A");
  }
  else if (modo == "Manual") {
    lcd->print("M");
  }
  else {
    lcd->print("*"); //Modo especial
  }
}

void PantallaLCD::limpiar() {
    lcd->clear();
}

void PantallaLCD::imprimir(String texto, int fila, int columna) {
  lcd->setCursor(columna, fila);
  lcd->print(texto);
}