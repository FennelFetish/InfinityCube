#pragma once

class BassListener
{
    public:
        BassListener(int micPin);
        
        bool readMic();
        bool hasHit() const { return hit; }
        void setSensitivity(int sens);
        
        int getLastFrameAvgAmp() const { return lastFrameAvg; }
    
    
    private:
        const int pin;
        
        int count;
        int frameAvg;
        int frameMin;
        int frameMax;
        long frameStart;
        int lastFrameAvg;
        
        static const int AMPBUFFER_SIZE = 50; //12; //15;
        int idxAmpBuffer;
        int ampBuffer[AMPBUFFER_SIZE];
        
        int hitCooldown;
        bool hit;
        
        float sensitivity;
        
        
        int completeFrame();
        
        void ampBufferWrite(int amp);
        void ampBufferEval(int* minAmp, int* maxAmp, int* avgAmp) const;
    
        bool evalHit(int amp, int minAmp, int maxAmp, int avgAmp);
};