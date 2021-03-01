#pragma once

#include "ofMain.h"
#include "ofxMaxim.h"
#include "ofxGui.h"

class AudioManager {

public:
    void setup(ofBaseApp* appPtr);
    void update();
    void draw();

    //--------------SETUPS-----------------//
    void envelopeSetup();  //set up envelopes
    void loadSamples();    //load audio samples
    void guiSetup();       //setup gui
    void clockSetup();     //setup clock
    void soundSetup(ofBaseApp* appPtr);     //setup audio

    //==============SOUNDS================//
    double playMetronome();
    double playKeyLoop();
    double playKeyLoopHi();
    double playKeyLoopLow();
    //------------------------------------//

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    //==================AUDIO=OUTPUT================//
    void audioOut(float* output, int bufferSize, int nChannels);
    //==============================================//


//====Audio-Setup====//
    //int bufferSize;
    //int sampleRate;
    //===================//


    //-----SETUP-CLOCK---//
    maxiClock clock;
    int playHead;
    //===================//


    //--MIX'S-&-OUTPUTS--//
    double mix;
    double  stereoMix[2], masterStereoOutput[2];
    //=========================================//


    //===========FX=============//
    //---------FILTER-----------//
    maxiFilter lowRes[2];
    double lowResFilterMix[2];

    //----------DELAY-----------//
    maxiDelayline delay[2];
    double delayMix[2], stereoDelayOutput[2];

    //----------REVERB-----------//
    maxiDelayline reverbsL[20], reverbsR[20];
    vector<double>reverbTimes;
    double reverbMix[2], reverbOutput[2];
    //=================================//




    //===========SAMPLES=================//

    //--SETUP-METRONOME--//
    maxiSample metronome;
    double met_out;
    //-------------//
    maxiEnv metEnv;
    double met_envOut;
    //-------------//
    int metTrigger;
    int metSeq[16] = { 1,0,0,0, 1,0,0,0,
                      1,0,0,0, 1,0,0,0 };
    //-------------//
    double metSpeed;
    double metSpeedSeq[16] = { 1.5,1,1,1.8, 1.5,1,1,1.8,
                              1.5,1,1,1,   1.5,1,1,1.8 };
    //====================//
    // [16] = 1 bar loop - 4 beats
    // [32] = 4 bar loop
    // [128] = 8 bar loop

    //---KEY-ARP-LOOP---//
    maxiSample keyArpLoop;
    double keyArp_out;
    //-------------//
    maxiEnv keyArpEnv;
    double keyArp_envOut;
    //-------------//
    int keyArpTrigger;
    int keyArpSeq[32] = { 1,0,0,0, 0,0,0,0,
                         0,0,0,0, 0,0,0,0,
                         0,0,0,0, 0,0,0,0,
                         0,0,0,0, 0,0,0,0 };

    //-------------//
    maxiMix keyArpStereo;
    double keyArpMix[2];
    //==================//


    //---KEY-LOOP-HI---//
    maxiSample keyHi;
    double keyHi_out;
    //-------------//
    maxiEnv keyHiEnv;
    double keyHi_envOut;
    //-------------//
    int keyHiTrigger;
    int keyHiSeq[64] = { 1,0,0,0, 0,0,0,0,
                        0,0,0,0, 0,0,0,0,
                        0,0,0,0, 0,0,0,0,
                        0,0,0,0, 0,0,0,0,
                        0,0,0,0, 0,0,0,0,
                        0,0,0,0, 0,0,0,0,
                        0,0,0,0, 0,0,0,0,
                        0,0,0,0, 0,0,0,0 };

    maxiMix keyHiStereo;
    double keyHiMix[2];
    //=================//


    //--KEY-LOOP-LOW-BASS-//
    maxiSample keyLow;
    double keyLow_out;
    //-------------//
    maxiEnv keyLowEnv;
    double keyLow_envOut;
    //-------------//
    int keyLowTrigger;
    int keyLowSeq[128] = { 1,0,0,0, 0,0,0,0,
                          0,0,0,0, 0,0,0,0,
                          0,0,0,0, 0,0,0,0,
                          0,0,0,0, 0,0,0,0,
                          0,0,0,0, 0,0,0,0,
                          0,0,0,0, 0,0,0,0,
                          0,0,0,0, 0,0,0,0,
                          0,0,0,0, 0,0,0,0,
                          0,0,0,0, 0,0,0,0,
                          0,0,0,0, 0,0,0,0,
                          0,0,0,0, 0,0,0,0,
                          0,0,0,0, 0,0,0,0,
                          0,0,0,0, 0,0,0,0,
                          0,0,0,0, 0,0,0,0,
                          0,0,0,0, 0,0,0,0,
                          0,0,0,0, 0,0,0,0 };

    maxiMix keyLowStereo;
    double keyLowMix[2];
    //==================//


    //----ATMOSPHERE-LOOP----//
    maxiSample atmos;
    double atmos_out;

    maxiOsc autoPanner;
    maxiMix atmosStereo;
    double atmosStereoOut[2];
    //=======================//


    //===========INTERACTIVE-SAMPLE-HITS=============//
    maxiSample hitHi;
    maxiEnv hitHiEnv;
    double hitHi_out, hitHi_envOut;
    double hitHiMix[2];
    maxiMix hitHiStereo;

    maxiSample hitMid;
    maxiEnv hitMidEnv;
    double hitMid_out, hitMid_envOut;
    double hitMidMix[2];
    maxiMix hitMidStereo;

    maxiSample hitLow;
    maxiEnv hitLowEnv;
    double hitLow_out, hitLow_envOut;
    double hitLowMix[2];
    maxiMix hitLowStereo;


    //====POLYSYNTH-EFFECT========//




    //========SYNTHESIS==============//
    maxiOsc sBass;
    double sBassOut, sBass_envOut;
    maxiEnv sBassEnv;

    maxiMix sBassStereo;
    double sBassMix[2];

    convert mtof;

    int sBassTrigger;
    int sBassLineTrigger[16] = { 1,0,0,1, 0,0,1,0, 1,0,0,1, 0,0,1,0 };

    int sBassNote;
    float sBassPitch;
    int sBassLinePitch[6] = { 51,56,63,
                           51,54,63 };

    //========================================//

    //-----------GUI-SETUP---------//
    ofxPanel gui;
    ofParameter<float> metGain;

    ofParameter<float> keyLoopGain;
    ofParameter<float> keyLoopPan;

    ofParameter<float> keyHiGain;
    ofParameter<float> keyHiPan;

    ofParameter<float> keyLowGain;
    ofParameter<float> keyLowPan;

    ofParameter<float> synthLineGain;
    ofParameter<float> synthLinePan;


    ofParameter<float> atmosGain;

    ofParameter<float> lowResFreq;
    ofParameter<float> lowResQ;

    ofParameter<float> dryDelayMix;
    ofParameter<float> delayTime;
    ofParameter<float> delayFeedback;

    ofParameter<float> dryReverbMix;
    ofParameter<float> reverbFeedback;
    //==============================//
};
