

typedef struct {
    uint8_t number;
    uint8_t position;
    uint8_t com_pin[NUMBER_OF_PINS];
    uint8_t valve_pin;
    uint8_t id;
} washer_t;



void washer_init(washer_t *unit, int number, const uint8_t com_pin[NUMBER_OF_PINS],
                 uint8_t number_of_pins, uint8_t valve_pin, uint8_t id)
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
//currently prints detergent strct info to the command line
//TODO: will print the washer ID to connection number to the screen
{
    printf(" Washer#%d, ID: %d, valve on pin: %d, communication pins are: %d"
	   ,unit.number, unit.id, unit.valve_pin, unit.com_pin[0]);

    for(int i = 1; i < NUMBER_OF_WASHERS; i++)
    {
        printf(", %d", unit.com_pin[i]);
    }
    printf("\n");
}
