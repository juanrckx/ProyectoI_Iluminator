#include "Casa.h"

Casa miCasa;

void setup() {
  miCasa.iniciar();
    // Test de estado inicial de botones
  Serial.println("\n=== TEST INICIAL BOTONES ===");
  
  // Configurar pines como INPUT_PULLUP y leer
  for(int i = 8; i <= 13; i++) {
    pinMode(i, INPUT_PULLUP);
    delay(10);
    int estado = digitalRead(i);
    Serial.print("Pin ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(estado == HIGH ? "HIGH (NO PRESIONADO)" : "LOW (PRESIONADO/ERROR)");
  }
  
  // Si algún pin muestra LOW sin presionar, hay problema de conexión
  Serial.println("=== FIN TEST ===");
}

void loop() {
  miCasa.actualizar();
  delay(50);  // Delay para estabilidad
}