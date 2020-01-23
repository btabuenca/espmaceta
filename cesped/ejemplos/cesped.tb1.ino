/* -*- mode: arduino; coding: utf-8 -*- */
//
////////////////////////////////////////////////////////////////////////////
// Fichero:    cesped.ino
// -------------------------------------------------------------------------
// Proyecto:     C.E.S.P.E.D.
// Autor:      José L. Domenech
// Descripción:
//
//   Ejemplo de conectar a una red WiFi.
// -------------------------------------------------------------------------
//   Historia: + 18/12/2019 - Primera versión
////////////////////////////////////////////////////////////////////////////

#include "conexion.h"
#include "cliente_tb.h"

void setup()
{
        Serial.begin(9600);
        delay(1000);
        
        Conexion.begin("", "");

        ClienteTB.begin("", 80, nullptr);
}

void loop()
{
     ClienteTB.enviar_telemetria("uMNyGKqKGu0WdUoqg1TY","{\"temperature\": 45}");
}
