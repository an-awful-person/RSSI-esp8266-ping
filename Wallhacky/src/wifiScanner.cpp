#include "wifiScanner.h"
#include "wifiConfig.h"

#include <HTTPClient.h>

#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiClient.h>
#include <WiFiGeneric.h>
#include <WiFiMulti.h>
#include <WiFiSTA.h>
#include <WiFiScan.h>
#include <WiFiServer.h>
#include <WiFiType.h>
#include <WiFiUdp.h>

#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

WifiScanner::WifiScanner() {

}

WifiScanner::WifiScanner(WifiConfig wifiConfig, String url){
    this->wifiConfig = wifiConfig;
    this->url = url;
}



void WifiScanner::Setup(WifiConfig wifiConfig, String url){
// Serial.begin(9600);
//   delay(10);
  // WiFi.mode(WIFI_STA);

  this->wifiConfig = wifiConfig;
  this->url = url;
  WiFi.begin(wifiConfig.getSSID(), wifiConfig.getAPPassword());

  String macAddressAsIDPart = WiFi.macAddress().substring(WiFi.macAddress().length() - 8);
  String actualApSSID = wifiConfig.getAPSSID()+macAddressAsIDPart;
  actualApSSID.replace(":","");

  WiFi.softAP(actualApSSID, wifiConfig.getAPPassword());

  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(500);
  //   Serial.print(".");
  // }
  Serial.println("");
  Serial.println("WiFi connected");
}



void WifiScanner::Loop(){
  if (WiFi.status() != WL_CONNECTED) {
Serial.println("Wi-Fi not connected...");
  } else {
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

    http.begin(client, this->url);
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
      Serial.println("url: "+ this->url);
      Serial.println(jsonPayload);
    }

    http.end();  // Free resources
  }

  // Clean up and free memory used by WiFi.scanNetworks
  WiFi.scanDelete();
  // delay(1000);
//   delay(100);
  }
}

