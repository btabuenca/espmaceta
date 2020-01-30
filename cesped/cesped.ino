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

#include <Thread.h>             // https://github.com/ivanseidel/ArduinoThread
#include <ThreadController.h>

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

/* para ejecutar el publicador */
//ThreadController threadControl = ThreadController();
//Thread thread = Thread();

WebServer servidor(http_service_port);

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
     configurarMQTT();

     // lanzar las tareas
     lanzar_tareas();
}

void loop()
{
     // tratar las peticiones del servidor HTTP
     servidor.handleClient();

     if (!cliente_mqtt.loop())
     {
          // reconectar
          if (cliente_mqtt.connect("MACETA-A", device_token, "", tb_topic, 0, 0, "{'desconexion': 'true'}")) {
               Serial.println("re-connected");
		
               mqtt.subscribe(tb_topic,  topic1_subscriber);
          } else {
               Serial.println(s+"failed, rc="+cliente_mqtt.state());
          }
          
     }
     // threadControl.run();
};

/* Función que envía el dato formateado en json al servicio de Thingsboard
 * configurado en ClienteTB 
    - json: cadena que contiene el dato como objeto json

   Devuelve el cuerpo de la respuesta recibida en la petición POST realizada al
   enviar el dato.
*/
const char *enviar_medida(const char *json) {
     Serial.print("enviar_medida(");
     Serial.print(json);
     Serial.println(")");
     String json_envio = String(json);
     Serial.println(json_envio);
     if (cliente_mqtt.publish(tb_topic, json)) {
          Serial.println(s + "Publicado con éxito (CLI: " + cliente_mqtt.state());
     } else {
          Serial.println(s + "No se consiguió publicar (CLI: " + cliente_mqtt.state());
     }

     return json;
}

void topic1_subscriber(String topic, String message) {
     // TODO: hacer algo útil con los mensajes recibidos.
     Serial.println(s+"Message arrived in function 1 ["+topic+"] "+message);
}


bool configurarMQTT() {
  Serial.print(s+"Connecting to MQTT: "+tb_host+" ... ");
  /* There are 3 QoS levels in MQTT: */
  /*   At most once (0) */
  /*   At least once (1) */
  /*   Exactly once (2). */
  if (cliente_mqtt.connect("MACETA-A", device_token, "", tb_topic, 0, 0, "{'desconexion': 'true'}")) {
    Serial.println("connected");
		
    mqtt.subscribe(tb_topic,  topic1_subscriber);
  } else {
    Serial.println(s+"failed, rc="+cliente_mqtt.state());
  }

  /* // Enable Thread */
  /* thread.onRun(publisher); */
  /* thread.setInterval(2000); */
  /* threadControl.add(&thread);      */
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
