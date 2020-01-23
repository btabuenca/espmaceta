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

        ClienteTB.begin("teamchibi.com", 8080, "eyJhbGciOiJIUzUxMiJ9.eyJzdWIiOiJ0ZW5hbnRAdGhpbmdzYm9hcmQub3JnIiwic2NvcGVzIjpbIlRFTkFOVF9BRE1JTiJdLCJ1c2VySWQiOiJiZGRiNzY1MC1mNGU3LTExZTktODE4OS00YmVhMTRjNDk5NjUiLCJlbmFibGVkIjp0cnVlLCJpc1B1YmxpYyI6ZmFsc2UsInRlbmFudElkIjoiYmM5ZTE0NTAtZjRlNy0xMWU5LTgxODktNGJlYTE0YzQ5OTY1IiwiY3VzdG9tZXJJZCI6IjEzODE0MDAwLTFkZDItMTFiMi04MDgwLTgwODA4MDgwODA4MCIsImlzcyI6InRoaW5nc2JvYXJkLmlvIiwiaWF0IjoxNTc5NTk5Mzc0LCJleHAiOjE1ODg1OTkzNzR9.zhlFEyN94bYW2OY7_G9qiXXHbi4LCvxrxetcRHY6U5rSyiQ5F8m5w0bcFzYx_0nmcNkWgGLgc2zYNHF7_5LWyA");
}

void loop()
{
     ClienteTB.enviar_telemetria("uMNyGKqKGu0WdUoqg1TY","{\"temperature\": 45}");
}
