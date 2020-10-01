#ifndef Detergent_h
#define Detergent_h
#include <Arduino.h>

#define ID_LIMIT 16

void detergent_init(void);

void detergent_open_valve(uint8_t detergent_number);

void detergent_close_all_valves(void);

uint8_t detergent_half_oz_per_ten_lbs(uint8_t detergent_number);

uint8_t detergent_load(void);

#endif
