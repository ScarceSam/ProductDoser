#ifndef Washer_h
#define Washer_h

#include <Arduino.h>

#define NUMBER_OF_PINS 5
#define ID_LIMIT 16 

typedef struct {
  uint8_t number;
  uint8_t i2cAddress;
  uint8_t com_pin[NUMBER_OF_PINS];
  uint8_t valve_pin;
  uint8_t washer_size;
  char washer_id[ID_LIMIT];
} washer_t;

void washer_init(void);

uint8_t washer_ready(void);

void washer_get_next(uint8_t next_in_queue[]);

washer_t washer_get_data(uint8_t washer_number);

void washer_open_valve(uint8_t washer_number);

void washer_close_valve(uint8_t washer_number);

#endif
