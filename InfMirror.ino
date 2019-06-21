#include <NeoPixelBus.h>

#include "BassListener.h"
#include "RotaryEncoder.h"
#include "DigiPoti.h"

#include "animations/Animation.h"
#include "animations/Rainbow.h"
#include "animations/Dot.h"
#include "animations/NodeBeam.h"
#include "animations/FadeFilter.h"
#include "animations/HueFilter.h"
#include "animations/MoveFilter.h"

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


NeoPixelBus<NeoGrbFeature, NeoEsp8266Dma800KbpsMethod> leds(NUM_LEDS, 0);

BassListener bassListener(A0);
RotaryEncoder rot(D3, D2, D4); // cl, dt, sw
DigiPoti poti(D6, D5);

AnimationContext animCtx(NUM_LEDS, 43);

// If beat was detected since last update
bool showBeat = false;
long lastLoop = 0;
long animTpf = 0;

int ledUpdateInterval = 0;
bool ledsUpdated = false;

int numAnimations = 0;
Animation** animations;



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
    
    numAnimations = 2;
    animations = new Animation*[numAnimations];
    /*animations[0] = new NodeBeam();
    animations[1] = new FadeFilter();
    animations[2] = new HueFilter();*/
    animations[0] = new Dot();
    animations[1] = new MoveFilter();
}



bool apply() {
    if(!leds.CanShow())
        return false;
    
    HsbColor color;
    for(int i=0; i<NUM_LEDS; ++i) {
        /*h[i] = constrain(h[i], 0, 255);
        s[i] = constrain(s[i], 0, 255);
        v[i] = constrain(v[i], 0, 255);*/
        
        color.H = animCtx.leds[i].h / 255.0f;
        color.S = animCtx.leds[i].s / 255.0f;
        color.B = animCtx.leds[i].b / 255.0f;
        leds.SetPixelColor(i, color);
    }
    
    leds.Show();
    return true;
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

    animCtx.leds[0].b = colorV;
    if(avg > 512 + gainRange)
        animCtx.leds[0].h = 85; // green
    else if(avg < 512 - gainRange)
        animCtx.leds[0].h = 0; // red
    else
        animCtx.leds[0].b = 0; // off
    
    // Sensitivity
    animCtx.leds[1].h = map(rot.getValue2(), 0, 100, 0, 85);
    animCtx.leds[1].b = colorV;
    
    // Beat
    animCtx.leds[2].h = 128;
    animCtx.leds[2].b = showBeat ? colorV : 0;
}





void loop() {
    long t0 = micros();
    long loopTpf = t0 - lastLoop;
    lastLoop = t0;
    animTpf += loopTpf;
    animCtx.timeSinceBeat += loopTpf;
    
    if(rot.update())
    {
        animCtx.brightnessFactor = rot.getValue1() / 255.0;
        bassListener.setSensitivity( rot.getValue2() );
    }
    
    bool frameCompleted = bassListener.readMic();
    if(frameCompleted)
        updateGain();
    
    if(bassListener.hasHit())
    {
        showBeat = true;
        animCtx.timeSinceBeat = 0;
        //Serial.println("-------------------------");
    }

    // 170 =~ 40fps, 222 =~ 30fps, 266 =~ 25fps, 340 = ~20fps, 400 = ~17fps   ---> outdated values (they are calculated based on 150 mics frametime)
    if(ledUpdateInterval++ > 130 && leds.CanShow())
    {
        if(!ledsUpdated)
        {
            for(int i=0; i<numAnimations; ++i)
                animations[i]->prepare(animCtx);
            
            for(int i=0; i<numAnimations; ++i)
                animations[i]->update(animCtx, animTpf, showBeat);
            
            //updateStatusLeds();
            showBeat = false;
            animTpf = 0;
            ledsUpdated = true;
        }
        
        if(apply()) {
            ledUpdateInterval = 0;
            ledsUpdated = false;
        }
    }
    

    long tDiff = micros() - t0;
    
    //Serial.print("loop tDiff: ");
    //Serial.println(tDiff);
    
    long sleepTime = 250 - tDiff; // 150
    if(sleepTime > 0)
        delayMicroseconds(sleepTime);
    else
        yield();
}
