#pragma once

#include <Arduino.h>

uint8_t sdcard_init(void);

String SDcard_read_string(String device, String setting);

uint32_t SDcard_read_int(String device, String setting);
