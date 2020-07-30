#pragma once

#include "../Animation.h"


class Caterpillar : public Animation
{
    private:
        float speed = 1.9f;
        float sinSpeed = speed * (2.0f * 3.1415926f) / 1000000.0f;
    
        int hueStart = 120;
        int hueEnd = 60;
        int hueDiff = hueEnd-hueStart;
        
        int huePerSecond = 30;
        int hueOnBeat = 0;
        
        static constexpr int maxLen = 51;
        static constexpr int minLen = 31;
        static constexpr int diffLen = maxLen-minLen;
        
        int len = maxLen;
        int lenHalf = len/2;
        int lastLen = len;
    
        int offset;
        
        unsigned long time;
        //long timeSinceBeat;
        long lastBeat;
        
        static float absf(float f) {
            return (f < 0) ? -f : f;
        }
        
        int getHue(int pos) {
            pos -= offset * ((float)len / maxLen);
            pos %= (len+1);
            float t = (float) (pos-lenHalf) / lenHalf;
            return hueEnd - absf(t)*hueDiff;
        }
        
        
        void updateHue(long tpf, bool beat) {
            float hueChange = huePerSecond * (tpf / 1000000.0f);
            if(beat)
                hueChange += hueOnBeat;
            
            hueStart += hueChange;
            hueStart %= 256;
            
            hueEnd += hueChange;
            hueEnd %= 256;
            
            hueDiff = hueEnd-hueStart;
        }
        
        
        void updateLength() {
            float t = time * sinSpeed;
            float s = (sin(t)+1.0f) * 0.5f;
            len = minLen + diffLen*s;
            lenHalf = len/2;
            
            if(len < lastLen)
                offset += 1 * (lastLen - len);
            lastLen = len;
        }
        
        
    public:
        Caterpillar() : offset(0), time(0), lastBeat(0)/*, timeSinceBeat(0)*/ {}
        
        
        virtual void update(AnimationContext& ctx, long tpf, bool beat) override {

                
            
            /*if(beat)
                timeSinceBeat = 0;
            timeSinceBeat += tpf;*/
            //if(ctx.timeSinceBeat < (500000 / speed))
                time += tpf;
            
            if(beat) {
                long tDiff = time - lastBeat;
                speed = tDiff / 2000000.0f;
                lastBeat = time;
            }
            
            updateHue(tpf, beat);
            updateLength();
            
            int b = ctx.brightnessFactor * 255;
            int i = 0;
            for(; i<ctx.numLeds; ++i) {
                ctx.leds[i].b = b;
                ctx.leds[i].h = getHue(i);
            }
            
            
            
            
            
            /*tSinceBeat += tpf;
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
            */
        }
    
};