#pragma once

#include "../Animation.h"


class Dot : public Animation
{
    private:
        static constexpr float beatLen = 100000.0f;
    
        long tSinceBeat;
        
        int dotIndex;
        
        int stayFrames;
        int stayFramesRemaining;
        
    
    public:
        Dot() : dotIndex(0), stayFrames(0), stayFramesRemaining(0) {}
        
        
        void setDotIndex(int index) {
            dotIndex = index;
        }
        
        void setStayFrames(int frames) {
            stayFrames = frames;
        }
        
        
        virtual void update(AnimationContext& ctx, long tpf, bool beat) override {
            tSinceBeat += tpf;
            if(beat) {
                tSinceBeat = 0;
                stayFramesRemaining = stayFrames;
            }
            
            stayFramesRemaining--;
            long t = (stayFramesRemaining > 0) ? 0 : tSinceBeat;
            
                
            float dotFactor = 1.0f - ((float)t / beatLen);
            if(dotFactor < 0)
                dotFactor = 0;
            
            ctx.leds[dotIndex].b = round(ctx.brightnessFactor * 255 * dotFactor);
        }
    
};