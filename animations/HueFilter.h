#pragma once

#include "../Animation.h"


class HueFilter : public Animation
{
    private:
        float hue; // 0-255
        float huePerSecond;
        float hueOnBeat;
    
    public:
        HueFilter() :
            huePerSecond(8), hueOnBeat(25)
        {
            hue = random(0, 255);
        }
            
        
        virtual void update(AnimationContext& ctx, long tpf, bool beat) override {
            if(beat)
                hue += hueOnBeat;
            
            float hueChange = huePerSecond * (tpf / 1000000.0f);
            hue += hueChange;
            if(hue > 255)
                hue -= 255;
            else if(hue < 0)
                hue += 255;

            for(int i=0; i<ctx.numLeds; ++i) {
                ctx.leds[i].h = hue;
            }
        }
    
};