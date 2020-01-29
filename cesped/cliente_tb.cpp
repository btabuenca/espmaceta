/* -*- mode: c++; coding: utf-8 -*- */
//
////////////////////////////////////////////////////////////////////////////
// Fichero:    cliente_tb.cpp
// -------------------------------------------------------------------------
// Proyecto:     C.E.S.P.E.D.
// Autor:      José L. Domenech
// Descripción:
//   Mantiene los datos de conexión con un servicio thingsboard
//
// Referencia a HTTPClient para ESP32:
// https://github.com/espressif/arduino-esp32/blob/master/libraries/HTTPClient/src/HTTPClient.h
// -------------------------------------------------------------------------
// Historia: + 18/12/2019 - Primera versión
////////////////////////////////////////////////////////////////////////////


#include "cliente_tb.h"

#include <vector>

// path de la URI de la api de telemetria
String TB_API_TELEMETRIA = "/api/v1/_TOKEN_DISPOSITIVO_/telemetry/";
// path de la URI de la api de alarmas 
String TB_API_ALARMAS    = "/api/alarm/_TIPO_DISPOSITIVO_/_ID_DISPOSITIVO_/";

respuesta_tb_t make_respuesta_tb(int codigo, String respuesta)
{
     return std::make_pair(codigo, respuesta);
}

// Cambia los parámetros de conexión:
//  - host: host del servicio
//  - puerto: puerto del servicio
//  - auth: token de autenticación
//  - proto: protocolo usado: Protocolo_t::http ó Protocolo_t::https
bool ClienteTBClass::begin(String host, int puerto, String auth, Protocolo_t proto)
{
     this->proto = proto;
     this->host = host;
     this->puerto = puerto;
     this->auth = auth;

     this->http_mutex = xSemaphoreCreateMutex(); // crea un mutex para el acceso exclusivo a http
};

// envia al dispositivo indentificado por token el dato JSON.
//  - token_dispositivo: identificador del dispositivo en el servicio
//  - json: json a enviar. p.e.: "{'nombre': 'valor'}"
//
// Devuelve el par (std::pair) formado por el codigo HTTP o de error (<= 0) y el
// contenido devuelto por el servidor. Si no se pudo establecer la conexión devuelve 999
respuesta_tb_t ClienteTBClass::enviar_telemetria(String token_dispositivo, String json)
{
     
     String uri_path = String(TB_API_TELEMETRIA);
     uri_path.replace("_TOKEN_DISPOSITIVO_", token_dispositivo);
     
     // acceso exclusivo a this->http
     if( xSemaphoreTake( this->http_mutex, pdMS_TO_TICKS(MAX_MS_ESPERA_MUTEX_HTTP)) == pdPASS ) {
     
          // iniciar comunicación con el servidor
          this->http.begin(this->host, this->puerto, uri_path);
     
          // indicar las cabeceras necesarias de la API:
          this->http.addHeader("Content-Type","application/json");
          // this->http.addHeader("X-Authorization", "Bearer " + this->auth);
     
          // enviar la petición y recibir el código HTTP (o error <= 0)
          int httpStatus = this->http.POST(json);
     
          respuesta_tb_t res = make_respuesta_tb(httpStatus, httpStatus > 0 ? this->http.getString() : "");

          http.end();

          xSemaphoreGive( this->http_mutex );

          return res;
     } else {
          respuesta_tb_t res = make_respuesta_tb(999,String("No se pudo acceder al recurso this->http"));

          return res;
     }

     // no debería llegar aquí
     return make_respuesta_tb(666,"¡Entra en pánico! ¡No deberías ver esto!");
};

// leer alarmas de un dispositivo desde el servicio Thingsboard
//  - tipo_dispositivo: tipo de dispositivo que contiene las alarmas
//  - id_dispositivo: identificador del dispositivo en el servicio
//  - cabeceras: vector de cabeceras que se incluirán, sirven para filtrar
//               las alarmas devueltas
//  - limite: número máximo de alarmas que se devolveran
//
// Devuelve el par (std::pair) formado por el codigo HTTP (si hubo un error
// <= 0) y el contenido devuelto por el servidor. Si no se pudo establecer
// la conexión devuelve 999 en como código HTTP
//
// Los parámetros admitidos (los mismos que la API thingsboard) son:
// - searchStatus
// - status
// - limit [OBLIGATORIO] (por eso lo incluyo con un valor por defecto=10)
// - startTime
// - endTime
// - ascOrder
// - offset
// - fetchOriginator
respuesta_tb_t ClienteTBClass::leer_alarmas(String tipo_dispositivo, String id_dispositivo, std::vector<cabecera_tb_t> cabeceras, int limite)
{
          
     String uri_path = String(TB_API_ALARMAS);
     uri_path.replace("_ID_DISPOSITIVO_", id_dispositivo);
     uri_path.replace("_TIPO_DISPOSITIVO",tipo_dispositivo);
     
     // acceso exclusivo a this->http
     if( xSemaphoreTake( this->http_mutex, pdMS_TO_TICKS(MAX_MS_ESPERA_MUTEX_HTTP)) == pdPASS ) {
     
          // iniciar comunicación con el servidor
          this->http.begin(this->host, this->puerto, uri_path);
     
          // indicar las cabeceras necesarias de la API:
          this->http.addHeader("Content-Type","application/json");
          this->http.addHeader("X-Authorization", "Bearer " + this->auth);
          this->http.addHeader("limit", String(limite));

          // rellenar las cabeceras de la petición
          for(auto cab : cabeceras) {
               this->http.addHeader(cab[0],cab[1],false,true);
          };
          
          // enviar la petición y recibir el código HTTP (o error <= 0)
          int httpStatus = this->http.GET();
     
          respuesta_tb_t res = make_respuesta_tb(httpStatus, httpStatus > 0 ? this->http.getString() : "");

          http.end();

          xSemaphoreGive( this->http_mutex );

          return res;
     } else {
          respuesta_tb_t res = make_respuesta_tb(999,String("No se pudo acceder al recurso this->http"));

          return res;
     }

     // no debería llegar aquí
     return make_respuesta_tb(666,"¡Entra en pánico! ¡No deberías ver esto!");
};

// Destructor: libera los recursos utilizados:
// - mutex
ClienteTBClass::~ClienteTBClass()
{
     vSemaphoreDelete( this->http_mutex );     
}

// objeto interfaz
ClienteTBClass ClienteTB;
