#include "RotaryEncoder.h"
//#include <arduino.h>


RotaryEncoder::RotaryEncoder(int pinClk, int pinDt, int pinSw) :
    pinClk(pinClk), pinDt(pinDt), pinSw(pinSw),
    lastClk(LOW), lastDt(LOW), lastSw(LOW)
{}


RotaryEncoder::Val::Val() :
    value(0), valueMin(0), valueMax(100), step(1)
{}


bool RotaryEncoder::getSwitchState() const { 
    return lastSw == HIGH;
}


void RotaryEncoder::setup()
{
    pinMode(pinClk, INPUT);
    pinMode(pinDt,  INPUT);
    pinMode(pinSw,  INPUT);

    // Enable Pull-Up Resistors
    digitalWrite(pinClk, HIGH);
    digitalWrite(pinDt,  HIGH);
    digitalWrite(pinSw,  HIGH);
}


// Returns whether value has changed
bool RotaryEncoder::update()
{
    bool valueChanged = false;
    
    // Always read switch
    lastSw = digitalRead(pinSw);
    
    // Sample data when clock changes
    int clk = digitalRead(pinClk);
    if(clk == lastClk)
        return false;
    lastClk = clk;

    int dt = digitalRead(pinDt);
    if(dt == lastDt)
        return false;
    lastDt = dt;

    // Only pay attention to Clk Low-High transition
    if(clk != HIGH)
        return false;

    // Process switch
    Val& val = (lastSw == STATE_VAL1) ? value1 : value2;
    
    if(dt == HIGH)
        val.value += val.step;
    else
        val.value -= val.step;
    
    val.value = constrain(val.value, val.valueMin, val.valueMax);
    
    /*Serial.print("Value ");
    Serial.print((lastSw==STATE_VAL1) ? "1 (brightness)" : "2 (sensitivity)");
    Serial.print(" changed to: ");
    Serial.println(val.value);*/
    
    return true;
}