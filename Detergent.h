#ifndef Detergent_h
#define Detergent_h

#include <Arduino.h>

#define ID_LIMIT 16

typedef struct{
  uint8_t number;
  uint8_t level_pin;
  uint8_t flow_pin;
  uint8_t valve_pin;
  uint16_t ounces_per_pound_x100 = 20;
  uint16_t calibration_volume_oz = 87;
  uint16_t calibration_time_sec = 300;
  char detergent_name[ID_LIMIT];
} detergent_t;

void detergent_init(void);

detergent_t detergetent_get_data(uint8_t detergent_number);

void detergent_open_valve(uint8_t detergent_number);

void detergent_close_all_valves(void);

uint16_t ounces_per_pound_x100(uint8_t detergent_number);

uint16_t calibration_volume_oz(uint8_t detergent_number);

uint16_t calibration_time_sec(uint8_t detergent_number);

#endif
