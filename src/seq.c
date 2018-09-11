#include "seq.h"
#include "ble.h"

uint8_t modeSeq = 0;
uint64_t sequenceSwitch = 0;
uint64_t seqRowMask = 0x111111111111;
uint64_t ring = 0x00;
uint8_t ringPos = 0;
uint64_t sequenceNote = 0;
uint8_t seqSound = 0;
uint64_t sequenceCol = 0;
uint64_t seqMask = 0x01;
uint8_t seqCounter = 0;
uint8_t modeSeqPause = 0;
uint8_t modeScra = 0;
uint64_t ringi = 0x00;
uint8_t j = 0;
uint8_t seqInst[] = {28, 28, 28, 28};

void seqPlaySound(void){
    sequenceCol = ring&sequenceSwitch;
    for(j = 0; j < 4; j++){
        sequenceNote = sequenceCol & (seqMask << j);
        if(sequenceNote){
            if(modeBle) bleMidiNoteOn(seqInst[j], 127);
            else midiNoteOn(1, seqInst[j], 127);
            delay_ms(1);
            if(modeBle) bleMidiNoteOff(seqInst[j], 127);
            else midiNoteOff(1, seqInst[j], 127);
        }
    }
    seqSound = 0;
}

void seqUpdate(void){
    if(modeSeq & !seqSound){
        ring <<= 4;
        ringPos++;
        seqMask <<= 4;
        if(ringPos == 12){
            ringPos = 0;
            ring = ringi;
            seqMask = 0x01;
        }
        seqSound = 1;
    }
}

void seqClear(void){
    modeSeqPause = 0;
    modeSeq = 0;
    sequenceSwitch = 0;
    seqRate = 9;
    seqCounter = 0;
    modeSeq = 0;
    seqRowMask = 0x111111111111;
    ring = 0x00;
    ringPos = 0;
    sequenceNote = 0;
    seqSound = 0;
    sequenceCol = 0;
    seqMask = 0x01;
    seqCounter = 0;
    modeScra = 0;
    ringi = 0x00;
}