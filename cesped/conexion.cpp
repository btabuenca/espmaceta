/* -*- mode: c++; coding: utf-8 -*- */
//
////////////////////////////////////////////////////////////////////////////
// Fichero:    conexion.cpp
// -------------------------------------------------------------------------
// Proyecto:     C.E.S.P.E.D.
// Autor:      José L. Domenech
// Descripción:
//   Mantiene los datos de la conexión WiFi.
// -------------------------------------------------------------------------
//   Historia: + 18/12/2019 - Primera versión
////////////////////////////////////////////////////////////////////////////
#include "conexion.h"

// Conecta a una red WiFi con el SSID y contraseña indicada.
void ConexionClass::begin(String ssid, String password)
{
     this->ssid = ssid;
     this->password = password;

     WiFi.disconnect(true); // desconectar si previamente estaba conectado.

     WiFi.mode(AF_SPA); // modo estación (STA=Station), desactiva modo AP
     // WiFi.begin requiere char* acabados en null:
     WiFi.begin(this->ssid.c_str(), this->password.c_str());

     while (WiFi.status() != WL_CONNECTED) {
          delay(500);
          Serial.print(".");
     }
     Serial.println();
     Serial.print("¡Conectado! IP: ");
     Serial.println(WiFi.localIP());
};

// objeto interfaz
ConexionClass Conexion;
