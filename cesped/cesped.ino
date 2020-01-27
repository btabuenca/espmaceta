/* -*- mode: arduino; coding: utf-8 -*- */
//
////////////////////////////////////////////////////////////////////////////
// Fichero:    cesped.ino
// -------------------------------------------------------------------------
// Proyecto:     C.E.S.P.E.D.
// Autor:      José L. Domenech
// Descripción:
//
//   Sistema de lectura de datos desde sensores y subida a un servicio Thingsboard.
//
// -------------------------------------------------------------------------
//   Historia: + 08/01/2020 - Primera versión
//             + 20/01/2020 - (Juan Luis+Victor) Lectura desde sensores
//             + 25/01/2020 - Nueva versión
//             + 27/01/2020 - (Juan Luis+Victor) Añadidos todos los sensores
////////////////////////////////////////////////////////////////////////////
//#include <iostream.h>   
//#include <string.h> 
#include "conexion.h"
#include "ota.h"
#include "tareas.h"
#include "cliente_tb.h"
#include "configuracion.h"

int i=0;
WebServer servidor(80);
const int pinGotas = 34;
const int pinLDR=35;
const int pinPeso = 39;
const int pinRele=5;         // pin del rele
const int pinHumedad1=32;
const int pinHumedad2=33;
const int pinBuzzer=22;

const int pinAzul=16;
const int pinRojo=17;
const int pinVerde=4;

const int pinEstadoRele=15;


/*int leer_temperatura_interna() {
     return (uint8_t)temprature_sens_read();
}*/

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

const char *enviar_medida(const char *json) {
     respuesta_tb_t res = ClienteTB.enviar_telemetria(device_token, json);

//     Serial.println(json);
     return (char *)res.second.c_str();
}

ptr_Tarea sensorHumedad1, sensorHumedad2, sensorPeso,sensorGotas, estadoRele;


void setup()
{
        Serial.begin(SERIAL_BAUDIOS);
        delay(1000);

        pinMode(pinRele, OUTPUT);
        pinMode(pinBuzzer, OUTPUT);
        pinMode(pinAzul, OUTPUT);
        pinMode(pinRojo, OUTPUT);
        pinMode(pinVerde, OUTPUT);
        pinMode(pinEstadoRele,INPUT);
        
        Serial.println("Iniciando");
        
        Conexion.begin(ssid, pswd);
        configurarServidor(servidor, tb_host);
        servidor.begin();
        
        ClienteTB.begin(tb_host, tb_http_port, autorizacion);

        sensorHumedad1 = new Tarea("sensorHumedad1",
                                      1000,
                                      1, // la misma prioridad a todas
                                      leerSensorHumedad1,
                                      json_humedad1,
                                      enviar_medida);

        sensorHumedad2 = new Tarea("sensorHumedad2",
                                      1000,
                                      1, // la misma prioridad a todas
                                      leerSensorHumedad2,
                                      json_humedad2,
                                      enviar_medida);

        sensorPeso = new Tarea("sensorPeso",
                                      1000,
                                      1, // la misma prioridad a todas
                                      leerSensorPeso,
                                      json_peso,
                                      enviar_medida);
        sensorGotas = new Tarea("sensorGotas",
                                      1000,
                                      1, // la misma prioridad a todas
                                      leerSensorGotas,
                                      json_gotas,
                                      enviar_medida);

        estadoRele = new Tarea("estadoRele",
                                      1000,
                                      1, // la misma prioridad a todas
                                      leerEstadoRele,
                                      json_rele,
                                      enviar_medida);

}

void loop()
{
     servidor.handleClient();
     Serial.print("    MD5 SKETCH: "); Serial.println(ESP.getSketchMD5().toString());
     Serial.print("            IP: "); Serial.println(Conexion.getIP().toString());
     Serial.print("FREE HEAP MEM.: "); Serial.println(ESP.getFreeHeap());
     Serial.println("===========================")
//     delay(10000);
}
