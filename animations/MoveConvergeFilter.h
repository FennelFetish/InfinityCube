#pragma once

#include "../Animation.h"


// Make this a filter with start and end index, use 2 instances for converge effect


class MoveConvergeFilter : public Animation
{
    private:
        float stepsPerSecond;
        short stepsOnBeat;
        
        unsigned int stepMicroseconds;
        unsigned int tStep;
        
        
        void move(AnimationContext& ctx, int idxStart, int idxEnd, int steps) {
            if(steps > 0) {
                int limit = idxStart+steps;
                for(int i=idxEnd; i>=limit; --i)
                    ctx.leds[i] = ctx.leds[i-steps];
                
                for(int i=idxStart; i<steps; ++i)
                    ctx.leds[i].b = 0;
            }
            else if(steps < 0) {
                // steps is negative!
                int limit = idxEnd+steps;
                int i=idxStart;
                for(; i<=limit; ++i)
                    ctx.leds[i] = ctx.leds[i-steps];
                
                for(; i<idxEnd; ++i)
                    ctx.leds[i].b = 0;
            }
        }
        
        
    
    public:
        MoveConvergeFilter(int stepsPerSecond=120, int stepsOnBeat=14) :
            stepsPerSecond(stepsPerSecond), stepsOnBeat(stepsOnBeat),
            stepMicroseconds(stepsPerSecond > 0 ? round(1000000 / stepsPerSecond) : 0),
            tStep(0)
        {}
            
        
        virtual void update(AnimationContext& ctx, long tpf, bool beat) override {
            tStep += tpf;
            if(tStep < stepMicroseconds && !(beat && stepsOnBeat != 0))
                return;
            
            int steps = 0;
            if(stepMicroseconds > 0) {
                steps = tStep / stepMicroseconds;
                tStep -= steps * stepMicroseconds;
            }
            
            if(beat)
                steps += stepsOnBeat;

            int idxCenter = ctx.numLeds/2;
            move(ctx, 0, idxCenter, steps);
            move(ctx, idxCenter+1, ctx.numLeds-1, -steps);
        }
};