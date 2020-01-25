/* -*- mode: arduino; coding: utf-8 -*- */
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

// velocidad a la que se comunicará con un puerto serie
const int SERIAL_BAUDIOS = 9600;

// datos de conexión WiFi ()
char *ssid = ""; // SSID de la WiFi
char *pswd = ""; // PASSWORD de la WiFi

// datos de conexión a ThingsBoard
char *tb_host = ""; // host en el que se aloja el servicio Thingsboard
const int tb_http_port = 80; // puerto en el que se aloja el servicio http de Thingsboard

// datos del dispositivo ThingsBoard que recoge las mediciones
char *device_token = ""; // token del dispositivo 
char *autorizacion = ""; // autorizacion (para lecturas)
