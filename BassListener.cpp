#include "BassListener.h"
#include <Arduino.h>

//#define PRINT_AMP
//#define PRINT_RANGE


namespace
{
    const int FRAME_SIZE = 170; //180; //220; //200; //220;
    const int HIT_COOLDOWN = 4;
    const int AMP_THRESHOLD = 9;
}



BassListener::BassListener(int micPin) :
    pin(micPin),
    count(0), frameAvg(0), frameMin(1025), frameMax(-1), frameStart(micros()), lastFrameAvg(0),
    idxAmpBuffer(0),
    hitCooldown(0), hit(false),
    sensitivity(0.75f)
{
    setSensitivity(50);
}


// Returns true when frame was completed
bool BassListener::readMic()
{
    hit = false;
    int val = 1024 - analogRead(pin);
    frameAvg += val;
    
    if(val > frameMax)
        frameMax = val;
    if(val < frameMin)
        frameMin = val;

    if(count++ < FRAME_SIZE)
        return false;
    count = 0;
    
    /*long now = micros();
    long frameTime = now - frameStart;
    frameStart = now;
    Serial.print("frame took ");
    Serial.println(frameTime);*/
    
    int amp = completeFrame();
    ampBufferWrite(amp);
    
    int minAmp, maxAmp, avgAmp;
    ampBufferEval(&minAmp, &maxAmp, &avgAmp);
    
    hit = evalHit(amp, minAmp, maxAmp, avgAmp);
    return true;
}


int BassListener::completeFrame()
{
    frameAvg /= FRAME_SIZE;
    lastFrameAvg = frameAvg;
    int amp = frameMax - frameMin;

#ifdef PRINT_AMP
    for(int i=0; i < amp; i += 3)
        Serial.print(".");

    Serial.print(" amp: ");
    Serial.print(amp);
    Serial.print(", min/max: ");
    Serial.print(frameMin);
    Serial.print("/");
    Serial.print(frameMax);
    Serial.print(", avg: ");
    Serial.println(frameAvg);
#endif

    frameAvg = 0;
    frameMin = 1025;
    frameMax = -1;

    // TODO: Needed?
    static const int ampLimit = 300; // 150
    if(amp > ampLimit) // 150
        amp = ampLimit;
    
    return amp;
}


void BassListener::ampBufferWrite(int amp)
{
    ampBuffer[idxAmpBuffer] = amp;
    
    idxAmpBuffer++;
    if(idxAmpBuffer >= AMPBUFFER_SIZE)
        idxAmpBuffer = 0;  
}


void BassListener::ampBufferEval(int* outMinAmp, int* outMaxAmp, int* outAvgAmp) const
{
    int minAmp = 1025;
    int maxAmp = 0;
    int avgAmp = 0;
    
    for(int i=0; i<AMPBUFFER_SIZE; ++i)
    {
        if(ampBuffer[i] < minAmp)
            minAmp = ampBuffer[i];
        if(ampBuffer[i] > maxAmp)
            maxAmp = ampBuffer[i];

        avgAmp += ampBuffer[i];
    }

    avgAmp /= AMPBUFFER_SIZE;
    
    *outMinAmp = minAmp;
    *outMaxAmp = maxAmp;
    *outAvgAmp = avgAmp;
}


bool BassListener::evalHit(int amp, int minAmp, int maxAmp, int avgAmp)
{
    //int threshold = ceil(0.3f * (maxAmp-minAmp));
    //int threshold = avgAmp * 1.2f; //9; //avgAmp - minAmp; //minAmp + ceil(0.3f * (maxAmp-minAmp));
    int threshold = avgAmp + (maxAmp-avgAmp) * 0.2f; //0.5f; //0.65f;
    if(threshold < AMP_THRESHOLD)
        threshold = AMP_THRESHOLD;
    
#ifdef PRINT_RANGE
    Serial.print("min/max/avg (thresh) amp: ");
    Serial.print(minAmp);
    Serial.print(" / ");
    Serial.print(maxAmp);
    Serial.print(" / ");
    Serial.print(avgAmp);
    Serial.print(" (");
    Serial.print(threshold);
    Serial.println(")");
#endif
    
    maxAmp -= threshold;
    if(maxAmp < 4)
        maxAmp = 4;

    amp -= threshold;
    if(amp < 0)
        amp = 0;

    float intensity = (float) amp / maxAmp;
    bool hit = intensity > sensitivity;

    if(hitCooldown > 0)
    {
        hit = false;
        hitCooldown--;
    }
    else if(hit)
        hitCooldown = HIT_COOLDOWN;

    return hit;
}



void BassListener::setSensitivity(int sens)
{
    if(sens > 100)
        sens = 100;
    else if(sens < 0)
        sens = 0;
    
    sensitivity = 1.0f - 0.99f * (sens / 100.0f);
    Serial.print("sensitivity set to ");
    Serial.println(sensitivity);
}