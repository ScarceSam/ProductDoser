#ifndef Feedline_h
#define Feedline_h

#include <Arduino.h>
#include "Washer.h"
#include "Detergent.h"

#define PUMP_ON HIGH
#define PUMP_OFF !PUMP_ON
#define VALVE_OPEN HIGH
#define VALVE_CLOSE !VALVE_OPEN

enum valves{ LINE_DRAIN_VALVE, WATER_VALVE, MANIFOLD_DRAIN_VALVE, ALL_VALVES };

void feedline_init(void);

uint32_t feedline_pump_start(uint8_t volume_oz);

void feedline_valve(uint8_t valve, uint8_t state);

uint32_t feedline_flush_time_milli(void);

void pulse_pump(void);

uint8_t feedline_flush_oz(void);

void feedline_update(void);

bool feedline_pumping(void);

#endif
