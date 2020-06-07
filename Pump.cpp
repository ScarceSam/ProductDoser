#include <Arduino.h>
#include "Pump.h"

static const uint8_t COIL_A = PUMP_COIL_A_PIN;
static const uint8_t COIL_B = PUMP_COIL_B_PIN;
static const bool POSITION_A = 0;
static const bool POSITION_B = !POSITION_A;
static const uint8_t PULSE_PER_SEC = 5;
static const uint8_t TENTH_OUNCE_PER_PULSE = 5;
static const uint32_t MILLI_PER_PULSE = (1000/PULSE_PER_SEC);


uint32_t pump_start(uint8_t volume_oz)
{
  
}
