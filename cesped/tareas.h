/* -*- mode: c++; coding: utf-8 -*- */
//
////////////////////////////////////////////////////////////////////////////
// Fichero:    tareas.h
// -------------------------------------------------------------------------
// Proyecto:     C.E.S.P.E.D.
// Autor:      José L. Domenech
// Descripcion:
//
//   Clase para configurar y lanzar tareas.
//
//    clase Tarea: clase auxiliar asociada a una tarea de tiempo real.
//     Ejecutará fe(ft(fl())) cada N ms.
//      + Tarea(...) constructor de la tarea
//      - nombre:    nombre de la tarea
//      - ms_espera: tiempo de ciclo de ejecución de la tarea (en ms.)
//      - prioridad: prioridad de la tarea
//      - fl:        función de lectura
//      - ft:        función de transformación
//      - fe:        función de envío
//      + parar():   para la ejecución de la tarea, destruyendola
//
//  tipo funcion_lectura_t:        tipo de la función de lectura: int()
//  tipo funcion_transformadora_t: tipo de la func. transformadora: char*(int)
//  tipo funcion_envio_t:          tipo de la func. de envío: char*(char*)
// -------------------------------------------------------------------------
//   Historia: + 18/12/2019 - Primera versión
////////////////////////////////////////////////////////////////////////////
#ifndef TAREAS_HPP
#define TAREAS_HPP

#include <Arduino.h> // String

#include <functional> // std::function
#include <memory> // std::unique_ptr

// memoria que se asigna a una tarea en su creación
#define tarea_STACK_SIZE		(10*configMINIMAL_STACK_SIZE)

// tipos de la funciones que se ejecutan dentro de una tarea:
typedef std::function<int(void)>                  funcion_lectura_t;
typedef std::function<const char *(int)>          funcion_transformadora_t;
typedef std::function<const char *(const char *)> funcion_envio_t;

// función básica que se ejecuta siempre como tarea - - -
static void funcion_tareas(void* pvParameters);

// clase para manejar una tarea.
struct Tarea
{
     // constructor
     Tarea(char *nombre, unsigned long ms_espera, int prioridad,
           funcion_lectura_t fl, funcion_transformadora_t ft, funcion_envio_t fe);

     // destructor
     ~Tarea();

     // para la ejecución de la tarea, destruyendola en el proceso
     void parar();

     // propiedades
     char *nombre;
     unsigned long ms_espera;
     int prioridad;
     funcion_lectura_t fl;
     funcion_transformadora_t ft;
     funcion_envio_t fe;
     bool marca_alto = false;
     TaskHandle_t *ptr_h_tarea;
};
typedef Tarea* ptr_Tarea;

#endif
