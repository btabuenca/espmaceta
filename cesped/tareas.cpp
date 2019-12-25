/* -*- mode: c++; coding: utf-8 -*- */
//
////////////////////////////////////////////////////////////////////////////
// Fichero:    tareas.cpp
// -------------------------------------------------------------------------
// Proyecto:     C.E.S.P.E.D.
// Autor:      José L. Domenech
// Descripcion:
//
//   Clase para configurar y lanzar tareas.
// -------------------------------------------------------------------------
//   Historia: + 18/12/2019 - Primera versión
////////////////////////////////////////////////////////////////////////////

#include "tareas.h"
#include <memory> // std::unique_ptr

//     - - - INTERFAZ Tarea - - -
// constructor
Tarea::Tarea(char *nombre, unsigned long ms_espera, int prioridad, funcion_lectura_t fl, funcion_transformadora_t ft, funcion_envio_t fe)
     :nombre{nombre}, ms_espera{ms_espera}, prioridad{prioridad},
      fl{fl}, ft{ft}, fe{fe},
      ptr_h_tarea{new TaskHandle_t()} // instanciar un handle para usar con la tarea
{
     // iniciar la tarea al crear
     xTaskCreate(
          funcion_tareas,
          nombre,
          tarea_STACK_SIZE,
          (void *)this,
          prioridad,
          this->ptr_h_tarea);
};

// destructor
Tarea::~Tarea()
{
     this->parar();
}

// para y destruye una tarea
void Tarea::parar()
{
     this->marca_alto = true;
};

//     - - - función básica que se ejecuta siempre como tarea - - -
static void funcion_tareas(void* pvParameters)
{
     // obtener el puntero a la estructura de datos deseada
     ptr_Tarea pxParameters;        
     pxParameters = ( ptr_Tarea ) pvParameters;

     // obtener el handle a la tarea que ejecuta esta función
     TaskHandle_t *ptr_my_task_handle = pxParameters->ptr_h_tarea;
        
     // calcular los ticks de frecuencia
     const TickType_t frecuencia = (pxParameters->ms_espera / portTICK_RATE_MS);

     // variable para almacenar ("automáticamente") el tiempo de ejecución
     TickType_t xLastWakeTime;
     xLastWakeTime = xTaskGetTickCount();

     // entrar en bucle infinito hasta que el valor apuntado por marca_alto sea "false"
     for(;!(pxParameters->marca_alto);)
     {
          vTaskDelayUntil( &xLastWakeTime, frecuencia);
          int lectura = pxParameters->fl();
          char *dato  = pxParameters->ft(lectura);
          char *res = pxParameters->fe(dato);
     }
     // si se sale del bucle: destruir la tarea
     if( *ptr_my_task_handle != NULL )
     {
          vTaskDelete( *ptr_my_task_handle );
               
          delete ptr_my_task_handle;          
     }
}
