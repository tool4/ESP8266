/*  This sketch sends data via HTTP GET requests to local web serwer.
 *
 *  Based on ESP8266 WiFiClient Example
 *
 */

#include <ESP8266WiFi.h>
#include <string.h>
#include <stdio.h>

// ssid.h should contain ssid and password defs, like this:
//const char* ssid     = "********";
//const char* password = "********";
#include <ssid.h>

// Deep sleep mode doesn't seem to work, looks like ESP8266
// right after reset (or waking from deep sleep) sets D7 PIN
// to HIGH state which activates PIR sensor and results in
// incorrect results. Disabled for now.
#define DEEP_SLEEP 0

#define VERBOSE 0
#define VERY_VERBOSE 0

ADC_MODE(ADC_VCC);

const char* host = "10.0.0.28";

void setup()
{
  Serial.begin(115200);
  delay(10);
  pinMode(D7, INPUT);

  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  WiFi.mode(WIFI_STA);
  //wifi_set_sleep_type(LIGHT_SLEEP_T);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

#if VERBOSE
  Serial.println("");
  Serial.println("WiFi connected");
#endif
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Going to sleep for 17 seconds");
  delay(17*1000);
}

int value = 0;
int lastPIRstate = 0;
int counter = 0;

void loop()
{
  ++value;
  ++counter;
  bool motionDetected = false;

  // Experimental, idea is to track battery life this way
  // getting here values around 2.7V which doesn't seem right.
  float voltage=0.0f;
  voltage = ESP.getVcc()/1023.0f;
  Serial.print("VCC: ");
  Serial.print(voltage);
  Serial.println(" V");

  char voltage_str[10];
  dtostrf(voltage, 4, 2, voltage_str);

  String url = (String)"/esp/esp.php?name=ESP&VCC=" + voltage_str;

  int pirState = digitalRead(D7);

  if( pirState == 1 )
  {
    url += "&motion=true";
    Serial.println("motion=true");
    if( pirState != lastPIRstate )
    {
      motionDetected = true;
    }
  }
  else
  {
    url += "&motion=false";
    Serial.println("motion=false");
  }

  lastPIRstate = pirState;

  if( motionDetected || counter == 60 )
  {
    Serial.print("connecting to ");
    Serial.println(host);

    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort))
    {
      Serial.println("connection failed");
      Serial.println("Going into deep sleep for 10 seconds");
      ESP.deepSleep(10e6);
    }

    Serial.print("Requesting URL: ");
    Serial.println(url);

    // This will send the request to the server
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0)
    {
      if (millis() - timeout > 5000)
      {
        Serial.println(">>> Client Timeout, resetting in 10 seconds !");
        client.stop();
        ESP.deepSleep(10e6);
      }
    }

#if VERY_VERBOSE
    // Read all the lines of the reply from server and print them to Serial
    while(client.available()){
      String line = client.readStringUntil('\r');
      Serial.print(line);
     }
    Serial.println();
    Serial.println("closing connection");
#endif
  }
  delay(1 * 1000);
  counter = counter % 60;

  if( motionDetected)
  {
#if DEEP_SLEEP
    Serial.println("Going into deep sleep for 57 seconds");
    ESP.deepSleep(57e6); // 57e6 is 57 seconds
#else
    Serial.println("Going to sleep for 57 seconds");
    delay(57*1000);
#endif
  }
}

