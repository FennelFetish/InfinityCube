#pragma once

#include "../Animation.h"


class Dot : public Animation
{
    private:
        static constexpr float beatLen = 100000.0f;
    
        long tSinceBeat;
    
    public:
        Dot() {}
        
        virtual void update(AnimationContext& ctx, long tpf, bool beat) override
        {
            tSinceBeat += tpf;
            if(beat)
                tSinceBeat = 0;
                
            float dotFactor = 1.0f - ((float)tSinceBeat / beatLen);
            if(dotFactor < 0)
                dotFactor = 0;
            
            ctx.leds[0].b = round(ctx.brightnessFactor * 255 * dotFactor);
        }
    
};