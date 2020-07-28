#pragma once

#include "../Animation.h"


class RandomizationFilter : public Animation
{
    private:
        static constexpr int zero = 127;
        static constexpr int maxRange = 6;
        
        Color8Bit* const offsets;
    
        float maxHPerSecond;
        float maxSPerSecond;
        float maxBPerSecond;
        
        uint8_t t;
    
    public:
        RandomizationFilter(AnimationContext& ctx, float h=4, float s=4, float b=4) :
            offsets(new Color8Bit[ctx.numLeds]),
            maxHPerSecond(h), maxSPerSecond(s), maxBPerSecond(b),
            t(0)
        {
            for(int i=0; i<ctx.numLeds; ++i) {
                offsets[i].h += zero;
                offsets[i].s += zero;
                offsets[i].b += zero;
            }
        }
        
        ~RandomizationFilter() {
            delete[] offsets;
        }
            
        
        virtual void update(AnimationContext& ctx, long tpf, bool beat) override {
            //float tFactor = tpf/1000000.0f;
            t++;
            if(t < 7)
                return;
            t = 0;
            
            // TODO: Store led colors as float / use HsbColor
            for(int i=0; i<ctx.numLeds; ++i) {
                offsets[i].h += 1 - random(0, 2);
                offsets[i].s += 1 - random(0, 2);
                offsets[i].b += 1 - random(0, 2);
                
                offsets[i].h = constrain(offsets[i].h, zero-maxRange, zero+maxRange);
                offsets[i].s = constrain(offsets[i].s, zero-maxRange, zero+maxRange);
                offsets[i].b = constrain(offsets[i].b, zero-maxRange, zero+maxRange);
                
                if(ctx.leds[i].b == 0)
                    continue;
                
                ctx.leds[i].h += (int)offsets[i].h - zero;
                ctx.leds[i].s += (int)offsets[i].s - zero;
                ctx.leds[i].b += (int)offsets[i].b - zero;
            }
        }
    
};