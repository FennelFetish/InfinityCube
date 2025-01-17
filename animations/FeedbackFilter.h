#pragma once

#include "../Animation.h"

class FeedbackFilter : public Animation
{
    private:
        Color8Bit* const copy;
        float feedback;
    
    public:
        FeedbackFilter(AnimationContext& ctx, float feedback) :
            copy(new Color8Bit[ctx.numLeds]), feedback(feedback)
        {}
        
        ~FeedbackFilter() {
            delete[] copy;
        }
        
        
        virtual void prepare(AnimationContext& ctx) {
            for(int i=0; i<ctx.numLeds; ++i) {
                copy[i] = ctx.leds[i];
            }
        }
        
        
        virtual void update(AnimationContext& ctx, long tpf, bool beat) override {
            for(int i=0; i<ctx.numLeds; ++i) {
                int val = ctx.leds[i].b + (copy[i].b * feedback);
                ctx.leds[i].b = constrain(val, 0, 255);
            }
        }
    
};