#include "DigiPoti.h"
#include <Arduino.h>

//#define PRINT_VAL


DigiPoti::DigiPoti(int incPin, int udPin) :
    pinInc(incPin), pinUD(udPin), pos(1)
{}


void DigiPoti::setup()
{
    pinMode(pinInc, OUTPUT);
    pinMode(pinUD,  OUTPUT);
  
    digitalWrite(pinInc, LOW);
    digitalWrite(pinUD,  LOW);
    
    // Initialize value at bottom (1)
    digitalWrite(pinUD, LOW);
    for(int i=0; i<150; ++i)
        pulseInc();

    pos = 1;
}


void DigiPoti::up()
{
    if(pos >= 100)
        return;
    
    digitalWrite(pinUD, HIGH);
    pulseInc();
    digitalWrite(pinUD, LOW); // --> for saving power?
    pos++;
    
#ifdef PRINT_VAL
    Serial.print("poti up: ");
    Serial.println(pos);
#endif
}


void DigiPoti::down()
{
    if(pos <= 0)
        return;
    
    digitalWrite(pinUD, LOW);
    pulseInc();
    pos--;
    
#ifdef PRINT_VAL
    Serial.print("poti down: ");
    Serial.println(pos);
#endif
}


void DigiPoti::pulseInc() const
{
    delayMicroseconds(12);
    digitalWrite(pinInc, HIGH);
    delayMicroseconds(12);
    digitalWrite(pinInc, LOW);
}