#ifndef System_h
#define System_h

#include <Arduino.h>
#include "Washer.h"
#include "Detergent.h"

#define PUMP_ON HIGH
#define PUMP_OFF !PUMP_ON
#define VALVE_OPEN HIGH
#define VALVE_CLOSE !VALVE_OPEN

void system_init(void);

uint8_t system_idle(void);

void system_start_dose(uint8_t washer, uint8_t detergent);

uint32_t dosage_time_calc(uint8_t washer, uint8_t detergent);

void system_pump(uint8_t state);

void system_valve(uint8_t valve, uint8_t state);

void system_update(void);

uint32_t water_flush_time(void);

void system_advance_step(void);

#endif
