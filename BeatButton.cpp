#include "BeatButton.h"
#include <arduino.h>

#define PRESS_MAX_GAP 1000000


void BeatButton::update(long now, long tpf) {
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
    
    int tempoAdjust = rot.getValue2() - lastTempoAdjust;
    if(tempoAdjust != 0) {
        microsPerBeat -= tempoAdjust * 250; // 0.25 ms
        /*Serial.print("Tempo adjusted by ");
        Serial.print(tempoAdjust);
        Serial.print("ms to ");
        Serial.println(microsPerBeat);*/
        
        lastTempoAdjust = rot.getValue2();
        tAutoBeat = 0;
    }
    
    // Auto beat
    //if(tAutoBeat > 0)
        tAutoBeat -= tpf;
    
    if(tAutoBeat <= 0 && microsPerBeat > 0) {
        hit = true;
        //tAutoBeat = microsPerBeat;
        tAutoBeat += microsPerBeat;
    }
}


void BeatButton::doPress(long now) {
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
    
    if(numPresses < 4) {
        microsPerBeat = 0;
        return;
    }
    
    long tDiff = now - firstPress;
    microsPerBeat = tDiff / (numPresses-1);
    
    /*Serial.print("-- tDiff: ");
    Serial.print(tDiff);
    Serial.print(", numPresses: ");
    Serial.print(numPresses);
    Serial.print(", microsPerBeat: ");
    Serial.println(microsPerBeat);*/
    
    tAutoBeat = 0; 
}