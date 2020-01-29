/* -*- mode: arduino; coding: utf-8 -*- */
//
////////////////////////////////////////////////////////////////////////////
// Fichero:    dispositivos.cpp
// -------------------------------------------------------------------------
// Proyecto:     C.E.S.P.E.D.
// Autor:      Víctor Fonseca Salmerón, Juan Luis Moreno Sancho
// Descripción:
//
//   Sistema de lectura de datos desde sensores y subida a un servicio Thingsboard.
//
// -------------------------------------------------------------------------
//   Historia: + 08/01/2020 - Primera versión
//             + 20/01/2020 - (Juan Luis+Victor) Lectura desde sensores
//             + 25/01/2020 - Nueva versión
//             + 27/01/2020 - (Juan Luis+Victor) Añadidos todos los sensores
//             + 29/01/2020 - Introducir la funcionalidad en una libreria
////////////////////////////////////////////////////////////////////////////
#include "dispositivos.h"

#include <Arduino.h>

#include "cliente_tb.h"

int leerSensorHumedad1() {
     int humedad=analogRead(pinHumedad1);
     return humedad;
}

int leerSensorHumedad2() {
     int humedad=analogRead(pinHumedad2);
     return humedad;
}

int leerSensorPeso() {
     int peso=analogRead(pinPeso);
     Serial.print("peso que lee el sensor: ");
     Serial.println(peso);
   //  peso = (peso*18.41)/1000;
     return peso;
}

int leerSensorGotas() {
     int gotas=analogRead(pinGotas);
     return gotas;
}

int leerEstadoRele() {
     int rele=digitalRead(pinEstadoRele);
     return rele;
}    


/* Función que devuelve una cadena que representa un objeto json:
    - medida: nombre del atributo
    - valor: valor del atributo

    Devuelve una cadena de la forma: {'medida': valor} */
const char *json_medida(String medida, int valor) {
     String resString {""};     
     resString = String("{\"" + medida + "\": ") + valor + "}";
     return resString.c_str();
}

const char *json_humedad1(int valor) {
  return json_medida("humedad1", valor);
}

const char *json_humedad2(int valor) {
  return json_medida("humedad2", valor);
}

const char *json_peso(int valor) {
  return json_medida("peso", valor);
}

const char *json_gotas(int valor) {
  return json_medida("gotas", valor);
}

const char *json_rele(int valor) {
  return json_medida("rele", valor);
}

void configurar_dispositivos() {
        pinMode(pinRele, OUTPUT);
        pinMode(pinBuzzer, OUTPUT);
        pinMode(pinAzul, OUTPUT);
        pinMode(pinRojo, OUTPUT);
        pinMode(pinVerde, OUTPUT);
        pinMode(pinEstadoRele,INPUT);
}
