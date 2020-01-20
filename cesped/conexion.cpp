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
//             + 08/01/2020 - Corregir el manejo de eventos WiFi
//             + 20/01/2020 - Añadir posibilidad de conf. IP estática
////////////////////////////////////////////////////////////////////////////
#include "conexion.h"

#include <esp_event_legacy.h>

void WiFiEstacionDesconectada(WiFiEvent_t event, WiFiEventInfo_t info)
{
     // si se ha detectado una desconexión...
     do {
          // ... esperar X min. ...
          delay(TIEMPO_ESPERA_RECONEXION_MS);
          // ... e intentar la conexión ...
          Conexion.conectar();
     } while (WiFi.status() == WL_CONNECTED);
     // ... hasta conseguirla.
};

// Constructor de la estructura IPFija, que sirve para pasar los parámetros de
// configuración de una ip estática a una ConexionClass
IPFija::IPFija(IPAddress local_IP,IPAddress gateway,IPAddress subnet,IPAddress primaryDNS,IPAddress secondaryDNS)
     :local_IP{local_IP}, gateway{gateway}, subnet{subnet}, primaryDNS{primaryDNS}, secondaryDNS{secondaryDNS}
{};

// conecta a una WiFi (con el ssid y password pasados en el constructor)
int ConexionClass::conectar()
{
     WiFi.disconnect(true); // desconectar si previamente estaba conectado.

     if (this->conf_ip != nullptr)
     {
          // Configures static IP address
          if (!WiFi.config(this->conf_ip->local_IP, this->conf_ip->gateway, this->conf_ip->subnet, this->conf_ip->primaryDNS, this->conf_ip->secondaryDNS)) {
               Serial.println("STA Failed to configure");
          };
     };

     
     WiFi.mode(WIFI_MODE_STA); // modo estación (STA=Station), desactiva modo AP
     // WiFi.begin requiere char* acabados en null:
     WiFi.begin(this->ssid.c_str(), this->password.c_str());

     while (WiFi.status() != WL_CONNECTED) {
          delay(500);
          Serial.print(".");
     };
     
     if (Serial.available())
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
     WiFi.onEvent(WiFiEstacionDesconectada, SYSTEM_EVENT_STA_DISCONNECTED);
}

// Conecta a una red WiFi con el SSID y contraseña indicada.
// configuración de la interfaz para asignarle unos parámetros fijos.
void ConexionClass::begin(String ssid, String password, IPFija *ip_fija)
{
     this->ssid = ssid;
     this->password = password;
     this->conf_ip = ip_fija;
     
     this->conectar();
          
     // registrar una función que actue en caso de que ocurra una desconexión
     // IDEA tomada de: https://github.com/espressif/esp-idf/blob/master/docs/en/api-guides/wifi.rst#wifi-event-sta-disconnected
     WiFi.onEvent(WiFiEstacionDesconectada, SYSTEM_EVENT_STA_DISCONNECTED);
}

// objeto interfaz
ConexionClass Conexion;
