#ifndef System_h
#define System_h

#include <Arduino.h>
#include "Washer.h"
#include "Detergent.h"

void system_init(void);

uint8_t system_idle(void);

void system_start_dose(washer_t washer, detergent_t detergent);

uint32_t dosage_time_calc(washer_t washer, detergent_t detergent);

void system_pump_on(void);

void system_pump_off(void);

void system_update(void);

#endif
