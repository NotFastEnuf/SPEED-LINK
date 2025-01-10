#include <globals.h>
extern WebSocketsServer webSocket;
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);
void updateClientData (void);
