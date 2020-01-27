/* -*- mode: arduino; coding: utf-8 -*- */
//
////////////////////////////////////////////////////////////////////////////
// Fichero:    cesped.ino
// -------------------------------------------------------------------------
// Proyecto:     C.E.S.P.E.D.
// Autor:      José L. Domenech
// Descripción:
//
//   Prueba de stress para conectar a una red WiFi + leer de un sensor + enviar los datos a Thingsboard desde tres procesos.
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

int leer_temperatura_mock1() {
     return 10;
}

int leer_temperatura_mock2() {
     return 25;
}

int leer_temperatura_mock3() {
     return 30;
}

const char *enviar_medida(const char *json) {
     std::pair<int,String> res = ClienteTB.enviar_telemetria(device_token, json);

//     Serial.println(json);
     return (char *)res.second.c_str();
}

ptr_Tarea sensor_temp_mock1, sensor_temp_mock2, sensor_temp_mock3;

void setup()
{
        Serial.begin(SERIAL_BAUDIOS);
        delay(1000);

        Serial.println("Iniciando");
        
        Conexion.begin(ssid, pswd);

        ClienteTB.begin(tb_host, tb_http_port, autorizacion);

        sensor_temp_mock1 = new Tarea("sensor_temp_mock1",
                                      1000,
                                      1, // la misma prioridad a todas
                                      leer_temperatura_mock1,
                                      json_temperature,
                                      enviar_medida);

        sensor_temp_mock2 = new Tarea("sensor_temp_mock2",
                                      1000,
                                      1, // la misma prioridad a todas
                                      leer_temperatura_mock2,
                                      json_temperature,
                                      enviar_medida);

        sensor_temp_mock3 = new Tarea("sensor_temp_mock3",
                                      1000,
                                      1, // la misma prioridad a todas
                                      leer_temperatura_mock3,
                                      json_temperature,
                                      enviar_medida);

}

void loop()
{
     Serial.print("IP: "); Serial.println(Conexion.getIP().toString());
     delay(10000);
}
