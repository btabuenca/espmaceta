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

#include <cstring>

#include "conexion.h"
#include "ota.h"
#include "tareas.h"
#include "dispositivos.h"
#include "configuracion.h"
#include "config_tb.h"

#define PRI_HUMEDAD_1 3
#define PRI_HUMEDAD_2 2
#define PRI_PESO      1
#define PRI_GOTAS     5
#define PRI_RELE      4

// periodos de las tareas en ms
#define PERIODO_HUMEDAD_1 14000
#define PERIODO_HUMEDAD_2 23000
#define PERIODO_PESO      36000
#define PERIODO_GOTAS     41000
#define PERIODO_RELE      52000

WiFiClient espClient;
PubSubClient cliente_mqtt(tb_host, tb_mqtt_port, espClient);
PubSubClientTools mqtt(cliente_mqtt);

WebServer servidor(http_service_port);

// String de utilidad:

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
     
     // lanzar las tareas
     lanzar_tareas();
}

void loop()
{
     // tratar las peticiones del servidor HTTP
     servidor.handleClient();
}

void topic1_subscriber(String topic, String message) {
     // TODO: hacer algo útil con los mensajes recibidos.
     Serial.print("Message arrived in function 1 [");
     Serial.print(topic);
     Serial.print("] ");
     Serial.println(message);
}

// conecta el cliente al servicio MQTT y se suscribe al tópico tb_topic
// (envía el paquete connect)
bool conectar_MQTT() {
  Serial.print("Connecting to MQTT: ");
  Serial.print(tb_host);
  Serial.print(" ... ");
  /* There are 3 QoS levels in MQTT: */
  /*   At most once (0) */
  /*   At least once (1) */
  /*   Exactly once (2). */
  bool res = cliente_mqtt.connect("MACETA-A", tb_device_token, "", tb_topic, 0, 0, "{'desconexion': 'true'}");

  if (res) {
       Serial.println("Conectado.");
       mqtt.subscribe(tb_topic,  topic1_subscriber);
  } else {
       Serial.println("No se pudo conectar.");
  }
  
  return cliente_mqtt.connected();
}

// desconecta el cliente del servicio
// (envía el paquete disconnet)
void desconectar_MQTT() {
     Serial.println("MQTT:disconnect");
     cliente_mqtt.disconnect();
}

// publica un mensaje
bool publicar_MQTT(char *json_msg)
{
     Serial.print("MQTT:Publicando: ");
     Serial.print(json_msg);
     Serial.print(" ... ");
     bool res = cliente_mqtt.publish(tb_topic, json_msg);

     Serial.print(cliente_mqtt.state());
     if (res)
       Serial.println(" enviado.");
     else
       Serial.println(" fallo.");
     
     return res;
}

bool recibir_publicacion_MQTT() {
     Serial.println("MQTT:recibiendo publicacion");
     
     bool res = cliente_mqtt.loop();

     return res;
}

/* Función que envía el dato formateado en json al servicio de Thingsboard
 * configurado en ClienteTB 
    - json: cadena que contiene el dato como objeto json

   Devuelve el cuerpo de la respuesta recibida en la petición POST realizada al
   enviar el dato.
*/
const char *enviar_medida(const char *json) {
     Serial.print("enviar_medida(");Serial.print(json);Serial.println(")");

     char *json_copia;

     std::size_t slen = strlen(json);

     json_copia = new char[slen+1];
     strncpy(json_copia, json, slen+1);

     conectar_MQTT();

     publicar_MQTT(json_copia);

     delete json_copia;

     recibir_publicacion_MQTT();
     
     desconectar_MQTT();
     
     return json;
}


/* Función que lanza las tareas que se encargan de la lectura, obtención de la
   estructura (json) a enviar y el envío */
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
