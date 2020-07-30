#pragma once

#include "Animation.h"


namespace AnimationType {
    enum Enum {
        Rainbow,
        Stars,
        StarsMoving,
        Strobo,
        NodeBeam,
        DotExplode,
        KnightRider,
        KnightRiderFull,
        SegmentPulse,
        SegmentStay,
        ColorConverge,
        Caterpillar,
        Noise,
        COUNT
    };
}



struct AnimProperties {
    float durationFactor;
    bool clearOnStart;
    
    AnimProperties() : 
        durationFactor(1.0f), clearOnStart(false)
    {}
};



class AnimationChanger
{
    private:
        static constexpr long minDuration = 3000000;
        static constexpr long maxDuration = 20000000;
        long tRemaining;
        
        static constexpr int historySize = 3;
        int history[historySize];
        int historyIndex;
        
        bool checkHistory(int animIdx);
        void addHistory(int animIdx);
        
        void addAnimation(Animation* anim);
        void deleteAnimations();


    public:
        Animation* animation;
        
    
        AnimationChanger();
        
        void update(AnimationContext& animCtx, long tpf, bool beat);
    
        AnimProperties create(AnimationContext& animCtx, AnimationType::Enum type);
        AnimProperties createRandom(AnimationContext& animCtx);
};
