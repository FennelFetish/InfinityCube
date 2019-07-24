#pragma once

#include "RotaryEncoder.h"

#define NUM_PRESSES 12


class BeatButton
{
    public:
        BeatButton(const RotaryEncoder& rotaryEncoder) :
            rot(rotaryEncoder), hit(false), swState(false), idxLastPress(0), microsPerBeat(500000), tAutoBeat(30000000)
        {}
        
        void update(long now, long tpf);
        
        bool consumeHit() {
            bool hasHit = hit;
            hit = false;
            return hasHit;
        }
    
    private:
        const RotaryEncoder& rot;
        bool hit;
        bool swState;
        
        long lastPresses[NUM_PRESSES];
        int idxLastPress;
        
        long microsPerBeat;
        long tAutoBeat;
        
        void doPress(long now);
    
};
