#include "httpClient.h"
#include <ESP8266HTTPClient.h>
String httpPostToENDpoint(String endPoint,String body,String token,String method){
    WiFiClient client;
    HTTPClient http;
    http.begin(client,"http://192.168.10.160:3007/api/v1/xnxx/"+endPoint);
    http.addHeader("Content-Type", "application/json");  
    if(validatingEndpoint(endPoint) == true){
        Serial.println(token);
        http.addHeader("Authorization",token);
    }
    int httpCode;
    if(method == "get"){
        httpCode = http.GET();
    }else if (method == "post"){
         httpCode = http.POST(body);
    }
    String payload = http.getString();
    Serial.println(httpCode);
    return payload;
}

int validatingEndpoint(String enpoint){
    String AuthEndpoint [] = {"id-tag","weight-got","try-protect"};
    for(int i=0;i<sizeof AuthEndpoint/sizeof AuthEndpoint[0];i++){
        if(AuthEndpoint[i] == enpoint){
            return true;
        }
    }
    return false;
}