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
            if(saturation) {
                for(int i=0; i<beamLen; ++i) {
                    edge[i+pos].s = minSaturation;
                }
            } else {
                for(int i=0; i<beamLen; ++i) {
                    edge[i+pos].b = ctx.brightnessFactor * 255;
                }
            }
        }
        
        
        void changeState(State newState) {
            lastState = state;
            state = newState;
        }
        
    
    public:
        bool fullStrip;
        bool saturation;
        uint8_t minSaturation;
        
    
        KnightRider(int stepsPerSecond=220, uint8_t beamLen=8) :
            fullStrip(false), beamLen(beamLen), pos(0), state(Forward), lastState(Waiting),
            stepsPerSecond(stepsPerSecond), microsPerStep(1000000/stepsPerSecond), t(0),
            saturation(false), minSaturation(40)
        {}
        
        
        virtual void update(AnimationContext& ctx, long tpf, bool beat) override {
            if(state == Waiting) {
                if(!beat)
                    return;
                
                changeState(lastState == Forward ? Backward : Forward);
                t = microsPerStep;
            }
            
            t += tpf;
            
            int edgeLength = fullStrip ? ctx.numLeds : ctx.edgeLength;
            while(t > microsPerStep) {
                t -= microsPerStep;
                
                if(state == Forward) {
                    pos += 1;
                    if(pos >= edgeLength-beamLen) {
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
            
            if(fullStrip) {
                applyToEdge(ctx, ctx.leds);
            } else {
                applyToEdge(ctx, ctx.edge1);
                applyToEdge(ctx, ctx.edge2);
                applyToEdge(ctx, ctx.edge3);
            }
        }
};