/* -*- mode: arduino; coding: utf-8 -*- */
//
////////////////////////////////////////////////////////////////////////////
// Fichero:    cesped.ino
// -------------------------------------------------------------------------
// Proyecto:     C.E.S.P.E.D.
// Autor:      José L. Domenech
// Descripción:
//
//   Ejemplo conectar a una red WiFi + leer de un sensor + enviar los datos a
// Thingsboard desde una Tarea de FreeRTOS
// -------------------------------------------------------------------------
// Historia: + 22/01/2020 - Primera versión
////////////////////////////////////////////////////////////////////////////

#include "conexion.h"
#include "tareas.h"
#include "cliente_tb.h"

#include "configuracion.h"

// definición de la lectura del sensor de temperatura interno para Arduino.
#ifdef __cplusplus
extern "C" {
#endif
 
        uint8_t temprature_sens_read();
 
#ifdef __cplusplus
}
#endif
 
uint8_t temprature_sens_read();

int leer_temperatura_interna() {
     return (uint8_t)temprature_sens_read();
}

const char *json_medida(String medida, int valor) {
     String resString {""};
     
     resString = String("{\"" + medida + "\": ") + valor + "}";
     
     return resString.c_str();
}

const char *json_temperature(int valor) {
  return json_medida("temperature", valor);
}

const char *enviar_medida(const char *json) {
     ClienteTB.begin(tb_host, tb_port, autorizacion);

     std::pair<int,String> *res = ClienteTB.enviar_telemetria(device, json);

     return (char *)res->second.c_str();
}

ptr_Tarea sensor_temp_interna;

void setup()
{
        Serial.begin(SERIAL_BAUDIOS);
        delay(1000);
        
        Conexion.begin(ssid, pswd);

        ClienteTB.begin(host, port, autorizacion);

        sensor_temp_interna = new Tarea("sensor_temp_interna",
                                         10000,
                                         1,
                                         leer_temperatura_interna,
                                         json_temperature,
                                         enviar_medida);

}

void loop()
{
}
