#include <accsessPoin.h>
#include <EEPROM_Access.h>

const char* ssid = "fakhrilak";      // Nama AP/Hotspot
const char* password = "fakhrilak";    // Password AP/Hotspot
const char* PARAM_INPUT_1 = "input1";
const char* PARAM_INPUT_2 = "input2";
const char* PARAM_INPUT_3 = "input3";
const char* PARAM_INPUT_4 = "input4";

const char index_html[] PROGMEM = R"rawliteral(
        <!DOCTYPE HTML><html><head>
        <title>ESP Input Form</title>
        <meta name="viewport" content="width=device-width, initial-scale=1">
        </head><body>
        <h1>WELCOME TO</h1>
        <h1>ODI IOT SERVICE</h1>
        <br>
        <br>
        <h3>SETTING YOUR'S SSID</h3>
        <form action="/get">
            SSID WIFI     : <input type="text" name="input1">
            <input type="submit" value="Submit">
        </form><br>
        <form action="/get">
            PASS WIFI     : <input type="text" name="input2">
            <input type="submit" value="Submit">
        </form><br>
        <br>
        <h3>SETTING YOUR'S AKUN</h3>
        <form action="/get">
            AKUN EMAIL : <input type="text" name="input3">
            <input type="submit" value="Submit">
        </form><br>
        <form action="/get">
            AKUN PASSW : <input type="text" name="input4">
            <input type="submit" value="Submit">
        </form>
        <style typr="text/css">h1{color:blue;text-align:center;}</style>
        </body></html>)rawliteral";
AsyncWebServer server(80);
void setting(){
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/html", index_html);
    });
     server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String ssidWifi;
    String passWifi;
    String akunEmail;
    String akunPass;
    String inputParam;
    if (request->hasParam(PARAM_INPUT_1)) {
      ssidWifi = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
      Serial.println(ssidWifi);
      writeString(0,ssidWifi);
      request->send(200, "text/html", "HTTP GET request sent to your ESP on input field (" 
                                     + inputParam + ") with value: " + ssidWifi +
                                     "<br><a href=\"/\">Return to Home Page</a>");
    }
    else if (request->hasParam(PARAM_INPUT_2)) {
      passWifi = request->getParam(PARAM_INPUT_2)->value();
      inputParam = PARAM_INPUT_2;
      Serial.println(passWifi);
      writeString(26,passWifi);
      request->send(200, "text/html", "HTTP GET request sent to your ESP on input field (" 
                                     + inputParam + ") with value: " + passWifi +
                                     "<br><a href=\"/\">Return to Home Page</a>");
    }
    else if (request->hasParam(PARAM_INPUT_3)) {
      akunEmail = request->getParam(PARAM_INPUT_3)->value();
      inputParam = PARAM_INPUT_3;
      Serial.println(akunEmail);
      writeString(51,akunEmail);
      request->send(200, "text/html", "HTTP GET request sent to your ESP on input field (" 
                                     + inputParam + ") with value: " + akunEmail +
                                     "<br><a href=\"/\">Return to Home Page</a>");
    }
    else if (request->hasParam(PARAM_INPUT_4)) {
      akunPass = request->getParam(PARAM_INPUT_4)->value();
      inputParam = PARAM_INPUT_4;
      Serial.println(akunPass);
      writeString(76,akunPass);
      request->send(200, "text/html", "HTTP GET request sent to your ESP on input field (" 
                                     + inputParam + ") with value: " + akunPass +
                                     "<br><a href=\"/\">Return to Home Page</a>");
    }
   });
  Serial.print("Setting AP (Access Point)…");
  WiFi.mode(WIFI_AP); 
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  
 
  
  server.begin();
}

