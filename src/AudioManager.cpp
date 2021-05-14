#include "AudioManager.h"

//--------------------------------------------------------------
AudioManager::~AudioManager()
{
    // otherwise exceptions occur on app exit
    ofSoundStreamClose();
}

//--------------------------------------------------------------
void AudioManager::soundSetup(ofBaseApp* appPtr) {

    //=======MAXIM-SOUND-SETUP=========//
    sampleRate = 44100;
    bufferSize = 512;
    ofxMaxiSettings::setup(sampleRate, 2, bufferSize);
    //=====================================================//
    ofSoundStreamSetup(2, 0, appPtr, sampleRate, bufferSize, 4);
    /* this has to happen at the end of setup - it switches on the DAC */
}


//===============================================================//
void AudioManager::setup(ofBaseApp* appPtr) {

    //=======OF-SETUP======//
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    ofBackground(0, 0, 0);
    //====================//


    //REVERB-SETUP
    for (int i = 0; i < 10; i++) {
        reverbTimes.push_back(ofRandom(0.02, 0.05));
    }

    //============================================//
    loadSamples();         //load samples
    envelopeSetup();
    clockSetup();   //setup tempo and ticks per beat
    guiSetup();    //setup gui
    soundSetup(appPtr);  //run at the end of setup!
    //============================================//
}

//--------------------------------------------------------------
void AudioManager::guiSetup() {
    gui.setup();
    
    //animation variables
    gui.add(cellSpeed.set("cell Speed", 21.6, 0, 60));
    gui.add(cellRadius.set("cell radius", 50, 0, 200));
    //------------------------------------------------//

    //audio mix
    gui.add(masterGain.set("master gain", 1, 0, 3));
    //------------------------------------------------//
    gui.add(metGain.set("metronome gain", 0.02, 0, 1.0));
    //------------------------------------------------//
    gui.add(sampleHitGain.set("sample Hit Gain", 2.38, 0.0, 4.0));
    gui.add(atmosGain.set("atmos gain", 2.0, 0, 7.0));
    //------------------------------------------------//
    gui.add(keyLoopGain.set("key loop gain", 0.0, 0, 4.0));
    gui.add(keyLoopPan.set("key loop Pan", 0.78, 0, 1.0));
    //------------------------------------------------//
    gui.add(keyHiGain.set("keys Hi gain", 0.00, 0, 3.0));
    gui.add(keyHiPan.set("keys Hi Pan", 1.0, 0, 1.0));
    //------------------------------------------------//
    gui.add(keyLowGain.set("keys Low gain", 0., 0, 3.5));
    gui.add(keyLowPan.set("keys Low Pan", 0.1, 0, 1.0));
    //------------------------------------------------//
    gui.add(synthLineGain.set("synth line gain", 0.18, 0, 0.5));
    gui.add(synthLinePan.set("synth line Pan", 0.05, 0, 1.0));
    
    gui.add(subBassGain.set("sub Bass Gain", 0.14, 0.0, 1.0));
    gui.add(polySynthGain.set("poly Synth Gain", 0.065, 0.0, 1.0));
    gui.add(polySynthPan.set("poly Synth Pan", 0.215, 0.0, 1.0));
    
    gui.add(noiseAttack.set("Character Noise Attack", 1500, 0.0, 20000));
    gui.add(noiseLevel.set("Noise Level", 0.004, 0.0, 0.05));
    
    gui.add(squareBassGain.set("square Bass Gain", 0.05, 0.0, 1.0));
    gui.add(sawSynthGain.set("saw Synth Gain", 0.15, 0.0, 1.0));
    
    

    //------------------------------------------------//
    gui.add(beatGain.set("beat gain", 1.2, 0.0, 4.0));
    //------------------------------------------------//
    gui.add(lowResFreq.set("low-Res filter freq", 4000, 0, 4000));
    gui.add(lowResQ.set("low-Res Filter Q", 0.5, 0, 4));
    
    gui.add(hiResFreq.set("hi-Res filter freq", 00, 0, 500));
    gui.add(hiResQ.set("hi-Res Filter Q", 0.5, 0, 4));
    //------------------------------------------------//
    gui.add(dryDelayMix.set("delay Wet/Dry Mix", 0.7, 0, 1));
    gui.add(delayFeedback.set("delay feedback", 0.5, 0, 0.9));
    gui.add(delayTime.set("delay time", 0.9, 0, 2));
    //------------------------------------------------//
    gui.add(dryReverbMix.set("reverb Wet/Dry Mix", 0.7, 0, 1));
    gui.add(reverbFeedback.set("reverb feedback", 0.95, 0, 0.99));
    //------------------------------------------------//
    
    gui.add(flangeEffect.setup("flange effect",
                               ofVec4f(436, 0.1, 0.01, 0.0), ofVec4f(0, 0, 0.001, 0.0), ofVec4f(800, 1, 10, 1)));
}

//--------------------------------------------------------------
void AudioManager::loadSamples() {

    //==================LOAD-SAMPLES-HERE===================//
    metronome.load(ofToDataPath("./audio/samples/rim.wav"));
    snare.load(ofToDataPath("./audio/samples/snare2.wav"));
    

    hit1.load(ofToDataPath("./audio/samples/synthSpace_hi.wav"));
    hit2.load(ofToDataPath("./audio/samples/synthSpace_mid.wav"));
    hit3.load(ofToDataPath("./audio/samples/synthSpace_low.wav"));
    hit4.load(ofToDataPath("./audio/samples/synthSpace4.wav"));
    hit5.load(ofToDataPath("./audio/samples/synthSpace5.wav"));
    hit6.load(ofToDataPath("./audio/samples/synthSpace6.wav"));
    hit7.load(ofToDataPath("./audio/samples/synthSpace7.wav"));
    hit8.load(ofToDataPath("./audio/samples/synthSpace8.wav"));
    

    hitSub.load(ofToDataPath("./audio/samples/subAtmosphere.wav"));

    //=====================LOAD-LOOPS=======================//
    atmos.load(ofToDataPath("./audio/loops/atmosphere.wav"));

    keyArpLoop.load(ofToDataPath("./audio/loops/keyArploop.wav"));
    keyHi.load(ofToDataPath("./audio/loops/keysLoopHi.wav"));
    keyLow.load(ofToDataPath("./audio/loops/keysLoopLow.wav"));

}

//--------------------------------------------------------------
void AudioManager::clockSetup() {
    //setup tempo and ticks per beat//
    //use for 2 bar samples
    clock.setTicksPerBeat(4);
    clock.setTempo(100);
}

//--------------------------------------------------------------
void AudioManager::envelopeSetup() {

    //AUDIO HITS ENVELOPES
    hit1Env.setAttack(0.2);
    hit1Env.setDecay(5);
    hit1Env.setSustain(5);
    hit1Env.setRelease(15000);
    
    hit2Env.setAttack(0.2);
    hit2Env.setDecay(5);
    hit2Env.setSustain(5);
    hit2Env.setRelease(15000);
    
    hit3Env.setAttack(0.2);
    hit3Env.setDecay(5);
    hit3Env.setSustain(5);
    hit3Env.setRelease(15000);
    
    hit4Env.setAttack(0.2);
    hit4Env.setDecay(5);
    hit4Env.setSustain(5);
    hit4Env.setRelease(15000);
    
    hit5Env.setAttack(0.2);
    hit5Env.setDecay(5);
    hit5Env.setSustain(5);
    hit5Env.setRelease(15000);
    
    hit6Env.setAttack(0.2);
    hit6Env.setDecay(5);
    hit6Env.setSustain(5);
    hit6Env.setRelease(15000);
    
    hit7Env.setAttack(0.2);
    hit7Env.setDecay(5);
    hit7Env.setSustain(5);
    hit7Env.setRelease(15000);
    
    hit8Env.setAttack(0.2);
    hit8Env.setDecay(5);
    hit8Env.setSustain(5);
    hit8Env.setRelease(15000);

    hitSubEnv.setAttack(0.2);
    hitSubEnv.setDecay(5);
    hitSubEnv.setSustain(5);
    hitSubEnv.setRelease(15000);
    
    
    
    //SYNTHESIS ENVELOPES

    
    sq_BassEnv.setAttack(10000);
    sq_BassEnv.setDecay(5);
    sq_BassEnv.setSustain(5);
    sq_BassEnv.setRelease(1000);
    
    sub_BassEnv.setAttack(2);
    sub_BassEnv.setDecay(5);
    sub_BassEnv.setSustain(5);
    sub_BassEnv.setRelease(4000);
    
    saw_OscEnv.setAttack(10000);
    saw_OscEnv.setDecay(5);
    saw_OscEnv.setSustain(5);
    saw_OscEnv.setRelease(5000);
    
    strings_OscEnv.setAttack(1000);
    strings_OscEnv.setDecay(500);
    strings_OscEnv.setSustain(500);
    strings_OscEnv.setRelease(10000);
    
//    poly_OscEnv.setAttack(0);
//    poly_OscEnv.setDecay(200);
//    poly_OscEnv.setSustain(0.2);
//    poly_OscEnv.setRelease(2000);
    
    
    
}

//--------------------------------------------------------------
double AudioManager::playMetronome() {

    //---------SETUP-METRONOME-&-SEQUENCE-TRIGGER---------//
    met_out = metronome.playOnce(metSpeed);

    met_envOut = metEnv.ar(met_out, 0.1, 1.9, 1, metTrigger) * metGain;

    if (metTrigger == 1) {
        //set playback position to start of sample
        metronome.trigger();
    }

    return met_envOut;

}

//--------------------------------------------------------------
void AudioManager::playKeyLoop() {

    //---------SETUP-KEY-LOOP-&-SEQUENCE-TRIGGER-------//
    keyArp_out = keyArpLoop.playOnce();

    keyArp_envOut = keyArpEnv.ar(keyArp_out, 0.1, 1.0, 1, keyArpTrigger) * keyLoopGain;
//    if (keyArpSwitch == 0) {
        if (keyArpTrigger == 1) {
            //This sets the playback position to the start of a sample
            keyArpLoop.trigger();
        }
//    }


    //send to stereo mix
    keyArpStereo.stereo(keyArp_envOut, keyArpMix, keyLoopPan);
}

//--------------------------------------------------------------
void AudioManager::playKeyLoopHi() {

    //---------SETUP-KEY-LOOP-Hi-&-SEQUENCE-TRIGGER-------//
    keyHi_out = keyHi.playOnce();

    keyHi_envOut = keyHiEnv.ar(keyHi_out, 0.1, 1.0, 1, keyHiTrigger) * keyHiGain;

    if (keyHiTrigger == 1) {
        //This sets the playback position to the start of a sample
        keyHi.trigger();
    }

    //send to stereo mix
    keyHiStereo.stereo(keyHi_envOut, keyHiMix, keyHiPan);
}

//--------------------------------------------------------------
void AudioManager::playKeyLoopLow() {

    //---------SETUP-KEY-LOOP-LOW-&-SEQUENCE-TRIGGER-------//
    keyLow_out = keyLow.playOnce();

    keyLow_envOut = keyLowEnv.ar(keyLow_out, 0.1, 1.0, 1, keyLowTrigger) * keyLowGain;

    if (keyLowTrigger == 1) {
        //This sets the playback position to the start of a sample
        keyLow.trigger();
    }

    //send to stereo mix
    keyLowStereo.stereo(keyLow_envOut, keyLowMix, keyLowPan);
}

//--------------------------------------------------------------
void AudioManager::sampleHits() {
    //---------ATMOSPHERE-LOOP-PLAY------//
    atmos_out = atmos.play() * atmosGain;
    atmosStereo.stereo(atmos_out, atmosStereoOut,
                       (autoPanner.sinewave(0.01) + 1) / 2);
    //===================================//
    
    //===================================//
    //---------SAMPLE-HITS---------------//
    //===================================//
    hit1_out = hit1.playOnce() * (1.8 * sampleHitGain);                       //play & gain
    hit1_envOut = hit1Env.adsr(hit1_out, hit1Env.trigger);  //envelope
    hit1Stereo.stereo(hit1_envOut, hit1Mix, 0.1);           //stereo mix
    //---------------------------------//
    hit2_out = hit2.playOnce() * (2.1 * sampleHitGain);
    hit2_envOut = hit2Env.adsr(hit2_out, hit2Env.trigger);
    hit2Stereo.stereo(hit2_envOut, hit2Mix, 0.8);
    //---------------------------------//
    hit3_out = hit3.playOnce() * (1.8 * sampleHitGain);
    hit3_envOut = hit3Env.adsr(hit3_out, hit3Env.trigger);
    hit3Stereo.stereo(hit3_envOut, hit3Mix, 0.3);
    //---------------------------------//
    hit4_out = hit4.playOnce() * (1.2 * sampleHitGain);
    hit4_envOut = hit4Env.adsr(hit4_out, hit4Env.trigger);
    hit4Stereo.stereo(hit4_envOut, hit4Mix, 0.8);
    //---------------------------------//
    hit5_out = hit5.playOnce() * (0.5 * sampleHitGain);
    hit5_envOut = hit5Env.adsr(hit5_out, hit5Env.trigger);
    hit5Stereo.stereo(hit5_envOut, hit5Mix, 0.09);
    //---------------------------------//
    hit6_out = hit6.playOnce() * (1.8 * sampleHitGain);
    hit6_envOut = hit6Env.adsr(hit6_out, hit6Env.trigger);
    hit6Stereo.stereo(hit6_envOut, hit6Mix, 0.6);
    //---------------------------------//
    hit7_out = hit7.playOnce() * (1.2 * sampleHitGain);
    hit7_envOut = hit7Env.adsr(hit7_out, hit7Env.trigger);
    hit7Stereo.stereo(hit7_envOut, hit7Mix, 0.01);
    //---------------------------------//
    hit8_out = hit8.playOnce() * (2.0 * sampleHitGain);
    hit8_envOut = hit8Env.adsr(hit8_out, hit8Env.trigger);
    hit8Stereo.stereo(hit8_envOut, hit8Mix, 0.3);
    //===================================//
    
    //triggered in sequence
    hitSub_out = hitSub.playOnce() * 8.0;
    hitSub_envOut = hitSubEnv.adsr(hitSub_out, subHitTrigger);
    hitSubStereo.stereo(hitSub_envOut, hitSubMix, 0.1);
    //===================================//
}

//-----------------------------------//
void AudioManager::beats(){
    
    //setup kick drum
    kick.setPitch(65);
    kick.useFilter = true;
    kick.cutoff = 2000;
    kick.setRelease(1500);
    kick_out = kick.play() * 0.2;
    kickStereo.stereo(kick_out, kickMix, 0.5);
    
    //setup hihats
    hiHat.setRelease(hiHatRelease);
    hiHat_out = hiHat.play() * 0.01;
    hiHatStereo.stereo(hiHat_out, hiHatMix, 0.9);
    
    //setup snare
    
    snare_out = snare.playOnce() * 0.25;
    snare_envOut = snareEnv.ar(snare_out, 0.1, 1.9, 1, snareTrigger);
    snareStereo.stereo(snare_out, snareMix, 0.25);
    
}


//-----------------------------------//
void AudioManager::charNoise(){
    
    //noise sound for movement
    noise_envOut = noiseEnv.adsr(1., noiseEnv.trigger);
    noise_out = (noiseOsc.noise() * noise_envOut)*noiseLevel;
    noiseFilter_out = noiseFilter.lores(noise_out, 200 + noise_envOut * 1000, 5);
    //noiseDelay_out = noiseDelay.dl(noiseFilter_out, 44100 * 0.9, 0.4);
    
    noiseStereo.stereo(noiseFilter_out, noiseMix,
                       (noisePanner.sinewave(0.25 * noise_envOut) + 2) / 5);
}

//-----------------------------------//
void AudioManager::synthLine(){
    //synth arp sound
    sBass_envOut = sBassEnv.ar(sBass.sinewave(sBassPitch), 0.05, 0.99, 5000, sBassTrigger) * synthLineGain;
    synthDelay_out = synthDelay.dl(sBass_envOut, 44100 * 0.9, 0.4);
    sBassStereo.stereo(synthDelay_out, sBassMix, synthLinePan);
}


//-----------------------------------//
void AudioManager::polySynth(){
    //--------------//
    //--POLY-SYNTH--//
    //--------------//
    poly_Osc_envOut = poly_OscEnv.adsr(1., poly_OscTrigger);
    
    poly_LFO_Out = poly_LFO.sinebuf(0.2);
    
    poly_Osc1Out = poly_Osc1.pulse(poly_Osc1Pitch, 0.2);
    
    //poly_Osc2Out = poly_Osc2.pulse(poly_Osc2Pitch, 0.8);
                   
    poly_OscOut = ((poly_Osc.pulse(poly_OscPitch, 0.6) + poly_Osc1Out) * poly_Osc_envOut)*polySynthGain;
    
    poly_OscFilter_out = poly_OscFilter.lores(poly_OscOut, 500 + poly_Osc_envOut + (poly_LFO_Out * 200), poly_Osc_envOut * 5);
    
    poly_OscDelay_out = poly_Delay.dl(poly_OscFilter_out, 44100 * 0.9, 0.4);//?
    
//    poly_OscDelay_out = flange.flange(poly_OscFilter_out, flangeEffect->x, flangeEffect->y, flangeEffect->z,flangeEffect->w);//?
    
    poly_OscStereo.stereo(poly_OscDelay_out, poly_OscMix, polySynthPan);
    //---------------//
}

//-----------------------------------//
void AudioManager::sawSqSynth(){
    
    //--------------//
    //SAWTOOTH-SYNTH//
    //--------------//
    saw_Osc_envOut = saw_OscEnv.adsr(1., saw_OscTrigger);
    
    saw_LFO_Out = saw_LFO.sinewave(saw_Osc_envOut * 10);
    
    saw_Osc1Out = saw_Osc1.saw(saw_Osc1Pitch);
    saw_Osc2Out = saw_Osc2.saw(saw_Osc2Pitch);
                   
    saw_OscOut = (((saw_Osc.saw(saw_OscPitch) + saw_Osc1Out + saw_Osc2Out) * saw_Osc_envOut)*saw_LFO_Out)*sawSynthGain;
    
    saw_OscDelay_out = saw_Delay.dl(saw_OscOut, 44100 * 0.9, 0.2);//?
    
    saw_OscFilter_out = saw_OscFilter.lores(saw_OscOut, 500 + saw_Osc_envOut * 100, 0);
    
    saw_OscStereo.stereo(saw_OscFilter_out, saw_OscMix, 0.7);
    //---------------//
    
    //---------------//
    //SQUARE-BASS-LINE
    //---------------//
    sq_Bass_envOut = sq_BassEnv.adsr(1., sq_BassTrigger);
    sq_BassOut = (sq_Bass.square(sq_BassPitch) * sq_Bass_envOut)*squareBassGain;
    
    sq_Delay_out = sq_Delay.dl(sq_BassOut, 44100 * 0.9, 0.2);
    
    sq_BassFilter_out = sq_BassFilter.lores(sq_BassOut, 200 + sq_Bass_envOut * 200, 5 + sq_Bass_envOut * 10);
    
    sq_BassStereo.stereo(sq_BassFilter_out, sq_BassMix, 0.1);
    //---------------//
}

//-----------------------------------//
void AudioManager::subBass(){
    //---------------//
    //SUB-BASS-LINE
    //---------------//
    sub_Bass_envOut = sub_BassEnv.adsr(1., sub_BassTrigger);
    sub_BassOut = (sub_Bass.sinewave(sub_BassPitch) * sub_Bass_envOut)*subBassGain;
    sub_BassStereo.stereo(sub_BassOut, sub_BassMix, 0.4);
    //---------------//
}


//--------------------------------------------------------------
void AudioManager::audioOut(float* output, int bufferSize, int nChannels) {

    stereoMix[0] = 0;
    stereoMix[1] = 0;

    lowResFilterMix[0] = 0;
    lowResFilterMix[1] = 0;

    delayMix[0] = 0;
    delayMix[1] = 0;

    stereoDelayOutput[0] = 0;
    stereoDelayOutput[1] = 0;

    reverbMix[0] = 0;
    reverbMix[1] = 0;

    reverbOutput[0] = 0;
    reverbOutput[1] = 0;

    masterStereoOutput[0] = 0;
    masterStereoOutput[1] = 0;

    snareMix[0] = 0;
    snareMix[1] = 0;

    for (int i = 0; i < bufferSize; i++) {

        //========================================//
        //-----CLOCK-&-SEQUENCER-SETUP-LOOPS------//
        //========================================//
        clock.ticker();     //start clock

        if (clock.tick) {   //iterate through sequences
            
            //use to trigger patterns at the beginning of every 4 bars
            pattern_trigger = pattern_triggerLine[playHead % 64];
            
            //-----------loop-triggers------------//
            keyArpTrigger = keyArpSeq[playHead % 32];
            keyHiTrigger = keyHiSeq[playHead % 64];
            keyLowTrigger = keyLowSeq[playHead % 128];
            
            subHitTrigger = subHitSeq[playHead % 256];
            
            if(subHitTrigger == 1) {
                hitSub.trigger();
            }
            //-----------------------------------//
            
            //------------BEATS---------------//
            //----------kick drum-------------//
            //switch trigger pattern sequence
            if (drumPattern == 1) {
                kickTrigger = kickSeq[playHead%32];
            } else if (drumPattern == 2) {
                kickTrigger = kickSeq2[playHead%32];
            } else if (drumPattern == 3) {
                kickTrigger = kickSeq3[playHead%32];
            }
            
            
            if (kickTrigger == 1) {
                kick.trigger();
            }
            
            
            //----------hi hats----------------//
            //switch trigger pattern sequence
            if (drumPattern == 1) {
                hiHatTrigger = hiHatSeq[playHead%32];
            } else if (drumPattern == 2) {
                hiHatTrigger = hiHatSeq2[playHead%32];
            } else if (drumPattern == 3) {
                hiHatTrigger = hiHatSeq3[playHead%32];
            } else if (drumPattern == 4) {
                hiHatTrigger = hiHatSeq4[playHead%32];
            } else if (drumPattern == 5) {
                hiHatTrigger = hiHatSeq5[playHead%32];
            }
            
            //hiHatTrigger = hiHatSeq[playHead%32];
            if (hiHatTrigger == 1) {
                hiHat.trigger();
                //change hi hat release value
                hiHatRelease = hiHatReleaseSeq[hiHatCount];
                hiHatCount++;
                if(hiHatCount > 9) {
                    hiHatCount = 0;
                }
            }
            
            //-----------snare-----------------//
            if (drumPattern == 1) {
                snareTrigger = snareSeq[playHead%32];
            } else if (drumPattern == 2) {
                snareTrigger = snareSeq2[playHead%32];
            } else if (drumPattern == 3) {
                snareTrigger = snareSeq3[playHead%32];
            } else if (drumPattern == 4) {
                snareTrigger = snareSeq4[playHead%32];
            }
            
            //reset snare sample to start
            if (snareTrigger == 1) {
                snare.trigger();
            }
            
            
            
            //-----------side stick-----------------//
            if (drumPattern == 5) {
                metTrigger = metSeq2[playHead % 16];
                metSpeed = metSpeedSeq[playHead % 16];
            }
            
 
            //--------------------------------//
            //USE FOR TRIGGERING SYNTH LINE
            //switch trigger pattern sequence
            if (synthPattern == 1) {
                sBassTrigger = sBassLineTrigger[playHead % 16];
            } else if (synthPattern == 2 || synthPattern == 3) {
                sBassTrigger = sBassLineTrigger2[playHead % 16];
            } else if (synthPattern == 4) {
                sBassTrigger = sBassLineTrigger3[playHead % 32];
            }
            
            //switch pitch pattern sequence
            
            if (sBassTrigger == 1) {
               
                if (synthPattern == 1) {
                    
                    sBassPitch = mtof.mtof(sBassLinePitch[sBassNote]);
                    sBassNote++; //increment the pitch line
                    if (sBassNote > 0) {
                        sBassNote = 0;
                    }
                    
                } else if (synthPattern == 2) {
                    
                    sBassPitch = mtof.mtof(sBassLinePitch2[sBassNote]);
                    sBassNote++; //increment the pitch line
                    if (sBassNote > 5) {
                        sBassNote = 0;
                    }
                    
                } else if (synthPattern == 3) {
                    
                    sBassPitch = mtof.mtof(sBassLinePitch3[sBassNote]);
                    sBassNote++; //increment the pitch line
                    if (sBassNote > 5) {
                        sBassNote = 0;
                    }
                    
                } else if (synthPattern == 4) {
                    
                    sBassPitch = mtof.mtof(sBassLinePitch4[sBassNote]-24);
                    sBassNote++; //increment the pitch line
                    if (sBassNote > 11) {
                        sBassNote = 0;
                    }
                    
                }
            }
            
            //--------------------SQAURE-BASS--------------------//
            if (squarePattern == 1) {
        
                    sq_BassTrigger = sq_BassLineTrigger[playHead % 64];
           
            }
    
            
            if (sq_BassTrigger == 1) {
                
                sq_BassPitch = mtof.mtof(sq_BassLinePitch[sequenceNote]);
//                sq_BassNote++; //increment the pitch line
//                if (sq_BassNote > 3) {
//                    sq_BassNote = 0;
//                }
                
            }
            //=============================================================//
            
            //-----------------SAW-SYNTH-------------------------------//
            if (sawPattern == 1) {
                saw_OscTrigger = saw_OscLineTrigger[playHead % 64];
            }
            
            if (saw_OscTrigger == 1) {
                saw_OscPitch = mtof.mtof(saw_OscLinePitch[sequenceNote]);
                saw_Osc1Pitch = mtof.mtof(saw_OscLinePitch2[sequenceNote]);
                saw_Osc2Pitch = mtof.mtof(saw_OscLinePitch3[sequenceNote]);
                
//                saw_OscNote++; //increment the pitch line
//                if (saw_OscNote > 3) {
//                    saw_OscNote = 0;
//                }
            }
            //=============================================================//
            
            //strings sound
            if (stringsPattern == 1) {
                strings_OscTrigger = strings_OscLineTrigger[playHead % 64];
            }
            
            if (strings_OscTrigger == 1) {
                strings_OscPitch = mtof.mtof(strings_OscLinePitch[sequenceNote]);
                strings_Osc1Pitch = mtof.mtof(strings_OscLinePitch2[sequenceNote]);
                strings_Osc2Pitch = mtof.mtof(strings_OscLinePitch3[sequenceNote]);
                
//                saw_OscNote++; //increment the pitch line
//                if (saw_OscNote > 3) {
//                    saw_OscNote = 0;
//                }
            }
            
            //--------------------SUB-BASS---------------------------------//
            if (sub_BassPattern == 1) {
                sub_BassTrigger = sub_BassLineTrigger[playHead % 128];
            }
            
            if (sub_BassPattern == 1) {
                if (sub_BassTrigger == 1) {
                    
                    sub_BassPitch = mtof.mtof(sub_BassLinePitch[sub_BassNote]);
                    sub_BassNote++; //increment the pitch line
                    if (sub_BassNote > 2) {
                        sub_BassNote = 0;
                    }
                }
            }
            
            //=============================================================//
  
            //--------------------POLY-SYNTH--------------------------//
            if (polyPattern == 1) {
                poly_OscTrigger = poly_OscLineTrigger[playHead % 16];
            } else if (polyPattern == 2) {
                poly_OscTrigger = poly_OscLineTrigger2[playHead % 16];
            } else if (polyPattern == 3) {
                poly_OscTrigger = poly_OscLineTrigger3[playHead % 16];
            } else if (polyPattern == 4) {
                poly_OscTrigger = poly_OscLineTrigger4[playHead % 16];
            } else if (polyPattern == 5) {
                poly_OscTrigger = poly_OscLineTrigger[playHead % 16];
            }
            
            if (poly_OscTrigger == 1) {
                
                if (polyPattern == 1 || polyPattern == 2) {
                    poly_OscPitch = mtof.mtof(poly_OscLinePitch[poly_OscNote]);
                    poly_Osc1Pitch = mtof.mtof(poly_OscLinePitch[poly_OscNote]+12);
                    //saw_Osc2Pitch = mtof.mtof(saw_OscLinePitch[saw_OscNote]+3);
                    
                    poly_OscNote++; //increment the pitch line
                    if (poly_OscNote > 0) {
                        poly_OscNote = 0;
                    }
                    
                } else if (polyPattern == 3) {
                    poly_OscPitch = mtof.mtof(poly_OscLinePitch2[poly_OscNote]);
                    poly_Osc1Pitch = mtof.mtof(poly_OscLinePitch2[poly_OscNote]+12);
                    
                    poly_OscNote++; //increment the pitch line
                    if (poly_OscNote > 5) {
                        poly_OscNote = 0;
                    }
                } else if (polyPattern == 4) {
                    poly_OscPitch = mtof.mtof(poly_OscLinePitch3[poly_OscNote]);
                    poly_Osc1Pitch = mtof.mtof(poly_OscLinePitch3[poly_OscNote]+12);
                    
                    poly_OscNote++; //increment the pitch line
                    if (poly_OscNote > 9) {
                        poly_OscNote = 0;
                    }
                } else if (polyPattern == 5) {
                    poly_OscPitch = mtof.mtof(poly_OscLinePitch4[sequenceNote-1]);
                    poly_Osc1Pitch = mtof.mtof(poly_OscLinePitch4b[sequenceNote-1]);
                    
                }
                
            }
            //=============================================================//
            
            if (pattern_trigger == 1) {
                sequenceNote++;
                if (sequenceNote > 3) {
                    sequenceNote = 0;
                }
            }
            
            playHead++;//iterate the playhead
            
        } //end of clock
        
        
  
        
        
       // std::cout<<poly_OscLinePitch4<<std::endl;
        //--------------//
        //SAWTOOTH-SYNTH//
        //--------------//
        strings_Osc_envOut = strings_OscEnv.adsr(1., strings_OscTrigger);
        
        strings_LFO_Out = strings_LFO.sinewave(strings_Osc_envOut * 10);
        
        strings_Osc1Out = strings_Osc1.saw(strings_Osc1Pitch);
        strings_Osc2Out = strings_Osc2.saw(strings_Osc2Pitch);
                       
        strings_OscOut = (((strings_Osc.saw(strings_OscPitch) + strings_Osc1Out + strings_Osc2Out) * strings_Osc_envOut)*strings_LFO_Out)*0.2;
        
        strings_OscDelay_out = strings_Delay.dl(strings_OscOut, 44100 * 0.9, 0.2);//?
        
        strings_OscFilter_out = strings_OscFilter.lores(strings_OscOut, 500 + saw_Osc_envOut * 100, 0);
        
        strings_OscStereo.stereo(strings_OscFilter_out, strings_OscMix, 0.7);
        //---------------//
        

        //========================================//
        //========================================//
        //cell animation sin
        cellSinOut = cellSin.sinewave(cellSpeed);
        //========================================//
        //interactive sound
        sq_out = sq.square(100) * (mapGain);
        sq_FilterOut = sq_Filter.lores(sq_out,
                                        (300 * (filterMod.sinewave(mapMod)+1)/2), 5);
        
        sqStereo.stereo(sq_FilterOut, sqMix, mousePan);
        //============================================//
        
        

        //--------------------Beat-Stereo-Mix-------------------------/
        //beat pre mix
        beats();
        beatMix[0] = (kickMix[0] + hiHatMix[0] + snareMix[0] + playMetronome()) * beatGain;
        beatMix[1] = (kickMix[1] + hiHatMix[1] + snareMix[1] + playMetronome()) * beatGain;
    
        //===============SAMPLE-HIT-MIX=============================//
        sampleHits(); //load samples and pass through mix
        hitMix[0] = hit1Mix[0] + hit2Mix[0] + hit3Mix[0] + hit4Mix[0] +
                    hit5Mix[0] + hit6Mix[0] + hit7Mix[0] + hit8Mix[0] +
                    hitSubMix[0];
        
        hitMix[1] = hit1Mix[1] + hit2Mix[1] + hit3Mix[1] + hit4Mix[1] +
                    hit5Mix[1] + hit6Mix[1] + hit7Mix[1] + hit8Mix[1] +
                    hitSubMix[1];
        
      
        //======================LOOP-MIX=&=OUTPUTS====================//
        subBass();
        sawSqSynth();
        polySynth();
        charNoise();//output from character movement sound
        synthLine();        //arp synth line
        
        //samples
        playKeyLoop();    //arpeggiated keyboard loop
        playKeyLoopHi();  //right hand of key board loop
        playKeyLoopLow(); //left hand of keyboard loop
        
        //=============================MIX==========================//
        stereoMix[0] = keyArpMix[0] + keyHiMix[0] + keyLowMix[0] + sBassMix[0] + beatMix[0] + sq_BassMix[0] + saw_OscMix[0] + poly_OscMix[0] + sub_BassMix[0] + strings_OscMix[0];
        stereoMix[1] = keyArpMix[1] + keyHiMix[1] + keyLowMix[1] + sBassMix[1] + beatMix[1] + sq_BassMix[1] + saw_OscMix[1] + poly_OscMix[1] + sub_BassMix[1] + strings_OscMix[1];
        //==========================================================//

        
        
        //pass mix through hi resfilter
        //===================HI-RES=FILTER=MIX======================//
        hiResFilterMix[0] = hiRes[0].hires(stereoMix[0],
                                             hiResFreq, hiResQ);
        hiResFilterMix[1] = hiRes[1].hires(stereoMix[1],
                                             hiResFreq, hiResQ);
        //==========================================================//
        
        //pass mix through low pass filter
        //===================LOWRES=FILTER=MIX======================//
        lowResFilterMix[0] = lowRes[0].lores(hiResFilterMix[0],
                                             lowResFreq, lowResQ);
        lowResFilterMix[1] = lowRes[1].lores(hiResFilterMix[1],
                                             lowResFreq, lowResQ);
        //==========================================================//




        //==============================DELAY==========================================//
        delayMix[0] = delay[0].dl(lowResFilterMix[0], 44100 * delayTime, delayFeedback);
        delayMix[1] = delay[1].dl(lowResFilterMix[1], 44100 * delayTime, delayFeedback);
        //=============================================================================//

        //=======================DELAY-WET-DRY=MIX================================================//
        stereoDelayOutput[0] = delayMix[0] * (1 - dryDelayMix) + (lowResFilterMix[0] * dryDelayMix);
        stereoDelayOutput[1] = delayMix[1] * (1 - dryDelayMix) + (lowResFilterMix[1] * dryDelayMix);
        //========================================================================================//




        //========================================REVERB-CHANNEL==================================//
        for (int i = 0; i < 10; i++) {
            reverbMix[0] = reverbsL[i].dl(lowResFilterMix[0], 44100 * reverbTimes[i], reverbFeedback)/ 10;
            reverbMix[1] = reverbsR[i].dl(lowResFilterMix[1], 44100 * reverbTimes[i], reverbFeedback)/ 10;
        }
        //=========================================================================================//

        //=======================REVERB-WET/DRY-MIX===========================================//
        reverbOutput[0] = reverbMix[0] * (1 - dryReverbMix) + (lowResFilterMix[0] * dryReverbMix);
        reverbOutput[1] = reverbMix[1] * (1 - dryReverbMix) + (lowResFilterMix[1] * dryReverbMix);
        //====================================================================================//




        //============================================================//
        //------------------MASTER-OUTPUT-MIX-------------------------//
        masterStereoOutput[0] = stereoDelayOutput[0] + reverbOutput[0];
        masterStereoOutput[1] = stereoDelayOutput[1] + reverbOutput[1];
        //============================================================//

        //-----------------------------MASTER-OUTPUT-(STEREO)-----------------------------//
        output[i * nChannels] = (masterStereoOutput[0] + atmosStereoOut[0] + hitMix[0] + noiseMix[0] + sqMix[0]) * masterGain;
        
        output[i * nChannels + 1] = (masterStereoOutput[1] + atmosStereoOut[1] + hitMix[1] + noiseMix[1] + sqMix[1]) * masterGain;
        //================================================================================//
        //================================================================================//


        //==========RESET-SEQUENCER=TRIGGERS=TO=RETRIGGER==========//
        metTrigger = 0;
        
        keyArpTrigger = 0;
        keyHiTrigger = 0;
        keyLowTrigger = 0;
        subHitTrigger = 0;
        
        poly_OscTrigger = 0;
        saw_OscTrigger = 0;
        sub_BassTrigger = 0;
        sq_BassTrigger = 0;
        sBassTrigger = 0;
        
        snareTrigger = 0;
        kickTrigger = 0;
        hiHatTrigger = 0;

        //--------------------------------------//
        //pass sound ouput to array fo animation
        soundBuffer[i] = cellSinOut + (sq_FilterOut*5);
    }

}


//--------------------------------------------------------------
void AudioManager::drawWaveform() {
    
    //------------//
    ofPushMatrix();
    //ofTranslate(WORLD_WIDTH / 2, WORLD_HEIGHT / 2, -1000);
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
    
    ofSetColor(255);
    ofNoFill();
    ofDrawCircle(0, 0, radius);
    
    for (int i = 0; i < 255; i++) {

        ofColor c = ofColor(0);
        float col = ofMap(soundBuffer[i], -0.8, 0.8, 100, 255);
        c.setHsb(col, i * 10, 200);
        ofFill();
        ofSetColor(c);

        float x = (soundBuffer[i] + (10 * soundBufMix[i])) * cos(float(i)) * cellRadius;
        float y = (soundBuffer[i] + (10 * soundBufMix[i])) * sin(float(i)) * cellRadius;

        ofDrawCircle(x, y, 1);

    }
    ofPopMatrix();
    //==========//
}

//--------------------------------------------------------------
void AudioManager::update(const ofVec2f player_position) {

    player_pos = player_position;
    
    if (randomSampleTriggered) {
        if (ofGetFrameNum() % 30 == 0) {
            randomSampleTriggered = false;
        }
    }
    
    
    //==============================================//
    //the following is used to control gain and pan of an objects sound
    //control gain, will need to use vectors to calculate the distance
//    if (ofGetMouseX() < ofGetWidth()/2) {
//        gainPanX = ofMap(ofGetMouseX(), 0, ofGetWidth()/2, 0, 0.01, false);
//    } else if (ofGetMouseX() > ofGetWidth()/2) {
//        gainPanX = ofMap(ofGetMouseX(), ofGetWidth()/2, ofGetWidth(), 0.01, 0, false);
//    }
//
//    if (ofGetMouseY() < ofGetHeight()/2) {
//        gainPanY = ofMap(ofGetMouseY(), 0, ofGetHeight()/2, 0, 0.01, false);
//    } else if (ofGetMouseY() > ofGetHeight()/2) {
//        gainPanY = ofMap(ofGetMouseY(), ofGetHeight()/2, ofGetHeight(), 0.01, 0, false);
//    }
    
    
    //calculate distance between mouse and object
    ofVec2f p1(ofGetMouseX(), ofGetMouseY());
    ofVec2f p2(ofGetWidth()/2, ofGetHeight()/2);
    
    //mapping the gain to an object
    double distance = p1.distance( p2 );
    mapGain = ofMap(distance, 0, radius*2.0, 0.01, 0.0);
    
    if ( distance > radius*1.5) {
        mapGain = 0;
    }
    
    //std::cout<<mapGain<<std::endl;
    
    //this is simple but works!! control pan
    if (ofGetMouseX() < ofGetWidth()/2) {
        mousePan = ofMap(ofGetMouseX(), 0, ofGetWidth()/2, 1, 0.5, false);
    } else if (ofGetMouseX() > ofGetWidth()/2) {
        mousePan = ofMap(ofGetMouseX(), ofGetWidth()/2, ofGetWidth(), 0.5, 0, false);
    }
    
    
    //mapping the modulation to an object
    double sDistance = p1.distance( p2 );
    mapMod = ofMap(sDistance, 0, radius, 3.0, 0.01);
    //==============================================//
    
    poly_OscEnv.setAttack(0 + ((poly_LFO_Out+1)/2) * 100);
    poly_OscEnv.setDecay(200 + ((poly_LFO_Out+1)/2) * 1000);
    poly_OscEnv.setSustain(0.2);
    poly_OscEnv.setRelease(2000 + ((poly_LFO_Out+1)/2) * 10000);
    
    
    noiseEnv.setAttack(noiseAttack);
    noiseEnv.setDecay(5);
    noiseEnv.setSustain(5);
    noiseEnv.setRelease(10000);
    
    //std::cout<<patternSwitch<<std::endl;
    
    //---Pattern-Switch-Selection--//
    if (patternSwitch == 0) {
        drumPattern = 0;
        polyPattern = 0;
        sub_BassPattern = 0;
        squarePattern = 0;
        sawPattern = 0;
        synthPattern = 0;
        stringsPattern = 0;
    } else if (patternSwitch == 1) {
        drumPattern = 0;
        polyPattern = 0;
        sub_BassPattern = 0;
        squarePattern = 0;
        sawPattern = 0;
        synthPattern = 1;
    } else if (patternSwitch == 2) {
        drumPattern = 0;
        polyPattern = 0;
        sub_BassPattern = 1;
        squarePattern = 0;
        sawPattern = 0;
        synthPattern = 1;
    } else if (patternSwitch == 3) {
        drumPattern = 0;
        polyPattern = 0;
        sub_BassPattern = 1;
        squarePattern = 1;
        sawPattern = 0;
        synthPattern = 1;
    } else if (patternSwitch == 4) {
        drumPattern = 0;
        polyPattern = 0;
        sub_BassPattern = 1;
        squarePattern = 1;
        sawPattern = 1;
        synthPattern = 0;
    } else if (patternSwitch == 5) {
        drumPattern = 0;
        polyPattern = 5;
        sub_BassPattern = 1;
        squarePattern = 1;
        sawPattern = 1;
        synthPattern = 1;
    } else if (patternSwitch == 6) {
        drumPattern = 0;
        polyPattern = 5;
        sub_BassPattern = 1;
        squarePattern = 1;
        sawPattern = 0;
        synthPattern = 3;
    } else if (patternSwitch == 7) {
        drumPattern = 0;
        polyPattern = 5;
        sub_BassPattern = 1;
        squarePattern = 1;
        sawPattern = 0;
        synthPattern = 2;
    } else if (patternSwitch == 8) {
        drumPattern = 5;
        polyPattern = 0;
        sub_BassPattern = 1;
        squarePattern = 1;
        sawPattern = 1;
        synthPattern = 3;
    } else if (patternSwitch == 9) {
        drumPattern = 0;
        polyPattern = 4;
        sub_BassPattern = 1;
        squarePattern = 0;
        sawPattern = 0;
        synthPattern = 4;
    } else if (patternSwitch == 10) {
        drumPattern = 1;
        polyPattern = 1;
        sub_BassPattern = 1;
        squarePattern = 0;
        sawPattern = 0;
        synthPattern = 4;
    }
   
   
    
    
}

//--------------------------------------------------------------
void AudioManager::draw() {
    
    drawWaveform();
    
}

//--------------------------------------------------------------

void AudioManager::drawGUI(bool enable) {
    if (enable) gui.draw();
}

//--------------------------------------------------------------
void AudioManager::playRandomSample() {

    if (!randomSampleTriggered) {
        randomSampleTriggered = true;
        //int r = (int)ofRandom(0, 3);
        switch (r)
        {
        case 0:
            hit1.trigger(); //resets sample position to 0
            hit1Env.trigger = 1; //sample is triggered if = 1
                
//            hitSub.trigger(); //resets sample position to 0
//            hitSubEnv.trigger = 1; //sample is triggered if = 1
            break;
        case 1:
            hit2.trigger(); //resets sample position to 0
            hit2Env.trigger = 1; //sample is triggered if = 1
            break;
        case 2:
            hit3.trigger(); //resets sample position to 0
            hit3Env.trigger = 1; //sample is triggered if = 1
            break;
        case 3:
            hit4.trigger(); //resets sample position to 0
            hit4Env.trigger = 1; //sample is triggered if = 1
            break;
        case 4:
            hit5.trigger(); //resets sample position to 0
            hit5Env.trigger = 1; //sample is triggered if = 1
            break;
        case 5:
            hit6.trigger(); //resets sample position to 0
            hit6Env.trigger = 1; //sample is triggered if = 1
            break;
        case 6:
            hit7.trigger(); //resets sample position to 0
            hit7Env.trigger = 1; //sample is triggered if = 1
        break;
        case 7:
            hit8.trigger(); //resets sample position to 0
            hit8Env.trigger = 1; //sample is triggered if = 1
        break;
        }
        
        
        
        if (r < 8) r++;
        else r = 0;
        
    }
}

//--------------------------------------------------------------
void AudioManager::keyPressed(int key) {

    //use for interactions to trigger samples
    if (key == '1') {
        
        hitSelect++;//increment selector
        
        if (hitSelect == 9) {//reset position
            hitSelect = 1;
        }
        
        if (hitSelect == 1) {
            hit1.trigger(); //resets sample position to 0
            hit1Env.trigger = 1; //sample is triggered if = 1
            
//            hitSub.trigger(); //resets sample position to 0
//            hitSubEnv.trigger = 1; //sample is triggered if = 1
            
        } else if (hitSelect == 2) {
            hit2.trigger(); //resets sample position to 0
            hit2Env.trigger = 1; //sample is triggered if = 1
         
        } else if (hitSelect == 3) {
            hit3.trigger(); //resets sample position to 0
            hit3Env.trigger = 1; //sample is triggered if = 1
        
        } else if (hitSelect == 4) {
            hit4.trigger(); //resets sample position to 0
            hit4Env.trigger = 1; //sample is triggered if = 1
            
        } else if (hitSelect == 5) {
            hit5.trigger(); //resets sample position to 0
            hit5Env.trigger = 1; //sample is triggered if = 1
      
        } else if (hitSelect == 6) {
            hit6.trigger(); //resets sample position to 0
            hit6Env.trigger = 1; //sample is triggered if = 1

        } else if (hitSelect == 7) {
            hit7.trigger(); //resets sample position to 0
            hit7Env.trigger = 1; //sample is triggered if = 1

        } else if (hitSelect == 8) {
            hit8.trigger(); //resets sample position to 0
            hit8Env.trigger = 1; //sample is triggered if = 1

        }
       
    }
    
    
    //trigger test for character movement
    if (key == '2') {
        noiseEnv.trigger = 1;
    }
    
    
    
    
    //switch drum patterns
    if (key == '3') {
        drumPattern++;
        if (drumPattern > 4){
            drumPattern = 0;
        }
    }
    
    
    
    //switch poly synth patterns
    if (key == '4') {
        polyPattern++;
        if (polyPattern > 5){
            polyPattern = 0;
        }
    }
    
    
    
    //switch synth bass arp pattern
    if (key == '5') {
        synthPattern++;
        if (synthPattern  > 4){
            synthPattern = 0;
        }
    }
    
    //switch synth bass arp pattern
    if (key == '6') {
        squarePattern++;
        if (squarePattern  > 1){
            squarePattern = 0;
        }
    }
    

    if (key == '7') {
        sub_BassPattern++;
        if (sub_BassPattern  > 1){
            sub_BassPattern = 0;
        }
    }
     
    
    if (key == '0') {
        patternSwitch++;
        if (patternSwitch  > 10){
            patternSwitch = 0;
        }
    }

    
}

//--------------------------------------------------------------
void AudioManager::keyReleased(int key) {
    //reset envelope position to allow retrigger
    hit1Env.trigger = 0;
    hit2Env.trigger = 0;
    hit3Env.trigger = 0;
    hit4Env.trigger = 0;
    hit5Env.trigger = 0;
    hit6Env.trigger = 0;
    hit7Env.trigger = 0;
    hit8Env.trigger = 0;
    
    
    hitSubEnv.trigger = 0;
    noiseEnv.trigger = 0;
    
    
//    sq_BassEnv.trigger = 0;
}



// ==============================> EVENT FUNCTIONS <============================== //

// triggered every time a point is collected/triggered
void AudioManager::event_point_collected()
{
    //cout << "POINT COLLECTED" << endl;
}

// triggered every time a collectable pulses/emits particles
// the position of the point is passed through incase needed for panning etc
void AudioManager::event_point_pulsed(const ofVec2f point_position)
{
    //cout << "POINT PULSED: " << point_position << endl;
}

// triggered when the final 'point' in a level has been collected
void AudioManager::event_level_complete()
{
    //cout << "LEVEL COMPLETE" << endl;
}

// triggered when a new level is loaded
// this is either be by pressing 'enter' after a level is complete, or pressing 'p' at any time
// ((i haven't made many levels yet so currently the game is just cycling through procedural levels))
void AudioManager::event_new_level_loaded()
{
    //cout << "NEW LEVEL LOADED" << endl;
}

// ==================================================================================== //



//--------------------------------------------------------------
void AudioManager::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void AudioManager::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void AudioManager::mousePressed(int x, int y, int button) {
    noiseEnv.trigger = 1;
}

//--------------------------------------------------------------
void AudioManager::mouseReleased(int x, int y, int button) {
    noiseEnv.trigger = 0;
}

//--------------------------------------------------------------
void AudioManager::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void AudioManager::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void AudioManager::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void AudioManager::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void AudioManager::dragEvent(ofDragInfo dragInfo) {

}
