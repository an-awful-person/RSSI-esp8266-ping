#ifndef COMPANY_IDENTIFIERS_H
#define COMPANY_IDENTIFIERS_H

#include <cstdint>
#include <ArduinoBLE.h>

struct CompanyIdentifier {
    uint16_t id;          // Company identifier
    const char* name;     // Company name
};

extern CompanyIdentifier companyIdentifiers[];
extern const char* getManufacturerName(uint16_t id);
extern void checkManufacturerData(BLEDevice peripheral);
extern const char* getManufacturerName(BLEDevice peripheral);

#endif