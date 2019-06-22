#pragma once

#include <NeoPixelBus.h>


class Color8Bit
{
    public:
        uint8_t h; // Hue
        uint8_t s; // Saturation
        uint8_t b; // Brightness
    
        Color8Bit() : h(0), s(255), b(0) {}
};



class AnimationContext
{
    public:
        const unsigned short numLeds;
        Color8Bit* const leds;
        
        const uint8_t edgeLength;
        Color8Bit* const edge1;
        Color8Bit* const edge2;
        Color8Bit* const edge3;
    
        float brightnessFactor;
        
        long timeSinceBeat; // microseconds
        
    
        AnimationContext(int numLeds, int edgeLength) :
            numLeds(numLeds),
            leds(new Color8Bit[numLeds]),
            
            edgeLength(edgeLength),
            edge1(leds),
            edge2(edge1 + edgeLength),
            edge3(edge2 + edgeLength),
            
            brightnessFactor(1.0),
            timeSinceBeat(0)
        {}
};



class Animation
{
    public:
        Animation* next;
        
        Animation() : next(nullptr) {}
        
    
        virtual void prepare(AnimationContext& ctx) {}
    
    
        /**
         * tpf: Time per frame in microseconds
         */
        virtual void update(AnimationContext& ctx, long tpf, bool beat) = 0;
};
