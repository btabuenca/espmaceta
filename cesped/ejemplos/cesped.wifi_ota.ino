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
////////////////////////////////////////////////////////////////////////////

#include "conexion.h"
#include "ota.h"

const char* host = "esp32";
const char* ssid = "xxx";
const char* password = "xxxx";

WebServer servidor(80);

void setup()
{
        Serial.begin(9600);
        delay(1000);
        
        Conexion.begin(ssid, password);

        configurarServidor(servidor, host);
}

void loop()
{
     servidor.handleClient();
     delay(1);
}
