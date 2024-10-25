#include "peripheralManager.h"
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

#include <esp_wifi.h>
#include <CsiCrawler.h>

const char* ssid = "Wallhack";  //your WiFi Name
const char* password = "tinfoil1";      //Your Wifi Password

const char* apSSID = "ESP_AP_";
const char* apPassword = "somepass_2468";

const char* apiEndPoint = "/api/NetworksScan/";

String url = "http://192.168.1.13:5000/api/NetworksScan/network_scan";


PeripheralManager peripheralManager;
CsiCrawler csiCrawler;

void DefaultCsiCallback(void *ctx, wifi_csi_info_t *info) {
        char macStr[18];
        snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
           info->mac[0], info->mac[1], info->mac[2],
           info->mac[3], info->mac[4], info->mac[5]);

        CsiResource resource (String(macStr), info->rx_ctrl.channel, info->rx_ctrl.rssi);
        csiCrawler.MutateCsiResource(resource);
}

void setup() {
  Serial.begin(9600);
  csiCrawler.Start(DefaultCsiCallback);

  wifi_init_config_t init_config = WIFI_INIT_CONFIG_DEFAULT();

  esp_wifi_init(&init_config);
  esp_wifi_start();

  peripheralManager.Setup();
  delay(10);
  // WiFi.mode(WIFI_STA);


  WiFi.begin(ssid, password);

  String macAddressAsIDPart = WiFi.macAddress().substring(WiFi.macAddress().length() - 8);
  String actualApSSID = apSSID+macAddressAsIDPart;
  actualApSSID.replace(":","");

  WiFi.softAP(actualApSSID, apPassword);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

void loop() {
  peripheralManager.Loop();
  Serial.println(csiCrawler.GetCsiResources());

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
  delay(1000);
  delay(10);
}
