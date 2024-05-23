#include <DHTesp.h>
#include <EspMQTTClient.h>

// GRUPO 5
//CAT

// Definições e constantes
char SSIDName[] = "Wokwi-GUEST"; // nome da rede WiFi
char SSIDPass[] = ""; // senha da rede WiFi

const int DHT_PIN = 15; // terminal do sensor de temperatura e umidade
const int TRIG_PIN = 12;   // Pino Trig conectado ao D12
const int ECHO_PIN = 15;   // Pino Echo conectado ao D15

const float raio = 10;
const float altura = 7;
const float distancia_maxima = altura;

char BrokerURL[] = "broker.hivemq.com"; // URL do broker MQTT
char BrokerUserName[] = ""; // nome do usuário para autenticar no broker MQTT
char BrokerPassword[] = ""; // senha para autenticar no broker MQTT
char MQTTClientName[] = "mqtt-mack-pub"; // nome do cliente MQTT
int BrokerPort = 1883; // porta do broker MQTT

String TopicoPrefixo = "TESTGRUPO5CAT"; // prefixo do tópico
String Topico_01 = TopicoPrefixo + "/Distancia"; // nome do tópico 01
String Topico_02 = TopicoPrefixo + "/Volume"; // nome do tópico 02

// Variáveis globais e objetos
DHTesp dhtSensor; // instancia o objeto dhtSensor a partir da classe DHTesp
EspMQTTClient clienteMQTT(SSIDName, SSIDPass, BrokerURL, BrokerUserName, BrokerPassword, MQTTClientName, BrokerPort); // inicializa o cliente MQTT

// Este protótipo de função deve estar sempre presente
void onConnectionEstablished() {
}

void enviarDados() {
  long duration;
  float distance;

  // Emitir um pulso ultrassônico
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Medir o tempo de retorno do eco
  duration = pulseIn(ECHO_PIN, HIGH);

  // Calcular a distância em centímetros
  distance = (duration / 2) * 0.0343;

  distance = min(distance, distancia_maxima);

  float volume = PI * raio * raio * (altura - distance);

  float volume_litros = volume / 1000.0;

  // Exibir a distância e o volume em litros no monitor serial
  Serial.print("Distancia: ");
  Serial.print(distance);
  Serial.print(" cm, Volume: ");
  Serial.print(volume_litros);
  Serial.println(" litros");

  // Enviar dados via MQTT
  clienteMQTT.publish(Topico_01, String(distance));
  clienteMQTT.publish(Topico_02, String(volume_litros));
}

// Setup
void setup() {
  Serial.begin(9600);

  dhtSensor.setup(DHT_PIN, DHTesp::DHT22); // inicializa o sensor de peso

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  clienteMQTT.enableDebuggingMessages(); // habilita mensagens de debug no monitor serial
}

// Loop
void loop() {
  clienteMQTT.loop(); // função necessária para manter a conexão com o broker MQTT ativa e coletar as mensagens recebidas
  enviarDados(); // função para publicar osdados no broker MQTT

  if (clienteMQTT.isWifiConnected() == 1) {
    Serial.println("Conectado ao WiFi!");
  } else {
    Serial.println("Não conectado ao WiFi!");
  }

  if (clienteMQTT.isMqttConnected() == 1) {
    Serial.println("Conectado ao broker MQTT!");
  } else {
    Serial.println("Não conectado ao broker MQTT!");
  }

  Serial.println("Nome do cliente: " + String(clienteMQTT.getMqttClientName())
    + " / Broker MQTT: " + String(clienteMQTT.getMqttServerIp())
    + " / Porta: " + String(clienteMQTT.getMqttServerPort())
  );

  delay(5000);
}

//Ideal para raças de porte médio (10 a 25 kg)
//cerca de 900 ml a 1300 ml de capacidade, 20 cm de diâmetro e 7 cm de altura.