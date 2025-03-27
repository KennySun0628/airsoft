//log.cpp
//Kenny Sun

#include "log.h"

void sendLog(String message){
    Serial.println(message);
    String jsonString = "";                           // create a JSON string for sending data to the client
    StaticJsonDocument<200> doc;                      // create a JSON container
    JsonObject object = doc.to<JsonObject>();         // create a JSON Object
    object["log"] = message;
    serializeJson(doc, jsonString);                   // convert JSON object to string
    webSocket.broadcastTXT(jsonString);   
}