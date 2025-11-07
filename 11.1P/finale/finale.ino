#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2
#define TDS_SENSOR A0
#define RELAY_PIN 3

#define TDS_ON_THRESHOLD 300   
#define TDS_OFF_THRESHOLD 300  
#define PUMP_RUN_TIME 10000 
#define PAUSE_TIME 3000        
#define CHECK_INTERVAL 10000   
#define SAMPLE_DELAY 500       

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

unsigned long lastPumpTime = 0;
bool pumpRunning = false;

void setup() {
  Serial.begin(9600);
  sensors.begin();
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);  
  delay(2000);
  Serial.println("System Ready");
  Serial.println("-----------------------------------");
}

void loop() {
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);
  int tdsValue = analogRead(TDS_SENSOR);

  Serial.print("Temp: ");
  Serial.print(tempC);
  Serial.print(" °C  |  TDS: ");
  Serial.print(tdsValue);
  Serial.println(" ppm");

  unsigned long currentTime = millis();

  if (!pumpRunning && tdsValue > TDS_ON_THRESHOLD && (currentTime - lastPumpTime) > PAUSE_TIME) {
    pumpRunning = true;
    digitalWrite(RELAY_PIN, LOW);  
    Serial.println("Pump: ON (High TDS detected)");

    unsigned long startTime = millis();

    while (millis() - startTime < PUMP_RUN_TIME) {
      sensors.requestTemperatures();
      tdsValue = analogRead(TDS_SENSOR);

      Serial.print("Running... TDS: ");
      Serial.println(tdsValue);

      if (tdsValue < TDS_OFF_THRESHOLD) {
        Serial.println("Pump: OFF (TDS now below 300)");
        break;
      }

      delay(SAMPLE_DELAY);
    }

    digitalWrite(RELAY_PIN, HIGH);
    pumpRunning = false;
    lastPumpTime = millis();
    Serial.println("-----------------------------------");
    delay(PAUSE_TIME);
  }

  delay(CHECK_INTERVAL);
}

