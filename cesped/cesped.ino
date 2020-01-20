/* -*- mode: arduino; coding: utf-8 -*- */
//
////////////////////////////////////////////////////////////////////////////
// Fichero:    cesped.ino
// -------------------------------------------------------------------------
// Proyecto:     C.E.S.P.E.D.
// Autor:      José L. Domenech
// Descripción:
//
//   Ejemplo de conectar a una red WiFi e iniciar un servicio Web para la
//   actualización OTA.
//
// -------------------------------------------------------------------------
//   Historia: + 08/01/2020 - Primera versión
//             + 20/01/2010 - (Juan Luis+Victor) Lectura desde sensores
////////////////////////////////////////////////////////////////////////////
//#include <iostream.h>   
//#include <string.h> 
#include "conexion.h"
#include "ota.h"
#include "tareas.h"

const char* host = "esp32";
const char* ssid = "";
const char* password = "";
static ptr_Tarea p_mi_tarea;
int i=0;
WebServer servidor(80);
const int pinGotas = 34;
int pin=5;
int pinAzul=32;
int pinRojo=33;
int pinVerde=25;

void setup()
{
        Serial.begin(9600);
        delay(1000);
        
        Conexion.begin(ssid, password);
        configurarServidor(servidor, host);
        servidor.begin();
        

     p_mi_tarea = new Tarea(
          "Prueba", // nombre
          3000,     // periodo ms
          1,        // prioridad
          []() -> int { 
                        int gotas=0;
                        gotas = analogRead(pinGotas);
                        Serial.println(pinGotas);
                        Serial.println(gotas);
                        return gotas; 
                      }, // fun. lectora
          [](int val) -> char* {
                                 pinMode(pin, OUTPUT);

                                 if(val<1000)
                                  digitalWrite(pin,LOW);
                                 else
                                  digitalWrite(pin,HIGH);
                                 return "Hola";
                               }, // fun. trans.
          [](char* val) -> char* { Serial.println(val); return val; }); // fun. envio
     p_mi_tarea = new Tarea(
          "LDR", // nombre
          3000,     // periodo ms
          2,        // prioridad
          []() -> int { 
                        int luz=0;
                        luz = analogRead(35);
                        Serial.print("Luz ");
                        Serial.println(luz);
                        return luz; 
                      }, // fun. lectora
          [](int val) -> char* {
                                 pinMode(pinAzul, OUTPUT);
                                 pinMode(pinVerde, OUTPUT);
                                 pinMode(pinRojo, OUTPUT);
                                 
                                 if(val>3500)//4095 oscuridad total
                                 {
                                  digitalWrite(pinRojo,HIGH);
                                  digitalWrite(pinAzul,LOW);
                                  digitalWrite(pinRojo,LOW);
                                 }
                                 if(val>1000&&val<3499)
                                 {
                                  digitalWrite(pinRojo,LOW);
                                  digitalWrite(pinAzul,HIGH);
                                  digitalWrite(pinVerde,LOW);
                                 }
                                 if(val<999)
                                 { 
                                  digitalWrite(pinRojo,LOW);
                                  digitalWrite(pinAzul,LOW);
                                  digitalWrite(pinVerde,HIGH);
                                 }
                                 return "Hola";
                               }, // fun. trans.
          [](char* val) -> char* { Serial.println(val); return val; }); // fun. envio
}

void loop()
{
  
     servidor.handleClient();
     delay(1);
     
}
