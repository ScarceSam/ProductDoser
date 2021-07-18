#pragma once

uint8_t sdcard_init(void);

uint8_t SDcard_read_string(const char* device, const char* setting, char* result, uint8_t max_len);

uint8_t SDcard_read_int(const char* device, const char* setting, uint8_t* result);

uint8_t SDcard_write_int(const char* device, const char* setting, uint8_t value);
