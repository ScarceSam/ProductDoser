#pragma once

#include <Arduino.h>

void flowsensor_init(int pin);
bool flowsensor_is_flowing(void);
