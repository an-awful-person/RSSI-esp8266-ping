#include "Arduino.h"
#include "peripheralManager.h"
#include "wifiScanner.h"
// #include <HTTPClient.h>

// #include <WiFi.h>
// #include <WiFiAP.h>
// #include <WiFiClient.h>
// #include <WiFiGeneric.h>
// #include <WiFiMulti.h>
// #include <WiFiSTA.h>
// #include <WiFiScan.h>
// #include <WiFiServer.h>
// #include <WiFiType.h>
// #include <WiFiUdp.h>

// #include <ArduinoJson.h>
// #include <ArduinoJson.hpp>

PeripheralManager peripheralManager;
WifiConfig wifiConfig(
  "Wallhack",
  "tinfoil1",
  "ESP_AP_",
  "somepass_2468",
  "/api/NetworksScan/"
);
String url = "http://192.168.1.13:5000/api/NetworksScan/network_scan";

WifiScanner wifiScanner(wifiConfig, url);




void setup() {
  Serial.begin(9600);
  while (!Serial);
  delay(10);

  peripheralManager.Setup();
  wifiScanner.Setup(wifiConfig, url);

}

void loop() {
  peripheralManager.Loop();
  wifiScanner.Loop();
  delay(50);
}
