//washer constants
const uint8_t NUMBER_OF_WASHERS = ;
const uint8_t NUMBER_OF_PINS = ;
const uint8_t WASHER_COM_PINS[NUMBER_OF_WASHERS][NUMBER_OF_PINS] = 
    {
    };
const uint8_t WASHER_VALVE[NUMBER_OF_WASHERS] = 
    {
    };
const uint8_t WASHER_ID[NUMBER_OF_WASHERS] = 
    {
    };

//Washer interupts
//First dimension == enough room for up to all washers in queue,
//second dimension == washer # and detergent #
volatile uint8_t DETERGENT_CALL_ISR_FLAG[NUMBER_OF_WASHERS][2] = {0};
static uint8_t DETERGENT_CALL_BUFFER[NUMBER_OF_WASHERS][2] = {0};

//detergent constants
const uint8_t NUMBER_OF_DETERGENTS = ;
const uint8_t DETERGENT_VALVE[NUBER_OF_DETERGENTS] = 
    {
    };
const uint8_t WATER_VALVE = ;
const uint8_t DRAIN_VALVE = ;


//pump constants
const uint8_t PUMP_PIN = ;


//display constants


//flowmeter constants
const uint8_t FLOW_PIN = ;

//logging constants

