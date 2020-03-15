#include "Washer.h"

void setup() {
  
  Serial.begin(115200);
  washer_init();

}

void loop() {

  for(int i = 0; i < NUMBER_OF_WASHERS; i++)
  {
    Serial.print("Washer number: ");
    Serial.println(washer[i].number);
    Serial.print("I2C address: ");
    Serial.println(washer[i].i2cAddress);
    Serial.print("Com pins: ");
    Serial.print(washer[i].com_pin[0]);
    Serial.print(", ");
    Serial.print(washer[i].com_pin[1]);
    Serial.print(", ");
    Serial.print(washer[i].com_pin[2]);
    Serial.print(", ");
    Serial.println(washer[i].com_pin[3]);
    Serial.print("Valve Pin: ");
    Serial.println(washer[i].valve_pin);
    Serial.print("Washer Size: ");
    Serial.println(washer[i].washerSize);
    Serial.print("Washer ID: ");
    Serial.println(washer[i].washerId);
    Serial.println();
  }
  delay(100000000);

}
