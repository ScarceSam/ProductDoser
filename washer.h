

typedef struct {
    uint8_t position;
    uint8_t com_pin[NUMBER_OF_PINS];
    uint8_t valve_pin;
    uint8_t id;
} washer_t;



void init_washer(washer_t unit, uint8_t com_pin, uint8_t number_of_pins, uint8_t valve_pin, uint8_t id);

