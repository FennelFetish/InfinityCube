#pragma once

#include "Animation.h"


class MoveFilter : public Animation
{
    private:
        float stepsPerSecond;
        uint8_t stepsOnBeat;
        
        unsigned int stepMicroseconds;
        unsigned int tStep;
    
    public:
        MoveFilter(int stepsPerSecond=120, int stepsOnBeat=14) :
            stepsPerSecond(stepsPerSecond), stepsOnBeat(stepsOnBeat),
            stepMicroseconds(round(1000000 / stepsPerSecond)),
            tStep(0)
        {}
            
        
        virtual void update(AnimationContext& ctx, long tpf, bool beat) override {
            tStep += tpf;
            if(tStep < stepMicroseconds)
                return;
            
            int steps = tStep / stepMicroseconds;
            tStep -= steps * stepMicroseconds;
            
            if(beat)
                steps += stepsOnBeat;
            
            for(int i=ctx.numLeds-steps; i>0; --i) {
                ctx.leds[i] = ctx.leds[i-steps];
            }
            
            for(int i=0; i<steps; ++i)
                ctx.leds[i].b = 0;
        }
    
};