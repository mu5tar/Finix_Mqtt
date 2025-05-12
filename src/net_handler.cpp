#include <WiFi.h>
#include <EthernetENC.h>
#include <SPI.h>
#include "config.h"
#include "net_handler.h"
#include "mqtt_handler.h"

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

WiFiClient wifiClient;
EthernetClient ethClient;

bool usingEthernet = false;
unsigned long lastEthAttempt = 0;

bool initWiFi() {
  Serial.println("\n[WiFi] Attempting connection...");

  if (!WiFi.config(config.wifi_ip, config.gateway, config.subnet, config.dns)) {
    Serial.println("[WiFi] Static IP configuration failed.");
    return false;
  }

  WiFi.begin(config.ssid, config.password);
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < config.wifi_timeout) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n[WiFi] Connected successfully.");
    usingEthernet = false;
    setMQTTClient(wifiClient);
    return true;
  }

  Serial.println("\n[WiFi] Connection failed.");
  return false;
}

bool initEthernet() {
  Serial.println("\n[Ethernet] Attempting connection...");

  SPI.begin(config.spi_sck, config.spi_miso, config.spi_mosi);
  SPI.setFrequency(8000000); 

  Ethernet.init(config.eth_cs_pin);
  Ethernet.begin(mac, config.eth_ip, config.dns, config.gateway, config.subnet);

  delay(1000);
  if (Ethernet.linkStatus() == LinkON && Ethernet.localIP() != IPAddress(0, 0, 0, 0)) {
    Serial.print("[Ethernet] Connected, IP: ");
    Serial.println(Ethernet.localIP());
    usingEthernet = true;
    setMQTTClient(ethClient);
    return true;
  }

  Serial.println("[Ethernet] Connection failed.");
  return false;
}

void logNetworkStatus() {
  if (usingEthernet) {
    Serial.print("[Network] Using Ethernet, IP: ");
    Serial.println(Ethernet.localIP());
  } else {
    Serial.print("[Network] Using WiFi, IP: ");
    Serial.println(WiFi.localIP());
  }
}

void checkNetworkConnection() {
  if (usingEthernet) {
    if (Ethernet.hardwareStatus() == EthernetNoHardware || Ethernet.linkStatus() != LinkON) {
      Serial.println("[Network] Ethernet link lost, trying WiFi...");
      if (initWiFi()) {
        Serial.println("[Network] Switched to WiFi.");
        usingEthernet = false;
      }
      lastEthAttempt = millis();
    }
  } else {
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("[Network] WiFi connection lost, trying Ethernet...");
      if (initEthernet()) {
        Serial.println("[Network] Switched to Ethernet.");
        WiFi.disconnect(); 
        usingEthernet = true;
      }
      lastEthAttempt = millis();
    }
  }
}

void initNetwork() {
  if (!initWiFi()) {
    Serial.println("[Network] WiFi failed, trying Ethernet...");
    if (!initEthernet()) {
      Serial.println("[Network] All connections failed.");
    }
  }
}
