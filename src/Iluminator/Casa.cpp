#include "LED.h"
#include "SensorLDR.h"
#include "Boton.h"
#include "PantallaLCD.h"
#include "Modos.h"
#include "Casa.h"
#include <Arduino.h>

//Constructor con pines de simulación:
Casa::Casa() :
  sala(new LED(2)),
  cocina(new LED(3)),
  cuarto1(new LED(4)),
  cuarto2(new LED(5)),
  cuarto3(new LED(6)),
  patioInterno(new LED(7)),
  patioFrontal(new LED(8)),
  patioTrasero(new LED(9)),

  sensor(new SensorLDR(A0, 500)),

  botonSala(new Boton(10)),
  botonCocina(new Boton(11)),
  botonCuarto1(new Boton(12)),
  botonCuarto2(new Boton(13)),
  botonCuarto3(new Boton(A1)),
  botonPatioInt(new Boton(A2)),
  botonPatioFront(new Boton(A3)),
  botonPatioTras(new Boton(A4)),
  botonAuto(new Boton(A5)),
  botonModo(new Boton(1)),        // Se usa pin 1 para simulación

  pantalla(new PantallaLCD()),

  modoActual(MODO_AUTO),
  autoActivo(true),
  ledsEncendidos(0),
  ultimaActualizacionLCD(0)
  {}

void Casa::iniciar() {
  //Iniciar todos los componentes
  sala->iniciar();
  cocina->iniciar();
  cuarto1->iniciar();
  cuarto2->iniciar();
  cuarto3->iniciar();
  patioInterno->iniciar();
  patioFrontal->iniciar();
  patioTrasero->iniciar();

  sensor->iniciar();
  sensor->setSimulacion(true);  // Modo simulación

  botonSala->iniciar();
  botonCocina->iniciar();
  botonCuarto1->iniciar();
  botonCuarto2->iniciar();
  botonCuarto3->iniciar();
  botonPatioInt->iniciar();
  botonPatioFront->iniciar();
  botonPatioTras->iniciar();
  botonAuto->iniciar();
  botonModo->iniciar();
  
  pantalla->iniciar();
  
  // Configurar simulación en botones
  botonSala->escribir(0);
  botonCocina->escribir(0);
  botonAuto->escribir(0);
  botonModo->escribir(0);
  
  Serial.begin(9600);
  Serial.println("Sistema de Iluminación Iniciado (Modo Simulación)");
}

void Casa::actualizar() {
  //Cambiar modo global
  if (botonModo->fuePresionado()) {
    cambiarModo();
  }

  //Activar/desactivar automático
  if (botonAuto->fuePresionado()) {
    autoActivo = !autoActivo;
    Serial.print("Modo auto: ");
    Serial.println(autoActivo ? "ACTIVADO" : "DESACTIVADO");
  }

  //Control según modo
  if (autoActivo && modoActual == MODO_AUTO) {
    controlAutomatico();
  }
  else {
    verificarBotonesManuales();
  }

  // Aplicar modo global si no es Auto
  if (modoActual != MODO_AUTO && modoActual != MODO_MANUAL) {
    aplicarModoGlobal(modoActual);
  }

  //Actualizar LCD cada 500ms
  if (millis() - ultimaActualizacionLCD > 500) {
    actualizarLCD();
    ultimaActualizacionLCD = millis();
  }
}

void Casa::cambiarModo() {
  modoActual = static_cast<ModoIluminacion>((modoActual + 1) % 7);
  Serial.print("Modo cambiado a: ");
  Serial.println(nombreModo(modoActual));

  // Si cambia a manual o auto, apagar modos especiales
  if (modoActual == MODO_MANUAL || modoActual == MODO_AUTO) {
    //Restaurar estado normal
    sala->escribir(sala->leer());
    cocina->escribir(cocina->leer());
    cuarto1->escribir(cuarto1->leer());
    cuarto2->escribir(cuarto2->leer());
    cuarto3->escribir(cuarto3->leer());
    patioInterno->escribir(patioInterno->leer());
    patioFrontal->escribir(patioFrontal->leer());
    patioTrasero->escribir(patioTrasero->leer());
  }
}

void Casa::aplicarModoGlobal(ModoIluminacion modo) {
  switch(modo) {
    case MODO_NOCHE:
      sala->escribir(50);  // Luz tenue
      cocina->escribir(0); // Apagado
      cuarto1->escribir(30);
      cuarto2->escribir(30);
      cuarto3->escribir(30);
      patioInterno->escribir(30);
      patioFrontal->escribir(10);
      patioTrasero->escribir(10);
      break;

    case MODO_LECTURA:
      sala->escribir(255);
      cocina->escribir(200);
      cuarto1->escribir(255);
      cuarto2->escribir(255);
      cuarto3->escribir(255);
      patioInterno->escribir(100);
      patioFrontal->escribir(0);
      patioTrasero->escribir(0);
      break;

    case MODO_FIESTA:
    // Efecto parpadeante
      static unsigned long ultimoCambioFiesta = 0;
      if (millis() - ultimoCambioFiesta > 300) {
        int estado = random(0, 2);
        sala->escribir(estado * 255);
        cocina->escribir(!estado * 255);
        cuarto1->escribir(!estado * 255);
        cuarto2->escribir(!estado * 255);
        cuarto3->escribir(!estado * 255);
        patioInterno->escribir(estado * 255);
        patioFrontal->escribir(!estado * 255);
        patioTrasero->escribir(estado * 255);
        ultimoCambioFiesta = millis();
      }
      break;

    case MODO_RELAJACION:
    // Luz suave
      sala->escribir(80);
      cocina->escribir(60);
      cuarto1->escribir(70);
      cuarto2->escribir(70);
      cuarto3->escribir(70);
      patioInterno->escribir(40);
      patioFrontal->escribir(30);
      patioTrasero->escribir(30);
      break;

    case MODO_APAGADO:
      sala->escribir(0);
      cocina->escribir(0);
      cuarto1->escribir(0);
      cuarto2->escribir(0);
      cuarto3->escribir(0);
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

  //Cuarto2
  if (botonCuarto2->fuePresionado()) {
    bool estado = cuarto2->leer();
    cuarto2->escribir(!estado);
    Serial.println("Cuarto2: " + String(!estado ? "ENCENDIDO" : "APAGADO"));
  }

  //Cuarto3
  if (botonCuarto3->fuePresionado()) {
    bool estado = cuarto3->leer();
    cuarto3->escribir(!estado);
    Serial.println("Cuarto3: " + String(!estado ? "ENCENDIDO" : "APAGADO"));
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
    sala->escribir(1);
    cocina->escribir(1);
    cuarto1->escribir(1);
    cuarto2->escribir(1);
    cuarto3->escribir(1);
    patioInterno->escribir(1);
    patioFrontal->escribir(1);
    patioTrasero->escribir(1);
  } 
  
  else {
    sala->escribir(0);
    cocina->escribir(0);
    cuarto1->escribir(0);
    cuarto2->escribir(0);
    cuarto3->escribir(0);
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
  if (cuarto2->leer()) contador++;
  if (cuarto3->leer()) contador++;
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

