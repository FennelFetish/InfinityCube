#pragma once

#include "../Animation.h"


class Stars : public Animation
{
    private:
        int spawnsPerSecond;
        long microsBetweenSpawns;
        long t;
        float hue; // 0-255
    
    public:
        float huePerSecond;
        float hueOnBeat;
        int idxStart;
        int idxEnd;
        
    
        Stars(int spawnsPerSecond=60, float startHue=0) :
            spawnsPerSecond(spawnsPerSecond), microsBetweenSpawns(1000000 / spawnsPerSecond), t(0),
            hue(startHue), huePerSecond(6), hueOnBeat(25),
            idxStart(0), idxEnd(0)
        {}
        
        virtual void update(AnimationContext& ctx, long tpf, bool beat) override
        {
            t += tpf;
            
            if(beat)
                hue += hueOnBeat;
            
            float hueChange = huePerSecond * (tpf / 1000000.0f);
            hue += hueChange;
            if(hue > 255)
                hue -= 255;
            else if(hue < 0)
                hue += 255;
            
            
            //int end = (idxEnd <= 0) ? ctx.numLeds-1 : idxEnd;
            int end = (idxEnd <= 0) ? ctx.numLeds : idxEnd+1;
            
            while(t > microsBetweenSpawns) {
                t -= microsBetweenSpawns;
                
                int pos = random(idxStart, end);
                ctx.leds[pos].h = hue;
                ctx.leds[pos].s = 255;
                ctx.leds[pos].b = ctx.brightnessFactor * 255;
            }
        }
    
};