#ifndef Detergent_h
#define Detergent_h

#include <Arduino.h>

#define ID_LIMIT 16

typedef struct{
  uint8_t number;
  uint8_t level_pin;
  uint8_t flow_pin;
  uint8_t valve_pin;
  uint8_t ounces_per_pound;
  uint8_t ounces_per_5minutes;
  char detergent_name[ID_LIMIT];
} detergent_t;

void detergent_init(void);

detergent_t detergetent_get_data(uint8_t detergent_number);

#endif
