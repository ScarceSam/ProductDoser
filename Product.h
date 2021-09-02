#pragma once

#define ID_LIMIT 16

void product_init(void);

void product_pump_on(uint8_t product_number);

void product_pump_off(uint8_t product_number);

void product_all_pumps_off(void);

uint8_t product_half_oz_per_ten_lbs(uint8_t product_number);

uint8_t product_load(void);

char* product_label(uint8_t product_number);

uint32_t product_pump_millis(uint8_t product_number, uint8_t volume_oz);

bool product_save_calibration(uint8_t product_number, uint8_t oz_min);
