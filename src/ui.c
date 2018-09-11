#include "midi.h"
#include "driver/tmr/drv_tmr.h"
#include "lcd.h"
#include "ui.h"
#include "looping.h"
#include "game.h"
#include "settings.h"
#include "seq.h"
#include "ble.h"

uint8_t menu = 0;
uint8_t menuSelect = 4;
uint8_t interface = 0;
uint8_t menu2 = 0;
uint8_t menu2Select = 10;
uint8_t menuSeq = 0;
uint8_t menuGame = 0;
uint8_t menuLoop = 0;
uint8_t menuLoopOption = 0;
uint8_t loopOption = 0;
uint8_t menuFree = 0;
uint8_t u = 0;
uint8_t pressSense;

uint8_t freeze = 0;
uint8_t freeze2 = 0;

void encoderRightClick(){
    if(!freeze){
    if(interface){
        if(menuSelect == 4)lcdUpdate(menu);
        else if(menuSelect == 0)lcdUpdateFree(menuFree);
        else if(menuSelect == 1)lcdUpdateGame(menuGame);
        else if(menuSelect == 2)lcdUpdateSeq(menuSeq);
        else if(menuSelect == 3)lcdUpdateLoop(menuLoop);
        interface = 0;
    }else{
        if(menuSelect == 4){
            if(menu == 0){
                menuSelect = 0;
                rowSet();
                lcdUpdateFree(0);
                menuFree = 0;
            }else if(menu == 1){
                lcdUpdateGame(0);
                modeGame = 1;
                menuSelect = 1;
                menuGame = 0;
            }else if(menu == 2){
                menuSelect = 2;
                rowSet();
                lcdUpdateSeq(0);
                menuSeq = 0;
            }else if(menu == 3){
                modeLoop = 1;
                menuSelect = 3;
                lcdUpdateLoop(0);
                menuLoop = 0;
            }
        }else if(menuSelect == 0){     //FREE MODE
            if(menuFree == 0){
                if(instRowSelect == 0){
                    freeze2 = 1;
                    instRowSelect = 1;
                }else{
                    freeze2 = 0;
                    instSetFlag = 0;
                    instRowSelect = 0;
                    lcdUpdateFree(menuFree);
                }
            }else if(menuFree == 1){
                freeze2 = 1;
                rowSet();
                lcdUpdateFree(menuFree);
                freeze2 = 0;
            }else if(menuFree == 2){
                lcdUpdate(menu);
                menuSelect = 4;
            }else if(menuFree == 3){    //scale
                if(scaleRowSelect == 0){
                    freeze2 = 1;
                    scaleRowSelect = 1;
                }else{
                    freeze2 = 0;
                    scaleSetFlag = 0;
                    scaleRowSelect = 0;
                    lcdUpdateFree(menuFree);
                }
            }
        }else if(menuSelect == 1){     //GAME MODE INCREMENTSSSSSsssSSSSSSS
            if(menuGame == 0){    //ss
                if(!gameStart){
                    //loopPlay = 0;
                    gameStart = 1;
                    DRV_TMR3_Start();
                    lcdUpdateGame(menuGame);
                }else{
                    DRV_TMR3_Stop();
                    DRV_TMR3_CounterClear();
                    gameClear();
                    lcdUpdateGame(menuGame);
                }
            }else if(menuGame == 1){    //pp
                if(gameStart){
                    DRV_TMR3_Stop();
                    gameStart = 0;
                    lcdUpdateGame(menuGame);
                    
                }else{
                    DRV_TMR3_Start();
                    gameStart = 1;
                    lcdUpdateGame(menuGame);
                }
            }else if(menuGame == 2){
                if(gameStart){
                    gameStart = 0;
                    DRV_TMR3_Stop();
                    DRV_TMR3_CounterClear();
                }
                gameClear();
                lcdUpdate(menu);
                menuSelect = 4;
            }
        }else if(menuSelect == 2){     //SEQ MODE INCREMENTSSSSSsssSSSSSSS
            if(menuSeq == 0){
                if(numFlag == 0){
                    freeze2 = 1;
                    numFlag = 1;
                    level = seqRate;
                    lcdUpdateRate();
                }else{
                    freeze2 = 0;
                    numFlag = 0;
                    lcdUpdateSeq(menuSeq);
                }
            }else if(menuSeq == 1){ //back
                lcdUpdate(menu);
                menuSelect = 4;
            }else if(menuSeq == 2){  //start sequencing
                if(!modeSeq){
                    ring = ringi;
                    modeSeq = 1;
                    lcdUpdateSeq(menuSeq);
                }else{
                    modeSeqPause = 0;
                    modeSeq = 0;
                    sequenceSwitch = 0;
                    lcdUpdateSeq(menuSeq);
                }
            }else if(menuSeq == 3){//select row again
                freeze2 = 1;
                rowSet();
                ring &= ringi << ringPos;
                lcdUpdateSeq(menuSeq);
                freeze2 = 0;
            }else if(menuSeq == 4){
                if(modeSeqPause && !modeSeq){
                    modeSeqPause = 0;
                    modeSeq = 1;
                    lcdUpdateSeq(menuSeq);
                    DRV_TMR1_Start();
                }else if(!modeSeqPause && modeSeq){
                    modeSeqPause = 1;
                    modeSeq = 0;
                    lcdUpdateSeq(menuSeq);
                    DRV_TMR1_Stop();
                }
            }else if(menuSeq == 5){
                if(modeScra){
                    freeze2 = 0;
                    modeScra = 0;
                    lcdUpdateSeq(menuSeq);
                }else{
                    freeze2 = 1;
                    modeScra = 1;
                    lcdUpdateScra();
                }
            }else if(menuSeq == 6){
                if(instRowSelect == 0){
                    instRowSelect = 1;
                    freeze2 = 1;
                }else{
                    freeze2 = 0;
                    instSetFlag = 0;
                    instRowSelect = 0;
                    lcdUpdateSeq(menuSeq);
                }
            }
        }else if(menuSelect == 3){     //LOOP MODE INCREMENTSSSSSsssSSSSSSS
            if(menuLoop == 0){     //scale
                if(scaleRowSelect == 0){
                    freeze2 = 1;
                    scaleRowSelect = 1;
                }else{
                    freeze2 = 0;
                    scaleSetFlag = 0;
                    scaleRowSelect = 0;
                    lcdUpdateLoop(menuLoop);
                }
            }else if(menuLoop == 1){     //record
                if(loopOption){
                    loopOption = 0;
                    if(menuLoopOption)loopRecord = 1;
                    else{
                        lcdUpdateLoop(menuLoop);
                        freeze2 = 0;
                    }
                }else{
                    if(loopRecord) loopRecord = 0;
                    else{
                        freeze2 = 1;
                        if(loopExist){
                            lcdUpdateLe();
                            delay_ms(500);
                            lcdUpdateLoopOption(menuLoopOption);
                            loopOption = 1;
                        }else loopRecord = 1;
                    }
                }
            }else if(menuLoop == 2){    //pp
                if(loopExist){
                    if(loopStartPause && !loopStart){
                        loopStartPause = 0;
                        loopStart = 1;
                        lcdUpdateLoop(menuLoop);
                        DRV_TMR3_Start();
                    }else if(!loopStartPause && loopStart){
                        loopStartPause = 1;
                        loopStart = 0;
                        lcdUpdateLoop(menuLoop);
                        DRV_TMR3_Stop();
                    }
                }
            }else if(menuLoop == 3){ //back
                lcdUpdate(menu);
                menuSelect = 4;
                modeLoop = 0;
            }else if(menuLoop == 4){    //inst
                if(instRowSelect == 0){
                    instRowSelect = 1;
                    freeze2 = 1;
                }else{
                    freeze2 = 0;
                    instSetFlag = 0;
                    instRowSelect = 0;
                    lcdUpdateLoop(menuLoop);
                }
            }else if(menuLoop == 5){      //ss
                if(loopExist){
                    if(!loopStart){
                        loopStart = 1;
                        loopN = 0;
                        DRV_TMR3_Start();
                        lcdUpdateLoop(menuLoop);
                    }else{
                        loopStartPause = 0;
                        loopStart = 0;
                        DRV_TMR3_Stop();
                        DRV_TMR3_CounterClear();
                        lcdUpdateLoop(menuLoop);
                    }
                }
            }
        }
    }}
}

void encoderRightIn(){
    if(modeScra){
        ring <<= 4;
        ringPos++;
        seqMask <<= 4;
        if(ringPos == 12){
            ringPos = 0;
            ring = ringi;
            seqMask = 0x01;
        }
        seqSound = 1;
    }else{
        if(menuSelect == 4){
            if(!modeBle) midiNoteOn(1, 32, 127);
            menu++;
            if(menu == 4)menu = 0;
            lcdUpdate(menu);
        }else if(menuSelect == 0){     //FREE MODE INCREMENTSSSSSsssSSSSSSS
            if(instSetFlag){
                instConfig[instChanged]++;
                if(instConfig[instChanged] == 129) instConfig[instChanged] = 1;
                midiSamplePlay();
            }else if(instRowSelect);
            else if(scaleSetFlag){
                if(scaleConfig[scaleChanged] != 9) scaleConfig[scaleChanged]++;
                midiSamplePlay();
            }else if(scaleRowSelect);
            else{
                if(!modeBle) midiNoteOn(1, 32, 127);
                menuFree++;
                if(menuFree == 4)menuFree = 0;
                lcdUpdateFree(menuFree);
            }
        }else if(menuSelect == 1){     //GAME MODE INCREMENTSSSSSsssSSSSSSS
            if(!modeBle) midiNoteOn(1, 32, 127);
            menuGame++;
            if(menuGame == 3)menuGame = 0;
            lcdUpdateGame(menuGame);
        }else if(menuSelect == 2){     //SEQ MODE INCREMENTSSSSSsssSSSSSSS
            if(instSetFlag){
                seqInst[instChanged]++;
                if(seqInst[instChanged] == 88) seqInst[instChanged] = 27;
                midiSamplePlay();
            }else if(instRowSelect);
            else if(numFlag){  //change rate
                if(level != 11){
                    if(!modeBle) midiNoteOn(1, 32, 127);
                    level++;
                    seqRate = level;
                    seqCounter = 0;
                }
            }
            else{
                if(!modeBle) midiNoteOn(1, 32, 127);
                menuSeq++;
                if(menuSeq == 7)menuSeq = 0;
                lcdUpdateSeq(menuSeq);
            }
        }else if(menuSelect == 3){     //LOOP MODE INCREMENTSSSSSsssSSSSSSS
            if(instSetFlag){
                instConfig[instChanged]++;
                if(instConfig[instChanged] == 129) instConfig[instChanged] = 1;
                midiSamplePlay();
            }else if(instRowSelect);
            else if(scaleSetFlag){
                if(scaleConfig[scaleChanged] != 9) scaleConfig[scaleChanged]++;
                midiSamplePlay();
            }else if(scaleRowSelect);
            else if(loopOption){
                if(!modeBle) midiNoteOn(1, 32, 127);
                menuLoopOption = !menuLoopOption;
                lcdUpdateLoopOption(menuLoopOption);
            }else{
                if(!modeBle) midiNoteOn(1, 32, 127);
                menuLoop++;
                if(menuLoop == 6)menuLoop = 0;
                lcdUpdateLoop(menuLoop);
            }
        }
    }
}

void encoderRightDe(){
    if(modeScra){
        ring >>= 4;
        ringPos--;
        seqMask >>= 4;
        if(ringPos == 0xff){
            ringPos = 11;
            ring = ringi << 44;
            seqMask = 0x100000000000;
        }
        seqSound = 1;
    }else{
        if(menuSelect == 4){
            if(!modeBle) midiNoteOn(1, 32, 127);
            menu--;
            if(menu == 0xff)menu = 3;
            lcdUpdate(menu);
        }else if(menuSelect == 0){     //FRE MODE INCREMENTSSSSSsssSSSSSSS
            if(instSetFlag){
                instConfig[instChanged]--;
                if(instConfig[instChanged] == 0) instConfig[instChanged] = 128;
                midiSamplePlay();
            }else if(instRowSelect);
            else if(scaleSetFlag){
                if(scaleConfig[scaleChanged] != 0) scaleConfig[scaleChanged]--;
                midiSamplePlay();
            }else if(scaleRowSelect);
            else{
                if(!modeBle) midiNoteOn(1, 32, 127);
                menuFree--;
                if(menuFree == 0xff)menuFree = 3;
                lcdUpdateFree(menuFree);
            }
        }else if(menuSelect == 1){     //GAME MODE INCREMENTSSSSSsssSSSSSSS
            if(!modeBle) midiNoteOn(1, 32, 127);
            menuGame--;
            if(menuGame == 0xff)menuGame = 2;
            lcdUpdateGame(menuGame);
        }else if(menuSelect == 2){     //SEQ MODE INCREMENTSSSSSsssSSSSSSS
            if(instSetFlag){
                seqInst[instChanged]--;
                if(seqInst[instChanged] == 26) seqInst[instChanged] = 87;
                midiSamplePlay();
            }else if(instRowSelect);
            else if(numFlag){  //change rate
                if(level != 0){
                    if(!modeBle) midiNoteOn(1, 32, 127);
                    level--;
                    seqRate = level;
                    seqCounter = 0;
                }
            }else{
                if(!modeBle) midiNoteOn(1, 32, 127);
                menuSeq--;
                if(menuSeq == 0xff)menuSeq = 6;
                lcdUpdateSeq(menuSeq);
            }
        }else if(menuSelect == 3){     //LOOP MODE INCREMENTSSSSSsssSSSSSSS
            if(instSetFlag){
                instConfig[instChanged]--;
                if(instConfig[instChanged] == 0) instConfig[instChanged] = 128;
                midiSamplePlay();
            }else if(instRowSelect);
            else if(scaleSetFlag){
                if(scaleConfig[scaleChanged] != 0) scaleConfig[scaleChanged]--;
                midiSamplePlay();
            }else if(scaleRowSelect);
            else if(loopOption){
                if(!modeBle) midiNoteOn(1, 32, 127);
                menuLoopOption = !menuLoopOption;
                lcdUpdateLoopOption(menuLoopOption);
            }else{
                if(!modeBle) midiNoteOn(1, 32, 127);
                menuLoop--;
                if(menuLoop == 0xff)menuLoop = 5;
                lcdUpdateLoop(menuLoop);
            }
        }
    }
}

void encoderLeftClick(){
    if(!freeze2){
    if(!interface){
        menu2 = 0;
        lcdUpdate2(0);
        interface = 1;
    }else{
        if(menu2 == 0){
            if(numFlag == 0){
                freeze = 1;
                numFlag = 1;
                level = vol;
            }else{
                freeze = 0;
                numFlag = 0;
                vol = level;
            }
        }else if(menu2 == 1){
            if(modeBle) modeBle = 0;
            else modeBle = 1;
            lcdUpdate2(menu2);
        }else if(menu2 == 2){
            freeze = 1;
            DRV_TMR3_Stop();
            DRV_TMR3_CounterClear();
            vol = 11;
            seqClear();
            loopClear();
            gameClear();
            midiReset();
            for(u = 0; u < 4; u++){
                rowConfig[u] = 0;
                scaleConfig[u] = 2+u;
                instConfig[u] = 1;
                seqInst[u] = 28;
            }
            rowLed = 0;
            modeBle = 0;
            pressSense = 1;
            menuSelect = 4;
            lcdUpdate2(menu2);
            freeze = 0;
        }else if(menu2 == 3){
            if(pressSense) pressSense = 0;
            else pressSense = 1;
            lcdUpdate2(menu2);
        }
    }}
}

void encoderLeftIn(void){
    if(!modeBle) midiNoteOn(1, 32, 127);
    if(numFlag && (menu2 == 0)){  //change vol
        if(level != 11) level++;
        midiSetChannelVolume(0, volCollection[level]);
        midiSetChannelVolume(1, volCollection[level]);
        midiSetChannelVolume(2, volCollection[level]);
    }else{
        menu2++;
        if(menu2 == 4)menu2 = 0;
        lcdUpdate2(menu2);
    }
}

void encoderLeftDe(void){
    if(!modeBle) midiNoteOn(1, 32, 127);
    if(numFlag && (menu2 == 0)){  //change vol
        if(level != 0) level--;
        midiSetChannelVolume(0, volCollection[level]);
        midiSetChannelVolume(1, volCollection[level]);
        midiSetChannelVolume(2, volCollection[level]);
    }else{
        menu2--;
        if(menu2 == 0xff)menu2 = 3;
        lcdUpdate2(menu2);
    }
}

void midiSamplePlay(){
    uint8_t backSeq = 0;
    uint8_t backLoop = 0;
    if(modeSeq){
        modeSeq = 0;
        backSeq = 1;
    }
    if(loopStart){
        loopStart = 0;
        backLoop = 1;
    }
    uint16_t color = 0;
    while(!color) color = 0x07 & rand();
    DRV_TMR0_Stop();
    if(instSetFlag) midiSetInstrument(0,instConfig[instChanged]);
    else if(scaleSetFlag) midiSetInstrument(0,instConfig[scaleChanged]);
    PLIB_PORTS_Write (PORTS_ID_0, PORT_CHANNEL_D, color|(color<<3)|(color<<6)|(color<<9));
    if(menuSelect == 2){
        midiNoteOn(1, seqInst[instChanged], 127);
    }else if(instSetFlag){
        midiNoteOn(0, 70, 127);
    }else if(scaleSetFlag){
        if(modeBle) bleMidiNoteOn(scaleConfig[scaleChanged], 127);
        else midiNoteOn(0, scaleConfig[scaleChanged] * 12 + 5, 127);
    }
    PLIB_PORTS_Write (PORTS_ID_0, PORT_CHANNEL_A, 0x00);
    delay_ms(100);
    if(menuSelect == 2){
        midiNoteOff(1, seqInst[instChanged], 127);
    }else if(instSetFlag){
        midiNoteOff(0, 70, 127);
    }else if(scaleSetFlag){
        if(modeBle) bleMidiNoteOff(scaleConfig[scaleChanged], 127);
        else midiNoteOff(0, scaleConfig[scaleChanged] * 12 + 5, 127);
    }
    DRV_TMR0_Start();
    if(backSeq){
        modeSeq = 1;
    }
    if(backLoop){
        loopStart = 1;
    }
}