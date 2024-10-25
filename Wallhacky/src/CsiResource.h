#include <WString.h>
#include <iostream>

class CsiResource {
    public:
        CsiResource(String mac, uint8_t channel, int rssi) {
            this->mac = String(mac);
            this->channel = String(channel);
            this->rssi = String(rssi);
        }
        String GetChannel() {return String(channel);}
        String GetRssi() {return String(rssi);}
        String GetMac() {return mac;}
    private:
        String channel;
        String rssi;
        String mac;
};