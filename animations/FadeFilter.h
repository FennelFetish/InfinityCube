#pragma once

#include "Animation.h"


class FadeFilter : public Animation
{
    private:
        uint8_t fadePerFrame;
    
    public:
        FadeFilter() : fadePerFrame(10) {}
        
        virtual void update(AnimationContext& ctx, long tpf, bool beat) override {
            for(int i=0; i<ctx.numLeds; ++i) {
                if(ctx.leds[i].b < fadePerFrame)
                    ctx.leds[i].b = 0;
                else
                    ctx.leds[i].b -= fadePerFrame;
            }
        }
    
};