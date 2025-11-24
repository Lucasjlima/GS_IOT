#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <ArduinoJson.h>

// --- Configurações de Wi-Fi (Wokwi usa essa rede virtual) ---
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// --- Configurações de MQTT ---
const char* mqtt_server = "broker.hivemq.com"; // Broker Público
const int mqtt_port = 1883;
const char* mqtt_topic = "balanceme/fiap/data";

// --- Hardware ---
#define DHTPIN 15
#define DHTTYPE DHT22
#define POTPIN 34
#define LEDPIN 2

DHT dht(DHTPIN, DHTTYPE);
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  pinMode(LEDPIN, OUTPUT);
  
  dht.begin();
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando em ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Tentando conexão MQTT...");
    String clientId = "ESP32BalanceMe-";
    clientId += String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str())) {
      Serial.println("Conectado!");
    } else {
      Serial.print("Falha, rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente em 5s");
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // --- Leitura dos Sensores ---
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  int potValue = analogRead(POTPIN);
  
  // Mapear potenciômetro (0-4095) para Batimentos (40-180 BPM)
  int bpm = map(potValue, 0, 4095, 40, 180);

  // Verificar leituras válidas
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Falha ao ler do sensor DHT!");
    return;
  }

  // --- Lógica de Negócio (BalanceMe) ---
  String alerta = "Normal";
  bool ledState = LOW;

  // Se BPM alto (Estresse) ou Temp alta (Desconforto) -> Alerta
  if (bpm > 120 || temperature > 30.0) {
    alerta = "ALERTA: Nível de Estresse/Desconforto Alto!";
    ledState = HIGH;
  } else if (bpm < 50) {
     alerta = "ALERTA: Sonolência/Inatividade Detectada";
     ledState = HIGH;
  }

  digitalWrite(LEDPIN, ledState);

  // --- Criar JSON ---
  StaticJsonDocument<200> doc;
  doc["temperatura"] = temperature;
  doc["umidade"] = humidity;
  doc["bpm"] = bpm;
  doc["alerta"] = alerta;

  char buffer[256];
  serializeJson(doc, buffer);

  // --- Publicar no MQTT ---
  client.publish(mqtt_topic, buffer);
  Serial.print("Publicado: ");
  Serial.println(buffer);

  delay(2000); // Publicar a cada 2 segundos
}