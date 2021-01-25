// *****************************************************************************
// temp_humidity_sesnor.cpp
// *****************************************************************************

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include "temp_humidity_sensor.h"

// DHT
//type of sensor in use:
#define DHTTYPE    DHT22     // DHT 22 (AM2302)
#define DHTPIN     D1     // Digital pin GPIO5 connected to the DHT sensor

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;    // will store last time DHT was updated

// Updates DHT readings every 10 seconds
const long interval = 10000;
DHT dht(DHTPIN, DHTTYPE);

// current temperature & humidity, updated in loop()
float t = 0.0;
float h = 0.0;
float sensor_temp_val = 0.0;
float sensor_humidity_val = 0.0;


// *****************************************************************************
// Function Definitions
// *****************************************************************************

// *****************************************************************************
// setup()
// *****************************************************************************
void sensor_setup() {
  //prep the sensor
  dht.begin();

  Serial.println(F("DHT22 Sensor Online"));
}


// *****************************************************************************
// Sensor loop() --> MUST BE CALLED IN MAIN LOOP()
// *****************************************************************************
void sensor_loop() {
  //now is used to measure time periods for loops
  unsigned long now = millis();

  // DHT
  // read temp & humidity
  if (now - previousMillis >= interval) {
    // save the last time you updated the DHT values
    previousMillis = now;

    // Read temperature as Celsius (the default)
    float newT = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    //float newT = dht.readTemperature(true);
    // Read Humidity
    float newH = dht.readHumidity();
    // if read failed, don't change t & h value

    if (isnan(newT) || isnan(newH)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      //TODO:: Add interface for error condition
      //setBlinkState(BLINK_STATE_NOK);
    } else {
      t = newT;
      sensor_temp_val = t;
      Serial.println(t);
      h = newH;
      sensor_humidity_val = h;
      Serial.println(h);
      //setBlinkState(BLINK_STATE_OK);
    }
  }
}
