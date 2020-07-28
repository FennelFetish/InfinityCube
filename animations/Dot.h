#pragma once

#include "../Animation.h"


class Dot : public Animation
{
    private:
        static constexpr float beatLen = 100000.0f;
    
        long tSinceBeat;
        
        int dotIndex;
        
    
    public:
        Dot() : dotIndex(0) {}
        
        
        void setDotIndex(int index) {
            dotIndex = index;
        }
        
        
        virtual void update(AnimationContext& ctx, long tpf, bool beat) override
        {
            tSinceBeat += tpf;
            if(beat)
                tSinceBeat = 0;
                
            float dotFactor = 1.0f - ((float)tSinceBeat / beatLen);
            if(dotFactor < 0)
                dotFactor = 0;
            
            ctx.leds[dotIndex].b = round(ctx.brightnessFactor * 255 * dotFactor);
        }
    
};