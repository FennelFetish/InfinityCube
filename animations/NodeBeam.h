#pragma once

#include "Animation.h"


class NodeBeam : public Animation
{
    private:
        static constexpr float beatLen = 200000.0f;
    
        uint8_t minBeamLen;
        long tSinceBeat;
        
        
        void applyToEdge(AnimationContext& ctx, Color8Bit* const edge, int len) {
            // Reset
            /*for(int i=0; i<ctx.edgeLength; ++i) {
                edge[i].h = 60;
                edge[i].s = 255;
                edge[i].b = 0;
            }*/
            
            for(int i=0; i<len; ++i) {
                edge[i].b = ctx.brightnessFactor * 255;
            }
            
            int last = ctx.edgeLength - len;
            for(int i=ctx.edgeLength-1; i>=last; --i) {
                edge[i].b = ctx.brightnessFactor * 255;
            }
        }
        
    
    public:
        NodeBeam() :
            minBeamLen(4),
            tSinceBeat(0)
        {}
        
        
        virtual void update(AnimationContext& ctx, long tpf, bool beat) override {
            tSinceBeat += tpf;
            if(beat)
                tSinceBeat = 0;

            float beamFactor = 1.0f - ((float)tSinceBeat / beatLen);
            if(beamFactor < 0)
                beamFactor = 0;

            uint8_t maxBeamLen = (ctx.edgeLength / 2) - minBeamLen;
            uint8_t beamLen = minBeamLen + round(maxBeamLen * beamFactor);
            
            applyToEdge(ctx, ctx.edge1, beamLen);
            applyToEdge(ctx, ctx.edge2, beamLen);
            applyToEdge(ctx, ctx.edge3, beamLen);
        }
};