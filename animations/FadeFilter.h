#pragma once

#include "../Animation.h"

// TODO: Fade needs to be independent of brightnessFactor -> scale by brightnessFactor?
class FadeFilter : public Animation
{
    private:
        uint8_t fadePerFrame;
        bool saturation;
    
    public:
        FadeFilter(int fadePerFrame=10, bool sat=false) :
            fadePerFrame(fadePerFrame), saturation(sat)
        {}
        
        virtual void update(AnimationContext& ctx, long tpf, bool beat) override {
            if(saturation) {
                for(int i=0; i<ctx.numLeds; ++i) {
                    if(ctx.leds[i].s > 255-fadePerFrame)
                        ctx.leds[i].s = 255;
                    else
                        ctx.leds[i].s += fadePerFrame;
                }
            }
            else {
                for(int i=0; i<ctx.numLeds; ++i) {
                    if(ctx.leds[i].b < fadePerFrame)
                        ctx.leds[i].b = 0;
                    else
                        ctx.leds[i].b -= fadePerFrame;
                }
            }
        }
    
};