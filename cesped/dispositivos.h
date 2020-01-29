/* -*- mode: arduino; coding: utf-8-unix -*- */
//
////////////////////////////////////////////////////////////////////////////
// Fichero:    dispositivos.h
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
#ifndef DISPOSITIVOS_HPP
#define DISPOSITIVOS_HPP

#include <Arduino.h>


const int pinLDR        = 36;
const int pinHumedad1   = 39;
const int pinHumedad2   = 34;
const int pinGotas      = 35;
const int pinPeso       = 32;
const int pinRele       = 13;         // pin del rele


const int pinBuzzer     = 22;

const int pinAzul       = 16;
const int pinRojo       = 17;
const int pinVerde      = 4;

const int pinEstadoRele = 15;

int leerSensorHumedad1();

int leerSensorHumedad2();

int leerSensorPeso();

int leerSensorGotas();

int leerEstadoRele();

/* Función que devuelve una cadena que representa un objeto json:
    - medida: nombre del atributo
    - valor: valor del atributo

    Devuelve una cadena de la forma: {'medida': valor} */
const char *json_medida(String medida, int valor);

const char *json_humedad1(int valor);

const char *json_humedad2(int valor);

const char *json_peso(int valor);

const char *json_gotas(int valor);

const char *json_rele(int valor);

/* Función que configura los dispositivos que se van a utilizar */
void configurar_dispositivos();
#endif
