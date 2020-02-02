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
//             + 30/01/2020 - Adaptación a MQTT
////////////////////////////////////////////////////////////////////////////

#include <WiFi.h>
#include <PubSubClient.h>
#include <PubSubClientTools.h>

#include "conexion.h"
#include "ota.h"
#include "ColaMensajes.h"
#include "tareas.h"
#include "dispositivos.h"
#include "configuracion.h"
#include "config_tb.h"

#include <memory>

#define PRI_HUMEDAD_1 1
#define PRI_HUMEDAD_2 1
#define PRI_PESO      1
#define PRI_GOTAS     1
#define PRI_RELE      1

#define PRI_PUBLI     3

// periodos de las tareas en ms
#define PERIODO_HUMEDAD_1 5000
#define PERIODO_HUMEDAD_2 5000
#define PERIODO_PESO      5000
#define PERIODO_GOTAS     5000
#define PERIODO_RELE      5000

#define PERIODO_PUBLI     10000

WiFiClient espClient;
PubSubClient cliente_mqtt(tb_host, tb_mqtt_port, espClient);
PubSubClientTools mqtt(cliente_mqtt);

// servidor HTTP
WebServer servidor(http_service_port);

// tareas que se lanzarán
ptr_Tarea sensorHumedad1, sensorHumedad2, sensorPeso, sensorGotas, estadoRele, tarea_publicar;

// String de utilidad:
const String s = "";

void setup()
{
     // iniciar puerto de salida serie
     Serial.begin(SERIAL_BAUDIOS);
     delay(1000);
        
     Serial.println("INICIANDO:");     
     
     // conectar a la red WiFi
     Conexion.begin(ssid, pswd);

     // configurar e iniciar el servidor HTTP conteniendo el servicio OTA
     configurarServidor(servidor, tb_host);
     servidor.begin();

     // configurar pines de los dispositivos
     configurar_dispositivos();
     
     // configurar el endpoint del servicio Thingsboard
     configurar_MQTT();
     
     // lanzar las tareas
     lanzar_tareas();
}; // setup

void loop()
{
     // tratar las peticiones del servidor HTTP
     servidor.handleClient();

     cliente_mqtt.loop();
}; // loop

void reconectar_mqtt() {
     // Serial.println("reconectar_mqtt");
     if (!cliente_mqtt.loop())
     {
          Serial.print("No conectado ... ");
          // reconectar
          if (cliente_mqtt.connect("MACETA-A", tb_device_token, "", tb_topic, 0, 0, "{'desconexion': 'true'}")) {
               Serial.println("re-conectado");
		
               mqtt.subscribe(tb_topic,  topic1_subscriber);
          } else {
               Serial.println(s+"fallo, rc="+cliente_mqtt.state());
          }
          
     }
}; // reconectar_mqtt

/* Función que envía el dato formateado en json al servicio de Thingsboard
 * configurado en ClienteTB 
 - json: cadena que contiene el dato como objeto json

 Devuelve el cuerpo de la respuesta recibida en la petición POST realizada al
 enviar el dato.
*/
const char *enviar_medida(const char *json) {
     Serial.println("enviar_medida");

     // los elementos de la cola se copian
     ColaMensajes.encolar(String(json));

     Serial.println(s+"Encolado "+json);
     return json;
}; // enviar_medida

void topic1_subscriber(String topic, String message) {
     // TODO: hacer algo útil con los mensajes recibidos.
     Serial.println(s+"Message arrived in function 1 ["+topic+"] "+message);
}; // topic1_subscriber


bool configurar_MQTT() {
     Serial.print(s+"Connecting to MQTT: "+tb_host+" ... ");
     /* There are 3 QoS levels in MQTT: */
     /*   At most once (0) */
     /*   At least once (1) */
     /*   Exactly once (2). */
     if (cliente_mqtt.connect("MACETA-A", tb_device_token, "", tb_topic, 0, 0, "{'desconexion': 'true'}")) {
          Serial.println("conectado");
		
          mqtt.subscribe(tb_topic,  topic1_subscriber);
     } else {
          Serial.println(s+"fallo, rc="+cliente_mqtt.state());
     }

}; // configurar_MQTT

// función que se ejecuta periódicamente, lee la cola de mensajes a publicar y
// los envía.
const char *publicador(const char *val) {
     Serial.println("Publicando");
     mensaje_t *ptr_msg;
     String json;

     reconectar_mqtt();
     
     // para todos los elementos encolados:
     Serial.print("Obtener mensajes en cola ... ");
     int mensajes_en_cola = ColaMensajes.mensajes();
     Serial.println(mensajes_en_cola);
     if (mensajes_en_cola > 0) {
          for(int i = 0; i < mensajes_en_cola; i++) {
               Serial.print("Desencolar ... ");
               json = ColaMensajes.desencolar()->mensaje;
          
               Serial.println(s+"desencolado: "+json);
     
               if (cliente_mqtt.publish(tb_topic, json.c_str())) {
                    Serial.println(s + "Publicado con éxito (CLI: " + cliente_mqtt.state());
               } else {
                    Serial.println(s + "No se consiguió publicar (CLI: " + cliente_mqtt.state());
               }          
          } // for
     } 
     
     return val;
}; // publicador

// Función que lanza las tareas que se encargan de la lectura, obtención de la
// estructura a enviar y el envío
void lanzar_tareas() {
     Serial.println("Lanzando tareas");
     
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
     
     // Ejecutar el publicador
     tarea_publicar = new Tarea("publicador",
                                PERIODO_PUBLI,
                                PRI_PUBLI,
                                []() -> int { return 0; },
                                [](int in) -> const char* { return "-"; },
                                publicador);
}; // lanzar_tareas
