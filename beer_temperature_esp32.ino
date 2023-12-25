/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com
*********/

#include <OneWire.h>
#include <DallasTemperature.h>
#include <ArduinoJson.h>
#include "BluetoothSerial.h"
#define TEMPERATURE_PRECISION 12

BluetoothSerial SerialBT;

// GPIO where the DS18B20 is connected to
const int oneWireBus = 4;

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);

const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_ARRAY_SIZE(2) + JSON_OBJECT_SIZE(1) + 2 * JSON_OBJECT_SIZE(2);

long waitTime = 30000;

//number of connected devices
int deviceCount = 1 ;
DeviceAddress tempDeviceAddress;


void setup() {
  // Start the Serial Monitor
  Serial.begin(115200);
  Serial.println("Setting up...");
  // Start the DS18B20 sensor
  pinMode(oneWireBus, INPUT);
  sensors.begin();
  Serial.println("After Sensor begin...");
  SerialBT.begin("Brew Thermometer - ESP32"); //Bluetooth device name
}

void loop() {
  if (SerialBT.connected()) {
    DynamicJsonDocument doc(capacity);
    JsonObject doc_0 = doc.to<JsonObject>();
    sensors.requestTemperatures();

    JsonArray sensorRoot = doc_0.createNestedArray("sensors");

    JsonObject sensor_1 = sensorRoot.createNestedObject();
    sensor_1["id"] = 0;
    sensor_1["temperature"] = sensors.getTempFByIndex(0);

    JsonObject sensor_2 = sensorRoot.createNestedObject();
    sensor_2["id"] = 1;
    sensor_2["temperature"] = sensors.getTempFByIndex(1);

    serializeJson(doc, SerialBT);
    SerialBT.println();
    checkInput();
    delay(waitTime);
  }

}

void checkInput() {
  String inString = "";
  if (SerialBT.available() > 0) {
    inString = SerialBT.readString();
  }
  if (inString.length() > 0) {
    waitTime = inString.toInt() * 1000;
    //    Serial.println(waitTime);

  }
}
