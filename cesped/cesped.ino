/* -*- mode: arduino; coding: utf-8 -*- */
//
////////////////////////////////////////////////////////////////////////////
// Fichero:    cesped.ino
// -------------------------------------------------------------------------
// Proyecto:     C.E.S.P.E.D.
// Autor:      José L. Domenech
// Descripción:
//
//   Ejemplo conectar a una red WiFi + leer de un sensor + enviar los datos a
// Thingsboard
// -------------------------------------------------------------------------
// Historia: + 22/01/2020 - Primera versión
////////////////////////////////////////////////////////////////////////////

#include "conexion.h"
#include "cliente_tb.h"

#include <ESP32.h>

#include "configuracion.h"

int valTemp = 0;


void setup()
{
        Serial.begin(SERIAL_BAUDIOS);
        delay(1000);
        
        Conexion.begin(ssid, pswd);

        ClienteTB.begin(host, port, nullptr);
}

void loop()
{
     valTemp = temprature_sens_read();
     ClienteTB.enviar_telemetria(device,"{\"temperature\": " + valTemp + "}");
     
}
