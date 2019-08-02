#include "BeatButton.h"
#include <arduino.h>

#define PRESS_MAX_GAP 1000000


void BeatButton::update(long now, long tpf)
{
    // Button
    if(rot.getSwitchState()) {
        if(!swState) {
            swState = true;
            hit = true;
            
            doPress(now);
        }
    }
    else {
        swState = false;
    }
    
    // Auto beat
    if(tAutoBeat > 0)
        tAutoBeat -= tpf;
    
    if(tAutoBeat <= 0 && microsPerBeat > 0) {
        hit = true;
        tAutoBeat = microsPerBeat;
    }
}


void BeatButton::doPress(long now)
{
    tAutoBeat = 30000000;
    
    // Increase index first so we can access last press with current index
    idxLastPress++;
    if(idxLastPress >= NUM_PRESSES)
        idxLastPress = 0;
    
    lastPresses[idxLastPress] = now;

    
    // Update bpm, find presses that count
    int idx = idxLastPress;
    int numPresses = 0;
    long firstPress = now;
    
    for(int i=0; i<NUM_PRESSES; i++) {
        if(lastPresses[idx] >= firstPress - PRESS_MAX_GAP) {
            numPresses++;
            firstPress = lastPresses[idx];
        }
        else
            break;
        
        idx--;
        if(idx < 0)
            idx = NUM_PRESSES - 1;
    }
    
    if(numPresses < 8) {
        microsPerBeat = 0;
        return;
    }
    
    long tDiff = now - firstPress;
    microsPerBeat = tDiff / (numPresses-1);
    
    Serial.print("-- tDiff: ");
    Serial.print(tDiff);
    Serial.print(", numPresses: ");
    Serial.print(numPresses);
    Serial.print(", microsPerBeat: ");
    Serial.println(microsPerBeat);
    
    tAutoBeat = 0; 
}