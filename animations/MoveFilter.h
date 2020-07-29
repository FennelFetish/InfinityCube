#pragma once

#include "../Animation.h"


class MoveFilter : public Animation
{
    protected:
        int stepsPerSecond;
        int stepsOnBeat;
        
        unsigned int stepMicroseconds;
        unsigned int tStep;
        int stepSign;
        
        
        void move(AnimationContext& ctx, int idxStart, int idxEnd, int steps) {
            // Move forward
            if(steps > 0) {
                int limit = idxStart+steps;
                for(int i=idxEnd; i>=limit; --i)
                    ctx.leds[i] = ctx.leds[i-steps];
                
                for(int i=idxStart; i<steps; ++i)
                    ctx.leds[i].b = 0;
            }
            // Move backward
            else if(steps < 0) {
                int limit = idxEnd+steps;
                int i = idxStart;
                for(; i<=limit; ++i)
                    ctx.leds[i] = ctx.leds[i-steps];
                
                for(; i<idxEnd; ++i)
                    ctx.leds[i].b = 0;
            }
        }
        
        
        virtual void move(AnimationContext& ctx, int steps) {
            move(ctx, 0, ctx.numLeds-1, steps);
        }
        
        
    private:
        unsigned int calcStepMicroseconds(int stepsPerSecond) {
            if(stepsPerSecond == 0)
                return 0;
            return round(1000000 / abs(stepsPerSecond));
        }
        
    
    public:
        MoveFilter(int stepsPerSecond=120, int stepsOnBeat=14) :
            stepsPerSecond(stepsPerSecond), stepsOnBeat(stepsOnBeat),
            stepMicroseconds(calcStepMicroseconds(stepsPerSecond)),
            tStep(0),
            stepSign(stepsPerSecond >= 0 ? 1 : -1)
        {}
            
        
        virtual void update(AnimationContext& ctx, long tpf, bool beat) override {
            tStep += tpf;
            if(tStep < stepMicroseconds && !(beat && stepsOnBeat != 0))
                return;
            
            int steps = 0;
            if(stepMicroseconds > 0) {
                steps = tStep / stepMicroseconds;
                tStep -= steps * stepMicroseconds;
                steps *= stepSign;
            }
            
            if(beat)
                steps += stepsOnBeat;
            
            move(ctx, steps);
        }
};