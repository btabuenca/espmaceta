/* -*- mode: arduino; coding: utf-8 -*- */
//
////////////////////////////////////////////////////////////////////////////
// Fichero:    cesped.ino
// -------------------------------------------------------------------------
// Proyecto:     C.E.S.P.E.D.
// Autor:      José L. Domenech
// Descripción:
//
//   Ejemplo de envío de telemetria a ThingsBoard.
// -------------------------------------------------------------------------
//   Historia: + 18/12/2019 - Primera versión
////////////////////////////////////////////////////////////////////////////

#include "conexion.h"
#include "cliente_tb.h"

char *ssid = "";
char *passwd = "";
char *device = "";
int tb_port = 80;
char *tb_host = "";

void setup()
{
        Serial.begin(9600);
        delay(1000);
        
        Conexion.begin(ssid, passwd);

        ClienteTB.begin(tb_host, tb_port, "");
}

void loop()
{
     ClienteTB.enviar_telemetria("","{\"temperature\": 45}");
     delay(15000);
}
