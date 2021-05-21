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



    //audio mix
    gui.add(masterGain.set("master gain", 0.5, 0, 3));
    //------------------------------------------------//
    gui.add(metGain.set("metronome gain", 0.1, 0, 1.0));
    //------------------------------------------------//
    gui.add(sampleHitGain.set("sample Hit Gain", 1.95, 0.0, 4.0));
    gui.add(atmosGain.set("atmos gain", 2.0, 0, 7.0));
    //------------------------------------------------//



    //------------------------------------------------//
    gui.add(synthLineGain.set("synth line gain", 0.7, 0, 2.0));
    gui.add(synthLinePan.set("synth line Pan", 0.05, 0, 1.0));

    gui.add(subBassGain.set("sub Bass Gain", 0.2, 0.0, 1.0));
    gui.add(polySynthGain.set("poly Synth Gain", 0.5, 0.0, 1.0));
    gui.add(polySynthPan.set("poly Synth Pan", 0.215, 0.0, 1.0));

    gui.add(noiseAttack.set("Character Noise Attack", 1500, 0.0, 20000));
    gui.add(noiseLevel.set("Noise Level", 0.009, 0.0, 0.05));

    gui.add(squareBassGain.set("square Bass Gain", 0.1, 0.0, 1.0));

    gui.add(sawSynthGain.set("saw Synth Gain", 0.4, 0.0, 1.0));

    gui.add(stringsSynthGain.set("strings Synth Gain ", 0.4, 0.0, 1.0));
    gui.add(stringsSynthPan.set("strings Synth Pan", 0.215, 0.0, 1.0));



    //------------------------------------------------//
    gui.add(beatGain.set("beat gain", 1.2, 0.0, 4.0));
    //------------------------------------------------//
    gui.add(lowResFreq.set("low-Res filter freq", 4000, 0, 4000));
    gui.add(lowResQ.set("low-Res Filter Q", 0.5, 0, 0));

    gui.add(hiResFreq.set("hi-Res filter freq", 00, 0, 500));
    gui.add(hiResQ.set("hi-Res Filter Q", 0.5, 0, 0));
    //------------------------------------------------//
    gui.add(dryDelayMix.set("delay Wet/Dry Mix", 0.7, 0, 1));
    gui.add(delayFeedback.set("delay feedback", 0.5, 0, 0.9));
    gui.add(delayTime.set("delay time", 0.9, 0, 2));
    //------------------------------------------------//
    gui.add(dryReverbMix.set("reverb Wet/Dry Mix", 0.7, 0, 1));
    gui.add(reverbFeedback.set("reverb feedback", 0.95, 0, 0.99));
    //------------------------------------------------//

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

}

//--------------------------------------------------------------
void AudioManager::playMetronome() {

    //---------SETUP-METRONOME-&-SEQUENCE-TRIGGER---------//
    met_out = metronome.playOnce(metSpeed);
    met_envOut = metEnv.ar(met_out, 0.1, 1.9, 1, metTrigger) * metGain;

    if (metTrigger == 1) {
        //set playback position to start of sample
        metronome.trigger();
    }

    //return met_envOut;
    metStereo.stereo(met_envOut, metMix, 0.1);
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
    hit1_out = hit1.playOnce() * (1.8 * sampleHitGain);     //play & gain
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
void AudioManager::beats() {

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
void AudioManager::charNoise() {

    //noise sound for movement
    noise_envOut = noiseEnv.adsr(1., noiseEnv.trigger);
    noise_out = (noiseOsc.noise() * noise_envOut) * noiseLevel;
    noiseFilter_out = noiseFilter.lores(noise_out, 200 + noise_envOut * 1000, 5);

    noiseStereo.stereo(noiseFilter_out, noiseMix,
        (noisePanner.sinewave(0.25 * noise_envOut) + 2) / 5);
}

//-----------------------------------//
void AudioManager::synthLine() {

    //synth arp sound
    if (synthPattern == 1) {
        sBassDynamicVolume = -0.5;
    }
    else if (synthPattern == 5) {
        sBassDynamicVolume = -0.4;
    }
    else {
        sBassDynamicVolume = 0.0;
    }

    sBass_envOut = sBassEnv.adsr(1, sBassTrigger);
    sBassOut = (sBass.sinewave(sBassPitch) * sBass_envOut) * (synthLineGain + sBassDynamicVolume);

    synthDelay_out = synthDelay.dl(sBassOut, 44100 * 0.9, 0.4);
    sBassStereo.stereo(synthDelay_out, sBassMix, synthLinePan);
}


//-----------------------------------//
void AudioManager::polySynth() {

    //--------------//
    //--POLY-SYNTH--//
    //--------------//

    if (polyPattern == 4) {
        playerDynamicPolyVolume = -0.3;
        playerDynamicPolyFilter = 0;
        playerDynamicPolyFilter = 200;
    }
    else if (polyPattern == 1) {
        playerDynamicPolyVolume = 0.01;
        playerDynamicPolyFilter = 400;
    }
    else if (polyPattern == 6) {
        playerDynamicPolyVolume = 0.01;
        playerDynamicPolyFilter = 600;
    }
    else {
        playerDynamicPolyVolume = 0;
        playerDynamicPolyFilter = 500;
    }


    poly_Osc_envOut = poly_OscEnv.adsr(1., poly_OscTrigger);
    poly_LFO_Out = poly_LFO.sinebuf(0.2);
    poly_Osc1Out = poly_Osc1.pulse(poly_Osc1Pitch, 0.2);

    poly_OscOut = ((poly_Osc.pulse(poly_OscPitch, 0.6) + poly_Osc1Out) * poly_Osc_envOut) * (polySynthGain + playerDynamicPolyVolume);

    poly_OscFilter_out = poly_OscFilter.lores(poly_OscOut, playerDynamicPolyFilter + poly_Osc_envOut + (poly_LFO_Out * 200), poly_Osc_envOut * 2);

    poly_OscDelay_out = poly_Delay.dl(poly_OscFilter_out, 44100 * 0.9, 0.4);//?

    poly_OscStereo.stereo(poly_OscDelay_out, poly_OscMix, polySynthPan);
    //---------------//
}

//-----------------------------------//
void AudioManager::sawSqSynth() {

    //--------------//
    //SAWTOOTH-SYNTH//
    //--------------//
    saw_Osc_envOut = saw_OscEnv.adsr(1., saw_OscTrigger);

    saw_LFO_Out = saw_LFO.sinewave(saw_Osc_envOut * 10);

    saw_Osc1Out = saw_Osc1.saw(saw_Osc1Pitch);
    saw_Osc2Out = saw_Osc2.saw(saw_Osc2Pitch);

    saw_OscOut = (((saw_Osc.saw(saw_OscPitch) + saw_Osc1Out + saw_Osc2Out) * saw_Osc_envOut) * saw_LFO_Out) * sawSynthGain;

    saw_OscDelay_out = saw_Delay.dl(saw_OscOut, 44100 * 0.9, 0.2);//?

    saw_OscFilter_out = saw_OscFilter.lores(saw_OscOut, 500 + saw_Osc_envOut * 100, 0);

    saw_OscStereo.stereo(saw_OscFilter_out, saw_OscMix, 0.7);
    //---------------//

    //---------------//
    //SQUARE-BASS-LINE
    //---------------//
    sq_Bass_envOut = sq_BassEnv.adsr(1., sq_BassTrigger);
    sq_BassOut = (sq_Bass.square(sq_BassPitch) * sq_Bass_envOut) * squareBassGain;

    sq_Delay_out = sq_Delay.dl(sq_BassOut, 44100 * 0.9, 0.2);

    sq_BassFilter_out = sq_BassFilter.lores(sq_BassOut, 200 + sq_Bass_envOut * 200, 5 + sq_Bass_envOut * 10);

    sq_BassStereo.stereo(sq_BassFilter_out, sq_BassMix, 0.1);
    //---------------//
}

//-----------------------------------//
void AudioManager::subBass() {
    //---------------//
    //SUB-BASS-LINE
    //---------------//
    sub_Bass_envOut = sub_BassEnv.adsr(1., sub_BassTrigger);
    sub_BassOut = (sub_Bass.sinewave(sub_BassPitch) * sub_Bass_envOut) * subBassGain;
    sub_BassStereo.stereo(sub_BassOut, sub_BassMix, 0.4);
    //---------------//
}


//-----------------------------------//
void AudioManager::stringsSynth() {
    //--------------//
    //SAWTOOTH-SYNTH//
    //--------------//

    if (stringsPattern == 3) {
        stringsDynamicVolume = -0.01;
        stringsDynamicFilterFreq = 500;
    }
    else {
        stringsDynamicVolume = 0;
        stringsDynamicFilterFreq = 1000;
    }
    strings_Osc_envOut = strings_OscEnv.adsr(1., strings_OscTrigger);

    strings_LFO_Out = strings_LFO.sinewave(strings_Osc_envOut);

    strings_OscOut = (strings_Osc.saw(strings_OscPitch + strings_LFO_Out) * strings_Osc_envOut) * 0.08;
    strings_Osc1Out = (strings_Osc1.saw(strings_Osc1Pitch + strings_LFO_Out * 2) * strings_Osc_envOut) * 0.08;
    strings_Osc2Out = (strings_Osc2.saw(strings_Osc2Pitch + strings_LFO_Out * 4) * strings_Osc_envOut) * 0.08;

    strings_OscDelay_out = strings_Delay.dl(strings_OscOut, 44100 * 0.9, 0.2);//?


    strings_OscFilter_out = strings_OscFilter.hires(strings_OscOut, 500 + strings_Osc_envOut * 10, 0);
    strings_OscFilter_out1 = strings_OscFilter1.hires(strings_Osc1Out, 500 + strings_Osc_envOut * 10, 0);

    strings_OscOutMix = (strings_OscOut + strings_Osc1Out + strings_Osc2Out) * (stringsSynthGain + stringsDynamicVolume);
    strings_OscFilter_out2 = strings_OscFilter.lores(strings_OscOutMix, stringsDynamicFilterFreq - strings_Osc_envOut * 1000, 2);


    strings_OscStereo.stereo(strings_OscOutMix, strings_OscMix, stringsSynthPan);
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
            //this ensures chord progresions are synchronised
            pattern_trigger = pattern_triggerLine[playHead % 64];
            pattern_triggerB = pattern_triggerLineB[playHead % 16];



            //TRIGGER SUB HIT SAMPLE
            if (subHitTrigger == 1) {
                hitSub.trigger();
            }
            //-----------------------------------//

            //------------BEATS---------------//
            //----------kick drum-------------//
            //switch trigger pattern sequence
            if (drumPattern == 1) {
                kickTrigger = kickSeq[playHead % 32];
            }
            else if (drumPattern == 2) {
                kickTrigger = kickSeq2[playHead % 32];
            }
            else if (drumPattern == 3) {
                kickTrigger = kickSeq3[playHead % 32];
            }

            //TRIGGER KICK DRUM
            if (kickTrigger == 1) {
                kick.trigger();
            }


            //----------hi hats----------------//
            //switch trigger pattern sequence
            if (drumPattern == 1) {
                hiHatTrigger = hiHatSeq[playHead % 32];
            }
            else if (drumPattern == 2) {
                hiHatTrigger = hiHatSeq2[playHead % 32];
            }
            else if (drumPattern == 3) {
                hiHatTrigger = hiHatSeq3[playHead % 32];
            }
            else if (drumPattern == 4) {
                hiHatTrigger = hiHatSeq4[playHead % 32];
            }
            else if (drumPattern == 5) {
                hiHatTrigger = hiHatSeq5[playHead % 32];
            }


            if (hiHatTrigger == 1) {
                hiHat.trigger();
                //change hi hat release value
                hiHatRelease = hiHatReleaseSeq[hiHatCount];
                hiHatCount++;
                if (hiHatCount > 9) {
                    hiHatCount = 0;
                }
            }

            //-----------snare-----------------//
            if (drumPattern == 1) {
                snareTrigger = snareSeq[playHead % 32];
            }
            else if (drumPattern == 2) {
                snareTrigger = snareSeq2[playHead % 32];
            }
            else if (drumPattern == 3) {
                snareTrigger = snareSeq3[playHead % 32];
            }
            else if (drumPattern == 4) {
                snareTrigger = snareSeq4[playHead % 32];
            }

            //reset snare sample to start
            if (snareTrigger == 1) {
                snare.trigger();
            }



            //-----------SIDE STICK SAMPLE--------------//
            metSpeed = metSpeedSeq[playHead % 16];

            if (drumPattern == 2) {
                metTrigger = metSeq3[playHead % 16];
            }
            else if (drumPattern == 5) {
                metTrigger = metSeq2[playHead % 16];

            }


            //--------------------------------//
            //USE FOR TRIGGERING SYNTH LINE
            //switch trigger pattern sequence
            if (synthPattern == 1) {
                sBassTrigger = sBassLineTrigger[playHead % 16];
            }
            else if (synthPattern == 2 || synthPattern == 3) {
                sBassTrigger = sBassLineTrigger2[playHead % 16];
            }
            else if (synthPattern == 4) {
                sBassTrigger = sBassLineTrigger3[playHead % 32];
            }
            else if (synthPattern == 5) {
                sBassTrigger = sBassLineTrigger4[playHead % 16];
            }


            //switch pitch pattern sequence

            if (sBassTrigger == 1) {

                if (synthPattern == 1) {

                    sBassPitch = mtof.mtof(sBassLinePitch[sBassNote]);
                    sBassNote++; //increment the pitch line
                    if (sBassNote > 0) {
                        sBassNote = 0;
                    }

                }
                else if (synthPattern == 2) {

                    sBassPitch = mtof.mtof(sBassLinePitch2[sBassNote]);
                    sBassNote++; //increment the pitch line
                    if (sBassNote > 5) {
                        sBassNote = 0;
                    }

                }
                else if (synthPattern == 3) {

                    sBassPitch = mtof.mtof(sBassLinePitch3[sBassNote]);
                    sBassNote++; //increment the pitch line
                    if (sBassNote > 5) {
                        sBassNote = 0;
                    }

                }
                else if (synthPattern == 4) {

                    sBassPitch = mtof.mtof(sBassLinePitch4[sBassNote] - 24);
                    sBassNote++; //increment the pitch line
                    if (sBassNote > 11) {
                        sBassNote = 0;
                    }

                }
                else if (synthPattern == 5) {

                    sBassPitch = mtof.mtof(sBassLinePitch5[sBassNote]);
                    sBassNote++; //increment the pitch line
                    if (sBassNote > 15) {
                        sBassNote = 0;
                    }

                }
            }

            //--------------------SQAURE-BASS--------------------//
            if (squarePattern == 1) {
                sq_BassTrigger = sq_BassLineTrigger[playHead % 64];
            }
            else if (squarePattern == 2) {
                sq_BassTrigger = sq_BassLineTriggerB[playHead % 64];
            }


            if (sq_BassTrigger == 1) {

                if (squarePattern == 1) {
                    sq_BassPitch = mtof.mtof(sq_BassLinePitch[sequenceNote]);

                }
                else if (squarePattern == 2) {
                    sq_BassPitch = mtof.mtof(sq_BassLinePitchB[sq_BassNote]);

                    sq_BassNote++; //increment the pitch line
                    if (sq_BassNote > 0) {
                        sq_BassNote = 0;
                    }

                }


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
            }
            //=============================================================//


            //--------------------STRINGS-SOUND-----------------------------//
            if (stringsPattern == 1 || stringsPattern == 2) {
                strings_OscTrigger = strings_OscLineTrigger[playHead % 64];
            }
            else if (stringsPattern == 3) {
                strings_OscTrigger = pattern_triggerLineB[playHead % 16];
            }


            if (strings_OscTrigger == 1) {

                if (stringsPattern == 1) {

                    strings_OscPitch = mtof.mtof(strings_OscLinePitch[sequenceNote]);
                    strings_Osc1Pitch = mtof.mtof(strings_OscLinePitch2[sequenceNote]);
                    strings_Osc2Pitch = mtof.mtof(strings_OscLinePitch3[sequenceNote]);

                }
                else if (stringsPattern == 2) {
                    strings_OscPitch = mtof.mtof(strings_OscLinePitchB[sequenceNote]);
                    strings_Osc1Pitch = mtof.mtof(strings_OscLinePitch2B[sequenceNote]);
                    strings_Osc2Pitch = mtof.mtof(strings_OscLinePitch3B[sequenceNote]);
                }
                else if (stringsPattern == 3) {
                    strings_OscPitch = mtof.mtof(strings_OscLinePitchC[sequenceNoteB]);
                    strings_Osc1Pitch = mtof.mtof(strings_OscLinePitch2C[sequenceNoteB]);
                    strings_Osc2Pitch = mtof.mtof(strings_OscLinePitch3C[sequenceNoteB]);
                }
            }
            //===================================//



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
            }
            else if (polyPattern == 2) {
                poly_OscTrigger = poly_OscLineTrigger2[playHead % 16];
            }
            else if (polyPattern == 3) {
                poly_OscTrigger = poly_OscLineTrigger3[playHead % 16];
            }
            else if (polyPattern == 4) {
                poly_OscTrigger = poly_OscLineTrigger4[playHead % 16];
            }
            else if (polyPattern == 5) {
                poly_OscTrigger = poly_OscLineTrigger[playHead % 16];
            }
            else if (polyPattern == 6) {
                poly_OscTrigger = poly_OscLineTrigger[playHead % 16];
            }

            if (poly_OscTrigger == 1) {

                if (polyPattern == 1 || polyPattern == 2) {
                    poly_OscPitch = mtof.mtof(poly_OscLinePitch[poly_OscNote]);
                    poly_Osc1Pitch = mtof.mtof(poly_OscLinePitch[poly_OscNote] + 12);
                    //saw_Osc2Pitch = mtof.mtof(saw_OscLinePitch[saw_OscNote]+3);

                    poly_OscNote++; //increment the pitch line
                    if (poly_OscNote > 0) {
                        poly_OscNote = 0;
                    }

                }
                else if (polyPattern == 3) {
                    poly_OscPitch = mtof.mtof(poly_OscLinePitch2[poly_OscNote]);
                    poly_Osc1Pitch = mtof.mtof(poly_OscLinePitch2[poly_OscNote] + 12);

                    poly_OscNote++; //increment the pitch line
                    if (poly_OscNote > 5) {
                        poly_OscNote = 0;
                    }
                }
                else if (polyPattern == 4) {
                    poly_OscPitch = mtof.mtof(poly_OscLinePitch3[poly_OscNote]);
                    poly_Osc1Pitch = mtof.mtof(poly_OscLinePitch3[poly_OscNote] + 12);

                    poly_OscNote++; //increment the pitch line
                    if (poly_OscNote > 9) {
                        poly_OscNote = 0;
                    }
                }
                else if (polyPattern == 5) {
                    poly_OscPitch = mtof.mtof(poly_OscLinePitch4[sequenceNote - 1]);
                    poly_Osc1Pitch = mtof.mtof(poly_OscLinePitch4b[sequenceNote - 1]);

                }
                else if (polyPattern == 6) {
                    poly_OscPitch = mtof.mtof(poly_OscLinePitch5[sequenceNoteB - 1]);
                    poly_Osc1Pitch = mtof.mtof(poly_OscLinePitch5[sequenceNoteB - 1] + 12);

                }

            }
            //=============================================================//
            if (pattern_triggerB == 1) {
                sequenceNoteB++;
                if (sequenceNoteB > 3) {
                    sequenceNoteB = 0;
                }
            }

            if (pattern_trigger == 1) {
                sequenceNote++;
                if (sequenceNote > 3) {
                    sequenceNote = 0;
                }
            }

            playHead++;//iterate the playhead

        } //end of clock
        //===================//
        //===================//



        //--------------------Beat-Stereo-Mix-------------------------/
        //beat pre mix
        playMetronome();
        beats();
        beatMix[0] = (kickMix[0] + hiHatMix[0] + snareMix[0] + metMix[0]) * beatGain;
        beatMix[1] = (kickMix[1] + hiHatMix[1] + snareMix[1] + metMix[1]) * beatGain;

        //===============SAMPLE-HIT-MIX=============================//
        sampleHits(); //load samples and pass through mix
        hitMix[0] = hit1Mix[0] + hit2Mix[0] + hit3Mix[0] + hit4Mix[0] +
            hit5Mix[0] + hit6Mix[0] + hit7Mix[0] + hit8Mix[0] +
            hitSubMix[0];

        hitMix[1] = hit1Mix[1] + hit2Mix[1] + hit3Mix[1] + hit4Mix[1] +
            hit5Mix[1] + hit6Mix[1] + hit7Mix[1] + hit8Mix[1] +
            hitSubMix[1];


        //======================LOOP-MIX=&=OUTPUTS====================//
        stringsSynth();
        subBass();
        sawSqSynth();
        polySynth();
        charNoise();   //output from character movement sound
        synthLine();   //arp synth line


        //=============================MIX==========================//
        stereoMix[0] = sBassMix[0] + beatMix[0] + sq_BassMix[0] + saw_OscMix[0] + poly_OscMix[0] + sub_BassMix[0] + strings_OscMix[0];
        stereoMix[1] = sBassMix[1] + beatMix[1] + sq_BassMix[1] + saw_OscMix[1] + poly_OscMix[1] + sub_BassMix[1] + strings_OscMix[1];
        //==========================================================//



        //pass mix through hi resfilter
        //===================HI-RES=FILTER=MIX======================//
        hiResFilterMix[0] = hiRes[0].hires(stereoMix[0],
            playerFilterHires, hiResQ);
        hiResFilterMix[1] = hiRes[1].hires(stereoMix[1],
            playerFilterHires, hiResQ);
        //==========================================================//

        //pass mix through low pass filter
        //===================LOWRES=FILTER=MIX======================//
        lowResFilterMix[0] = lowRes[0].lores(hiResFilterMix[0],
            playerFilter, lowResQ);
        lowResFilterMix[1] = lowRes[1].lores(hiResFilterMix[1],
            playerFilter, lowResQ);
        //==========================================================//

        //pass mix through delay channels
        //==============================DELAY==========================================//
        delayMix[0] = delay[0].dl(lowResFilterMix[0], 44100 * delayTime, playerDelayFeedback);
        delayMix[1] = delay[1].dl(lowResFilterMix[1], 44100 * delayTime, playerDelayFeedback);
        //=============================================================================//

        //=======================DELAY-WET-DRY=MIX================================================//
        stereoDelayOutput[0] = delayMix[0] * (1 - playerDelayMix) + (lowResFilterMix[0] * playerDelayMix);
        stereoDelayOutput[1] = delayMix[1] * (1 - playerDelayMix) + (lowResFilterMix[1] * playerDelayMix);
        //========================================================================================//




        //========================================REVERB-CHANNEL==================================//
        for (int i = 0; i < 10; i++) {
            reverbMix[0] = reverbsL[i].dl(lowResFilterMix[0], 44100 * reverbTimes[i], reverbFeedback) / 10;
            reverbMix[1] = reverbsR[i].dl(lowResFilterMix[1], 44100 * reverbTimes[i], reverbFeedback) / 10;
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
        output[i * nChannels] = (masterStereoOutput[0] + atmosStereoOut[0] + hitMix[0] + noiseMix[0]) * masterGain; //+ sqMix[0]

        output[i * nChannels + 1] = (masterStereoOutput[1] + atmosStereoOut[1] + hitMix[1] + noiseMix[1]) * masterGain; //+ sqMix[1]
        //================================================================================//
        //================================================================================//


        //==========RESET-SEQUENCER=TRIGGERS=TO=RETRIGGER==========//
        poly_OscTrigger = 0;
        saw_OscTrigger = 0;
        sub_BassTrigger = 0;
        sq_BassTrigger = 0;
        sBassTrigger = 0;
        strings_OscTrigger = 0;

        snareTrigger = 0;
        kickTrigger = 0;
        hiHatTrigger = 0;
        metTrigger = 0;
        subHitTrigger = 0;


        hit1Env.trigger = 0;
        hit2Env.trigger = 0;
        hit3Env.trigger = 0;
        hit4Env.trigger = 0;
        hit5Env.trigger = 0;
        hit6Env.trigger = 0;
        hit7Env.trigger = 0;
        hit8Env.trigger = 0;
        //--------------------------------------//

    }
}


//--------------------------------------------------------------
void AudioManager::update(const ofVec2f player_position) {

    player_pos = player_position;

    //MAP-PLAYER-POSITION-TO-SOUND-EFFECTS-AND-ENVELOPES
    //-------------------------------------//
    //set low pass filter to player y pos, top of map all frequencies pass bottom of map apply filter effect
    double partWorldHeight = HALF_WORLD_HEIGHT / 3;
    if (player_pos.y > partWorldHeight) {
        playerFilter = ofMap(player_pos.y, partWorldHeight, HALF_WORLD_HEIGHT, 4000, 20);
    }
    else {
        playerFilter = 4000;
    }
    //-------------------------------------//

    playerDelayFeedback = ofMap(player_pos.y, HALF_WORLD_HEIGHT, -HALF_WORLD_HEIGHT, 0.1, 0.8);
    playerDelayMix = ofMap(player_pos.y, HALF_WORLD_HEIGHT, -HALF_WORLD_HEIGHT, 0.9, 0.1);

    //set hi res filter to activate at the top and bottom of WORLD
    if (player_pos.y > partWorldHeight) {
        playerFilterHires = ofMap(player_pos.y, partWorldHeight, HALF_WORLD_HEIGHT, 0, 100.0);
    }
    else if (player_pos.y < -partWorldHeight * 2) {
        playerFilterHires = ofMap(player_pos.y, -partWorldHeight * 2, -HALF_WORLD_HEIGHT, 0, 400.0);
    }
    else {
        playerFilterHires = 0;
    }

    //-------------------------------------//
    //synth line attack and release are afected when player is in the left and right segments of WORLD WIDTH
    double partWorldWidth = HALF_WORLD_WIDTH / 3;
    if (player_pos.x > partWorldWidth) {
        playerSynthLineAttack = ofMap(player_pos.x, partWorldWidth, HALF_WORLD_WIDTH, 3, 500);
        playerSynthLineRelease = ofMap(player_pos.x, partWorldWidth, HALF_WORLD_WIDTH, 600, 700);
    }
    else if (player_pos.x < -partWorldWidth) {
        playerSynthLineAttack = ofMap(player_pos.x, -partWorldWidth, -HALF_WORLD_WIDTH, 3, 500);
        playerSynthLineRelease = ofMap(player_pos.x, -partWorldWidth, -HALF_WORLD_WIDTH, 600, 200);
    }
    else {
        playerSynthLineAttack = 3;
        playerSynthLineRelease = 600;
    }
    //-------------------------------------//


    //--UPDATE ENVELOPES--//
    //use to apply modulation and player positioning
    poly_OscEnv.setAttack(0 + ((poly_LFO_Out + 1) / 2) * 100);
    poly_OscEnv.setDecay(200 + ((poly_LFO_Out + 1) / 2) * 1000);
    poly_OscEnv.setSustain(0.2);
    poly_OscEnv.setRelease(2000 + ((poly_LFO_Out + 1) / 2) * 10000);

    noiseEnv.setAttack(noiseAttack);
    noiseEnv.setDecay(5);
    noiseEnv.setSustain(5);
    noiseEnv.setRelease(10000);

    sBassEnv.setAttack(playerSynthLineAttack);
    sBassEnv.setDecay(2);
    sBassEnv.setSustain(2);
    sBassEnv.setRelease(playerSynthLineRelease);
    //--------------------------------------------------//



    //-------------------------------------//
    //USED TO SELECT RANDOM HIT SAMPLE
    if (randomSampleTriggered) {
        if (ofGetFrameNum() % 30 == 0) {
            randomSampleTriggered = false;
        }
    }
    //==============================================//




    //---PATTERN-SWITCH-SELECTION-LOGIC-//
    if (patternSwitch == 0) {
        drumPattern = 0;
        polyPattern = 0;
        sub_BassPattern = 0;
        squarePattern = 0;
        sawPattern = 0;
        synthPattern = 0;
        stringsPattern = 0;
    }
    else if (patternSwitch == 1) {
        drumPattern = 0;
        polyPattern = 0;
        sub_BassPattern = 0;
        squarePattern = 0;
        sawPattern = 0;
        synthPattern = 1;
        stringsPattern = 0;
    }
    else if (patternSwitch == 2) {
        drumPattern = 0;
        polyPattern = 0;
        sub_BassPattern = 1;
        squarePattern = 0;
        sawPattern = 0;
        synthPattern = 1;
        stringsPattern = 0;
    }
    else if (patternSwitch == 3) {
        drumPattern = 0;
        polyPattern = 0;
        sub_BassPattern = 1;
        squarePattern = 1;
        sawPattern = 0;
        synthPattern = 1;
        stringsPattern = 0;
    }
    else if (patternSwitch == 4) {
        drumPattern = 0;
        polyPattern = 0;
        sub_BassPattern = 1;
        squarePattern = 1;
        sawPattern = 1;
        synthPattern = 0;
    }
    else if (patternSwitch == 5) {
        drumPattern = 0;
        polyPattern = 5;
        sub_BassPattern = 1;
        squarePattern = 1;
        sawPattern = 1;
        synthPattern = 1;
        stringsPattern = 0;
    }
    else if (patternSwitch == 6) {
        drumPattern = 0;
        polyPattern = 5;
        sub_BassPattern = 1;
        squarePattern = 1;
        sawPattern = 0;
        synthPattern = 3;
        stringsPattern = 0;
    }
    else if (patternSwitch == 7) {
        drumPattern = 0;
        polyPattern = 5;
        sub_BassPattern = 1;
        squarePattern = 1;
        sawPattern = 0;
        synthPattern = 2;
        stringsPattern = 0;
    }
    else if (patternSwitch == 8) {
        drumPattern = 5;
        polyPattern = 0;
        sub_BassPattern = 1;
        squarePattern = 1;
        sawPattern = 1;
        synthPattern = 3;
        stringsPattern = 0;
    }
    else if (patternSwitch == 9) {
        drumPattern = 0;
        polyPattern = 4;
        sub_BassPattern = 1;
        squarePattern = 0;
        sawPattern = 0;
        synthPattern = 4;
        stringsPattern = 0;
    }
    else if (patternSwitch == 10) {
        drumPattern = 1;
        polyPattern = 1;
        sub_BassPattern = 1;
        squarePattern = 0;
        sawPattern = 0;
        synthPattern = 4;
        stringsPattern = 0;
    }
    else if (patternSwitch == 11) {
        drumPattern = 1;
        polyPattern = 1;
        sub_BassPattern = 1;
        squarePattern = 0;
        sawPattern = 0;
        synthPattern = 4;
        stringsPattern = 2;
    }
    else if (patternSwitch == 12) {
        drumPattern = 1;
        polyPattern = 1;
        sub_BassPattern = 1;
        squarePattern = 2;
        sawPattern = 0;
        synthPattern = 4;
        stringsPattern = 2;
    }
    else if (patternSwitch == 13) {
        drumPattern = 0;
        polyPattern = 6;
        sub_BassPattern = 0;
        squarePattern = 0;
        sawPattern = 0;
        synthPattern = 0;
        stringsPattern = 0;
    }
    else if (patternSwitch == 14) {
        drumPattern = 0;
        polyPattern = 6;
        sub_BassPattern = 0;
        squarePattern = 0;
        sawPattern = 0;
        synthPattern = 0;
        stringsPattern = 3;
    }
    else if (patternSwitch == 15) {
        drumPattern = 2;
        polyPattern = 6;
        sub_BassPattern = 0;
        squarePattern = 0;
        sawPattern = 0;
        synthPattern = 5;
        stringsPattern = 3;
    }
    else if (patternSwitch == 16) {
        drumPattern = 3;
        polyPattern = 0;
        sub_BassPattern = 1;
        squarePattern = 2;
        sawPattern = 0;
        synthPattern = 5;
        stringsPattern = 0;
    }


}

//--------------------------------------------------------------
void AudioManager::draw() {

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

        }
        else if (hitSelect == 2) {
            hit2.trigger(); //resets sample position to 0
            hit2Env.trigger = 1; //sample is triggered if = 1

        }
        else if (hitSelect == 3) {
            hit3.trigger(); //resets sample position to 0
            hit3Env.trigger = 1; //sample is triggered if = 1

        }
        else if (hitSelect == 4) {
            hit4.trigger(); //resets sample position to 0
            hit4Env.trigger = 1; //sample is triggered if = 1

        }
        else if (hitSelect == 5) {
            hit5.trigger(); //resets sample position to 0
            hit5Env.trigger = 1; //sample is triggered if = 1

        }
        else if (hitSelect == 6) {
            hit6.trigger(); //resets sample position to 0
            hit6Env.trigger = 1; //sample is triggered if = 1

        }
        else if (hitSelect == 7) {
            hit7.trigger(); //resets sample position to 0
            hit7Env.trigger = 1; //sample is triggered if = 1

        }
        else if (hitSelect == 8) {
            hit8.trigger(); //resets sample position to 0
            hit8Env.trigger = 1; //sample is triggered if = 1

        }

    }

    //------------------------------------------------------//

    //move up or down through the patterns

    if (key == '0')
    {
        set_pattern(patternSwitch + 1);
    }
    else if (key == '9')
    {
        set_pattern(patternSwitch - 1);
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
}



// ==========================> EVENT FUNCTIONS <============================== //

// triggered every time a point is collected/triggered
void AudioManager::event_point_collected()
{
    //cout << "POINT COLLECTED" << endl;
    playRandomSample(); // play random hit sample when point collected
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

    hitSub.trigger(); //reset sample position to allow for retriggering
    subHitTrigger = 1; // play sub hit sample when all items collected

}

// triggered when a new level is loaded
// this is either be by pressing 'enter' after a level is complete, or pressing 'p' at any time
// ((i haven't made many levels yet so currently the game is just cycling through procedural levels))
void AudioManager::event_new_level_loaded()
{
    //cout << "NEW LEVEL LOADED" << endl;

    //once level has finished change audio pattern
    increment_pattern();
}

void AudioManager::event_player_started_moving()
{
    noiseEnv.trigger = 1;
}
void AudioManager::event_player_stopped_moving()
{
    noiseEnv.trigger = 0;
}

void AudioManager::set_pattern(const int pattern)
{
    patternSwitch = pattern;
    //cout << "AudioManager: pattern: " << patternSwitch << endl;
}

void AudioManager::increment_pattern()
{
    patternSwitch++;
    if (patternSwitch > 16) {
        patternSwitch = 0;
    }
    //cout << "AudioManager: pattern: " << patternSwitch << endl;
}

//========================================================================= //



//--------------------------------------------------------------
void AudioManager::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void AudioManager::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void AudioManager::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void AudioManager::mouseReleased(int x, int y, int button) {

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
