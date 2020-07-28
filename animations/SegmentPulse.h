#pragma once

#include "../Animation.h"


class SegmentPulse : public Animation
{
    private:
        bool randomColor;
        
        int lastIdx;

        int getSegment() {
            int edgeIdx;
            
            do {
                edgeIdx = random(0, 3); // 3 exclusive
            } while(edgeIdx == lastIdx);
            
            lastIdx = edgeIdx;
            return edgeIdx;
        }

    
    public:
        SegmentPulse() : randomColor(false), lastIdx(-1) {}
        
        
        void setRandomizeColor(bool enabled) {
            randomColor = enabled;
        }
        
        
        
        virtual void update(AnimationContext& ctx, long tpf, bool beat) override
        {
            if(!beat)
                return;
            
            int edgeIdx = getSegment();
            Color8Bit* edge = ctx.leds + (edgeIdx * ctx.edgeLength);
            
            for(int i=0; i<ctx.edgeLength; ++i) {
                edge[i].b = ctx.brightnessFactor * 255;
            }
            
            if(randomColor) {
                int color = random(0, 256);
                for(int i=0; i<ctx.edgeLength; ++i) {
                    edge[i].h = color;
                }
            }
        }
    
};