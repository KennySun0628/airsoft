//log.h
//Kenny Sun
#ifndef LOG_H
#define LOG_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <WebSocketsServer.h>

extern WebSocketsServer webSocket;
void sendLog(String message);

#endif