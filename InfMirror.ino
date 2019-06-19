#include <NeoPixelBus.h>
#include "BassListener.h"
#include "RotaryEncoder.h"
#include "DigiPoti.h"

const int NUM_LEDS = 130;
//const int DATA_PIN = 1;   // 1 2 4 5 6 7


// TODO: Terminator Resistor against reflection on the signal line? https://forums.adafruit.com/viewtopic.php?f=25&t=52041
// TODO: Add a Big Smoothing Capacitor: https://learn.sparkfun.com/tutorials/ws2812-breakout-hookup-guide/hardware-hookup
// TODO: Add an In-Line Resistor On the Data Signal (220 Ohm) (Hat uses 330 Ohm) https://electronics.stackexchange.com/questions/177019/why-does-a-resistor-in-my-neopixel-data-line-work-at-all
// TODO: Limit power draw in software: https://github.com/FastLED/FastLED/wiki/Power-notes
//                                     https://github.com/FastLED/FastLED/wiki/FastLED-Temporal-Dithering
// Use diode to lower Vcc into first LED - Vdata must be 0.7*Vcc: http://hackaday.com/2017/01/20/cheating-at-5v-ws2812-control-to-use-a-3-3v-data-line/
// Best practices: https://learn.adafruit.com/adafruit-neopixel-uberguide/best-practices

// Use this math: https://github.com/FastLED/FastLED/wiki/High-performance-math
// (-> avoids if(red>255) conditions etc...)


uint8_t h[NUM_LEDS];
uint8_t s[NUM_LEDS];
uint8_t v[NUM_LEDS];
float t;
uint8_t hh;

long tLedUpdate = 0;
int framesSinceHit = 100000;
bool showBeat = false;

NeoPixelBus<NeoGrbFeature, NeoEsp8266Dma800KbpsMethod> leds(NUM_LEDS, 0);
BassListener bassListener(A0);
RotaryEncoder rot(D3, D2, D4); // cl, dt, sw
DigiPoti poti(D6, D5);



void setup() {
    Serial.begin (115200);
    Serial.println();
    
    poti.setup();
    
    rot.setup();
    
    // LED Brightness
    rot.setValue1(80); // 88   // 250 is too much!
    rot.setMin1(5);
    rot.setMax1(rot.getValue1()); 
    rot.setStep1(3);
    
    // Mic Sensitivity
    rot.setValue2(75);
    rot.setMin2(0);
    rot.setMax2(100);
    rot.setStep2(3);
    bassListener.setSensitivity(rot.getValue2());
    
    // this resets all the neopixels to an off state
    leds.Begin();
    leds.Show();
    
    for(int i=0; i<NUM_LEDS; ++i) {
        h[i] = 20; //((float) i / NUM_LEDS * 255) + 0.5;
        s[i] = 255;
        v[i] = 70;
    }
    
    t = 0;
    hh = 0;
}



bool apply() {
    if(!leds.CanShow())
        return false;
    
    HsbColor color;
    for(int i=0; i<NUM_LEDS; ++i) {
        /*h[i] = constrain(h[i], 0, 255);
        s[i] = constrain(s[i], 0, 255);
        v[i] = constrain(v[i], 0, 255);*/
        
        color.H = h[i] / 255.0;
        color.S = s[i] / 255.0;
        color.B = v[i] / 255.0;
        leds.SetPixelColor(i, color);
    }
    
    leds.Show();
    return true;
}


void anim1() {
    float vFactor = (framesSinceHit+1) / 1500.0f;
    vFactor = pow(vFactor, 0.33);
    
    int startIndex = int(t*200) % NUM_LEDS;
    hh = int(t*40.0f) % 255;
    
    int i=0;
    for(; i<startIndex; ++i) {
        v[i] = 1;
        h[i] = hh;
    }
    
    int len = 60; //53;
    for(int k=0; k<len; ++k, ++i) {
        v[i] = pow((float)k/len, 2.0) * 255.0f * vFactor;
        h[i] = hh;
    }
    
    for(; i<NUM_LEDS; ++i) {
        v[i] = 1;
        h[i] = hh;
    }
}


void anim2() {
    int colorVHigh = rot.getValue1(); //20; // 80, 70
    int colorVLow  = colorVHigh / 5; //3; // 20, 50
    int colorVDiff = colorVHigh - colorVLow;
    
    float vFactor = (framesSinceHit+1) / 1500.0f;
    vFactor = pow(vFactor, 0.33);
    int colorV = colorVHigh - constrain(vFactor * colorVDiff, 0, colorVDiff);
    
    for(int i=0; i<NUM_LEDS; ++i) {
        double val = 0.5 * ( 1.0 + sin(t*1.0 + (2.0 * i / NUM_LEDS * 3.1415926)) );
        h[i] = 255.0 * val;
        //h[i] = round(h[i]/30.0) * 30.0;
        //s[i] = 55.0 + 200.0 * val;
        //v[i] = 3.0 + 47.0 * val;
        //h[i] += 3;
        v[i] = colorV;
    }
}


void anim2a() {
    int colorVHigh = rot.getValue1(); //20; // 80, 70
    int colorVLow  = colorVHigh / 5; //3; // 20, 50
    int colorVDiff = colorVHigh - colorVLow;
    
    float vFactor = (framesSinceHit+1) / 1500.0f;
    vFactor = pow(vFactor, 0.33);
    int colorV = colorVHigh - constrain(vFactor * colorVDiff, 0, colorVDiff);
    
    for(int i=0; i<NUM_LEDS; ++i) {
        /*double val = sin(t);
        if(val > 0)
            h[i] = 60;
        else
            h[i] = 120;
        
        double val = 0.5 * ( 1.0 + sin(t*1.0 + (0.33 * i / NUM_LEDS * 3 * 2.0 * 3.1415926)) );*/
        double val = 0.5 * ( 1.0 + sin(t*1.0 + (2.0 * i / NUM_LEDS * 3.1415926)) );
        h[i] = 0.0 + 255.0 * val;
        //h[i] = round(h[i]/10.0) * 10.0;
        //s[i] = 55.0 + 200.0 * val;
        //v[i] = 3.0 + 47.0 * val;
        //h[i] += 3;
        v[i] = colorV;
    }
}


void anim3() {
    uint8_t h1 = int(t*40.0f) % 255;
    uint8_t h2 = h1 + 128;
    h[0] = h[1] = h[2] = h[4] = h[8] = h1;
    h[3] = h[5] = h[6] = h[7] = h[9] = h2;
}


void updateGain() {
    static const int targetGain = 512; // 300 (why 300?)
    static const int gainRange = 15;
    
    int avg = bassListener.getLastFrameAvgAmp();
    if(avg > targetGain + gainRange)
        poti.down();
    else if(avg < targetGain - gainRange)
        poti.up();
}


void updateStatusLeds() {
    static const int gainRange = 15;
    static const int colorV = 20;
    
    // Gain status
    int avg = bassListener.getLastFrameAvgAmp();

    v[0] = colorV;
    if(avg > 512 + gainRange)
        h[0] = 85; // green
    else if(avg < 512 - gainRange)
        h[0] = 0; // red
    else
        v[0] = 0; // off
    
    // Sensitivity
    h[1] = map(rot.getValue2(), 0, 100, 0, 85);
    v[1] = colorV;
    
    // Beat
    h[2] = 128;
    v[2] = showBeat ? colorV : 0;
    showBeat = false;
}



int ledUpdateInterval = 0;

void loop() {
    long t0 = micros();
    
    if(rot.update())
    {
        int rotVal = rot.getValue2();
        bassListener.setSensitivity(rotVal);
    }
    
    bool frameCompleted = bassListener.readMic();
    if(frameCompleted)
        updateGain();
    
    framesSinceHit++;
    if(bassListener.hasHit())
    {
        framesSinceHit = 0;
        t += 0.25;
        Serial.println("-------------------------");
        showBeat = true;
    }

    // 170 =~ 40fps, 222 =~ 30fps, 266 =~ 25fps, 340 = ~20fps, 400 = ~17fps
    if(ledUpdateInterval++ > 222 && leds.CanShow()) // 222
    {
        anim2();
        updateStatusLeds();
        if(apply())
            ledUpdateInterval = 0;
    }
    
    t += 0.00015;
    
    long tDiff = micros() - t0;
    long sleepTime = 150 - tDiff;
    if(sleepTime > 0)
        delayMicroseconds(sleepTime);
    else
        yield();
}
