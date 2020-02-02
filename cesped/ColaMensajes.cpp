/* -*- mode: c++; coding: utf-8 -*- */
//
////////////////////////////////////////////////////////////////////////////
// Fichero:    ColaMensajes.h
// -------------------------------------------------------------------------
// Proyecto:     C.E.S.P.E.D.
// Autor:      José L. Domenech
// Descripcion:
//
//   Clase para comunicar mensajes entre tareas
//
//   Se puede usar el objeto interfaz: ColaMensajes
//
// -------------------------------------------------------------------------
//   Historia: + 01/02/2020 - Primera versión
////////////////////////////////////////////////////////////////////////////
#include "ColaMensajes.h"

//     - - - INTERFAZ Cola de Mensajes MQTT - - -
mensaje_t::mensaje_t()
     :mensaje{""}
{};

mensaje_t::mensaje_t(String mensaje)
     :mensaje{mensaje}
{};

ColaMensajesClass::ColaMensajesClass(int tamano)
     :tamano{tamano}, ticks_espera{QUEUE_MQTT_MAXIMO_MS_ESPERA != portMAX_DELAY ? QUEUE_MQTT_MAXIMO_MS_ESPERA / portTICK_RATE_MS : portMAX_DELAY}
{
     this->cola = xQueueCreate( TAMANO_COLA_MQTT, sizeof( ptr_mensaje_t ) );;
     if(this->cola == NULL){
          Serial.println("Error creating the queue");
     }
};

void ColaMensajesClass::encolar(String msg)
{
     ptr_mensaje_t ptr_msg_cola = new mensaje_t(msg);
     ptr_msg_cola->mensaje = msg;

     xQueueSend(this->cola, &ptr_msg_cola, this->ticks_espera);
}

ptr_mensaje_t ColaMensajesClass::desencolar()
{

     ptr_mensaje_t ptr_msg_cola;

     xQueueReceive(this->cola, &ptr_msg_cola, this->ticks_espera);
     
     return ptr_msg_cola;

}

int ColaMensajesClass::mensajes() {
    return uxQueueMessagesWaiting( this->cola );
}
     

ColaMensajesClass::~ColaMensajesClass()
{
     vQueueDelete ( this->cola );
}

ColaMensajesClass ColaMensajes;
