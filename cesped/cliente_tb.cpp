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

// path de la URI de la api de telemetria
String TB_API_TELEMETRIA = "/api/v1/_TOKEN_DISPOSITIVO_/telemetry/";
// path de la URI de la api de alarmas 
String TB_API_ALARMAS = "/api/alarm/_DEVICE_TYPE_/_DEVICE_ID_/";

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
};

// envia al dispositivo indentificado por token el dato JSON.
//  - token_dispositivo: identificador del dispositivo en el servicio
//  - json: json a enviar. p.e.: "{'nombre': 'valor'}"
//
// devuelve el par (std::pair) formado por el codigo HTTP y el contenido
// devuelto por el servidor
std::pair<int,String> ClienteTBClass::enviar_telemetria(String token_dispositivo, String json)
{
     String uri_path = String(TB_API_TELEMETRIA);
     uri_path.replace("_TOKEN_DISPOSITIVO_", token_dispositivo);
     
     // iniciar comunicación con el servidor
     this->http.begin(this->host, this->puerto, uri_path);
     
     // indicar las cabeceras necesarias de la API:
     this->http.addHeader("Content-Type","application/json");
     this->http.addHeader("X-Authorization", "Bearer " + this->auth);
     
     // enviar la petición y devolver el código
     int httpStatus = this->http.POST(json);

     return std::make_pair(httpStatus, this->http.getString());
};


// objeto interfaz
ClienteTBClass ClienteTB;


