/* -*- mode: c++; coding: utf-8 -*- */
//
////////////////////////////////////////////////////////////////////////////
// Fichero:    ota.h
// -------------------------------------------------------------------------
// Proyecto:     C.E.S.P.E.D.
// Autor:      José L. Domenech
// Descripción:
//
//  La función `configurarServidor' configura las rutas a las que responderá el
//  servidor y publica el nombre al que responderá el servidor mediante mDNS.
//
//   - GET:/ - Página para autorizar el acceso.
//   - GET:/serverIndex - Página
//   - POST:/update - URI al que enviar (mediante POST (param:Update)) el
//                    firmware que se cargará.
//
//
// Referencia a WebServer ESP32: https://github.com/espressif/arduino-esp32/blob/master/libraries/WebServer/src/WebServer.h
// -------------------------------------------------------------------------
// Historia: + 07/01/2020 - Primera versión
////////////////////////////////////////////////////////////////////////////
#ifndef SERVIDOR_OTA_HPP
#define SERVIDOR_OTA_HPP

#include <WebServer.h> // TODO: usar HTTPS

// Utilidad para configurar el servidor:
//  * server: servidor WebServer iniciado.
//  * host: nombre del servidor que será anunciado mediante mDNS
//
// Tras la configuración (incluyendo la publicación mDNS) el servidor se debe
// iniciar (server.begin();) y empezar a servir las conexiones entrantes
// (server.handleClient();)
//
//   - GET:/ - Página para autorizar el acceso.
//   - GET:/serverIndex - Página
//   - POST:/update - URI al que enviar (mediante POST (param:Update)) el
//                    firmware que se cargará.
//
void configurarServidor(WebServer &server, char *host);

#endif
