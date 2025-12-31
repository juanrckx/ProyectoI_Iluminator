#include "LED.h"
#include "SensorLDR.h"
#include "Boton.h"
#include "PantallaLCD.h"
#include "Modos.h"
#include "Casa.h"
#include <Arduino.h>

//Constructor con pines
Casa::Casa() :
  // LEDS - 6 espacios 
  sala(new LED(2)),
  cocina(new LED(3)),
  cuarto1(new LED(4)),
  patioInterno(new LED(5)),
  patioFrontal(new LED(6)),
  patioTrasero(new LED(7)),

  sensor(new SensorLDR(A0, 250)),

  botonSala(new Boton(8)),
  botonCocina(new Boton(9)),
  botonCuarto1(new Boton(10)),
  botonPatioInt(new Boton(11)),
  botonPatioFront(new Boton(12)),
  botonPatioTras(new Boton(13)),      

  pantalla(new PantallaLCD()),

  modoActual(MODO_AUTO),
  modoAnterior(MODO_AUTO),
  ledsEncendidos(0),
  ultimaActualizacionLCD(0),
  ultimaActualizacionPot(0)
{}

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
  botonPatioInt->iniciar();
  botonPatioFront->iniciar();
  botonPatioTras->iniciar();

  pinMode(PIN_POTENCIOMETRO, INPUT);

  pantalla->iniciar();
  
}

ModoIluminacion Casa::obtenerModoDesdePot(int valorPot) {
    const int NUM_MODOS = 7;
    int segmento = map(valorPot, 0, 1022, 0, NUM_MODOS - 1);
    
    if (segmento < 0) segmento = 0;
    if (segmento >= NUM_MODOS) segmento = NUM_MODOS - 1;
    
    return static_cast<ModoIluminacion>(segmento);
}

void Casa::actualizar() {
  //Leer potenciomtro
  if (millis() - ultimaActualizacionPot > 200) {
    leerPotenciometro();
    ultimaActualizacionPot = millis();
  }

  //Ejecutar lógica segun modo
  switch(modoActual) {
    case MODO_AUTO:
      controlAutomatico();
      break;
    
    case MODO_MANUAL:
      verificarBotonesManuales();
      break;

    default:
      aplicarModoGlobal(modoActual);
      break;
  }

  //Actualizar LCD
  if (millis() - ultimaActualizacionLCD > 500) {
    actualizarLCD();
    ultimaActualizacionLCD = millis();

    //DEBUG
    Serial.print("Modo: ");
    Serial.print(nombreModo(modoActual));
    Serial.print(" | Luz: ");
    Serial.print(sensor->leer());
    Serial.print(" | LEDs ON: ");
    Serial.println(contarLEDsEncendidos());
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

  Serial.print("Modo cambiado a: ");
  Serial.println(nombreModo(nuevoModo));

  // Si cambia a manual o auto, apagar modos especiales
  if (modoActual == MODO_MANUAL || modoActual == MODO_AUTO) {

    if (nuevoModo == MODO_MANUAL) {
      sala->escribir(0);
      cocina->escribir(0);
      cuarto1->escribir(0);
      patioInterno->escribir(0);
      patioFrontal->escribir(0);
      patioTrasero->escribir(0);
    }

  } 
}

void Casa::aplicarModoGlobal(ModoIluminacion modo) {
  switch(modo) {
    case MODO_NOCHE:
      sala->escribir(50);  // Luz tenue
      cocina->escribir(0); // Apagado
      cuarto1->escribir(30);
      patioInterno->escribir(30);
      patioFrontal->escribir(10);
      patioTrasero->escribir(10);
      break;

    case MODO_LECTURA:
      sala->escribir(255);
      cocina->escribir(200);
      cuarto1->escribir(255);
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
      patioInterno->escribir(40);
      patioFrontal->escribir(40);
      patioTrasero->escribir(128);
      break;

    case MODO_APAGADO:
      sala->escribir(0);
      cocina->escribir(0);
      cuarto1->escribir(0);
      patioInterno->escribir(0);
      patioFrontal->escribir(0);
      patioTrasero->escribir(0);
      break;
  }
}

void Casa::verificarBotonesManuales() {
  // Sala
  if (botonSala->fuePresionado()) {
    bool estado = sala->leer();
    sala->escribir(!estado);
    Serial.println("Sala: " + String(!estado ? "ENCENDIDO" : "APAGADO"));
  }

  // Cocina
  if (botonCocina->fuePresionado()) {
    bool estado = cocina->leer();
    cocina->escribir(!estado);
    Serial.println("Cocina: " + String(!estado ? "ENCENDIDO" : "APAGADO"));
  }

  // Cuarto1
  if (botonCuarto1->fuePresionado()) {
    bool estado = cuarto1->leer();
    cuarto1->escribir(!estado);
    Serial.println("Cuarto1: " + String(!estado ? "ENCENDIDO" : "APAGADO"));
  }

  // Patio Interno
  if (botonPatioInt->fuePresionado()) {
    bool estado = patioInterno->leer();
    patioInterno->escribir(!estado);
    Serial.println("Patio Interno: " + String(!estado ? "ENCENDIDO" : "APAGADO"));
  }

  // Patio Frontal
  if (botonPatioFront->fuePresionado()) {
    bool estado = patioFrontal->leer();
    patioFrontal->escribir(!estado);
    Serial.println("Patio Frontal: " + String(!estado ? "ENCENDIDO" : "APAGADO"));
  }

  //Patio Trasero
  if (botonPatioTras->fuePresionado()) {
    bool estado = patioTrasero->leer();
    patioTrasero->escribir(!estado);
    Serial.println("Patio Trasero: " + String(!estado ? "ENCENDIDO" : "APAGADO"));
  }
}

void Casa::controlAutomatico() {
  bool esDeNoche = !sensor->esDeDia();

  if (esDeNoche) {
    sala->escribir(255);
    cocina->escribir(255);
    cuarto1->escribir(255);
    patioInterno->escribir(255);
    patioFrontal->escribir(255);
    patioTrasero->escribir(255);
  } 
  
  else {
    sala->escribir(0);
    cocina->escribir(0);
    cuarto1->escribir(0);
    patioInterno->escribir(0);
    patioFrontal->escribir(0);
    patioTrasero->escribir(0);
  }
}

int Casa::contarLEDsEncendidos() {
  int contador = 0;
  if (sala->leer()) contador++;
  if (cocina->leer()) contador++;
  if (cuarto1->leer()) contador++;
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

