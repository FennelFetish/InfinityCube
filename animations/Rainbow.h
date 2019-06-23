#pragma once

#include "../Animation.h"


class AnimRainbow : public Animation
{
    private:
        float t;
    
    public:
        AnimRainbow() : t(0) {}
        
        virtual void update(AnimationContext& ctx, long tpf, bool beat) override
        {
            t += tpf / 1000000.0f;
            
            int colorVHigh = ctx.brightnessFactor * 255; //20; // 80, 70
            int colorVLow  = colorVHigh / 5; //3; // 20, 50
            int colorVDiff = colorVHigh - colorVLow;
            
            float vFactor = ctx.timeSinceBeat / 200000.0f;
            vFactor = pow(vFactor, 0.33);
            int colorV = colorVHigh - constrain(vFactor * colorVDiff, 0, colorVDiff);
            
            for(int i=0; i<ctx.numLeds; ++i) {
                double val = 0.5 * ( 1.0 + sin(-t*1.0 + (2.0 * i / ctx.numLeds * 3.1415926)) );
                ctx.leds[i].h = 255.0 * val;
                //h[i] = round(h[i]/30.0) * 30.0;
                //s[i] = 55.0 + 200.0 * val;
                //v[i] = 3.0 + 47.0 * val;
                //h[i] += 3;
                ctx.leds[i].s = 255;
                
                if(ctx.leds[i].h > 80)
                    ctx.leds[i].b = colorV;
                else
                    ctx.leds[i].b = 0;
            }
        }
    
};