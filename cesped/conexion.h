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
//     - conectar: realiza la conexión con el ssid y el passwd almacenado,
//                 y registra funciones para controlar eventos de la conexión
//                 
//    Utilizar el objeto exportado Conexion con su método begin(ssid,passw)
//    para realizar la conexión
//
// Motivación:
//   ConexionClass es una abstracción "fina" sobre WiFiClass, pero
//   permite almacenar los datos de conexión.
//   -------------------------------------------------------------------------
//   Historia: + 18/12/2019 - Primera versión
//             + 31/12/2019 - Añadir control de eventos de la conexión
//             + 20/01/2020 - Añadir posibilidad de conf. IP estática
////////////////////////////////////////////////////////////////////////////
#ifndef CONEXION_HPP
#define CONEXION_HPP

#include <Arduino.h>

#include <WiFi.h>

// esperar 3 min. antes de reconectar
const int TIEMPO_ESPERA_RECONEXION_MS = 3 * 60 * 1000;

// Clase auxiliar para almacenar la configuración de la IP fija
class IPFija
{
public:
     IPFija(IPAddress local_IP, IPAddress gateway, IPAddress subnet, IPAddress primaryDNS, IPAddress secondaryDNS);
private:
     IPAddress local_IP;
     IPAddress gateway;
     IPAddress subnet;
     IPAddress primaryDNS;
     IPAddress secondaryDNS;
     
     friend class ConexionClass; 
}; // IPFija

class ConexionClass
{
public:
     // Cambia el ssid y el passwd he intenta una conexión nueva.
     //  - ssid: SSID de la red WiFi a conectar
     //  - password: contraseña de la red WiFi protegida
     void begin(String ssid, String password);


     // Cambia el ssid y el passwd he intenta una conexión nueva.
     // configura la interfaz con un ip estática
     //  - ssid: SSID de la red WiFi a conectar
     //  - password: contraseña de la red WiFi protegida
     //  - ip_fija: configuración de ip estática
     void begin(String ssid, String password, IPFija *ip_fija);


     // Realiza la conexión con el ssid y el passwd almacenado (y la
     // configuración de ip estática si fue proporcionada anteriormente), y
     // registra funciones para controlar eventos de la conexión.
     //   - devuevle el estado de la WiFi (debería ser ¡SIEMPRE! WL_CONNECTED)
     int conectar();

     
     // Devuelve el SSID almacenado.
     String getSSID();

private:
     // propiedades
     String ssid, password;
     IPFija *conf_ip = nullptr;
}; // ConexionClass

// objeto interfaz
extern ConexionClass Conexion;

#endif
