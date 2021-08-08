#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "accsessPoin.h"
#include "EEPROM_Access.h"
#include <SocketIoClient.h>
#include <httpClient.h>
#include <ArduinoJson.h>
SocketIoClient Socket;
StaticJsonDocument<200> doc;
String token="-";
String status = "0";
void event(const char * payload, size_t length) {
  Serial.print("ini response socket : ");
  Serial.println(payload);
  String email = read_String(51);
  String Data =  "{\"state\":\""+status+"\",\"email\":\""+email+"\"}";
  Socket.emit("res-broad-giveMeState",Data.c_str());

}

void event2(const char * payload, size_t length) {
  Serial.print("ini response changeStatus : ");
  Serial.println(payload);
  status = String(payload);
  Serial.println(status);
  String email = read_String(51);
  String Data =  "{\"state\":\""+status+"\",\"email\":\""+email+"\"}";
  Socket.emit("res-broad-giveMeState",Data.c_str());
}


void setup() {
 // put your setup code here, to run once:
  Serial.begin(9600);
  //  String data = "-";

  // Serial.print("Writing Data:");
  // Serial.println(data);
  // writeString(0, data);
  // writeString(26,data);
  // writeString(51, data);
  // writeString(76,data);
  delay(10);
  String data1 = read_String(0);
  String data2 = read_String(26);
  String data3 = read_String(51);
  String data4 = read_String(76);
  if(data1 == "-" and data2 == "-" and data3 == "-" and data4 == "-"){
    setting();
  }else{
    Serial.println(data1);
    Serial.println(data2);
    Serial.println(data3);
    Serial.println(data4);
    const char* ssid = data1.c_str();
    const char* password = data2.c_str();
    WiFi.begin(ssid, password);
    Serial.print("Connecting");
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print(".");
    }
    Serial.println();

    Serial.print("Connected, IP address: ");
    Serial.println(WiFi.localIP());
    String payload = httpPostToENDpoint("login",
    "{\"email\":\""+data3+"\",\"password\":\""+data4+"\"}","x","post");
    Serial.println(payload);
    deserializeJson(doc, payload);
    String tkn = doc["token"];
    token = tkn;
    if(token != "-"){
        Socket.begin("192.168.10.160",3007);
        String status = "braoad-giveMeState-" + data3;
        String change = "toChangeMicon-"+data3;
        Socket.on(status.c_str(), event);
        Socket.on(change.c_str(),event2);
    }
    
  }
}
 
void loop(){
   Socket.loop();
   if(status=="0"){
     ///lock, gabisa ngapa ngapain
   }else if(status == "1"){
     ///jika di tap maka add kunci
   }else if(status == "2"){
     ///jika di tap maka open kunci
   }
}