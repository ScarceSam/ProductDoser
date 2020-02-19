#include <stdio.h>
#include <stdint.h>
#include "constants.h"
#include "washer.h"
#include "detergent.h"
#include "pump.h"
#include "display.h"
#include "flowmeter.h"
#include "logging.h"

washer_t washer[NUMBER_OF_WASHERS];
detergent_t detergent[NUMBER_OF_DETERGENTS];

void setup(void)
{
    // initialize washers
    for(int i = 0; i < NUMBER_OF_WASHERS; i++)
    {
        washer_init(&washer[i], (i + 1), WASHER_COM_PINS[i], NUMBER_OF_PINS, WASHER_VALVE[i], WASHER_ID[i]);
        washer_print_info(washer[i]);
    }

    // initialize washer interupts

    // initialize detergents
    for(int i = 0; i < NUMBER_OF_DETERGENTS; i++)
    {
        detergent_init(&detergent[i], (i + 1), DETERGENT_VALVE[i]);
        detergent_print_info(detergent[i]);
    }

    // initialize pump
    pump_init(PUMP_PIN);

    // initialize display

    // initialize flowmeter
    flow_init(FLOW_PIN);

    // initialize logging
}


void loop(void)
{
    //create washer queue

    //update washer queue
    //placing any interrupts into queue

    //reset timer if appropriate

}


int main(void)
{
    setup();

    while(0)
    {
        loop();
    }

    return 0;
}
