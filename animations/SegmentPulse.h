#pragma once

#include "../Animation.h"


class SegmentPulse : public Animation
{
    private:
        bool randomColor;
        int randomHalfChance; // 0-100
        
        int lastIdx;

        int getSegment() {
            int edgeIdx;
            
            do {
                edgeIdx = random(0, 3); // 3 exclusive
            } while(edgeIdx == lastIdx);
            
            lastIdx = edgeIdx;
            return edgeIdx;
        }
        
        
        void pulse(AnimationContext& ctx, int start, int end) {
            int edgeIdx = getSegment();
            Color8Bit* edge = ctx.leds + (edgeIdx * ctx.edgeLength);
            
            for(int i=start; i<end; ++i) {
                edge[i].b = ctx.brightnessFactor * 255;
            }

            if(randomColor) {
                int color = random(0, 256);
                for(int i=start; i<end; ++i) {
                    edge[i].h = color;
                }
            }
        }

    
    public:
        SegmentPulse() :
            randomColor(false), randomHalfChance(0), lastIdx(-1)
        {}
        
        
        void setRandomizeColor(bool enabled) {
            randomColor = enabled;
        }
        
        void setRandomHalfChance(int chance) {
            randomHalfChance = chance;
        }
        
        
        virtual void update(AnimationContext& ctx, long tpf, bool beat) override {
            if(!beat)
                return;
            
            int start = 0;
            int end   = ctx.edgeLength;
            
            if(randomHalfChance > 0) {
                int rnd = random(0, 100);
                if(randomHalfChance > rnd) {
                    int half = random(0, 2); // exclusive 2
                    
                    int halfLen = ctx.edgeLength / 2;
                    if(half == 0)
                        end = halfLen;
                    else
                        start = halfLen;
                }
            }

            pulse(ctx, start, end);
        }
    
};