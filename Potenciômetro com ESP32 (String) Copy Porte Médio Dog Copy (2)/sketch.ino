#define pinoSensor 36


//GRUPO 5

#include <DHTesp.h>
#include <EspMQTTClient.h>

//Para saber mais sobre esta biblioteca, acessar https://github.com/plapointe6/EspMQTTClient

// Constants
const char* SSIDName = "Wokwi-GUEST"; // WiFi network name
const char* SSIDPass = ""; // WiFi network password
const int DHT_PIN = 15; // DHT sensor pin
const int sensorPin = 36; // Sensor pin for weight measurement

const char* BrokerURL = "broker.hivemq.com"; // MQTT broker URL
const char* BrokerUserName = ""; // MQTT broker username
const char* BrokerPassword = ""; // MQTT broker password
const char* MQTTClientName = "mqtt-grupo5-pub"; // MQTT client name
const int BrokerPort = 1883; // MQTT broker port

const String TopicoPrefixo = "TESTGRUPO5DOG"; // Topic prefix
const String Topico_01 = TopicoPrefixo + "/Peso"; // Topic name for weight data

DHTesp dhtSensor; // DHT sensor object
EspMQTTClient clienteMQTT(SSIDName, SSIDPass, BrokerURL, BrokerUserName, BrokerPassword, MQTTClientName, BrokerPort); // MQTT client object

void onConnectionEstablished() {
  // No implementation needed
}

void enviarDados() {
  int valorSensor = analogRead(sensorPin);
  float voltagem = valorSensor * (3.3 / 4098);
  float peso = (valorSensor * 320)/4095;
  
  String payload = String(peso, 2);
  clienteMQTT.publish(Topico_01, payload);
}


void setup() {
  Serial.begin(9600);
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22); // Initialize DHT sensor
  clienteMQTT.enableDebuggingMessages(); // Enable debug messages
}

void loop() {
  clienteMQTT.loop(); // Maintain MQTT connection
  enviarDados(); // Send weight data to MQTT broker
  
  if (clienteMQTT.isWifiConnected()) {
    Serial.println("Conectado ao WiFi!");
  } else {
    Serial.println("Nao conectado ao WiFi!");
  }

  if (clienteMQTT.isMqttConnected()) {
    Serial.println("Conectado ao broker MQTT!");
  } else {
    Serial.println("Nao conectado ao broker MQTT!");
  }

  Serial.println("Nome do cliente: " + String(clienteMQTT.getMqttClientName())
    + " / Broker MQTT: " + String(clienteMQTT.getMqttServerIp())
    + " / Porta: " + String(clienteMQTT.getMqttServerPort())
  );

  delay(5000);
}



//DOG
//Porte médio (de 8 a 20 kg)
//o ideal é oferecer entre 160 g e 320 g de ração por dia. 
//2 refeições = 320 / 2 = 160
