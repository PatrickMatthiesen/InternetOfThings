#include <WiFi.h>
#include <PubSubClient.h>

//global 
WiFiClient espClient;
PubSubClient mqtt_client(espClient);

// WiFi settings
const char *ssid = "sensors";  
const char *password = "";  // ask sebastion or get from git history

// MQTT Broker settings
const char *mqtt_broker = "influx.itu.dk";     // EMQX broker endpoint
const char *mqtt_topic = "iot2025/test";      // MQTT topic
const char *mqtt_username = "iot2025";   // MQTT username for authentication
const char *mqtt_password = "balloonatic";   // MQTT password for authentication
const int mqtt_port = 1883;         // MQTT port (TCP), non-SSL

void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");

  // Setup MQTT broker connection
  mqtt_client.setServer(mqtt_broker, mqtt_port);
  mqtt_client.setKeepAlive(60);
  mqtt_client.setCallback(mqttCallback);
  while (!mqtt_client.connected()) {
      String client_id = "esp32-client-" + String(WiFi.macAddress());
      Serial.printf("Connecting to MQTT Broker as %s.....\n", client_id.c_str());
      if (mqtt_client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
          Serial.println("Connected to MQTT broker");
          mqtt_client.subscribe(mqtt_topic);
          mqtt_client.publish(mqtt_topic, "No this is P"); // Publish some message upon successful connection
      } else {
          Serial.print("Failed, rc=");
          Serial.print(mqtt_client.state());
          Serial.println(" try again in 5 seconds");
          delay(5000);
      }
  }
}

void mqttCallback(char *mqtt_topic, byte *payload, unsigned int length) {
  Serial.print("Message received on mqtt_topic: ");
  Serial.println(mqtt_topic);
  Serial.print("Message: ");
  for (unsigned int i = 0; i < length; i++) {
      Serial.print((char) payload[i]);
  }
  Serial.println("\n-----------------------");
  mqtt_client.publish(mqtt_topic, "Thanks for your message, i will get back to you in 10 buissines days. ");
}

void loop() {
    if (!mqtt_client.connected()) {
      while (!mqtt_client.connected()) {
          String client_id = "esp32-client-" + String(WiFi.macAddress());
          Serial.printf("Connecting to MQTT Broker as %s.....\n", client_id.c_str());
          if (mqtt_client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
              Serial.println("Connected to MQTT broker");
              mqtt_client.subscribe(mqtt_topic);
          } else {
              Serial.print("Failed, rc=");
              Serial.print(mqtt_client.state());
              Serial.println(" try again in 5 seconds");
              delay(5000);
          }
      }
    }
    mqtt_client.loop();
}