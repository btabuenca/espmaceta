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
// -------------------------------------------------------------------------
//   Historia: + 18/12/2019 - Primera versión
////////////////////////////////////////////////////////////////////////////
#ifndef CONEXION_HPP
#define CONEXION_HPP

#include "WiFi.h"

class ConexionClass
{
public:
     // Conecta a una red WiFi con el SSID y contraseña indicada.
     //  - ssid: SSID de la red WiFi a conectar
     //  - password: contraseña de la red WiFi protegida
     void begin(char *ssid, char *password);
     
private:
     // propiedades
     char *ssid, *password;
};

// objeto interfaz
extern ConexionClass Conexion;

#endif
