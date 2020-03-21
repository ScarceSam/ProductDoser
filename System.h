#ifndef System_h
#define System_h

#include <Arduino.h>
#include "Washer.h"
#include "Detergent.h"

uint8_t system_idle(void);

void system_dose(washer_t washer, detergent_t detergent);

#endif
