/* -*- mode: c++; coding: utf-8 -*- */
//
////////////////////////////////////////////////////////////////////////////
// Fichero:    conexion.h
// -------------------------------------------------------------------------
// Proyecto:     C.E.S.P.E.D.
// Autor:      José L. Domenech
// Descripción:
//   Mantiene los datos de la conexión WiFi.
//     - begin: Cambia el ssid y el passwd he intenta una conexión nueva.
//
//    Utilizar el objeto exportado Conexion con su método begin(ssid,passw)
//    para realizar la conexión
//
// Motivación:
//   ConexionClass es una abstracción "fina" sobre WiFiClass, pero
//   permite almacenar los datos de conexión.
//   -------------------------------------------------------------------------
//   Historia: + 18/12/2019 - Primera versión
////////////////////////////////////////////////////////////////////////////
#ifndef CONEXION_HPP
#define CONEXION_HPP

#include "Arduino.h"
#include "WiFi.h"

class ConexionClass
{
public:
     // Conecta a una red WiFi con el SSID y contraseña indicada.
     //  - ssid: SSID de la red WiFi a conectar
     //  - password: contraseña de la red WiFi protegida
     void begin(String ssid, String password);

     // devuelve el SSID almacenado
     String getSSID();

private:
     // propiedades
     String ssid, password;
};

// objeto interfaz
extern ConexionClass Conexion;

#endif
