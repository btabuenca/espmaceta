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

#include "configuracion.h"

int valTemp = 0;
String valJSON = "";

#ifdef __cplusplus
extern "C" {
#endif
 
        uint8_t temprature_sens_read();
 
#ifdef __cplusplus
}
#endif
 
uint8_t temprature_sens_read();


void setup()
{
        Serial.begin(SERIAL_BAUDIOS);
        delay(1000);
        
        Conexion.begin(ssid, pswd);

        ClienteTB.begin(host, port, autorizacion);
}

void loop()
{
     valTemp = temprature_sens_read();
     valJSON = String("{\"temperature\": ") + valTemp + "}";
     ClienteTB.enviar_telemetria(device, valJSON);
     delay(10000);
     
}
