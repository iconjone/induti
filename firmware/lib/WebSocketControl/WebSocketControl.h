#include <ESPAsyncWebServer.h>

#ifndef WEBSOCKETCONTROL_H
#define WEBSOCKETCONTROL_H
void onWsEvent(AsyncWebSocket, AsyncWebSocketClient, AwsEventType , void*, uint8_t , size_t );
#endif