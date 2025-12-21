#include "Casa.h"

Casa miCasa;

void setup() {
  miCasa.iniciar();
}

void loop() {
  miCasa.actualizar();
  delay(50);            // Delay para estabilidad
}