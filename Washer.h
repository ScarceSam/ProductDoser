#ifndef Washer_h
#define Washer_h

#include <Arduino.h>

#define ID_LIMIT 16 

void washer_init(void);

uint8_t washer_ready(void);

void washer_get_next(uint8_t next_in_queue[]);

void washer_open_valve(uint8_t washer_number);

void washer_close_all_valves(void);

uint8_t washer_size(uint8_t washer_number);

void washer_update(void);

#endif
