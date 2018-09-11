#include "ble.h"

char * itoc(uint8_t);

char *blereset = "ATZ";
char *bleEn = "AT+BLEMIDIEN=1";
char *bleNoteOn = "AT+BLEMIDITX=90";
char *bleNoteOff = "AT+BLEMIDITX=80";
char buffer[3] = {'-', 0, 0};
uint8_t modeBle = 0;

bool outCommand(void){
    while (*bleCommand != '\0'){
        while(PLIB_USART_TransmitterBufferIsFull(USART_ID_2));
        PLIB_USART_TransmitterByteSend(USART_ID_2, *bleCommand);
        bleCommand++;
    }
    return false;
}

bool bleReset(void){
    bleCommand = blereset;
    return outCommand();
}

bool bleMidiEn(void){
    bleCommand = bleEn;
    return outCommand();
}

bool bleMidiNoteOn(uint8_t note, uint8_t vel){
    bleCommand = bleNoteOn;
    outCommand();
    bleCommand = itoc(note);
    outCommand();
    bleCommand = itoc(vel);
    outCommand();
    while(PLIB_USART_TransmitterBufferIsFull(USART_ID_2));
    PLIB_USART_TransmitterByteSend(USART_ID_2, '\n');
}

bool bleMidiNoteOff(uint8_t note, uint8_t vel){
    bleCommand = bleNoteOff;
    outCommand();
    bleCommand = itoc(note);
    outCommand();
    bleCommand = itoc(vel);
    outCommand();
    while(PLIB_USART_TransmitterBufferIsFull(USART_ID_2));
    PLIB_USART_TransmitterByteSend(USART_ID_2, '\n');
}

char * itoc(uint8_t i) {
    if(i/16 == 0) buffer[1] = '0';
    else if(i/16 == 1) buffer[1] = '1';
    else if(i/16 == 2) buffer[1] = '2';
    else if(i/16 == 3) buffer[1] = '3';
    else if(i/16 == 4) buffer[1] = '4';
    else if(i/16 == 5) buffer[1] = '5';
    else if(i/16 == 6) buffer[1] = '6';
    else if(i/16 == 7) buffer[1] = '7';
    if(i%16 == 0) buffer[2] = '0';
    else if(i%16 == 1) buffer[2] = '1';
    else if(i%16 == 2) buffer[2] = '2';
    else if(i%16 == 3) buffer[2] = '3';
    else if(i%16 == 4) buffer[2] = '4';
    else if(i%16 == 5) buffer[2] = '5';
    else if(i%16 == 6) buffer[2] = '6';
    else if(i%16 == 7) buffer[2] = '7';
    else if(i%16 == 8) buffer[2] = '8';
    else if(i%16 == 9) buffer[2] = '9';
    else if(i%16 == 10) buffer[2] = 'A';
    else if(i%16 == 11) buffer[2] = 'B';
    else if(i%16 == 12) buffer[2] = 'C';
    else if(i%16 == 13) buffer[2] = 'D';
    else if(i%16 == 14) buffer[2] = 'E';
    else if(i%16 == 15) buffer[2] = 'F';
    return &buffer[0];
}