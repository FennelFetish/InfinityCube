#pragma once

#include "../Animation.h"


class HueOffsetFilter : public Animation
{
    private:
        float huePerSecond;
        float hueOnBeat;
    
    public:
        HueOffsetFilter(float perSec=8, float onBeat=25) :
            huePerSecond(perSec), hueOnBeat(onBeat)
        {}
            
        
        virtual void update(AnimationContext& ctx, long tpf, bool beat) override {
            float hueChange = huePerSecond * (tpf / 1000000.0f);
            if(beat)
                hueChange += hueOnBeat;
            
            for(int i=0; i<ctx.numLeds; ++i) {
                ctx.leds[i].h += hueChange;
            }
        }
    
};