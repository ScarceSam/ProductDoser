

typedef struct {
    uint8_t position;
    uint8_t com_pin[NUMBER_OF_PINS];
    uint8_t valve_pin;
    uint8_t id;
} washer_t;



void washer_init(washer_t *unit, const uint8_t com_pin[4], const uint8_t number_of_pins, const uint8_t valve_pin, uint8_t id)
{
    unit->id = id;
    unit->valve_pin = valve_pin;
    for(int i = 0; i < NUMBER_OF_WASHERS; i++)
    {
        unit->com_pin[i] = com_pin[i];
    }
}

