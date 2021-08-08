#include <SPI.h> 
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "accsessPoin.h"
#include "EEPROM_Access.h"
#include <SocketIoClient.h>
#include <httpClient.h>
#include <ArduinoJson.h>
#include <MFRC522.h>

#define RELAY 3
#define SS_PIN 4
#define RST_PIN 5
MFRC522 mfrc522(SS_PIN, RST_PIN); 

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
  // String data = "-";

  // Serial.print("Writing Data:");
  // Serial.println(data);
  // writeString(0, data);
  // writeString(26,data);
  // writeString(51, data);
  // writeString(76,data);
  // delay(10);
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
        
        SPI.begin();      
        mfrc522.PCD_Init();
        pinMode(RELAY, OUTPUT);   
        digitalWrite(RELAY, HIGH);
        Serial.println("Put your card to the reader...");
        Serial.println();
      } 
  }
}
 
void loop(){
   Socket.loop();
   if(status=="0"){
     ///lock, gabisgia ngapa ngapain
   }else if(status == "1"){
     ///jika di tap maka add kunci
   }
   else if(status == "2"){
     ///jika di tap maka open kunci
        if ( ! mfrc522.PICC_IsNewCardPresent()) 
        {
          return;
        }
        if ( ! mfrc522.PICC_ReadCardSerial()) 
        {
          return;
        }
        Serial.print("UID tag :");
        String content= "";
        byte letter;
        for (byte i = 0; i < mfrc522.uid.size; i++) 
        {
          Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
          Serial.print(mfrc522.uid.uidByte[i], HEX);
          content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
          content.concat(String(mfrc522.uid.uidByte[i], HEX));
        }
        Serial.println();
        Serial.print("Message : ");
        content.toUpperCase();
        if (content.substring(1) == "XX XX XX XX")
        {
          Serial.println("Authorized access");
          Serial.println();
          delay(500);
          digitalWrite(RELAY, LOW);
          delay(2000);
          digitalWrite(RELAY, HIGH);
        }
      
      else   {
          Serial.println(" Access denied");
          delay(500);
          digitalWrite(RELAY, HIGH);
          delay(2000);
          digitalWrite(RELAY, LOW);
        }
      }
   }