#pragma once

#include <NeoPixelBus.h>


class Color8Bit
{
    public:
        uint8_t h; // Hue
        uint8_t s; // Saturation
        uint8_t b; // Brightness
    
        Color8Bit() : h(0), s(0), b(0) {}
};



class AnimationContext
{
    public:
        const unsigned short numLeds;
        Color8Bit* const leds;
    
        float brightnessFactor;
        
        long timeSinceBeat; // microseconds
        
    
        AnimationContext(int numLeds) :
            numLeds(numLeds),
            leds(new Color8Bit[numLeds]),
            brightnessFactor(1.0),
            timeSinceBeat(0)
        {}
};



class Animation
{
    public:
        /**
         * tpf: Time per frame in microseconds
         */
        virtual void update(AnimationContext& ctx, long tpf, bool beat) = 0;
};
