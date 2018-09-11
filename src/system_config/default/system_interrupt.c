#define pressRight PORTDbits.RD13
#define pressLeft PORTBbits.RB15


#include <xc.h>
#include <sys/attribs.h>
#include "app.h"
#include "system_definitions.h"
#include "looping.h"
#include "midi.h"
#include "ui.h"
#include "game.h"
#include "seq.h"
#include "settings.h"
#include "ble.h"

// *****************************************************************************
// *****************************************************************************
// Section: System Interrupt Vector Functions
// *****************************************************************************
// *****************************************************************************
int led_GND[12] = {0b1111111111111110, 0b1111111111111101, 0b1111111111111011, 0b1111111111110111,
                   0b1111111111101111, 0b1111111111011111, 0b1111111110111111, 0b1111111101111111,
                   0b1111110111111111, 0b1111101111111111, 0b1011111111111111, 0b0111111111111111};
int led_anode[12] = {0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x0400,0x0800};
uint64_t curr = 0;
int ledPos =0;
uint8_t onflag = 0;
uint64_t digitalSwitch;
uint8_t flagRight = 0;
uint8_t flagLeft = 0;
uint8_t blink = 0;
uint16_t anode = 0;
uint8_t ledCounter = 0;
//Encoder
int8_t oldAB = 3;
int8_t encoderStates[] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};
int8_t encoderState;
int8_t encoderVal;
int8_t oldAB2 = 3;
int8_t encoderState2;
int8_t encoderVal2;
//num
uint8_t scaleSetFlag;


void __ISR(_CHANGE_NOTICE_VECTOR, ipl1AUTO) _IntHandlerChangeNotification(void)
{
    /* TODO: Add code to process interrupt here */
    PLIB_INT_SourceFlagClear(INT_ID_0,INT_SOURCE_CHANGE_NOTICE);
    //press button
    if(pressRight && flagRight){
        if(!modeBle) midiNoteOn(1, 27, 127);
        encoderRightClick();
        flagRight = 0;
    }else if(!pressRight){
        flagRight = 1;
    }
    if(pressLeft  && flagLeft){
        if(!modeBle) midiNoteOn(1, 27, 127);
        encoderLeftClick();
        flagLeft = 0;
    }else if(!pressLeft){
        flagLeft = 1;
    }
    //right Encoder
    if(interface == 0){
        oldAB <<= 2;
        oldAB |= (PORTD & 0xc000) >> 14;
        encoderVal += encoderStates[(oldAB & 0x0f)];
        if(encoderVal > 3){
            encoderState = '+';
            encoderRightIn();
            encoderVal = 0;
        }else if(encoderVal < -3){
            encoderState = '-';
            encoderRightDe();
            encoderVal = 0;
        }else{
            encoderState = '*';
        }
    }
    //left encoder
    else{
        oldAB2 <<= 2;
        oldAB2 |= (PORTB & 0x0030) >> 4;
        encoderVal2 += encoderStates[(oldAB2 & 0x0f)];
        if(encoderVal2 > 3){
            encoderState2 = '+';
            encoderLeftIn();
            encoderVal2 = 0;
        }else if(encoderVal2 < -3){
            encoderState2 = '-';
            encoderLeftDe();
            encoderVal2 = 0;
        }else{
            encoderState2 = '*';
        }
    }
}

void __ISR(_TIMER_1_VECTOR, ipl4AUTO) IntHandlerDrvTmrInstance0(void)
{
    PLIB_INT_SourceFlagClear(INT_ID_0,INT_SOURCE_TIMER_1);
    if(curr == 0){
        ledPos = 0;
        if(menuSelect == 3) curr = digitalSwitch;
        else if(modeGame) curr = digitalSwitch | gameSwitch;
        else if(modeSeq) curr = sequenceSwitch | ring | digitalSwitch;
        else curr = digitalSwitch;
        if(numFlag) curr = (curr & 0xeeeeeeeeeeee) | numSwitch;
    }
    if(onflag){
        PLIB_PORTS_Write (PORTS_ID_0, PORT_CHANNEL_A, led_GND[ledPos / 4]);
        anode = 0;
        do{
            if(curr & 0x0001){
                if(instSetFlag | scaleSetFlag){
                    anode |= led_anode[ledPos % 4 * 3 + rand()%4] | led_anode[ledPos % 4 * 3 + rand()%4];
                }else if(numFlag){
                    if(ledPos%4 == 0)  anode |= led_anode[ledPos % 4  * 3 + 2];
                    else anode |= led_anode[ledPos % 4 * 3 + 1] | led_anode[ledPos % 4  * 3 + 2];
                }else if(modeLoop){
                    anode |= led_anode[ledPos % 4 * 3] | led_anode[ledPos % 4  * 3 + 1];
                }else if(modeGame){
                    anode |= led_anode[ledPos % 4 * 3 + 2];
                }else{
                    if(rowConfig[ledPos % 4] == 0) anode |= led_anode[ledPos % 4 * 3] | led_anode[ledPos % 4  * 3 + 2];
                    anode |= led_anode[ledPos % 4 * 3 + 1] | led_anode[ledPos % 4  * 3 + 2];
            }}
            curr >>= 1;
            ledPos++;
        }while(ledPos % 4);
        PLIB_PORTS_Write (PORTS_ID_0, PORT_CHANNEL_D, anode);
        onflag = 0;
    }else{
        while(!(curr & 0x0f) && (curr != 0)){
            curr >>= 4;
            ledPos += 4;
        }
        onflag = 1;
        PLIB_PORTS_Write (PORTS_ID_0, PORT_CHANNEL_D, 0x00);
        PLIB_PORTS_Write (PORTS_ID_0, PORT_CHANNEL_A, 0xffff);
    }
}

void __ISR(_TIMER_2_VECTOR, ipl2AUTO) IntHandlerDrvTmrInstance1(void)
{
    PLIB_INT_SourceFlagClear(INT_ID_0,INT_SOURCE_TIMER_2);
    uint8_t j = 0;
    seqCounter++;
    if(seqCounter == rateCollection[seqRate]){
        seqUpdate();
        if(rowSetFlag){
            if(blink){
                if(menuSelect == 0)PLIB_PORTS_Write (PORTS_ID_0, PORT_CHANNEL_D, 0b101101101101);
                else if(menuSelect == 2)PLIB_PORTS_Write (PORTS_ID_0, PORT_CHANNEL_D, 0b110110110110);
                PLIB_PORTS_Write (PORTS_ID_0, PORT_CHANNEL_A, 0b1111111111111110);
                blink = 0;
            }else{
                PLIB_PORTS_Write (PORTS_ID_0, PORT_CHANNEL_A, 0x00);
                PLIB_PORTS_Write (PORTS_ID_0, PORT_CHANNEL_D, rowLed);
                blink = 1;
            }
        }
        if(instRowSelect){
            if(blink){
                if(modeLoop) PLIB_PORTS_Write (PORTS_ID_0, PORT_CHANNEL_D, 0b011011011011);
                else PLIB_PORTS_Write (PORTS_ID_0, PORT_CHANNEL_D, instLed);
                PLIB_PORTS_Write (PORTS_ID_0, PORT_CHANNEL_A, 0b1111111111111110);
                blink = 0;
            }else{
                PLIB_PORTS_Write (PORTS_ID_0, PORT_CHANNEL_A, 0x00);
                PLIB_PORTS_Write (PORTS_ID_0, PORT_CHANNEL_D, 0x00);
                blink = 1;
            }
        }
        if(scaleRowSelect){
            if(blink){
                if(modeLoop) PLIB_PORTS_Write (PORTS_ID_0, PORT_CHANNEL_D, 0b011011011011);
                else PLIB_PORTS_Write (PORTS_ID_0, PORT_CHANNEL_D, instLed);
                PLIB_PORTS_Write (PORTS_ID_0, PORT_CHANNEL_A, 0b1111111111111110);
                blink = 0;
            }else{
                PLIB_PORTS_Write (PORTS_ID_0, PORT_CHANNEL_A, 0x00);
                PLIB_PORTS_Write (PORTS_ID_0, PORT_CHANNEL_D, 0x00);
                blink = 1;
            }
        }
        seqCounter = 0;
    }
}

void __ISR(_TIMER_3_VECTOR, ipl3AUTO) IntHandlerDrvTmrInstance2(void)
{
    PLIB_INT_SourceFlagClear(INT_ID_0,INT_SOURCE_TIMER_3);
    if(seqSound) seqPlaySound();
    if(gameStart) gamePlay();
    if(loopStart) loopPlay();
}
    
void __ISR(_TIMER_5_VECTOR, ipl0AUTO) IntHandlerDrvTmrInstance3(void)
{
    PLIB_INT_SourceFlagClear(INT_ID_0,INT_SOURCE_TIMER_5);
}
/*******************************************************************************
 End of File
*/

