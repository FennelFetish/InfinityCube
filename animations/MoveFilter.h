#pragma once

#include "../Animation.h"


class MoveFilter : public Animation
{
    private:
        float stepsPerSecond;
        short stepsOnBeat;
        
        unsigned int stepMicroseconds;
        unsigned int tStep;
    
    public:
        MoveFilter(int stepsPerSecond=120, int stepsOnBeat=14) :
            stepsPerSecond(stepsPerSecond), stepsOnBeat(stepsOnBeat),
            stepMicroseconds(stepsPerSecond > 0 ? round(1000000 / stepsPerSecond) : 0),
            tStep(0)
        {}
            
        
        virtual void update(AnimationContext& ctx, long tpf, bool beat) override {
            tStep += tpf;
            if(tStep < stepMicroseconds || !(beat && stepsOnBeat != 0))
                return;
            
            int steps = 0;
            if(stepMicroseconds > 0) {
                steps = tStep / stepMicroseconds;
                tStep -= steps * stepMicroseconds;
            }
            
            if(beat)
                steps += stepsOnBeat;
            
            if(steps > 0) {
                for(int i=ctx.numLeds-steps; i>0; --i)
                    ctx.leds[i] = ctx.leds[i-steps];
                
                for(int i=0; i<steps; ++i)
                    ctx.leds[i].b = 0;
            }
            else if(steps < 0) {
                int limit = ctx.numLeds-steps;
                int i=0;
                for(; i<limit; ++i)
                    ctx.leds[i] = ctx.leds[i-steps];
                
                for(; i<ctx.numLeds; ++i)
                    ctx.leds[i].b = 0;
            }
            

        }
    
};