#ifndef Detergent_h
#define Detergent_h
#include <Arduino.h>

#define ID_LIMIT 16

void detergent_init(void);

void detergent_open_valve(uint8_t detergent_number);

void detergent_close_all_valves(void);

uint16_t ounces_per_pound_x100(uint8_t detergent_number);

uint16_t calibration_volume_oz(uint8_t detergent_number);

uint16_t calibration_time_sec(uint8_t detergent_number);

#endif
