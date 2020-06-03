// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include "LTimer.h"
#include "DHT.h"

#define DHTPIN 2     // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT11   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
#include <LWiFi.h>
#include <WiFiClient.h>
#include "MCS.h"

#define _SSID "jason"
#define _KEY "1234567890"
MCSDevice mcs("DPBGPBBQ","c6hcfkumNvEPQfGY");
MCSDisplayFloat temp("tem");
MCSDisplayFloat humidity("wet");
DHT dht(DHTPIN, DHTTYPE);

LTimer timer1(LTIMER_1);
float h,t;
void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));
  while(WL_CONNECTED != WiFi.status())
  {
    Serial.print("WiFi.begin(");
    Serial.print(_SSID);
    Serial.print(",");
    Serial.print(_KEY);
    Serial.println(")...");
    WiFi.begin(_SSID, _KEY);
  }
  Serial.println("WiFi connected !!");
  mcs.addChannel(temp);
  mcs.addChannel(humidity);
  timer1.begin();
  timer1.start(10000,LTIMER_REPEAT_MODE,_callback1,NULL);
  dht.begin();
}

void loop() {
   h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  t = dht.readTemperature();
  mcs.process(100);
   while(!mcs.connected())
  {
    Serial.println("re-connect to MCS...");
    mcs.connect();
    if(mcs.connected())
      Serial.println("MCS connected !!");
  }
  temp.set(t);
  humidity.set(h);
}

void _callback1(void *usr_data)
{
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.println(t);
  
}
