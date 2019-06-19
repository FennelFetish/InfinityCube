#pragma once


class DigiPoti
{
    public:
        DigiPoti(int incPin, int udPin);
        void setup();
        
        void up();
        void down();
        
        
    private:
        const int pinInc;
        const int pinUD;
        
        int pos;
        
        
        void pulseInc() const;
};