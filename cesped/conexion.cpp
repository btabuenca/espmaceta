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
void ConexionClass::begin(char *ssid, char *password)
{
     this->ssid = ssid;
     this->password = password;

     WiFi.disconnect(true); // desconectar si previamente estaba conectado.
     
     WiFi.begin(this->ssid, this->password);

     while (WiFi.status() != WL_CONNECTED) {
          delay(500);
          Serial.print(".");
     }
     Serial.println();
     Serial.println("¡Conectado!");
};

// objeto interfaz
ConexionClass Conexion;
