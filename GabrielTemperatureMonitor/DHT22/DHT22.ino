#include "DHT.h"

#define DHTTYPE DHT22 // AM2302
#define DHTPIN 2 // subject to change

struct TemperatureHumidityDataPoll {
  float temp;
  float humi;
  bool isFailed; 
 };

DHT dht(DHTPIN, DHTTYPE);

void setup() {
    
    Serial.begin(9600);
    dht.begin();
 }

struct TemperatureHumidityDataPoll pollTemperature(bool now) {
  // wait between polls
  if (!now) {
    delay(5000);
  }
  // data reads take around 250ms
  // sensor data might be off by 2 seconds (slow cheap sensors)
  float h = dht.readHumidity();
  // T in Celsius
  float t = dht.readTemperature();
  // declares the data structure to be returned
  struct TemperatureHumidityDataPoll pollData;
  pollData.isFailed = (isnan(h) || isnan(t));
  if (!pollData.isFailed) {
    pollData.temp = t;
    pollData.humi = h;  
  }
  return (pollData);
}

void relayPollingErrors() {
  // write to serial
  Serial.println("Failed to poll DHT sensor!");
  // warn external system of failure
  // blink LED?
}
 void loop() {
  struct TemperatureHumidityDataPoll dataPolled = pollTemperature(false);
  if (dataPolled.isFailed) {
    // immediate retry once
    dataPolled = pollTemperature(true);
    if (dataPolled.isFailed) {
      // things went South, call Houston
      relayPollingErrors();
    }
  }
 }

