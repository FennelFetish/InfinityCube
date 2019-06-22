#pragma once

#include "Animation.h"


class Strobo : public Animation
{
    private:
        uint8_t onDuration;
        uint8_t offDuration;
        uint8_t t;
        bool on;
    
    public:
        Strobo() : onDuration(1), offDuration(3), on(false) {}
        
        virtual void update(AnimationContext& ctx, long tpf, bool beat) override {
            t++;
            uint8_t duration = on ? onDuration : offDuration;
            if(t < duration)
                return;
            t = 0;
            
            on ^= true;
            
            uint8_t b = on ? ctx.brightnessFactor * 255 : 0;
            for(int i=0; i<ctx.numLeds; ++i) {
                ctx.leds[i].b = b;
            }
        }
    
};