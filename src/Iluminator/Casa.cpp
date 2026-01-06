#include "LED.h"
#include "SensorLDR.h"
#include "Boton.h"
#include "PantallaLCD.h"
#include "Modos.h"
#include "Casa.h"
#include <Arduino.h>

//Constructor con pines
Casa::Casa() :
  // LEDS - 7 espacios 
  sala(new LED(3)),
  cocina(new LED(5)),
  cuarto1(new LED(6)),
  cuarto2(new LED(11)),
  patioInterno(new LED(9)),
  patioFrontal(new LED(10)),
  patioTrasero(new LED(7)),

  sensor(new SensorLDR(A0, 250)),

  botonSala(new Boton(8)),
  botonCocina(new Boton(4)),
  botonCuarto1(new Boton(A3)),
  botonCuarto2(new Boton(A2)),
  botonPatioInt(new Boton(2)),
  botonPatioFront(new Boton(12)),
  botonPatioTras(new Boton(13)),      

  pantalla(new PantallaLCD()),

  modoActual(MODO_AUTO),
  modoAnterior(MODO_AUTO),
  ledsEncendidos(0),
  ultimaActualizacionLCD(0),
  ultimaActualizacionPot(0)

  // Inicializar estados manuales
  {
  for(int i = 0; i < 7; i++) {
    estadoManual[i] = false;
    overrideManual[i] = false;
    intensidadAutoArray[i] = 0;
  }
}

void Casa::iniciar() {
  Serial.begin(9600);
  Serial.println("Iniciando Sistema...");

  //Iniciar todos los componentes
  sala->iniciar();
  Serial.println("LED Sala: OK");

  cocina->iniciar();
  Serial.println("LED Cocina: OK");

  cuarto1->iniciar();
  Serial.println("LED Cuarto1: OK");

  cuarto2->iniciar();

  patioInterno->iniciar();
  Serial.println("LED patioInt: OK");

  patioFrontal->iniciar();
  Serial.println("LED patioFront: OK");

  patioTrasero->iniciar();
  Serial.println("LED patioTras: OK");


  sensor->iniciar();

  botonSala->iniciar();
  botonCocina->iniciar();
  botonCuarto1->iniciar();
  botonCuarto2->iniciar();
  botonPatioInt->iniciar();
  botonPatioFront->iniciar();
  botonPatioTras->iniciar();

  pinMode(PIN_POTENCIOMETRO, INPUT);

  pantalla->iniciar();
  for(int i = 0; i < 7; i++) {
    estadoManual[i] = false;
    overrideManual[i] = false;
  }
}

void Casa::aplicarControlCombinado() {
    // Solo aplica en modos AUTO y MANUAL
    if (modoActual != MODO_AUTO && modoActual != MODO_MANUAL) {
        return;
    }
    
    // Array de LEDs para fácil acceso
    LED* leds[] = {sala, cocina, cuarto1, cuarto2, patioInterno, patioFrontal, patioTrasero};
    
    for (int i = 0; i < 7; i++) {
        int intensidadFinal;
        
        if (modoActual == MODO_MANUAL) {
            // Solo manual: 255 si está encendido, 0 si está apagado
            intensidadFinal = estadoManual[i] ? 255 : 0;
        } 
        else if (modoActual == MODO_AUTO) {
            // AUTO: OR entre automático y manual
            if (overrideManual[i]) {
                // Usuario sobrescribió manualmente
                intensidadFinal = estadoManual[i] ? 255 : 0;
            } 
            else {
                // Usar intensidad automática proporcional
                intensidadFinal = intensidadAutoArray[i];
            }
        }
        
        // Aplicar la intensidad al LED
        leds[i]->escribir(intensidadFinal);
    }
}

void Casa::toggleLEDManual(int index) {
  if(index < 0 || index >= 7) return;

  estadoManual[index] = !estadoManual[index];
  overrideManual[index] = true; // Marcar como sobreescrito
}

ModoIluminacion Casa::obtenerModoDesdePot(int valorPot) {
    const int NUM_MODOS = 7;
    int segmento = map(valorPot, 0, 1022, 0, NUM_MODOS - 1);
    
    if (segmento < 0) segmento = 0;
    if (segmento >= NUM_MODOS) segmento = NUM_MODOS - 1;
    
    return static_cast<ModoIluminacion>(segmento);
}

void Casa::verificarBotones() {
  if(modoActual >= MODO_NOCHE && modoActual <= MODO_APAGADO) {
    return;
  }

  // Mapeo de botones a índices
  if (botonSala->fuePresionado()) {
    toggleLEDManual(SALA);
  }
  if (botonCocina->fuePresionado()) {
    toggleLEDManual(COCINA);
  }
  if (botonCuarto1->fuePresionado()) {
    toggleLEDManual(CUARTO1);
  }

  if (botonCuarto2->fuePresionado()) {
    toggleLEDManual(CUARTO2);
  }

  if (botonPatioInt->fuePresionado()) {
    toggleLEDManual(PATIO_INT);
  }
  if (botonPatioFront->fuePresionado()) {
    toggleLEDManual(PATIO_FRONT);
  }
  if (botonPatioTras->fuePresionado()) {
    toggleLEDManual(PATIO_TRAS);
  }
}

void Casa::actualizar() {
  //Leer potenciomtro
  if (millis() - ultimaActualizacionPot > 200) {
    leerPotenciometro();
    ultimaActualizacionPot = millis();
  }

  verificarBotones();

  //Ejecutar lógica segun modo
  switch(modoActual) {
    case MODO_AUTO:
      controlAutomatico();
      aplicarControlCombinado();
      break;
    
    case MODO_MANUAL:
      aplicarControlCombinado();
      break;

    default:
      aplicarModoGlobal(modoActual);
      // Resetear overrides cuando entramos en modo especial
      for(int i = 0; i < 7; i++) {
        overrideManual[i] = false;
      }
      break;
  }

  //Actualizar LCD
  if (millis() - ultimaActualizacionLCD > 500) {
    actualizarLCD();
    ultimaActualizacionLCD = millis();
  }
}

void Casa::leerPotenciometro() {
  int valorPot = analogRead(PIN_POTENCIOMETRO);
  ModoIluminacion nuevoModo = obtenerModoDesdePot(valorPot);

  //Solo cambiar si es diferente al anterior
  if (nuevoModo != modoAnterior) {
    cambiarModo(nuevoModo);
    modoAnterior = nuevoModo;
  }
}

void Casa::cambiarModo(ModoIluminacion nuevoModo) {
  modoActual = nuevoModo;

  // Si cambia a manual o auto, apagar modos especiales
  if (nuevoModo == MODO_AUTO || nuevoModo == MODO_MANUAL) {
    for(int i = 0; i < 7; i++) {
      overrideManual[i] = false;
    }
  } 
}

void Casa::aplicarModoGlobal(ModoIluminacion modo) {
  switch(modo) {
    case MODO_NOCHE:
      sala->escribir(50);  // Luz tenue
      cocina->escribir(0); // Apagado
      cuarto1->escribir(30);
      cuarto2->escribir(30);
      patioInterno->escribir(30);
      patioFrontal->escribir(10);
      patioTrasero->escribir(0);
      break;

    case MODO_LECTURA:
      sala->escribir(255);
      cocina->escribir(200);
      cuarto1->escribir(255);
      cuarto2->escribir(255);
      patioInterno->escribir(100);
      patioFrontal->escribir(0);
      patioTrasero->escribir(0);
      break;

    case MODO_FIESTA:
    // Efecto parpadeante
      static unsigned long ultimoCambioFiesta = 0;
      if (millis() - ultimoCambioFiesta > 50) {
        int estado = random(0, 2);
        sala->escribir(!estado * 255);
        cocina->escribir(estado * 255);
        cuarto1->escribir(!estado * 255);
        cuarto2->escribir(!estado * 255);
        patioInterno->escribir(estado * 255);
        patioFrontal->escribir(!estado * 255);
        patioTrasero->escribir(estado * 255);
        ultimoCambioFiesta = millis();
      }
      break;

    case MODO_RELAJACION:
    // Luz suave
      sala->escribir(150);
      cocina->escribir(40);
      cuarto1->escribir(150);
      cuarto2->escribir(150);
      patioInterno->escribir(40);
      patioFrontal->escribir(40);
      patioTrasero->escribir(128);
      break;

    case MODO_APAGADO:
      sala->escribir(0);
      cocina->escribir(0);
      cuarto1->escribir(0);
      cuarto2->escribir(0);
      patioInterno->escribir(0);
      patioFrontal->escribir(0);
      patioTrasero->escribir(0);
      break;
  }
}


void Casa::controlAutomatico() {
  int intensidadAuto = sensor->calcularIntensidadLED(255);
  // Aplicar intensidad proporcional a todos los LEDs (solo si no hay override manual)
  // Pero primero, actualizar el array intensidadAuto[] para usar en aplicarControlCombinado()
  for (int i = 0; i < 7; i++) {
      intensidadAutoArray[i] = intensidadAuto;
  }
}


int Casa::contarLEDsEncendidos() {
  int contador = 0;
  if (sala->leer()) contador++;
  if (cocina->leer()) contador++;
  if (cuarto1->leer()) contador++;
  if (cuarto2->leer()) contador++;
  if (patioInterno->leer()) contador++;
  if (patioFrontal->leer()) contador++;
  if (patioTrasero->leer()) contador++;
  return contador;
}

void Casa::actualizarLCD() {
  ledsEncendidos = contarLEDsEncendidos();
  int valorLuz = sensor->leer();
  
  pantalla->mostrarInfo(valorLuz, nombreModo(modoActual), ledsEncendidos);
}

