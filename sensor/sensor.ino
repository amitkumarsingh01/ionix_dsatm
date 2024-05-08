#include <Wire.h>
#include <Adafruit_BMP085.h>
#include "DHT.h"
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Redmi";
const char* password = "qw123qw123";
const char* server = "api.thingspeak.com";
const String apiKey = "QT6WX1K3E3F1GKPL";

const int sensorPin = 25;
#define DHT11PIN 18
#define DHTTYPE DHT11

Adafruit_BMP085 bmp;
DHT dht(DHT11PIN, DHTTYPE);

void setup() {
  Serial.begin(9600);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085/BMP180 sensor, check wiring!");
    while (1) {}
  }

  dht.begin();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    float temperature = bmp.readTemperature();
    float pressure = bmp.readPressure();
    float altitude = bmp.readAltitude();
    float sealevelPressure = bmp.readSealevelPressure();
    float realAltitude = bmp.readAltitude(102000);
    float humidity = dht.readHumidity();
    int mqSensorValue = analogRead(sensorPin);

    // Display data on Serial Monitor
    Serial.println("Data:");
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" *C");
    Serial.print("Pressure: ");
    Serial.print(pressure);
    Serial.println(" Pa");
    Serial.print("Altitude: ");
    Serial.print(altitude);
    Serial.println(" meters");
    Serial.print("Pressure at sealevel (calculated): ");
    Serial.print(sealevelPressure);
    Serial.println(" Pa");
    Serial.print("Real altitude: ");
    Serial.print(realAltitude);
    Serial.println(" meters");
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println("%");
    Serial.print("MQ Sensor: ");
    Serial.print(mqSensorValue);
    Serial.println(" ppm");
    Serial.println();

    // Send data to ThingSpeak
    sendDataToThingSpeak(temperature, pressure, altitude, sealevelPressure, realAltitude, humidity, mqSensorValue);

    delay(10000); // Delay for 10 seconds before sending next set of data
  }
}

void sendDataToThingSpeak(float temperature, float pressure, float altitude, float sealevelPressure, float realAltitude, float humidity, int mqSensorValue) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Your API key with the field names
    String url = "http://api.thingspeak.com/update?api_key=" + apiKey;
    url += "&field1=" + String(temperature);
    url += "&field2=" + String(pressure);
    url += "&field3=" + String(altitude);
    url += "&field4=" + String(sealevelPressure);
    url += "&field5=" + String(realAltitude);
    url += "&field6=" + String(humidity);
    url += "&field7=" + String(mqSensorValue);

    Serial.println("Sending data to ThingSpeak...");
    Serial.println(url);

    http.begin(url);

    int httpResponseCode = http.GET();

    if (httpResponseCode == 200) {
      Serial.println("Data sent to ThingSpeak successfully!");
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("WiFi Disconnected! Unable to send data to ThingSpeak.");
  }
}
