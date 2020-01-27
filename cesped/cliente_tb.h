/* -*- mode: c++; coding: utf-8 -*- */
//
////////////////////////////////////////////////////////////////////////////
// Fichero:    cliente_tb.h
// -------------------------------------------------------------------------
// Proyecto:     C.E.S.P.E.D.
// Autor:      José L. Domenech
// Descripción:
//
//   Mantenimiento de los datos de conexión con un servicio thingsboard y acceso
//   a la API del servicio.
//
//   Clase ClienteTBClass: mantiene los datos de conexión con el servicio ThingsBoard
//
//    + begin: cambia los parámetros de conexión: host, puerto, token de
//    autenticación que se utilizaran al hacer las peticiones de la API
//    + enviar_telemetria: envia un dato String con formato JSON.
//      Devuelve el resultado como estructura std::pair<CodigoHTTP,respuesta>.
//    + ~ClienteTBClass: destructor, libera los recursos utilizados
//
//   Utilizar usando el objeto interfaz ClienteTB.begin(host, puerto, auth [, protocolo]);
//   El token de autorización se puede obtener con:
//     curl -X POST --header 'Content-Type: application/json' --header 'Accept: application/json' -d '{"username":"tenant@thingsboard.org", "password":"tenant"}' 'http://THINGSBOARD_URL/api/auth/login'
//
// -------------------------------------------------------------------------
//   Historia: + 18/12/2019 - Primera versión
////////////////////////////////////////////////////////////////////////////
#ifndef CLIENTE_THINGSBOARD_HPP
#define CLIENTE_THINGSBOARD_HPP

#include <utility> // std::pair
#include <Arduino.h> // String
#include <WiFi.h>
#include <HTTPClient.h> // HTTPClient

#define MAX_MS_ESPERA_MUTEX_HTTP 1000

enum class Protocolo_t { http, https };

// Clase para mantener los datos de conexión a un servicio ThingsBoard
class ClienteTBClass
{
public:
     // Cambia los parámetros de conexión:
     //  - host: host del servicio
     //  - puerto: puerto del servicio
     //  - auth: token de autenticación
     //  - proto: protocolo usado: "https" o "http"
     //
     // devuelve true si se consiguió la conexión
     bool begin(String host, int puerto, String auth, Protocolo_t proto=Protocolo_t::http);

     // envia al dispositivo indentificado por token el dato JSON.
     //  - token_dispositivo: identificador del dispositivo en el servicio
     //  - json: json a enviar. p.e.: "{'nombre': 'valor'}"
     //
     // devuelve el par (std::pair) formado por el codigo HTTP (si hubo un error
     // <= 0) y el contenido devuelto por el servidor. Si no se pudo establecer
     // la conexión devuelve 999 en como código HTTP
     std::pair<int,String> enviar_telemetria(String token_dispositivo, String json);

     // Destructor: libera los recursos utilizados:
     // - mutex
     ~ClienteTBClass();
private:
     // propiedades:
     Protocolo_t proto;
     String host;
     int puerto;
     String auth;

     HTTPClient http; // instantacia de HTTPClient
     SemaphoreHandle_t http_mutex; // mutex para el acceso exclusivo a http
     // En FreeRTOS los mutexes poseen escalado de prioridad
};

// objeto interfaz
extern ClienteTBClass ClienteTB;

#endif
