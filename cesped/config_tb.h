/* -*- mode: arduino; coding: utf-8-unix -*- */
//
////////////////////////////////////////////////////////////////////////////
// Fichero:    config_tb.h
// -------------------------------------------------------------------------
// Proyecto:     C.E.S.P.E.D.
// Autor:      José L. Domenech
// Descripción:
//
//   Fichero para mantener la configuración de algunas opciones del proyecto
// -------------------------------------------------------------------------
// Historia: + 23/01/2020 - Primera versión
////////////////////////////////////////////////////////////////////////////
#ifndef CONFIG_TB_HPP
#define CONFIG_TB_HPP

// datos de conexión a ThingsBoard
char *tb_host = ""; // host en el que se aloja el servicio Thingsboard
const int tb_http_port = 80; // puerto en el que se aloja el servicio http de Thingsboard
const int tb_mqtt_port = 1883; // puerto en el que se envía al servicio mqtt de Thingsboard
// datos del dispositivo ThingsBoard que recoge las mediciones
char *tb_device_token = ""; // token del dispositivo 

char *tb_topic = "v1/devices/me/telemetry";
#endif
