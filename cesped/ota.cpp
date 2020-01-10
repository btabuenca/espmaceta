/* -*- mode: c++; coding: utf-8 -*- */
//
////////////////////////////////////////////////////////////////////////////
// Fichero:    ota.cpp
// -------------------------------------------------------------------------
// Proyecto:     C.E.S.P.E.D.
// Autor:      José L. Domenech
// Descripción:
//  La función `configurarServidor' configura las rutas a las que responderá el servidor:
//
//   - GET:/ - Página para autorizar el acceso.
//   - GET:/serverIndex - Página
//   - POST:/update - URI al que enviar (mediante POST (param:Update)) el
//                    firmware que se cargará.
//
//
// Referencia a WebServer ESP32: https://github.com/espressif/arduino-esp32/blob/master/libraries/WebServer/src/WebServer.h
// -------------------------------------------------------------------------
// Historia: + 07/01/2020 - Primera versión
////////////////////////////////////////////////////////////////////////////

#include "ota.h"

#include <ESPmDNS.h>
#include <Update.h>

/*
 * Login page
 */

// TODO: utilizar un método mejor de autorización.
const char* loginIndex = 
 "<form name='loginForm'>"
    "<table width='20%' bgcolor='A09F9F' align='center'>"
        "<tr>"
            "<td colspan=2>"
                "<center><font size=4><b>MACETA-A Página de Inicio</b></font></center>"
                "<br>"
            "</td>"
            "<br>"
            "<br>"
        "</tr>"
        "<td>Usuario:</td>"
        "<td><input type='text' size=25 name='userid'><br></td>"
        "</tr>"
        "<br>"
        "<br>"
        "<tr>"
            "<td>Password:</td>"
            "<td><input type='Password' size=25 name='pwd'><br></td>"
            "<br>"
            "<br>"
        "</tr>"
        "<tr>"
            "<!-- La autorización se hace en el cliente :( -->"
            "<td><input type='submit' onclick='check(this.form)' value='Login'></td>"
        "</tr>"
    "</table>"
"</form>"
"<script>"
    "function check(form)"
    "{"
      "if(form.userid.value=='admin' && form.pwd.value=='admin')"
      "{"
        "window.open('/serverIndex')"
      "}"
      "else"
      "{"
        " alert('Error Password o Usuario')"
      "}"
    "}"
"</script>";
 
/*
 * Server Index Page
 */
// TODO: No utilizar recursos externos. 
const char* serverIndex = 
"<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
"<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
   "<input type='file' name='update'>"
        "<input type='submit' value='Update'>"
    "</form>"
 "<div id='prg'>progress: 0%</div>"
 "<script>"
  "$('form').submit(function(e){"
     "e.preventDefault();"
     "var form = $('#upload_form')[0];"
     "var data = new FormData(form);"
     " $.ajax({"
        "url: '/update',"
        "type: 'POST',"
        "data: data,"
        "contentType: false,"
        "processData:false,"
        "xhr: function() {"
           "var xhr = new window.XMLHttpRequest();"
           "xhr.upload.addEventListener('progress', function(evt) {"
                  "if (evt.lengthComputable) {"
                     "var per = evt.loaded / evt.total;"
                     "$('#prg').html('progress: ' + Math.round(per*100) + '%');"
                  "}"
           "}, false);"
           "return xhr;"
        "},"
        "success:function(d, s) {"
           "console.log('success!')" 
        "},"
        "error: function (a, b, c) {"
        "}"
      "});"
   "});"
 "</script>";

// Utilidad para configurar el servidor:
//  * server: servidor WebServer iniciado.
//  * host: nombre del servidor que será anunciado mediante mDNS
//
// Tras la configuración el servidor se debe iniciar (server.begin();) y empezar
// a servir las conexiones entrantes (server.handleClient();)
//
//   - GET:/ - Página para autorizar el acceso.
//   - GET:/serverIndex - Página
//   - POST:/update - URI al que enviar (mediante POST (param:Update)) el
//                    firmware que se cargará.
//
void configurarServidor(WebServer &server, String host) {

     /* usar mdns para publicar el nombre de nuestro host */
  if (!MDNS.begin(host.c_str())) { //http://<host>.local
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }

  Serial.println("mDNS responder started");

// Configuración del servidor:
// URIS del servidor:
//   - GET:/
//   - GET:/serverIndex
//   - GET:/update

/* return index page which is stored in serverIndex */
  server.on("/", HTTP_GET, [&server]() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", loginIndex);
  });
  
  server.on("/serverIndex", HTTP_GET, [&server]() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", serverIndex);
  });
  
  /*handling uploading firmware file */
  server.on("/update", HTTP_POST, [&server]() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart();
  }, [&server]() {
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
      Serial.printf("Update: %s\n", upload.filename.c_str());
      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      /* flashing firmware to ESP*/
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) { //true to set the size to the current progress
        Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
      } else {
        Update.printError(Serial);
      }
    }
  });

//  server.begin();
//
//  void loop(void) {
//   server.handleClient();
//   delay(1);
// }
}
