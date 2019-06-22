#pragma once

#include "Animation.h"


class KnightRider : public Animation
{
    private:
        uint8_t beamLen;
        uint8_t pos;
        bool forward;
        
        int stepsPerSecond;
        int microsPerStep;
        long t;
        
        
        void applyToEdge(AnimationContext& ctx, Color8Bit* const edge) {
            int i = 0;
            
            for(; i<pos; ++i) {
                edge[i].b = 0;
            }
            
            for(int k=0; k<beamLen; ++i, ++k) {
                edge[i].b = ctx.brightnessFactor * 255;
            }
            
            for(; i<ctx.edgeLength; ++i) {
                edge[i].b = 0;
            }
        }
        
    
    public:
        KnightRider(int stepsPerSecond=40) :
            beamLen(12), pos(0), forward(true),
            stepsPerSecond(stepsPerSecond), microsPerStep(1000000/stepsPerSecond), t(0)
        {}
        
        
        virtual void update(AnimationContext& ctx, long tpf, bool beat) override {
            t += tpf;
            
            while(t > microsPerStep) {
                t -= microsPerStep;
                
                if(forward) {
                    pos += 1;
                    if(pos > ctx.edgeLength-beamLen)
                        forward = false;
                } else {
                    pos -= 1;
                    if(pos == 0)
                        forward = true;
                }
            }
            
            applyToEdge(ctx, ctx.edge1);
            applyToEdge(ctx, ctx.edge2);
            applyToEdge(ctx, ctx.edge3);
        }
};