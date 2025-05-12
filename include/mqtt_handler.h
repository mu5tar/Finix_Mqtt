#ifndef MQTT_HANDLER_H
#define MQTT_HANDLER_H

#include <PubSubClient.h>

void setupMQTT();
void handleMQTT();
void setMQTTClient(Client &client);

#endif