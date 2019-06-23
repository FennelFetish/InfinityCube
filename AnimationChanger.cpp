#include "AnimationChanger.h"

#include "animations/Rainbow.h"
#include "animations/Dot.h"
#include "animations/Stars.h"
#include "animations/Strobo.h"
#include "animations/NodeBeam.h"
#include "animations/KnightRider.h"

#include "animations/FadeFilter.h"
#include "animations/HueFilter.h"
#include "animations/MoveFilter.h"
#include "animations/FeedbackFilter.h"
#include "animations/RandomizationFilter.h"


AnimationChanger::AnimationChanger() :
    animation(nullptr), tRemaining( random(minDuration, maxDuration) )
{
    
}


void AnimationChanger::addAnimation(Animation* anim) {
    if(animation == nullptr) {
        animation = anim;
        return;
    }
    
    Animation* a = animation;
    while(a->next != nullptr)
        a = a->next;
    
    a->next = anim;
}


void AnimationChanger::deleteAnimations() {
    while(animation != nullptr) {
        Animation* nextAnim = animation->next;
        delete animation;
        animation = nextAnim;
    }
}


void AnimationChanger::update(AnimationContext& animCtx, long tpf, bool beat) {
    tRemaining -= tpf;
    if(beat && tRemaining <= 0) {
        AnimProperties props = createRandom(animCtx);
        tRemaining = random(minDuration, maxDuration) * props.durationFactor;
    }
}


AnimProperties AnimationChanger::create(AnimationContext& animCtx, AnimationType::Enum type) {
    //type = AnimationType::KnightRider;
    
    Serial.print("Changing animation to: ");
    Serial.println(type);
    
    deleteAnimations();
    
    AnimProperties props;
    
    switch(type) {
        case AnimationType::Rainbow:
            addAnimation( new AnimRainbow() );
            break;
            
        case AnimationType::Stars:
            addAnimation( new Stars(220) );
            addAnimation( new FadeFilter(6) );
            addAnimation( new MoveFilter(0, 2) );
            break;
            
        case AnimationType::StarsMoving:
            addAnimation( new Stars(100) );
            addAnimation( new FadeFilter(10) );
            addAnimation( new MoveFilter(11, 3) );
            break;
            
        case AnimationType::Strobo:
            addAnimation( new Strobo() );
            addAnimation( new HueFilter() );
            props.durationFactor = 0.5f;
            break;
            
        case AnimationType::NodeBeam:
            addAnimation( new NodeBeam() );
            addAnimation( new HueFilter() );
            addAnimation( new FadeFilter(20) );
            //addAnimation( new RandomizationFilter(animCtx) );
            break;
            
        case AnimationType::Dot:
            addAnimation( new Dot() );
            addAnimation( new MoveFilter(60, 0) );
            addAnimation( new FeedbackFilter(animCtx, 0.12) );
            addAnimation( new HueFilter() );
            //addAnimation( new FadeFilter(40) );
            break;
            
        case AnimationType::KnightRider:
            addAnimation( new Stars(80) );
            addAnimation( new FadeFilter(20) );
            addAnimation( new KnightRider() );
            addAnimation( new HueFilter() );
            //addAnimation( new FeedbackFilter(animCtx, 0.6) );
            //addAnimation( new FadeFilter(40) );
            break;
    }
    
    return props;
}



AnimProperties AnimationChanger::createRandom(AnimationContext& animCtx) {
    AnimationType::Enum type = (AnimationType::Enum) random(0, AnimationType::COUNT);
    return create(animCtx, type);
}
