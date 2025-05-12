
   #ifndef CONFIG_H
   #define CONFIG_H
   
   #include <Arduino.h>
   #include <IPAddress.h>
   
   struct NetworkConfig {
     const char* ssid = "smart";
     const char* password = "19961996";
   
     const IPAddress wifi_ip = IPAddress(192, 168, 1, 50);
     const IPAddress eth_ip = IPAddress(192, 168, 1, 60);
     const IPAddress gateway = IPAddress(192, 168, 1, 1);
     const IPAddress subnet = IPAddress(255, 255, 255, 0);
     const IPAddress dns = IPAddress(192, 168, 1, 1);
     

     const uint8_t eth_cs_pin = 5;
     const uint8_t spi_mosi = 23;
     const uint8_t spi_miso = 19;
     const uint8_t spi_sck = 18;
   
     const char* mqtt_server = "192.168.1.7";
     const int mqtt_port = 1883;
     const char* mqtt_client_id = "finix_id_Fan";
     const char* mqtt_sub_topic = "finix/Fan/control";
     const char* mqtt_pub_topic = "finix/Fan/status";
     const char* mqtt_user = "finix";
     const char* mqtt_pass = "Finixtech!@#";
   
     const unsigned long connection_interval = 10000;
     const unsigned long wifi_timeout = 10000;
     const unsigned long eth_reconnect_interval = 300000;
   };
   
   extern NetworkConfig config;
   
   #endif
   

 
   

   
   



 
   

  