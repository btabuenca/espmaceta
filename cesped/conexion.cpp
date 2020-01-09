/* -*- mode: c++; coding: utf-8 -*- */
//
////////////////////////////////////////////////////////////////////////////
// Fichero:    conexion.cpp
// -------------------------------------------------------------------------
// Proyecto:     C.E.S.P.E.D.
// Autor:      José L. Domenech
// Descripción:
//   Mantiene los datos de la conexión WiFi.
//
// -------------------------------------------------------------------------
//   Historia: + 18/12/2019 - Primera versión
////////////////////////////////////////////////////////////////////////////
#include "conexion.h"

void WiFiEstacionDesconectada()
{
     // si se ha detectado una desconexión...
     do {
          // ... esperar X min. ...
          delay(TIEMPO_ESPERA_RECONEXION_MS);
          // ... e intentar la conexión ...
          Conexion.conectar();
     } while (WiFi.status() == WL_CONNECTED);
     // ... hasta conseguirla.
}

int ConexionClass::conectar()
{
     WiFi.disconnect(true); // desconectar si previamente estaba conectado.

     WiFi.mode(WIFI_MODE_STA); // modo estación (STA=Station), desactiva modo AP
     // WiFi.begin requiere char* acabados en null:
     WiFi.begin(this->ssid.c_str(), this->password.c_str());

     while (WiFi.status() != WL_CONNECTED) {
          delay(500);
          Serial.print(".");
     }
     
     if Serial.available()
     {
          Serial.println();
          Serial.print("¡Conectado! IP: ");
          Serial.println(WiFi.localIP());
     }
     
     return WiFi.status();
}

// Conecta a una red WiFi con el SSID y contraseña indicada.
void ConexionClass::begin(String ssid, String password)
{
     this->ssid = ssid;
     this->password = password;

     this->conectar();
     // registrar una función que actue en caso de que ocurra una desconexión
     // IDEA tomada de: https://github.com/espressif/esp-idf/blob/master/docs/en/api-guides/wifi.rst#wifi-event-sta-disconnected
     WiFi.on(WiFiEstacionDesconectada, WIFI_EVENT_STA_DISCONNECTED);
}

// objeto interfaz
ConexionClass Conexion;
