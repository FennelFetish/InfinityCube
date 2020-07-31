#pragma once

#include "../Animation.h"
#include "../SimplexNoise.h"


class Noise : public Animation
{
    private:
        SimplexNoise noise;
        float x;
        float y;
        float hueOffset;
        double time;
        float bFactor;
        static constexpr float minBFactor = 0.0f;
        

        float getNoise(int pos) {
            static const float xScale = 0.07f;
            pos += x;
            return noise.fractal(2, pos * xScale, y);
        }

        
    
    public:
        Noise() : x(0), y(0), hueOffset(0), time(0), bFactor(minBFactor)
        {
            x += random(0, 500) / 10.0f;
            y += random(0, 500) / 10.0f;
        }
        

        virtual void update(AnimationContext& ctx, long tpf, bool beat) override {
            time += tpf / 1000000.0;
            //hueOffset = 40 + sin(0.3f * time)*90; // -50 .. +130
            hueOffset = 30 + sin(0.25 * time)*100; // -70 .. +130
            
            float sec = tpf / 1000000.0f;
            //x -= 3.5f * sec;
            y += 0.5f * sec; // 0.25
            
            //bFactor -= sec * 2.0f;
            bFactor -= sec * 1.0f;
            bFactor *= 0.8f;
            bFactor = constrain(bFactor, minBFactor, 1.0f);
            
            if(beat) {
                time += 0.08;
                //x -= 2.0f;
                bFactor = 1.0f;
            }
            
            for(int i=0; i<ctx.numLeds; ++i) {
                float n = getNoise(i);
                float b = ((n+bFactor) / (1.0f+bFactor)) * 255.0f * ctx.brightnessFactor;
                //float h = ((n+1.0f) * 0.5f) * 90 + hueOffset;
                float h = ((n+1.0f) * 0.5f) * 100 + hueOffset;
                
                ctx.leds[i].b = constrain(b, 0, 255);
                ctx.leds[i].h = constrain(h, 0, 255);
            }
        }
    
};