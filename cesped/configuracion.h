/* -*- mode: arduino; coding: utf-8-unix -*- */
//
////////////////////////////////////////////////////////////////////////////
// Fichero:    configuracion.h
// -------------------------------------------------------------------------
// Proyecto:     C.E.S.P.E.D.
// Autor:      José L. Domenech
// Descripción:
//
//   Fichero para mantener la configuración de algunas opciones del proyecto
// -------------------------------------------------------------------------
// Historia: + 23/01/2020 - Primera versión
////////////////////////////////////////////////////////////////////////////
#ifndef CONFIGURACION_HPP
#define CONFIGURACION_HPP

/* puerto del servidor HTTP */
int http_service_port = 80;

// velocidad a la que se comunicará con un puerto serie
const int SERIAL_BAUDIOS = 9600;

// datos de conexión WiFi ()
char *ssid = ""; // SSID de la WiFi
char *pswd = ""; // PASSWORD de la WiFi

#endif
