#pragma once

#include <Arduino.h>
#include "Washer.h"
#include "Detergent.h"

//values to use with valve function
#define PUMP_ON HIGH
#define PUMP_OFF !PUMP_ON
#define VALVE_OPEN HIGH
#define VALVE_CLOSE !VALVE_OPEN

//valve names for use with valve function
enum valves{ LINE_DRAIN_VALVE, WATER_VALVE, MANIFOLD_DRAIN_VALVE, ALL_VALVES };

//initializing function
void feedline_init(void);

//update function
void feedline_update(void);

//starting pump with desired volume
uint32_t feedline_pump_start(uint8_t volume_oz);

//is the pump active
bool feedline_is_pumping(void);

//operate valves
void feedline_valve(uint8_t valve, uint8_t state);

//return the flush volume
uint8_t feedline_flush_oz(void);

uint8_t feedline_manifold_oz(void);

bool feedline_load(void);

void feedline_flush(void);

void feedline_run_pump(bool);
