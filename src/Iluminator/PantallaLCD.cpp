#include "PantallaLCD.h"
#include <Arduino.h>

PantallaLCD::PantallaLCD(int addr, int cols, int rows)
  : direccionI2C(addr), columnas(cols), filas(rows), simulacion(false) {
    lcd = new LiquidCrystal_I2C(direccionI2C, columnas, filas);
  }

PantallaLCD::~PantallaLCD() {
  delete lcd;
}

void PantallaLCD::iniciar() {
  if (!simulacion) {
    lcd->init();
    lcd->backlight();
    lcd->clear();
    lcd->print("Sistema Listo");
    delay(10000);
  }
}

void PantallaLCD::escribir(int valor) {
  // No aplica directamente
}

int PantallaLCD::leer() {
  // Tampoco aplica
  return 0;
}

void PantallaLCD::mostrarInfo(int valorLuz, String modo, int ledsEncendidos) {
  if (simulacion) {
    Serial.println("=== LCD SIMULADO ===");
    Serial.print("Luz: ");
    Serial.print(valorLuz);
    Serial.print(" | Modo: ");
    Serial.print(modo);
    Serial.print(" | LEDs ON:");
    Serial.println(ledsEncendidos);
    Serial.println("====================");
    return;
  }

  limpiar();
  lcd->setCursor(0, 0);
  lcd->print("Luz: ");
  lcd->print(valorLuz);
  lcd->print(" Modo:");
  lcd->print(modo.substring(0,4)); //Solo los primeros 4 chars

  lcd->setCursor(0, 1);
  lcd->print("LEDs ON:");
  lcd->print(ledsEncendidos);
  lcd->print("/6");
}

void PantallaLCD::limpiar() {
  if (!simulacion) {
    lcd->clear();
  }
}

void PantallaLCD::imprimir(String texto, int fila, int columna) {
  if (simulacion) {
    Serial.print("LCD[");
    Serial.print(fila);
    Serial.print(",");
    Serial.print(columna);
    Serial.print("]: ");
    Serial.println(texto);
  }
  else {
    lcd->setCursor(columna, fila);
    lcd->print(texto);
  }
}