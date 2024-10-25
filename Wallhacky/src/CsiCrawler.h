#include <esp_wifi_types.h>
#include <cstdint>
#include <Arduino.h>
#include <vector>
#include <CsiResource.h>

class CsiCrawler {
    public:
    void Start(void (*callback)(void *ctx, wifi_csi_info_t *info));
    void MutateCsiResource(CsiResource resource);
    String GetCsiResources();
    private:
    std::vector<CsiResource> csiResources;
    void WifiInit();
    void WifiCsiInit(void (*callback)(void *ctx, wifi_csi_info_t *info));
};