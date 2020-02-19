typedef struct {
    uint8_t number;
    uint8_t valve_pin;
} detergent_t;

void detergent_init(detergent_t *unit, uint8_t number, const uint8_t valve_pin)
{
    unit->number = number;
    unit->valve_pin = valve_pin;
}

void detergent_print_info(detergent_t unit)
//curently prints detergent struct info to the comand line
//TODO: will be to print the detergent type to detergent connection number to screen
{
    printf(" Detergent #%d, valve on pin: %d \n", unit.number, unit.valve_pin);
}
