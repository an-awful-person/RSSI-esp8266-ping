#include <CsiCrawler.h>
#include <esp_wifi.h>
#include <esp_wifi_types.h>
#include <nvs_flash.h>
#include <Arduino.h>

void CsiCrawler::Start(void (*callback)(void *ctx, wifi_csi_info_t *info)) {
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());

    WifiInit();
    WifiCsiInit(callback);
    Serial.println("All things should be started");
}

void CsiCrawler::WifiInit()
{
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    ESP_ERROR_CHECK(esp_netif_init());
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_NONE));
}

void CsiCrawler::WifiCsiInit(void (*callback)(void *ctx, wifi_csi_info_t *info))
{
    ESP_ERROR_CHECK(esp_wifi_set_promiscuous(true));

    wifi_csi_config_t csi_config = {
        .lltf_en           = true,
        .htltf_en          = true,
        .stbc_htltf2_en    = true,
        .ltf_merge_en      = true,
        .channel_filter_en = true,
        .manu_scale        = false,
        .shift             = false,
    };

    ESP_ERROR_CHECK(esp_wifi_set_csi_config(&csi_config));
    ESP_ERROR_CHECK(esp_wifi_set_csi_rx_cb(callback, NULL));
    ESP_ERROR_CHECK(esp_wifi_set_csi(true));
}

void CsiCrawler::MutateCsiResource(CsiResource resource) {
    int existsIndex = -1;
    for(int i=0; i < csiResources.size(); i++) {
        if(csiResources[i].GetMac() == resource.GetMac()) {
            existsIndex = i;
        }
    }

    if(existsIndex == -1) {
        csiResources.push_back(resource);
    } else {
        csiResources[existsIndex] = resource;
    }
}

String CsiCrawler::GetCsiResources() {
    String result;

    result += "--Amount of devices: [ " + String(csiResources.size()) + "]----------\n";
    for(int i = 0; i < csiResources.size(); i++) {
        result += "MAC: ";
        result += csiResources[i].GetMac();
        result += " Channel: ";
        result += csiResources[i].GetChannel();
        result += " RSSI: ";
        result += csiResources[i].GetRssi();
        result += "\n";
    }
    result += "---------------";
    return result;
}