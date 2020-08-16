#ifndef Detergent_h
#define Detergent_h
#include <Arduino.h>

#define ID_LIMIT 16

void detergent_init(void);

void detergent_open_valve(uint8_t detergent_number);//TODO: Delete

void detergent_close_all_valves(void);//TODO: delete

uint8_t detergent_half_oz_per_ten_lbs(uint8_t detergent_number); //TODO: change to long dispensingtime(washerlbs)

uint8_t detergent_load(void);

//TODO: add void dispense(product time)

//TODO: add void update() monitor dispesing ?return busy status?

//TODO: add int is_pumping()

#endif
