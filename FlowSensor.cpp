#include <Arduino.h>

static volatile uint32_t pulseTime[256] = {0};
static volatile uint8_t timeCursor = 0;
static uint8_t delayMillis = 50;

void incrementCount()
{
    ++timeCursor;
    pulseTime[timeCursor] = millis();
}

void flowsensor_init(int pin)
{
  attachInterrupt(digitalPinToInterrupt(pin), incrementCount, CHANGE);
}

bool flowsensor_is_flowing(void)
{

}
