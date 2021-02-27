#pragma once

//values to use with valve function
#define VALVE_OPEN HIGH
#define VALVE_CLOSE !VALVE_OPEN

//valve names for use with valve function
enum valves{ LINE_DRAIN_VALVE, WATER_VALVE, MANIFOLD_DRAIN_VALVE, ALL_VALVES };

//initializing function
void feedline_init(void);

//operate valves
void feedline_valve(uint8_t valve, uint8_t state);

//return the flush volume
uint32_t feedline_flush_millis(void);

bool feedline_load(void);

void feedline_flush(void);

int feedline_menu_manual(char displaied_text[4][21], int buttons_pressed);
