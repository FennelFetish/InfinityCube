#pragma once

#include "../Animation.h"


class KnightRider : public Animation
{
    private:
        enum State { Forward, Backward, Waiting };
    
        uint8_t beamLen;
        uint8_t pos;
        State state;
        State lastState;
        
        int stepsPerSecond;
        int microsPerStep;
        long t;
        
        
        void applyToEdge(AnimationContext& ctx, Color8Bit* const edge) {
            for(int i=0; i<beamLen; ++i) {
                edge[i+pos].b = ctx.brightnessFactor * 255;
            }
        }
        
        
        void changeState(State newState) {
            lastState = state;
            state = newState;
        }
        
    
    public:
        KnightRider(int stepsPerSecond=220) :
            beamLen(8), pos(0), state(Forward), lastState(Waiting),
            stepsPerSecond(stepsPerSecond), microsPerStep(1000000/stepsPerSecond), t(0)
        {}
        
        
        virtual void update(AnimationContext& ctx, long tpf, bool beat) override {
            if(state == Waiting) {
                if(!beat)
                    return;
                
                changeState(lastState == Forward ? Backward : Forward);
                t = microsPerStep;
            }
            
            t += tpf;
            
            while(t > microsPerStep) {
                t -= microsPerStep;
                
                if(state == Forward) {
                    pos += 1;
                    if(pos >= ctx.edgeLength-beamLen) {
                        changeState(Waiting);
                        break;
                    }
                }
                else if(state == Backward) {
                    pos -= 1;
                    if(pos == 0) {
                        changeState(Waiting);
                        break;
                    }
                }
            }
            
            applyToEdge(ctx, ctx.edge1);
            applyToEdge(ctx, ctx.edge2);
            applyToEdge(ctx, ctx.edge3);
        }
};