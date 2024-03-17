int sensorPin = A0;    // input for LDR and rain sensor

unsigned long previousMillis = 0;
unsigned long currentMillis  = 0;

void setup() {
  delay(1000);
  Serial.begin(115200);

  pinMode(sensorPin, INPUT);
}

void loop() {
  delaySensor(2000);
}

// LDR, DHT, DS18B20, PH, Salinity, Ultrasonic, Rain sensor
void delaySensor(unsigned long interval) {
  currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    //=============== Rain Sensor =============== 
    int sensorValue2 = analogRead(sensorPin);
    sensorValue2 = constrain(sensorValue2, 150, 440); 
    sensorValue2 = map(sensorValue2, 150, 440, 1023, 0); 
    Serial.println(sensorValue2);
    if (sensorValue2 >= 20){
      Serial.println("rain is detected");
    }
    else {
      Serial.println("rain not detected");
    }
    //==================================================

  }
}
