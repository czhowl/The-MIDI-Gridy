/*******************************************************************************
  MPLAB Harmony Project Main Source File

  Company:
    Microchip Technology Inc.
  
  File Name:
    main.c

  Summary:
    This file contains the "main" function for an MPLAB Harmony project.

  Description:
    This file contains the "main" function for an MPLAB Harmony project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state 
    machines of all MPLAB Harmony modules in the system and it calls the 
    "SYS_Tasks" function from within a system-wide "super" loop to maintain 
    their correct operation. These two functions are implemented in 
    configuration-specific files (usually "system_init.c" and "system_tasks.c")
    in a configuration-specific folder under the "src/system_config" folder 
    within this project's top-level folder.  An MPLAB Harmony project may have
    more than one configuration, each contained within it's own folder under
    the "system_config" folder.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

//Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "system/common/sys_module.h"   // SYS function prototypes
#include "driver/tmr/drv_tmr.h"
#include "driver/adc/drv_adc_static.h"
#include "midi.h"
#include "lcd.h"
#include "driver/usart/drv_usart.h"
#include "peripheral/adc/plib_adc.h"
#include "peripheral/int/plib_int.h"
#include "ui.h"
#include "system/random/sys_random.h"
#include "looping.h"
#include "game.h"
#include "settings.h"
#include "seq.h"
#include "ble.h"


// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

void delay_ms(int t);
void buttonScan(void);
void pureButtonScan(void);

//Button Scan Function
uint16_t power[12] = {0x01, 0x02, 0x04, 0x08, 0x40, 0x80, 0x100, 0x200, 0x1000, 0x2000, 0x4000, 0x8000};
uint8_t pressureSwitch[48] = {0};
uint8_t prevPressureSwitch[48] = {0};
uint64_t digitalSwitch = 0;
uint64_t prevDigitalSwitch = 0;
uint16_t key = 0;
uint64_t mask;
uint32_t pressureVal = 0;
uint8_t pressSense = 1;

int main ( void )
{
    /* Initialize all MPLAB Harmony modules, including application(s). */
    SYS_Initialize ( NULL );
    DRV_ADC_Open();
    //midi
    midiReset();
    //sequence
    DRV_TMR0_Start();
    DRV_TMR1_Start();
    DRV_TMR2_Start();
    
    midiSetChannelBank(0, VS1053_BANK_DEFAULT);
    midiSetChannelVolume(0, 127); 
    midiSetInstrument(0,24);
    midiSetChannelBank(1, VS1053_BANK_DRUMS1);
    midiSetChannelVolume(1, 127); 
    midiSetInstrument(1,0);
    midiSetChannelBank(2, VS1053_BANK_DEFAULT);
    midiSetChannelVolume(2, 127); 
    midiSetInstrument(2,24);
    
    lcdUpdate(0);
    PLIB_PORTS_Write (PORTS_ID_0, PORT_CHANNEL_A, 0x00);
    PLIB_PORTS_Write (PORTS_ID_0, PORT_CHANNEL_D, 0x00);
    int j = 0;
    while ( true )
    {   
        buttonScan();
        if(instRowSelect) instSet();
        else if(numFlag) numSet();
        else if(scaleRowSelect) scaleSet();
        if(loopRecord) loopingRecord();
    }
    /* Execution should not come here during normal operation */
    return ( EXIT_FAILURE );
}



void buttonScan(){
    int k = 0;
    int j = 0;
    mask = 0x0001;
    prevDigitalSwitch = digitalSwitch;
    for(key = 0; key < 12; key++) {
        PLIB_PORTS_Write (PORTS_ID_0, PORT_CHANNEL_G, 0);   //Clear Port G
        delay_ms(1);
        PLIB_PORTS_Write (PORTS_ID_0, PORT_CHANNEL_G, power[key]);   //Set Each Pin in Port G
        delay_ms(1);
        pressureVal = 0;   //Clear Pressure Value
        IFS1bits.AD1IF = 0;   //Clear ADC Interrupt Flag
        AD1CON1bits.ASAM = 1;   //Start Auto Sample and Auto Convert
        while(!IFS1bits.AD1IF);   //Check If 4 ADC Result Generated
        for(j = 0; j < 4; j++){
            pressureVal = DRV_ADC_SamplesRead(j);   //Read Each ADC Result on Each Row
            if(pressureVal > 0x20){ //pressed     //Check Analog Offset
                if(modeSeq & (rowConfig[j] == 2)){ //Check If in Sequencing Mode
                    if((prevDigitalSwitch & mask) == 0){  //Check If Button Was Not Pressed Previously
                        if((sequenceSwitch & mask) == 0){  //Check If the Button Is Off
                            sequenceSwitch |= mask;     //Toggle On Button
                        }else{  //Previously Pressed
                            sequenceSwitch &= ~mask;   //Toggle Off Button
                        }
                    }
                }
                digitalSwitch |= mask;
                if(pressSense) pressureSwitch[k] = pressureVal/8;
                else pressureSwitch[k] = 127;
            }else{
                digitalSwitch &= ~mask;
                pressureSwitch[k] = 0;
            }
            if(modeGame){
                if((digitalSwitch & mask) != (prevDigitalSwitch & mask)){
                    midiSetInstrument(0, gameInst[k % 4]);
                    if(digitalSwitch & mask){
                        if(modeBle) bleMidiNoteOn(gameScale[k], 127);
                        else midiNoteOn(0, gameScale[k], 127);
                        prevPressureSwitch[k] = pressureSwitch[k];
                    }else{
                        if(modeBle) bleMidiNoteOff(gameScale[k], 127);
                        else midiNoteOff(0, gameScale[k], 127);
                    }
                }
            }else{
                if(rowConfig[k % 4] != 2){
                    if((digitalSwitch & mask) != (prevDigitalSwitch & mask)){
                        midiSetInstrument(0, instConfig[k % 4]);
                        if(digitalSwitch & mask){
                            if(modeBle) bleMidiNoteOn(k / 4 + scaleConfig[k % 4] * 12, pressureSwitch[k]);
                            else midiNoteOn(0,k / 4 + scaleConfig[k % 4] * 12, pressureSwitch[k]);     //Switch note for each column
                            if(loopRecord && (loopN < 198)){
                                loop.note[loopN].onOff = 1;
                                loop.note[loopN].time = DRV_TMR3_CounterValueGet();
                                loop.note[loopN].velocity = pressureSwitch[k];
                                loop.note[loopN].pos = k;
                                loopN++;
                            }
                            prevPressureSwitch[k] = pressureSwitch[k];
                        }else{
                            if(modeBle) bleMidiNoteOff(k / 4 + scaleConfig[k % 4] * 12, pressureSwitch[k]);
                            else midiNoteOff(0, k / 4 + scaleConfig[k % 4] * 12, prevPressureSwitch[k]);
                            if(loopRecord && (loopN < 199)){
                                loop.note[loopN].onOff = 0;
                                loop.note[loopN].time = DRV_TMR3_CounterValueGet();
                                loop.note[loopN].velocity = prevPressureSwitch[k];
                                loop.note[loopN].pos = k;
                                loopN++;
                            }
                        }
                    }
                }
            }
            mask <<= 1;
            k++;
        }
        ADC1BUF0 = 0;
        ADC1BUF1 = 0;
        ADC1BUF2 = 0;
        ADC1BUF3 = 0;
    }
}

void pureButtonScan(void){
    int k = 0;
    int j = 0;
    mask = 0x0001;
    prevDigitalSwitch = digitalSwitch;
    for(key = 0; key < 12; key++) {
        PLIB_PORTS_Write (PORTS_ID_0, PORT_CHANNEL_G, 0);   //Clear Port G
        delay_ms(1);
        PLIB_PORTS_Write (PORTS_ID_0, PORT_CHANNEL_G, power[key]);   //Set Each Pin in Port G
        delay_ms(1);
        pressureVal = 0;   //Clear Pressure Value
        IFS1bits.AD1IF = 0;   //Clear ADC Interrupt Flag
        AD1CON1bits.ASAM = 1;   //Start Auto Sample and Auto Convert
        while(!IFS1bits.AD1IF);   //Check If 4 ADC Result Generated
        for(j = 0; j < 4; j++){
            pressureVal = DRV_ADC_SamplesRead(j);   //Read Each ADC Result on Each Row
            if(pressureVal > 0x20){ //pressed     //Check Analog Offset
                digitalSwitch |= mask;
                if(pressSense) pressureSwitch[k] = pressureVal/8;
                else pressureSwitch[k] = 127;
            }else{
                digitalSwitch &= ~mask;
                pressureSwitch[k] = 0;
            }
            mask <<= 1;
            k++;
        }
        ADC1BUF0 = 0;
        ADC1BUF1 = 0;
        ADC1BUF2 = 0;
        ADC1BUF3 = 0;
    }
}