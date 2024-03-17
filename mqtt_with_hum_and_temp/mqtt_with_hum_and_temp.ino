/*********
  M. H. M. Ramli 
  SEA-IC, UiTM Shah Alam
  Please report if there is any bugs in the program :: tronexia@gmail.com

  Program: MQTT Weather Transmitter and broadcast publicly all received data 
*********/
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <MQTT.h>    // BY Joel Gaehwiller

// Library and constants for DHT11
#include <DHT.h>
#define DHTPIN D5     // Digital pin connected to the DHT sensor IO D5
#define DHTTYPE DHT11   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);
const byte ledPin5 = D6; 

#define sub1 "sub/data/one"          // Send data every set interval...publish everytime
#define sub2 "sub/data/two"     // Send data upon Request.....publish upon request
#define sub3 "sub/data/three"      // Listen to request.....subscrice
const char ssid[]     = "Moon Knight_2.4GHz";         // replace with your WiFi SSID
const char password[] = "satusampailapan";          // replace with your WiFi password

const char clientId[] = "unique ID";              // replace with your MQTT Client Id
const char server[]   = "www.txio.live";  // replace with your MQTT Broker txio.uitm.edu.my/seaic.uitm.edu.

WiFiClient net;
MQTTClient client;

//ID
String loraId ="7xx"; //01 ~ 99

// Variables
float tmp, hmd;
String txData;

// time constants
unsigned long lastMillis = 0;
///////////////////////////////////////////////
void setup() {
  //pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ledPin5, OUTPUT);
  // STEP 1: initialize Serial Monitor
  Serial.begin(115200);

  // STEP 2 - Initialize WiFi connectivity
  WiFi.begin(ssid, password);
  client.begin(server, net);
  client.onMessage(messageReceived);
  dht.begin();
  // STEP 3 - Connecting to MQTT broker
  connect();
}

void loop() {
  //check MQTT connection
  checkConnect();
  //lastMillis = millis();
//  dhtSensor();        //data acquisition process
  DataProcessing();
  BroadCast();
//  delay(1000);
}
///////////////////////////////////////////////
void DataProcessing(){
  if (runEvery(6000)) {
  hmd = dht.readHumidity();
  hmd = hmd/1.0f;
  tmp = dht.readTemperature();
  tmp= tmp/1.0f;
  float f = dht.readTemperature(true);
  if (isnan(tmp) || isnan(hmd) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
    Serial.println("===================================");
    client.publish(sub1, String(tmp));
    client.publish(sub2, String(hmd));
    Serial.println("TMP: "+ String(tmp) + ", " +"HMD: "+ String(hmd));
  }
}

//-----------------------------------------------------------------------------------
boolean runEvery(unsigned long interval)
{
  static unsigned long previousMillis = 0;
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    return true;
  }
  return false;
}
////////////////////////////////////////////////////////////////////
// Sub-function to connect to WiFi and MQTT broker
void connect() {
  
  Serial.print("Connecting to WiFi ...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println(" connected!");

  Serial.print("Connecting to MQTT broker ...");
  while (!client.connect(clientId)) {
    Serial.print(".");
    delay(500);
  }
  Serial.println(" connected!");

 // client.subscribe(topic);
 client.subscribe(sub1);
 client.subscribe(sub2);
 client.subscribe(sub3);
}
////////////////////////////////////////////////////////////////////
void messageReceived(String &topic, String &payload) {
//Serial.println("incoming: " + topic + " - " + payload); 
  Serial.println("Topic: " +  topic);
  Serial.println("Payload: " +  payload);
  // Note: Do not use the client in the callback to publish, subscribe or
  // unsubscribe as it may cause deadlocks when other things arrive while
  // sending and receiving acknowledgments. Instead, change a global variable,
  // or push to a queue and handle it in the loop after calling `client.loop()`.
  String sensorType = topic;

 
}
////////////////////////////////////////////////////////////////////
void checkConnect(){
  client.loop();

  if (!client.connected()) {
    connect();
  }
}

void BroadCast(){
 // publish a message roughly every second.
  if (millis() - lastMillis > 5000) {
    lastMillis = millis();
    client.publish(sub1, String(txData));
  }
}
