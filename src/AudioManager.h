#pragma once

#include "ofMain.h"
#include "ofxMaxim.h"
#include "ofxGui.h"

#include "Controller.h" // <--- for global world dimensions

class AudioManager {

public:
    ~AudioManager();

    void setup(ofBaseApp* appPtr);
    void update(ofVec2f player_position);
    void draw();
    void drawGUI(bool enable);
    void drawWaveform();

    //--------------SETUPS-----------------//
    void envelopeSetup();                  //set up envelopes
    void loadSamples();                    //load audio samples
    void guiSetup();                       //setup gui
    void clockSetup();                     //setup clock
    void soundSetup(ofBaseApp* appPtr);    //setup audio


    //==============SOUNDS================//
    double playMetronome();
    void playKeyLoop();
    void playKeyLoopHi();
    void playKeyLoopLow();
    void sampleHits();
    void beats();
    void charNoise();
    void synthLine();
    void polySynth();
    void sawSqSynth();
    void subBass();
    //------------------------------------//

    //used to trigger random samples when
    void playRandomSample();
    bool randomSampleTriggered = false;
    int r = 0;

    void keyPressed(int key);
    void keyReleased(int key);

    // ----------> EVENT FUNCTIONS <---------- //
    void event_point_collected();
    void event_point_pulsed(ofVec2f point_position);
    void event_level_complete();
    void event_new_level_loaded();

    // the player pos starts at (0, 0) and ranges from (-HALF_WORLD_WIDTH, HALF_WORLD_WIDTH) horizontally, and (-HALF_WORLD_HEIGHT, HALF_WORLD_HEIGHT) vertically
    ofVec2f player_pos;
    // -------------------------------------------- //

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
    int bufferSize;
    int sampleRate;

    // used for drawing waveform
    float soundBuffer[512];
    float soundBufMix[512];

    maxiOsc cellSin;  //  animation for cell pattern
    double cellSinOut;

    //===================//

    //-----SETUP-CLOCK---//
    maxiClock clock;
    int playHead;
    //===================//

    //--MIX'S-&-OUTPUTS--//
    double mix;
    double stereoMix[2], masterStereoOutput[2];
    //=========================================//


    //========MASTER=FX=========//
    //variables to set player position to control efffects
    double playerFilter;
    double playerFilterHires;
    double playerDelayFeedback;
    double playerDelayMix;
    //---------FILTER-----------//
    maxiFilter lowRes[2];
    double lowResFilterMix[2];


    maxiFilter hiRes[2];
    double hiResFilterMix[2];

    //----------DELAY-----------//
    maxiDelayline delay[2];
    double delayMix[2], stereoDelayOutput[2];

    //----------REVERB-----------//
    maxiDelayline reverbsL[10], reverbsR[10];
    vector<double>reverbTimes;
    double reverbMix[2], reverbOutput[2];
    //=================================//

    maxiFlanger flange;
    double flangeOut;


    //====interactive-object-sound==//
    maxiOsc sq, filterMod;
    double sq_out;
    maxiEnv sqEnv;
    double sq_envOut;
    maxiFilter sq_Filter;
    double sq_FilterOut;
    double sqMix[2];
    maxiMix sqStereo;


    //experiment with panning object sound
    double gainPanX;
    double gainPanY;
    double mousePan;
    double modPan;

    double mapGain;
    double mapMod;
    double radius = 300;


    //---NOISE-BURST-CHARACTER-SOUND--//
    double noise, noise_envOut, noise_out;
    maxiEnv noiseEnv;
    maxiOsc noiseOsc;
    //int noiseTrigger=0;
    maxiFilter noiseFilter;
    double noiseFilter_out;
    maxiDelayline noiseDelay;
    double noiseDelay_out;
    //double noiseMix;
    maxiMix noiseStereo;
    double noiseMix[2];
    maxiOsc noisePanner;
    //==================//

    //PATTERN SWITCH - this will switch the collection of sounds
    //---------------------------------------------------------//
    int patternSwitch;

    //TRIGGER LINE
    int pattern_trigger;
    int pattern_triggerLine[64] = { 1,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
                                    0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
                                    0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
                                    0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0 };
    int sequenceNote = 0;
    //================//

    //===========BEATS================//
    int drumPattern = 0;
    maxiKick kick;
    double kick_out;
    int kickTrigger;


    int kickSeq[32] = { 0,1,0,0, 0,0,0,0,
                         0,0,0,0, 0,0,0,0,
                         0,1,0,0, 0,0,0,0,
                         0,0,0,0, 0,0,0,0 };

    int kickSeq2[32] = { 0,0,1,0, 0,0,0,0,
                         0,0,0,0, 0,0,0,0,
                         0,1,0,0, 0,0,0,0,
                         0,0,0,0, 0,0,0,0 };

    int kickSeq3[32] = { 0,1,0,0, 0,0,0,1,
                         0,0,0,0, 0,0,1,0,
                         0,0,0,0, 0,0,0,0,
                         0,0,0,0, 0,0,0,0 };




    double kickMix[2];
    maxiMix kickStereo;
    //-----------------------------------------//
    maxiHats hiHat;
    double hiHat_out;
    int hiHatTrigger;

    int hiHatSeq[32] = { 0,0,1,0, 0,0,0,0,
                         0,0,1,0, 0,0,0,0,
                         0,0,1,0, 0,0,0,0,
                         0,0,1,0, 0,0,0,0 };

    int hiHatSeq2[32] = { 0,1,0,0, 0,0,1,0,
                          0,0,1,0, 0,0,0,0,
                          0,0,0,0, 0,0,1,0,
                          0,0,1,0, 0,0,0,0 };

    int hiHatSeq3[32] = { 0,1,0,0, 0,0,0,0,
                          0,1,0,0, 0,0,0,0,
                          0,1,0,0, 0,0,0,0,
                          0,1,0,0, 0,0,0,1 };

    int hiHatSeq4[32] = { 1,0,1,0, 0,1,0,1,
                          0,1,1,0, 0,1,0,0,
                          1,0,1,0, 0,1,0,1,
                          0,1,1,0, 0,1,0,0 };

    int hiHatSeq5[32] = { 0,0,1,0, 0,0,0,0,
                          0,0,0,0, 0,0,0,0,
                          0,0,0,0, 0,0,0,1,
                          0,0,0,0, 0,0,0,0 };

    int hiHatCount = 0;
    double hiHatRelease;
    double hiHatReleaseSeq[10] = { 100,200,300,100,3000,
                                   500,100,100,100,2000 };


    double hiHatMix[2];
    maxiMix hiHatStereo;

    //-----------------------------------------//
    maxiSample snare;
    maxiEnv snareEnv;
    double snare_out, snare_envOut;
    int snareTrigger;

    int snareSeq[32] = { 0,0,0,0, 1,0,0,0,
                         0,0,0,0, 0,0,0,0,
                         0,0,0,0, 0,0,0,0,
                         0,0,0,0, 0,0,0,1 };

    int snareSeq2[32] = { 0,0,0,0, 1,0,0,0,
                          0,0,0,0, 0,0,0,0,
                          0,0,0,0, 0,1,0,0,
                          0,0,0,0, 0,0,0,1 };

    int snareSeq3[32] = { 0,0,0,0, 1,0,0,0,
                          0,1,0,0, 0,0,1,0,
                          0,0,0,0, 1,0,0,0,
                          0,0,0,0, 0,0,0,1 };

    int snareSeq4[32] = { 0,0,0,0, 0,0,0,0,
                          0,0,0,0, 0,0,0,0,
                          0,0,0,0, 0,0,1,0,
                          0,0,0,0, 0,0,0,0 };



    double snareMix[2];
    maxiMix snareStereo;

    //-----------------------------------------//
    double beatMix[2];
    //-----------------------------------------//

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

    int metSeq2[16] = { 0,0,0,0, 1,1,0,0,
                        0,0,0,0, 0,0,1,0 };
    //-------------//
    double metSpeed;
    double metSpeedSeq[16] = { 1.5,1,1,1.8, 1.5,1,1,1.8,
                              1.5,1,1,1,   1.5,1,1,1.8 };
    //====================//
    // [16] = 1 bar loop - 4 beats
    // [32] = 4 bar loop
    // [128] = 8 bar loop


    //---KEY-ARP-LOOP---//
    //int keyArpSwitch = 0;
    maxiSample keyArpLoop;
    double keyArp_out;

    maxiEnv keyArpEnv;
    double keyArp_envOut;

    int keyArpTrigger;
    int keyArpSeq[32] = { 1,0,0,0, 0,0,0,0,
                          0,0,0,0, 0,0,0,0,
                          0,0,0,0, 0,0,0,0,
                          0,0,0,0, 0,0,0,0 };


    maxiMix keyArpStereo;
    double keyArpMix[2];
    //==================//


    //---KEY-LOOP-HI---//
    maxiSample keyHi;
    double keyHi_out;

    maxiEnv keyHiEnv;
    double keyHi_envOut;

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

    maxiEnv keyLowEnv;
    double keyLow_envOut;

    int keyLowTrigger;

    //4 BAR LOOP
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


    //==========INTERACTIVE-SAMPLE-HITS==========//
    int hitSelect = 0;
    double hitMix[2];

    maxiSample hit1;
    maxiEnv hit1Env;
    double hit1_out, hit1_envOut;
    double hit1Mix[2];
    maxiMix hit1Stereo;

    maxiSample hit2;
    maxiEnv hit2Env;
    double hit2_out, hit2_envOut;
    double hit2Mix[2];
    maxiMix hit2Stereo;

    maxiSample hit3;
    maxiEnv hit3Env;
    double hit3_out, hit3_envOut;
    double hit3Mix[2];
    maxiMix hit3Stereo;

    maxiSample hit4;
    maxiEnv hit4Env;
    double hit4_out, hit4_envOut;
    double hit4Mix[2];
    maxiMix hit4Stereo;

    maxiSample hit5;
    maxiEnv hit5Env;
    double hit5_out, hit5_envOut;
    double hit5Mix[2];
    maxiMix hit5Stereo;

    maxiSample hit6;
    maxiEnv hit6Env;
    double hit6_out, hit6_envOut;
    double hit6Mix[2];
    maxiMix hit6Stereo;

    maxiSample hit7;
    maxiEnv hit7Env;
    double hit7_out, hit7_envOut;
    double hit7Mix[2];
    maxiMix hit7Stereo;

    maxiSample hit8;
    maxiEnv hit8Env;
    double hit8_out, hit8_envOut;
    double hit8Mix[2];
    maxiMix hit8Stereo;


    //-------------//
    maxiSample hitSub;
    maxiEnv hitSubEnv;
    double hitSub_out, hitSub_envOut;
    double hitSubMix[2];
    maxiMix hitSubStereo;

    //may not need this anymore??
    int subHitTrigger;
    int subHitSeq[256] = { 1,0,0,0, 0,0,0,0,
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
                              0,0,0,0, 0,0,0,0,
                              0,0,0,0, 0,0,0,0,
                              0,0,0,0, 0,0,0,0 };
    //========================================//
    //========================================//

    //=====STRINGS-SYNTH======//
    int stringsPattern;
    //OSCILLATORS
    maxiOsc strings_Osc, strings_Osc1, strings_Osc2;
    double strings_OscOut, strings_Osc1Out, strings_Osc2Out, strings_OscOutMix;

    //LFO
    maxiOsc strings_LFO;
    double strings_LFO_Out;

    //ENVELOPE
    maxiEnv strings_OscEnv;
    double  strings_Osc_envOut;

    //STEREO MIX
    maxiMix strings_OscStereo;
    double strings_OscMix[2];

    //DELAY
    maxiDelayline strings_Delay;
    double strings_OscDelay_out;

    //FILTER
    maxiFilter strings_OscFilter, strings_OscFilter1, strings_OscFilter2;
    double strings_OscFilter_out, strings_OscFilter_out1, strings_OscFilter_out2;

    //TRIGGER LINE
    int strings_OscTrigger;
    int strings_OscLineTrigger[64] = { 1,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
                                        0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
                                        0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
                                        0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0 };

    //PITCH LINE
    float strings_OscPitch, strings_Osc1Pitch, strings_Osc2Pitch;
    int strings_OscLinePitch[4] = { 63, 61, 66, 61 };
    int strings_OscLinePitch2[4] = { 68, 63, 68, 68 };
    int strings_OscLinePitch3[4] = { 70, 68, 73, 72 };

    int strings_OscLinePitchB[4] = { 77, 77, 77, 77 };
    int strings_OscLinePitch2B[4] = { 78, 78, 78, 78 };
    int strings_OscLinePitch3B[4] = { 82, 82, 82, 82 };
    int strings_OscNote;
    //========================================//
    //========================================//





    //====POLY-SYNTH========//
    int polyPattern;
    //OSCILLATORS
    maxiOsc poly_Osc, poly_Osc1, poly_Osc2;
    double poly_OscOut, poly_Osc1Out, poly_Osc2Out;

    //LFO
    maxiOsc poly_LFO;
    double poly_LFO_Out;

    //ENVELOPE
    maxiEnv poly_OscEnv;
    double  poly_Osc_envOut;

    //STEREO MIX
    maxiMix poly_OscStereo;
    double poly_OscMix[2];

    //DELAY
    maxiDelayline poly_Delay;
    double poly_OscDelay_out;

    //FILTER
    maxiFilter poly_OscFilter;
    double poly_OscFilter_out;

    //TRIGGER LINE
    int poly_OscTrigger;
    //    int poly_OscLineTrigger[64] =  { 1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1,
    //                                     0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    //                                     0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    //                                     0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0 };

    int poly_OscLineTrigger[16] = { 1,0,0,1, 0,0,1,0, 1,0,0,1, 0,0,1,0 };
    int poly_OscLineTrigger2[16] = { 0,1,0,1, 1,0,1,0, 0,0,0,0, 0,0,0,1 };
    int poly_OscLineTrigger3[16] = { 1,1,1,1, 1,0,1,0, 1,1,1,1, 1,0,1,0 };
    int poly_OscLineTrigger4[16] = { 1,1,1,1, 1,0,1,1, 1,1,1,0, 0,0,0,0 };
    //PITCH LINE
    float poly_OscPitch, poly_Osc1Pitch, poly_Osc2Pitch;
    int poly_OscLinePitch[5] = { 27, 64, 61, 63, 70 };
    int poly_OscLinePitch2[6] = { 63, 56, 58, 61, 63, 63 };
    int poly_OscLinePitch3[10] = { 63, 68, 63, 66, 63,
                                   61, 63, 68, 63, 66 };

    int poly_OscLinePitch4[4] = { 51, 56, 58, 56 };
    int poly_OscLinePitch4b[4] = { 58, 61, 66, 65 };

    //    27, 32, 30, 25
    int poly_OscNote;

    //========================================//
    //========================================//

//    //=====STRINGS-SYNTH======//
//    int stringsPattern;
//    //OSCILLATORS
//    maxiOsc strings_Osc, strings_Osc1, strings_Osc2;
//    double strings_OscOut, strings_Osc1Out, strings_Osc2Out;
//
//    //LFO
//    maxiOsc strings_LFO;
//    double strings_LFO_Out;
//
//    //ENVELOPE
//    maxiEnv strings_OscEnv;
//    double  strings_Osc_envOut;
//
//    //STEREO MIX
//    maxiMix strings_OscStereo;
//    double strings_OscMix[2];
//
//    //DELAY
//    maxiDelayline strings_Delay;
//    double strings_OscDelay_out;
//
//    //FILTER
//    maxiFilter strings_OscFilter;
//    double strings_OscFilter_out;
//
//    //TRIGGER LINE
//    int strings_OscTrigger;
//    int strings_OscLineTrigger[64] =  { 1,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
//                                        0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
//                                        0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
//                                        0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0 };
//
//    //PITCH LINE
//    float strings_OscPitch, strings_Osc1Pitch, strings_Osc2Pitch;
//    int strings_OscLinePitch[4] = { 63, 61, 66, 61};
//    int strings_OscLinePitch2[4] = { 68, 63, 68, 68};
//    int strings_OscLinePitch3[4] = { 70, 68, 73, 72};
//    int strings_OscNote;
//    //========================================//
//    //========================================//





    //=====SAW-SYNTH======//
    int sawPattern;
    //OSCILLATORS
    maxiOsc saw_Osc, saw_Osc1, saw_Osc2;
    double saw_OscOut, saw_Osc1Out, saw_Osc2Out;

    //LFO
    maxiOsc saw_LFO;
    double saw_LFO_Out;

    //ENVELOPE
    maxiEnv saw_OscEnv;
    double  saw_Osc_envOut;

    //STEREO MIX
    maxiMix saw_OscStereo;
    double saw_OscMix[2];

    //DELAY
    maxiDelayline saw_Delay;
    double saw_OscDelay_out;

    //FILTER
    maxiFilter saw_OscFilter;
    double saw_OscFilter_out;

    //TRIGGER LINE
    int saw_OscTrigger;
    int saw_OscLineTrigger[64] = { 1,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
                                    0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
                                    0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
                                    0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0 };

    //PITCH LINE
    float saw_OscPitch, saw_Osc1Pitch, saw_Osc2Pitch;
    int saw_OscLinePitch[4] = { 63, 61, 66, 61 };
    int saw_OscLinePitch2[4] = { 68, 63, 68, 68 };
    int saw_OscLinePitch3[4] = { 70, 68, 73, 72 };
    int saw_OscNote;
    //========================================//
    //========================================//




    //====SQUARE-BASS========//
    int squarePattern;
    //OSCILLATORS
    maxiOsc sq_Bass;
    double sq_BassOut;

    //ENVELOPE
    maxiEnv sq_BassEnv;
    double sq_Bass_envOut;

    //STEREO MIX
    maxiMix sq_BassStereo;
    double sq_BassMix[2];

    //DELAY
    maxiDelayline sq_Delay;
    double sq_Delay_out;

    //FILTER
    maxiFilter sq_BassFilter;
    double sq_BassFilter_out;

    //TRIGGER LINE
    int sq_BassTrigger;
    int sq_BassLineTrigger[64] = { 1,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
                                    0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
                                    0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
                                    0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0 };

    //PITCH LINE
    float sq_BassPitch;
    int sq_BassLinePitch[4] = { 27, 32, 30, 25 };
    int sq_BassNote;
    //========================================//
    //========================================//

    //====SUB-BASS========//
    int sub_BassPattern;
    //OSCILLATORS
    maxiOsc sub_Bass;
    double sub_BassOut;

    //ENVELOPE
    maxiEnv sub_BassEnv;
    double sub_Bass_envOut;

    //STEREO MIX
    maxiMix sub_BassStereo;
    double sub_BassMix[2];


    //TRIGGER LINE
    int sub_BassTrigger;
    int sub_BassLineTrigger[128] = { 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
                                      0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
                                      0,0,0,0, 0,1,0,0, 0,0,0,0, 0,0,0,0,
                                      0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
                                      0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
                                      0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
                                      0,0,0,0, 0,0,1,0, 0,0,0,0, 0,0,0,1,
                                      0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0 };

    //PITCH LINE
    float sub_BassPitch;
    int sub_BassLinePitch[4] = { 27, 39, 30, 31 };
    int sub_BassNote;
    //========================================//
    //========================================//






    //========SYNTH-BASS-LINE-ARP==============//
    int synthPattern = 0;//use to swith synth pattern
    convert mtof;

    //OSCILLATORS
    maxiOsc sBass;
    double sBassOut;

    //ENVELOPE
    maxiEnv sBassEnv;
    double sBass_envOut;

    //STEREO MIX
    maxiMix sBassStereo;
    double sBassMix[2];

    //DELAY
    maxiDelayline synthDelay;
    double synthDelay_out;

    //TRIGGER LINE
    int sBassTrigger;
    int sBassLineTrigger[16] = { 1,0,0,1, 0,0,0,0, 0,0,0,0, 0,0,0,0 };
    int sBassLineTrigger2[16] = { 1,0,0,1, 0,0,1,0, 1,0,0,1, 0,0,1,0 };

    int sBassLineTrigger3[32] = { 1,1,0,1, 0,0,0,0, 0,0,0,0, 0,0,0,0,
                                  0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0 };


    //PITCH LINE
    float sBassPitch;

    int sBassLinePitch[16] = { 39, 46, 65, 61,
                                63, 65, 55, 63,
                                63, 68, 70, 65,
                                65, 75, 70, 78 };

    int sBassLinePitch2[6] = { 51, 56, 63,
                              51, 54, 63 };

    int sBassLinePitch3[6] = { 39, 46, 51,
                               56, 58, 63 };

    int sBassLinePitch4[12] = { 66, 68, 63,
                                66, 70, 63,
                                68, 70, 63,
                                68, 70, 61
    };



    int sBassNote;

    //========================================//
    //========================================//






    //-----------GUI-SETUP---------//
    ofxPanel gui;
    ofParameter<float> cellSpeed;
    ofParameter<float> cellRadius;

    ofParameter<float> masterGain;
    ofParameter<float> metGain;

    ofParameter<float> sampleHitGain;

    ofParameter<float> atmosGain;

    ofParameter<float> keyLoopGain;
    ofParameter<float> keyLoopPan;

    ofParameter<float> keyHiGain;
    ofParameter<float> keyHiPan;

    ofParameter<float> keyLowGain;
    ofParameter<float> keyLowPan;

    ofParameter<float> synthLineGain;
    ofParameter<float> synthLinePan;

    ofParameter<float> subBassGain;
    ofParameter<float> polySynthGain;
    ofParameter<float> polySynthPan;
    ofParameter<float> squareBassGain;
    ofParameter<float> sawSynthGain;
    ofParameter<float> stringsSynthGain;
    ofParameter<float> stringsSynthPan;

    ofParameter<float> beatGain;

    ofParameter<float> lowResFreq;
    ofParameter<float> lowResQ;

    ofParameter<float> hiResFreq;
    ofParameter<float> hiResQ;

    ofParameter<float> dryDelayMix;
    ofParameter<float> delayTime;
    ofParameter<float> delayFeedback;

    ofParameter<float> dryReverbMix;
    ofParameter<float> reverbFeedback;

    ofParameter<float> noiseAttack;
    ofParameter<float> noiseLevel;

    ofxVec4Slider flangeEffect;
    //==============================//
};
