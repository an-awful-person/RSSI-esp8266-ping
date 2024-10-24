#include<wstring.h>
#include <iostream>
#include <chrono>
#include <ctime>

class PeripheralInfo{
    public:
    PeripheralInfo(String address, String localName, String rssi){
        this->address = address;
        this->localName = localName;
        this->rssi = rssi;
    }
    String GetAddress() {return address;}
    String GetLocalName() {return localName;}
    String GetRSSI() {return rssi;}
    void ResetTimeoutTimer(){
        timeMutated = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        }
    bool IsTimedOut(){
        time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        time_t timeSinceLastReset = now - timeMutated;
        if(timeSinceLastReset > 10){
            return true;
        }
        return false;
    }
    private:
    String address = "";
    String localName = "";
    String rssi = "";
    time_t timeMutated;
};