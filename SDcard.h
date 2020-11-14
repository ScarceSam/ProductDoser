#pragma once

uint8_t sdcard_init(void);

String SDcard_read_string(String device, String setting);

uint8_t SDcard_read_string(const char* device, const char* setting, char* result);

int32_t SDcard_read_int(String device, String setting);

uint8_t SDcard_read_int(const char* device, const char* setting, uint8_t* result);
