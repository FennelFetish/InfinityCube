#include "AnimationChanger.h"

#include "animations/Rainbow.h"
#include "animations/Dot.h"
#include "animations/Stars.h"
#include "animations/Strobo.h"
#include "animations/NodeBeam.h"
#include "animations/KnightRider.h"
#include "animations/SegmentPulse.h"

#include "animations/FadeFilter.h"
#include "animations/HueFilter.h"
#include "animations/HueOffsetFilter.h"
#include "animations/MoveFilter.h"
#include "animations/MoveConvergeFilter.h"
#include "animations/MoveSegmentFilter.h"
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
    //type = AnimationType::StarsMoving;
    
    //Serial.print("Changing animation to: ");
    //Serial.println(type);
    
    deleteAnimations();
    
    AnimProperties props;
    
    switch(type) {
        case AnimationType::Rainbow:
            addAnimation( new AnimRainbow() );
            break;
            
        case AnimationType::Stars:
            addAnimation( new Stars(220) );
            addAnimation( new FadeFilter(6) );
            addAnimation( new MoveFilter(0, 3) );
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
            
        case AnimationType::DotExplode: {
            Dot* dot1 = new Dot();
            dot1->setDotIndex(animCtx.edgeLength-1);
            addAnimation(dot1);
            addAnimation( new MoveSegmentFilter(0, animCtx.edgeLength-1, -100, 0) );
            
            Dot* dot2 = new Dot();
            dot2->setDotIndex(animCtx.edgeLength);
            addAnimation(dot2);
            addAnimation( new MoveSegmentFilter(animCtx.edgeLength, animCtx.numLeds-1, 100, 0) );
            
            addAnimation( new FeedbackFilter(animCtx, 0.18) );
            addAnimation( new HueFilter() );
            addAnimation( new FadeFilter(1) );
            break;
        }
            
        case AnimationType::KnightRider:
            addAnimation( new Stars(80) );
            addAnimation( new FadeFilter(20) );
            addAnimation( new KnightRider() );
            addAnimation( new HueFilter() );
            break;
            
        case AnimationType::KnightRiderFull: {
            addAnimation( new Stars(80) );
            addAnimation( new FadeFilter(20) );
            KnightRider* knightRider = new KnightRider(520, 36);
            knightRider->fullStrip = true;
            addAnimation(knightRider);
            addAnimation( new HueFilter() );
            break;
        }
            
        case AnimationType::SegmentPulse:
            addAnimation( new Stars(40) );
            addAnimation( new SegmentPulse() );
            addAnimation( new FadeFilter(10) );
            addAnimation( new HueFilter(8,43) );
            break;
            
        case AnimationType::SegmentStay: {
            SegmentPulse* segmentPulse = new SegmentPulse();
            segmentPulse->setRandomizeColor(true);
            addAnimation( segmentPulse );
            addAnimation( new FadeFilter(2) );
            addAnimation( new HueOffsetFilter(110,127) );
            break;
        }
            
        case AnimationType::ColorConverge: {
            Stars* stars1 = new Stars(20); // 20
            stars1->idxStart = animCtx.numLeds - 8;
            stars1->hueOnBeat = 0;//20;
            addAnimation(stars1);
            
            Stars* stars2 = new Stars(20, 128);
            stars2->idxEnd = 8;
            stars2->hueOnBeat = 0;//20;
            addAnimation(stars2);
            
            addAnimation( new FadeFilter(1) );
            addAnimation( new MoveConvergeFilter(9, 6) );
            addAnimation( new HueOffsetFilter(0,23) );
            
            KnightRider* knightRider = new KnightRider(520, 27);
            knightRider->fullStrip = true;
            knightRider->saturation = true;
            addAnimation(knightRider);
            
            addAnimation( new FadeFilter(70, true) );
        
            break;
        }
    }
    
    return props;
}



AnimProperties AnimationChanger::createRandom(AnimationContext& animCtx) {
    AnimationType::Enum type = (AnimationType::Enum) random(0, AnimationType::COUNT);
    return create(animCtx, type);
}
