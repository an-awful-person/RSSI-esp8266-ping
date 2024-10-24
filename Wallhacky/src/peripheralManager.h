#include <vector>
#include <peripheralInfo.h>

class PeripheralManager {
    public:
    void Setup();
    void Loop();
    void MutatePeripheralInfo(PeripheralInfo info);
    void PurgePeripherals();
    String GetPeripheralsInfo();
    private:
    std::vector<PeripheralInfo> peripheralInfos;
    
};