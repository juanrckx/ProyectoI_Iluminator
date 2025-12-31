#include "Casa.h"

Casa miCasa;

void setup() {
  miCasa.iniciar();
  Serial.println("Prueba de botones:");
  for(int i=8; i<=13; i++) {
    pinMode(i, INPUT_PULLUP);
    Serial.print("Pin ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(digitalRead(i));
  }
}

void loop() {
  miCasa.actualizar();
  delay(500);            // Delay para estabilidad
}