#include washer.h
#include detergent.h
#include pump.h
#include display.h
#include flowmeter.h
#include logging.h
#include constants.h


void setup(void)
{
    // initialize washers
    for(int i = 0; i < NUMBER_OF_WASHERS; i++)
    {
        washer_init(i, WASHER_COM_PIN[i], NUMBER_OF_PINS, WASHER_VALVE[i], WASHER_ID[i]);
    }

    // initialize washer interupts

    // initialize detergents
    for(int i = 0; i < NUMBER_OF_DETERGENTS; I++)
    {
        detergent_init(i, DETERGENT_VALVE[i]);
    }

    // initialize pump
    pump_init(PUMP_PIN);

    // initialize display

    // initialize flowmeter
    flow_init(FLOW_PIN);

    // initialize logging
}


int loop(void) // return 0 when not pumping
{
    //return variable
    static uint8_t is_running = 0;

    //create washer queue

    //update washer queue
    //place any intupts into queue
    is_running = #of items in queue

    //

}


int main(void)
{
    setup();

    while(1)
    {
        uint8_t is_pumping = loop();

        if(!is_pumping)
        {
            // reset timming
        }
    }

    return 0;
}
