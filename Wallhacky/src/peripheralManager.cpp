#include <ArduinoBLE.h>
#include "peripheralManager.h"
#include <CompanyIdentifiers.h>


void PeripheralManager::Setup(){
    Serial.begin(9600);
    while (!Serial);

    // Start Bluetooth scanning
    if (!BLE.begin()) {
        Serial.println("Starting Bluetooth module failed!");
        while (1);
    }

    Serial.println("Bluetooth scanner active, scanning for devices...");
}

void PeripheralManager::Loop(){
    for(int i=0; i<50; i++){
         // Start scanning for nearby BLE devices:
        BLE.scan();

        BLEDevice peripheral = BLE.available();
        
        // Check if any Bluetooth devices are found
        if (peripheral) {
            const char* manufacturer = getManufacturerName(peripheral);
            PeripheralInfo newInfo(peripheral.address(), peripheral.localName(), String(peripheral.rssi()), String(manufacturer));
            MutatePeripheralInfo(newInfo);
            //std::string res = std::string(("MAC:["+ peripheral.address() + "] NAME:["+ peripheral.localName() + "] RSSI:["+ peripheral.rssi() + "] DeviceName: [" + peripheral.deviceName() + "]").c_str());
            
            //tryAddResult(std::string(peripheral.address().c_str()), res);   
        }
        delay(50);  // Small delay before next scan
    }
        PurgePeripherals();
        Serial.println(GetPeripheralsInfo());
   
}

void PeripheralManager::MutatePeripheralInfo(PeripheralInfo info) {
    int existsIndex = -1;
    for(int i=0; i<peripheralInfos.size(); ++i){
        if(peripheralInfos[i].GetAddress() == info.GetAddress()){
            existsIndex = i;
        }
    }

    info.ResetTimeoutTimer();
    if(existsIndex == -1){
        //New
        peripheralInfos.push_back(info);
    }else{
        //Mutated
        peripheralInfos[existsIndex] = info;
    }

}   

void PeripheralManager::PurgePeripherals() {
    std::vector<PeripheralInfo> remainingInfos;
     for(int i=0; i<peripheralInfos.size(); ++i){
        if(!peripheralInfos[i].IsTimedOut()){
            remainingInfos.push_back(peripheralInfos[i]);
        }
     }
     peripheralInfos = remainingInfos;
}

String PeripheralManager::GetPeripheralsInfo() {
    String result;
    result += "--Amount of peripherals : [ " + String(peripheralInfos.size()) + "]----------\n";
     for(int i=0; i<peripheralInfos.size(); ++i){
        result += "Address: ";
        result += peripheralInfos[i].GetAddress();
        result += " LocalName: ";
        result += peripheralInfos[i].GetLocalName();
        result += " RSSI: ";
        result += peripheralInfos[i].GetRSSI();
        result += " Manufacturer: ";
        result += peripheralInfos[i].GetManufacturer();
        result += "\n";

     }
     result += "------------";
    
     return result;
}