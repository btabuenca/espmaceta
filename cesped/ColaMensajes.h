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
// -------------------------------------------------------------------------
//   Historia: + 01/02/2020 - Primera versión
////////////////////////////////////////////////////////////////////////////
#ifndef COLA_MENSAJES_HPP
#define COLA_MENSAJES_HPP

#include <Arduino.h>

// cola de mensajes mqtt
#define TAMANO_COLA_MQTT 20

#define QUEUE_MQTT_MAXIMO_MS_ESPERA 1000
// máximo tiempo de espera (en Ticks):
//  - portMAX_DELAY para infinito

// clase que contiene el mensaje a enviar mediante MQTT
struct mensaje_t {
     // constructores:
     mensaje_t();
     mensaje_t(String mensaje);
     
     // propiedades:
     // topic no es necesario
     String mensaje;
};
typedef mensaje_t* ptr_mensaje_t;

class ColaMensajesClass {
 public:
     // crea una cola de mensajes con una capacidad inicial indicada por tamaño
     ColaMensajesClass(int tamano=TAMANO_COLA_MQTT);

     // introduce en la cola un mensaje con los campos del mensaje inicializados
     // con los parámetros (sólo el string)
     void encolar(String str);

     // devuelve un puntero a un mensaje encolado y lo desencola
     ptr_mensaje_t desencolar();

     // devuelve el número de mensajes en la cola
     int mensajes();

     // destruye la cola de mensajes
     ~ColaMensajesClass();
 private:
     int tamano;
     QueueHandle_t cola;
     int ticks_espera;
};
     
extern ColaMensajesClass ColaMensajes;

#endif
