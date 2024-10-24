#include <ESP8266HTTPClient.h>

#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

#include <ArduinoWiFiServer.h>
#include <BearSSLHelpers.h>
#include <CertStoreBearSSL.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiGratuitous.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiClientSecureBearSSL.h>
#include <WiFiServer.h>
#include <WiFiServerSecure.h>
#include <WiFiServerSecureBearSSL.h>
#include <WiFiUdp.h>

const char* ssid = "router";  //your WiFi Name
const char* password = "wachtwoord";      //Your Wifi Password

const char* apSSID = "ESP_AP_";
const char* apPassword = "somepass_2468";

const char* apiEndPoint = "/api/NetworksScan/";

String url = "http://192.168.2.195:5000/api/NetworksScan/network_scan";

void setup() {
  Serial.begin(115200);
  delay(10);
  // WiFi.mode(WIFI_STA);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); 

  String macAddressAsIDPart = WiFi.macAddress().substring(WiFi.macAddress().length() - 8);
  String actualApSSID = apSSID+macAddressAsIDPart;
  actualApSSID.replace(":","");

  WiFi.softAP(actualApSSID, apPassword);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

void loop() {
  Serial.println("Starting Wi-Fi scan...");
  // Perform a scan to list nearby Wi-Fi networks
  int numNetworks = WiFi.scanNetworks();

  if (numNetworks == 0) {
    Serial.println("No networks found");
  } else {
    Serial.println("Networks found:");
    // Loop through the detected networks
    for (int i = 0; i < numNetworks; ++i) {
      // Print the SSID and RSSI of each network
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));    // Network name
      Serial.print(" (RSSI: ");
      Serial.print(WiFi.RSSI(i));    // Signal strength
      Serial.println(" dBm)");
    }
  }

  //Now post them to the API
  if(WiFi.status() == WL_CONNECTED){
    WiFiClient client;
    HTTPClient http;

    http.begin(client, url);
    http.addHeader("Content-Type", "application/json");

    StaticJsonDocument<256> jsonDoc;
    jsonDoc["macAddress"] = WiFi.macAddress();
    JsonArray pingArray = jsonDoc.createNestedArray("networkPings");
    for (int i = 0; i < numNetworks; ++i) {
      JsonObject networkPing = pingArray.createNestedObject();
      networkPing["ssid"] = WiFi.SSID(i);
      networkPing["bssid"] = WiFi.BSSIDstr(i);
      networkPing["rssi"] = WiFi.RSSI(i);
    }

    String jsonPayload;
    serializeJson(jsonDoc, jsonPayload);

    Serial.println("Sending networkscan...");
    int httpResponseCode = http.POST(jsonPayload);

        // Check the response
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("HTTP Response code: " + String(httpResponseCode));
      Serial.println("Response: " + response);
    } else {
      Serial.println("Error on sending POST: " + String(httpResponseCode));
      Serial.println("url: "+url);
      Serial.println(jsonPayload);
    }

    http.end();  // Free resources
  }

  // Clean up and free memory used by WiFi.scanNetworks
  WiFi.scanDelete();
  // delay(1000);
  digitalWrite(LED_BUILTIN, LOW); 
  delay(100);
  digitalWrite(LED_BUILTIN, HIGH); 
}