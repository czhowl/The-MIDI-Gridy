#include "looping.h"
#include "midi.h"
#include "settings.h"
#include "lcd.h"
#include "ui.h"
#include "ble.h"


void buttonScan(void);

uint32_t loopCounter = 0;
uint8_t loopN = 0;
uint8_t loopRecord = 0;
uint8_t loopStart = 0;
uint8_t loopExist = 0;
uint8_t menuLoop;
uint8_t modeLoop = 0;
uint8_t loopStartPause = 0;

void loopingRecord(){
    loopStart = 0;
    int l = 0;
    //configuration
    for(l = 0; l < 4; l++){
        loop.loopSetting.inst[l] = instConfig[l];
        loop.loopSetting.scale[l] = scaleConfig[l];
    };
    loop.usedLength = 0;
    //notification
    lcdUpdateGetReady();
    midiSetInstrument(0,0x59);
    for(l = 0; l < 3; l++){
        if(modeBle) bleMidiNoteOn(70, 127);
        else midiNoteOn(0, 70, 127);
        delay_ms(200);
        if(modeBle) bleMidiNoteOff(70, 127);
        else midiNoteOff(0, 70, 127);
        delay_ms(200);
    }
    //start record
    lcdUpdateRecording();
    DRV_TMR3_Start();
    DRV_TMR3_CounterClear();
    loopN = 0;
    while((loopN < 300) && (DRV_TMR3_CounterValueGet() < 5000000) && loopRecord){
        buttonScan();
    }
    loop.length = DRV_TMR3_CounterValueGet();
    lcdUpdateRf();
    if(loop.note[loopN - 1].onOff) loop.usedLength = loopN;
    loop.usedLength = loopN;
    DRV_TMR3_Stop();
    DRV_TMR3_CounterClear();
    loopExist = 1;
    loopRecord = 0;
    delay_ms(500);
    lcdUpdateLoop(menuLoop);
    freeze2 = 0;
}

void loopPlay(){
    loopCounter = DRV_TMR3_CounterValueGet();
    if((loopCounter >= loop.note[loopN].time) && loop.note[loopN].onOff){
        midiSetInstrument(2, loop.loopSetting.inst[(loop.note[loopN].pos)%4]);
        if(modeBle) bleMidiNoteOn(loop.note[loopN].pos/4 + loop.loopSetting.scale[(loop.note[loopN].pos)%4] * 12, loop.note[loopN].velocity);
        else midiNoteOn(2, loop.note[loopN].pos/4 + loop.loopSetting.scale[(loop.note[loopN].pos)%4] * 12, loop.note[loopN].velocity);
        loopN++;
    }else if(loopCounter >= loop.note[loopN].time && !(loop.note[loopN].onOff)){
        midiSetInstrument(2, loop.loopSetting.inst[(loop.note[loopN].pos)%4]); 
        if(modeBle) bleMidiNoteOff(loop.note[loopN].pos/4 + loop.loopSetting.scale[(loop.note[loopN].pos)%4] * 12, loop.note[loopN].velocity);
        else midiNoteOff(2, loop.note[loopN].pos/4 + loop.loopSetting.scale[(loop.note[loopN].pos)%4] * 12, loop.note[loopN].velocity);
        loopN++;
    }
    if(loopN == loop.usedLength){
        loopN = 0;
        DRV_TMR3_CounterClear();
    }
}

void loopClear(){
    int l = 0;
    loopStart = 0;
    loopN = 0;
    loop.usedLength = 0;
    loop.length = 0;
    for(l = 0; l < 4; l++){
        loop.loopSetting.inst[l] = 0;
        loop.loopSetting.scale[l] = 0;
    };
    loopExist = 0;
    loopCounter = 0;
    loopRecord = 0;
    loopExist = 0;
    loopStartPause = 0;
}