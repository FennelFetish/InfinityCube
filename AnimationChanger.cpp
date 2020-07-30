#include "AnimationChanger.h"

#include "animations/Rainbow.h"
#include "animations/Dot.h"
#include "animations/Stars.h"
#include "animations/Strobo.h"
#include "animations/NodeBeam.h"
#include "animations/KnightRider.h"
#include "animations/SegmentPulse.h"
#include "animations/Caterpillar.h"
#include "animations/Noise.h"

#include "animations/FadeFilter.h"
#include "animations/HueFilter.h"
#include "animations/HueOffsetFilter.h"
#include "animations/MoveFilter.h"
#include "animations/MoveSegmentFilter.h"
#include "animations/FeedbackFilter.h"
#include "animations/RandomizationFilter.h"


AnimationChanger::AnimationChanger() :
    animation(nullptr), tRemaining( random(minDuration, maxDuration) ), historyIndex(0)
{
    for(int i=0; i<historySize; ++i)
        history[i] = -1;
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
        
        if(props.clearOnStart) {
            for(int i=0; i<animCtx.numLeds; ++i)
                animCtx.leds[i].b = 0;
        }
    }
}


AnimProperties AnimationChanger::create(AnimationContext& animCtx, AnimationType::Enum type) {
    //type = AnimationType::DotExplode;
    
    //Serial.print("Changing animation to: ");
    //Serial.println(type);
    
    deleteAnimations();
    AnimProperties props;
    
    switch(type) {
        case AnimationType::Rainbow:
            addAnimation( new AnimRainbow() );
            break;
            
        case AnimationType::Stars: {
            addAnimation( new Stars(220) );
            addAnimation( new FadeFilter(6) );
            addAnimation( new MoveFilter(0, 3) );
            break;
        }
            
        case AnimationType::StarsMoving:
            addAnimation( new Stars(100) );
            addAnimation( new FadeFilter(10) );
            addAnimation( new MoveFilter(11, 3) );
            break;
            
        case AnimationType::Strobo:
            addAnimation( new Strobo() );
            addAnimation( new HueFilter() );
            props.durationFactor = 0.33f;
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
            addAnimation( new MoveSegmentFilter(0, animCtx.edgeLength-1, -90, 0) );
            
            Dot* dot2 = new Dot();
            dot2->setDotIndex(animCtx.edgeLength);
            addAnimation(dot2);
            addAnimation( new MoveSegmentFilter(animCtx.edgeLength, animCtx.numLeds-1, 90, 0) );
            
            addAnimation( new FeedbackFilter(animCtx, 0.26) );
            addAnimation( new HueFilter() );
            addAnimation( new FadeFilter(7) );
            
            props.clearOnStart = true;
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
            
        case AnimationType::SegmentPulse: {
            SegmentPulse* segPulse = new SegmentPulse();
            segPulse->setRandomHalfChance(25);
        
            addAnimation( new Stars(40) );
            addAnimation( segPulse );
            addAnimation( new FadeFilter(10) );
            addAnimation( new HueFilter(8, 43) );
            break;
        }
            
        case AnimationType::SegmentStay: {
            SegmentPulse* segPulse = new SegmentPulse();
            segPulse->setRandomizeColor(true);
            
            addAnimation( segPulse );
            addAnimation( new FadeFilter(2) );
            addAnimation( new HueOffsetFilter(110, 127) );
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
            
            int half = animCtx.numLeds / 2;
            addAnimation( new MoveSegmentFilter(0, half, 9, 6) );
            addAnimation( new MoveSegmentFilter(half+1, animCtx.numLeds-1, -9, -6) );
            
            addAnimation( new HueOffsetFilter(0, 23) );
            
            KnightRider* knightRider = new KnightRider(520, 27);
            knightRider->fullStrip = true;
            knightRider->saturation = true;
            addAnimation(knightRider);
            
            addAnimation( new FadeFilter(70, true) );
            break;
        }
        
        case AnimationType::Caterpillar: {
            addAnimation( new Caterpillar() );
            //props.durationFactor = 33.33f;
            break;
        }
        
        case AnimationType::Noise: {
            addAnimation( new Noise() );
            break;
        }
    }
    
    return props;
}


bool AnimationChanger::checkHistory(int animIdx) {
    for(int i=0; i<historySize; ++i) {
        if(history[i] == animIdx)
            return true;
    }
    
    return false;
}


void AnimationChanger::addHistory(int animIdx) {
    history[historyIndex] = animIdx;
    historyIndex++;
    if(historyIndex >= historySize)
        historyIndex = 0;
}


AnimProperties AnimationChanger::createRandom(AnimationContext& animCtx) {
    int animIdx = random(0, AnimationType::COUNT);
    if(checkHistory(animIdx)) // Only retry once
        animIdx = random(0, AnimationType::COUNT);
        
    addHistory(animIdx);
    
    AnimationType::Enum type = (AnimationType::Enum) animIdx;
    return create(animCtx, type);
}
