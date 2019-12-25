/* -*- mode: arduino; coding: utf-8 -*- */
//
////////////////////////////////////////////////////////////////////////////
// Fichero:    cesped.ino
// -------------------------------------------------------------------------
// Proyecto:     C.E.S.P.E.D.
// Autor:      José L. Domenech
// Descripción:
//
//   Ejemplo de crear una tarea.
// -------------------------------------------------------------------------
//   Historia: + 18/12/2019 - Primera versión
////////////////////////////////////////////////////////////////////////////

#include "tareas.h"

ptr_Tarea p_mi_tarea;

void setup()
{
        Serial.begin(9600);
        delay(1000);
        
        p_mi_tarea = new Tarea(
                "Prueba", // nombre
                1500,     // periodo ms
                1,        // prioridad
                []() -> int { return 10; }, // fun. lectora
                [](int val) -> char* { return "Hola"; }, // fun. trans.
                [](char* val) -> char* { Serial.println(val); return val; }); // fun. envio
        
}

void loop()
{ }
