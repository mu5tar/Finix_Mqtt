#include <Arduino.h>
#include <PubSubClient.h>
#include "config.h"
#include "status_led.h"
#include "net_handler.h"
#include "motor_control.h"

PubSubClient mqttClient;
unsigned long lastConnectionAttempt = 0;
bool lastConnectionStatus = false;



void mqttCallback(char* topic, byte* payload, unsigned int length) {
  payload[length] = '\0';
  String msg = String((char*)payload);
  msg.trim();
  msg.toLowerCase();

  Serial.println("Received message: " + msg);

  if (String(topic) == config.mqtt_sub_topic) {
    if (msg == "off") {
parseMessage( msg);  
//delay(500);
      mqttClient.publish(config.mqtt_pub_topic, "Motor off done");
    } else if (msg.startsWith("on")) {
  parseMessage( msg);
  
      mqttClient.publish(config.mqtt_pub_topic, "Motor on done");
    }
  }
}

void setMQTTClient(Client &client) {
  mqttClient.setClient(client);
}

bool reconnectMQTT() {
  if (mqttClient.connected()) return true;

  unsigned long now = millis();
  if (now - lastConnectionAttempt < config.connection_interval) return false;

  lastConnectionAttempt = now;

  Serial.print("[MQTT] Attempting connection to ");
  Serial.println(config.mqtt_server);

  if (mqttClient.connect(config.mqtt_client_id, config.mqtt_user, config.mqtt_pass)) {
    Serial.println("[MQTT] Connected!");
    mqttClient.subscribe(config.mqtt_sub_topic);
    indicateStatus(true);
    lastConnectionStatus = true;
    return true;
  } else {
    Serial.print("[MQTT] Failed, rc=");
    Serial.println(mqttClient.state());
    indicateStatus(false);
    lastConnectionStatus = false;
    return false;
  }
}

void setupMQTT() {
  mqttClient.setServer(config.mqtt_server, config.mqtt_port);
  mqttClient.setCallback(mqttCallback);
  mqttClient.setKeepAlive(60);
  mqttClient.setBufferSize(256);
}

void handleMQTT() {
  reconnectMQTT();  // will only reconnect if needed and enough time passed
  mqttClient.loop();
}
