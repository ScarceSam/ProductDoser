#ifndef Feedline_h
#define Feedline_h

#include <Arduino.h>
#include "Washer.h"
#include "Detergent.h"

#define PUMP_ON HIGH
#define PUMP_OFF !PUMP_ON
#define VALVE_OPEN HIGH
#define VALVE_CLOSE !VALVE_OPEN

void feedline_init(void);

uint8_t feedline_idle(void);

void feedline_start_dose(uint8_t washer, uint8_t detergent);

uint32_t dosage_time_calc(uint8_t washer, uint8_t detergent);

void feedline_pump(uint8_t state);

void feedline_valve(uint8_t valve, uint8_t state);

void feedline_update(void);

uint32_t water_flush_time_milli(void);

void feedline_advance_step(void);

#endif
