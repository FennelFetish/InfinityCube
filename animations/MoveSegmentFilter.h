#pragma once

#include "MoveFilter.h"


class MoveSegmentFilter : public MoveFilter
{
    private:
        int firstIdx;
        int lastIdx;
        
    
    public:
        MoveSegmentFilter(int firstIdx, int lastIdx, int stepsPerSecond=120, int stepsOnBeat=14) :
            MoveFilter(stepsPerSecond, stepsOnBeat),
            firstIdx(firstIdx), lastIdx(lastIdx)
        {}
        
        
        virtual void move(AnimationContext& ctx, int steps) override {
            MoveFilter::move(ctx, firstIdx, lastIdx, steps);
        }
};