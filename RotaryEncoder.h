#pragma once
#include <arduino.h>

class RotaryEncoder
{
    public:
        RotaryEncoder(int pinClk, int pinDt, int pinSw);
        
        void setup();
        bool update();
        
        void setValue1(int val)     { value1.value = val; }
        int getValue1() const       { return value1.value; }
        void setMin1(int min)       { value1.valueMin = min; }
        void setMax1(int max)       { value1.valueMax = max; }
        void setStep1(int step)     { value1.step = step; }
        
        void setValue2(int val)     { value2.value = val; }
        int getValue2() const       { return value2.value; }
        void setMin2(int min)       { value2.valueMin = min; }
        void setMax2(int max)       { value2.valueMax = max; }
        void setStep2(int step)     { value2.step = step; }
        
        bool getSwitchState() const;
        

    private:
        static constexpr int STATE_VAL1 = LOW; //HIGH
    
        const int pinClk;
        const int pinDt;
        const int pinSw;
        
        int lastClk;
        int lastDt;
        int lastSw;
        
        
        struct Val {
            int value;
            int valueMin;
            int valueMax;
            int step;
            
            Val();
        };
        
        Val value1;
        Val value2;
};