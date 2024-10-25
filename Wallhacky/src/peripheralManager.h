#include <vector>
#include <peripheralInfo.h>
#include "wifiConfig.h"

class PeripheralManager {
    public:
    void Setup();
    void Loop();
    void MutatePeripheralInfo(PeripheralInfo info);
    void PurgePeripherals();
    String GetPeripheralsInfo();
    void PostToApi(WifiConfig wifiConfig, String url);
    private:
    std::vector<PeripheralInfo> peripheralInfos;
};