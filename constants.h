//washer constants
#define NUMBER_OF_WASHERS 
#define NUMBER_OF_PINS 
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
#define NUMBER_OF_DETERGENTS 
const uint8_t DETERGENT_VALVE[NUBER_OF_DETERGENTS] = 
    {
    };
#define WATER_VALVE 
#define DRAIN_VALVE 


//pump constants
#define PUMP_PIN 


//display constants


//flowmeter constants
#define FLOW_PIN 

//logging constants

