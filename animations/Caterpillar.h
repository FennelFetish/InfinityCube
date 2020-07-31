#pragma once

#include "../Animation.h"


class Caterpillar : public Animation
{
    private:
        static constexpr int speed = 100;
    
        static constexpr int maxLen = 71;
        static constexpr int minLen = 41;
        int len;
        unsigned long offset;
        
        static constexpr int gap = 80;
    
        bool growing;
        
        int start;
        int end;
    
    
        void printOne(AnimationContext& ctx, int first, int last) {
            if(first > ctx.numLeds)
                return;
            if(last < 0)
                return;
            
            int b = ctx.brightnessFactor * 255;
            float step = 1.0f / (last-first+1);
            float t = 0;
            
            if(first < 0) {
                t += step * -first;
                first = 0;
            }
            else if(last >= ctx.numLeds) {
                last = ctx.numLeds-1;
            }
            
            for(int i=first; i<=last; ++i) {
                ctx.leds[i].b = b;
                ctx.leds[i].h = t * 100.0f;
                
                t += step;
            }
        }
        
        
    public:
        Caterpillar() :
            len(minLen), offset(0), growing(true),
            start(0), end(minLen)
        {}
        
        
        virtual void update(AnimationContext& ctx, long tpf, bool beat) override {
            if(beat)
                growing ^= true;
            
            int change = speed * (tpf / 1000000.0f);
            
            if(growing && len < maxLen) {
                if(len + change > maxLen)
                    change = maxLen - len;
                
                len += change;
                end += change;
            }
            else if(!growing && len > minLen) {
                if(len - change < minLen)
                    change = len - minLen;
                
                len -= change;
                start += change;
            }
            
            if(start >= ctx.numLeds) {
                start -= gap;
                end -= gap;
            }

            for(int i=0; i<=ctx.numLeds; ++i)
                ctx.leds[i].b = 0;
            
            printOne(ctx, start, end);
            printOne(ctx, start-gap, end-gap);
            printOne(ctx, start-gap-gap, end-gap-gap);
        }
};