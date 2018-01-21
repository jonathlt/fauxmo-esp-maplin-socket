#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "fauxmoESP.h"
#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

// -----------------------------------------------------------------------------
// Using lwip v1.4 Prebuilt
// Using https://bitbucket.org/bibbbi/fauxmoesp/get/5bcd3b483a69.zip
// -----------------------------------------------------------------------------

#define WIFI_SSID "myssid"
#define WIFI_PASS "mypassword"
#define SERIAL_BAUDRATE                 115200

fauxmoESP fauxmo;

// -----------------------------------------------------------------------------
// Wifi
// -----------------------------------------------------------------------------

void wifiSetup() {

    // Set WIFI module to STA mode
    WiFi.mode(WIFI_STA);

    // Connect
    Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    // Wait
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(100);
    }
    Serial.println();

    // Connected!
    Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
}

void callback(uint8_t device_id, const char * device_name, bool state) {
  Serial.print("Device "); Serial.print(device_name); 
  Serial.print(" state: ");
  if (state) {
    Serial.println("ON");
    mySwitch.switchOn(1, 2);
  } else {
    Serial.println("OFF");
    mySwitch.switchOff(1, 2);
  }
}

void setup() {
    // Init serial port and clean garbage
    Serial.begin(SERIAL_BAUDRATE);
    Serial.println("FauxMo demo sketch");
    Serial.println("After connection, ask Alexa/Echo to 'turn <devicename> on' or 'off'");

    // Wifi
    wifiSetup();

    // Fauxmo
    fauxmo.addDevice("switch two");
    fauxmo.onMessage(callback);

    // Transmitter is connected to ESP12E pin D1  
    mySwitch.enableTransmit(D1);
}

void loop() {
  fauxmo.handle();
}
