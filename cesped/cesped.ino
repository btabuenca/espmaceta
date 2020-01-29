/* -*- mode: arduino; coding: utf-8-unix -*- */
//
////////////////////////////////////////////////////////////////////////////
// Fichero:    cesped.ino
// -------------------------------------------------------------------------
// Proyecto:     C.E.S.P.E.D.
// Autor:      José L. Domenech
// Descripción:
//
//   Sistema de lectura de datos desde sensores y subida a un servicio Thingsboard.
//
// -------------------------------------------------------------------------
//   Historia: + 08/01/2020 - Primera versión
//             + 20/01/2020 - (Juan Luis+Victor) Lectura desde sensores
//             + 25/01/2020 - Nueva versión
//             + 27/01/2020 - (Juan Luis+Victor) Añadidos todos los sensores
////////////////////////////////////////////////////////////////////////////

#include "conexion.h"
#include "ota.h"
#include "tareas.h"
#include "dispositivos.h"
#include "cliente_tb.h"
#include "configuracion.h"
#include "config_tb.h"

#define PRI_HUMEDAD_1 2
#define PRI_HUMEDAD_2 2
#define PRI_PESO      1
#define PRI_GOTAS     4
#define PRI_RELE      3

// periodos de las tareas en ms
#define PERIODO_HUMEDAD_1 4000
#define PERIODO_HUMEDAD_2 3000
#define PERIODO_PESO      6000
#define PERIODO_GOTAS     1000
#define PERIODO_RELE      2000

WebServer servidor(http_service_port);

void setup()
{
     // iniciar puerto de salida serie
     Serial.begin(SERIAL_BAUDIOS);
     delay(1000);
        
     Serial.println("Iniciando");

     // conectar a la red WiFi
     Conexion.begin(ssid, pswd);

     // configurar e iniciar el servidor HTTP conteniendo el servicio OTA
     configurarServidor(servidor, tb_host);
     servidor.begin();

     // configurar pines de los dispositivos
     configurar_dispositivos();
     
     // configurar el endpoint del servicio Thingsboard
     ClienteTB.begin(tb_host, tb_http_port, autorizacion);

     // lanzar las tareas
     lanzar_tareas();
}

void loop()
{
     // tratar las peticiones del servidor HTTP
     servidor.handleClient();
     // mostrar información de depuración
     Serial.print("    MD5 SKETCH: "); Serial.println(ESP.getSketchMD5());
     Serial.print("            IP: "); Serial.println(Conexion.getIP().toString());
     Serial.print("FREE HEAP MEM.: "); Serial.println(ESP.getFreeHeap());
     Serial.println("===========================");
     delay(5000);
};

/* Función que envía el dato formateado en json al servicio de Thingsboard
 * configurado en ClienteTB 
    - json: cadena que contiene el dato como objeto json

   Devuelve el cuerpo de la respuesta recibida en la petición POST realizada al
   enviar el dato.
*/
const char *enviar_medida(const char *json) {
     respuesta_tb_t res = ClienteTB.enviar_telemetria(device_token, json);

     return (char *)res.second.c_str();
}


// Función que lanza las tareas que se encargan de la lectura, obtención de la
// estructura a enviar y el envío
void lanzar_tareas() {
     ptr_Tarea sensorHumedad1, sensorHumedad2, sensorPeso,sensorGotas, estadoRele;
 
     sensorHumedad1 = new Tarea("sensorHumedad1",
                                PERIODO_HUMEDAD_1,
                                PRI_HUMEDAD_1,
                                leerSensorHumedad1,
                                json_humedad1,
                                enviar_medida);

     sensorHumedad2 = new Tarea("sensorHumedad2",
                                PERIODO_HUMEDAD_2,
                                PRI_HUMEDAD_2,
                                leerSensorHumedad2,
                                json_humedad2,
                                enviar_medida);

     sensorPeso = new Tarea("sensorPeso",
                            PERIODO_PESO,
                            PRI_PESO,
                            leerSensorPeso,
                            json_peso,
                            enviar_medida);
     
     sensorGotas = new Tarea("sensorGotas",
                             PERIODO_GOTAS,
                             PRI_GOTAS,
                             leerSensorGotas,
                             json_gotas,
                             enviar_medida);

     estadoRele = new Tarea("estadoRele",
                            PERIODO_RELE,
                            PRI_RELE,
                            leerEstadoRele,
                            json_rele,
                            enviar_medida);
}
