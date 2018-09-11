#include "settings.h"

uint8_t level = 3;
uint8_t vol = 11;
uint8_t seqRate = 9;
uint8_t scaleChanged = 0;
uint8_t numFlag = 0;
uint8_t rateCollection[] = {12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
uint8_t scaleCollection[] = {13, 10, 27, 34, 41, 48, 55, 62, 69, 76, 83, 90};
uint8_t volCollection[] = {17, 27, 37, 47, 57, 67, 77, 87, 97, 107, 117, 127};
uint8_t scaleConfig[4] = {5, 5, 5, 5};
uint8_t scaleSetFlag = 0;
uint8_t scaleRowSelect = 0;
//inst
uint16_t instLed = 0;
uint8_t instSetFlag = 0;
uint8_t instRowSelect = 0;
uint8_t instConfig[4] = {1,5,10,15};
uint8_t instChanged = 0;
//row
char rowConfig[4] = {0,0,0,0};
uint8_t rowSetFlag = 1;
uint16_t rowLed = 0;

uint64_t numSwitch = 0;

uint64_t digitalSwitch;

void rowSet(){
    lcdUpdateRow();
    DRV_TMR0_Stop();
    uint8_t setFlag = 0;
    uint8_t row = 0;
    uint8_t noSeq = 0;
    rowSetFlag = 1;
    while(!setFlag){
        pureButtonScan();
        for(row = 0; row < 4; row++){
            if(digitalSwitch & (0x01 << row)){
                if(menuSelect == 0)rowConfig[row] = 0;
                else if(menuSelect == 2)rowConfig[row] = 2;
                else if(menuSelect == 3)rowConfig[row] = 3;
                setFlag = 1;
                PLIB_PORTS_Write (PORTS_ID_0, PORT_CHANNEL_D, 0x00);
                PLIB_PORTS_Write (PORTS_ID_0, PORT_CHANNEL_A, 0x00);
                delay_ms(1);
                if(rowConfig[row] == 0){
                    PLIB_PORTS_Write (PORTS_ID_0, PORT_CHANNEL_D, (0b101 << (row*3)));
                    rowLed &= ~(0b111 << (row*3));
                    rowLed |= 0b101 << (row*3);
                }else if(rowConfig[row] == 2){
                    PLIB_PORTS_Write (PORTS_ID_0, PORT_CHANNEL_D, (0b110 << (row*3)));
                    rowLed &= ~(0b111 << (row*3));
                    rowLed |= 0b110 << (row*3);
                }else if(rowConfig[row] == 3){
                    PLIB_PORTS_Write (PORTS_ID_0, PORT_CHANNEL_D, (0b011 << (row*3)));
                    rowLed &= ~(0b111 << (row*3));
                    rowLed |= 0b011 << (row*3);
                }
                rowSetFlag = 0;
                delay_ms(1000);
                PLIB_PORTS_Write (PORTS_ID_0, PORT_CHANNEL_D, 0x00);
                PLIB_PORTS_Write (PORTS_ID_0, PORT_CHANNEL_A, 0x00);
                break;
    }}}
    DRV_TMR0_Start();
    for(row = 0; row < 4; row++){
        if(rowConfig[row] == 2){
            ringi |= 0b01 << row;
            noSeq++;
        }else{
            ringi &= ~(0b01 << row);
            sequenceSwitch &= ~(seqRowMask << row);
        }
    }
    if(!noSeq) modeSeq = 0;
}


void numSet(){
    uint8_t num = 0;
    digitalSwitch = 0;
    while(numFlag){
        while(num < (level+1)){
            numSwitch <<= 4;
            numSwitch |= 0x01;
            num++;
        }
        delay_ms(100);
        num = 0;
        numSwitch = 0;
    }
}

void instSet(){     //wait for press row
    lcdUpdateInst();
    DRV_TMR0_Stop();
    uint8_t row = 0;
    uint8_t rowinMode = 0;
    uint8_t rowCheck = 0x01;
    instChanged = 0;
    instLed = 0;
    for(row = 0; row < 4; row++){
        if(rowConfig[row] == menuSelect){
            instLed |= rowLed & (0b0111 << (row*3));
            rowinMode |= 0b01 << row;
    }}
    if(modeLoop) rowinMode = 0x0f;
    while(instRowSelect){
        pureButtonScan();
        for(row = 0; row < 4; row++){
            if(digitalSwitch & rowinMode){   //if row pressed
                while(rowCheck != (digitalSwitch & rowinMode)){
                    rowCheck <<= 1;
                    instChanged++;
                }
                instSetFlag = 1;
                DRV_TMR0_Start();  //start shuffling light
                while(instSetFlag){ 
                    digitalSwitch <<= 4;
                    digitalSwitch |= 0x0f & rand();
                    delay_ms(50);
                }
                digitalSwitch = 0;
                break;
    }}} 
}

void scaleSet(){     //wait for press row
    lcdUpdateScale();
    DRV_TMR0_Stop();
    uint8_t row = 0;
    uint8_t rowinMode = 0;
    uint8_t rowCheck = 0x01;
    scaleChanged = 0;
    instLed = 0;
    for(row = 0; row < 4; row++){
        if(rowConfig[row] == menuSelect){
            instLed |= rowLed & (0b0111 << (row*3));
            rowinMode |= 0b01 << row;
    }}
    while(scaleRowSelect){
        pureButtonScan();
        for(row = 0; row < 4; row++){
            if(digitalSwitch & rowinMode){   //if row pressed
                while(rowCheck != (digitalSwitch & rowinMode)){
                    rowCheck <<= 1;
                    scaleChanged++;
                }
                scaleSetFlag = 1;
                DRV_TMR0_Start();  //start shuffling light
                while(scaleSetFlag){ 
                    digitalSwitch <<= 4;
                    digitalSwitch |= 0x0f & rand();
                    delay_ms(50);
                }
                digitalSwitch = 0;
                break;
    }}} 
}