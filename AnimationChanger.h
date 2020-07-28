#pragma once

#include "Animation.h"


namespace AnimationType {
    enum Enum {
        Rainbow,
        Stars,
        StarsMoving,
        Strobo,
        NodeBeam,
        Dot,
        KnightRider,
        KnightRiderFull,
        SegmentPulse,
        SegmentStay,
        ColorConverge,
        COUNT
    };
}



struct AnimProperties {
    float durationFactor;
    
    AnimProperties() : 
        durationFactor(1.0f)
    {}
};



class AnimationChanger
{
    private:
        static constexpr long minDuration = 3000000;
        static constexpr long maxDuration = 20000000;
        long tRemaining;
    
        void addAnimation(Animation* anim);
        void deleteAnimations();


    public:
        Animation* animation;
        
    
        AnimationChanger();
        
        void update(AnimationContext& animCtx, long tpf, bool beat);
    
        AnimProperties create(AnimationContext& animCtx, AnimationType::Enum type);
        AnimProperties createRandom(AnimationContext& animCtx);
};
