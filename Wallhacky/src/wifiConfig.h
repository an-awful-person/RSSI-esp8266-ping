#ifndef HEADER_H_WIFICONFIG
 #define HEADER_H_WIFICONFIG


class WifiConfig {
    public:
        WifiConfig()
        {}
        WifiConfig(
        const char* ssid,
        const char* password,
        const char* apSSID,
        const char* apPassword, 
        const char* apiEndPoint) 
    {
        this->ssid = ssid; 
        this->password = password;
        this->apSSID = apSSID;
        this->apPassword = apPassword; 
        this->apiEndPoint = apiEndPoint; 
    }

    const char* getSSID(){return ssid;}
    const char* gePassword(){return password;}
    const char* getAPSSID(){return apSSID;}
    const char* getAPPassword(){return apPassword;}
    const char* getEndPoint(){return apiEndPoint;}


    private:
        const char* ssid = ""; 
        const char* password = "";
        const char* apSSID = "";
        const char* apPassword = ""; 
        const char* apiEndPoint = ""; 
};

#endif