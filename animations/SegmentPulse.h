#pragma once

#include "../Animation.h"


class SegmentPulse : public Animation
{
    private:

    
    public:
        SegmentPulse() {}
        
        virtual void update(AnimationContext& ctx, long tpf, bool beat) override
        {
            if(!beat)
                return;
            
            int edgeIdx = random(0, 3); // 3 exclusive
            Color8Bit* edge = ctx.leds + (edgeIdx * ctx.edgeLength);
            
            for(int i=0; i<ctx.edgeLength; ++i) {
                edge[i].b = ctx.brightnessFactor * 255;
            }
        }
    
};