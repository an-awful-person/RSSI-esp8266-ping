#ifndef HEADER_H_WIFISCANNER
 #define HEADER_H_WIFISCANNER

#include <WString.h>
#include "wifiConfig.h"

class WifiScanner {
    public:
        WifiConfig wifiConfig;
        String url = "";

        WifiScanner();
        WifiScanner(WifiConfig wifiConfig, String url);

        void Setup(WifiConfig wifiConfig, String url);
        void Loop(); 
};
#endif