

typedef struct {
    uint8_t number;
    uint8_t position;
    uint8_t com_pin[NUMBER_OF_PINS];
    uint8_t valve_pin;
    uint8_t id;
} washer_t;



void washer_init(washer_t *unit, int number, const uint8_t com_pin[NUMBER_OF_PINS], const uint8_t number_of_pins, const uint8_t valve_pin, uint8_t id)
{
    unit->number = number;
    unit->id = id;
    unit->valve_pin = valve_pin;
    for(int i = 0; i < NUMBER_OF_WASHERS; i++)
    {
        unit->com_pin[i] = com_pin[i];
    }
}

void washer_print_info(washer_t unit)
{
    printf(" Washer ID: %d, communication pins are: %d, %d, %d, %d, valve on pin: %d \n", unit.id, unit.com_pin[0], unit.com_pin[1], unit.com_pin[2], unit.com_pin[3], unit.valve_pin);
}
