#define pump D6

unsigned long previousMillis = 0;
unsigned long currentMillis  = 0;

void setup() {
  delay(1000);
  Serial.begin(115200);
  
  pinMode(pump, OUTPUT);
  digitalWrite(pump, LOW);
}

void loop() {
  delay(2000);
  digitalWrite(pump, LOW);
  Serial.println("Pump ON");
  
  delay(2000);
  digitalWrite(pump, HIGH);
  Serial.println("Pump OFF");
}
