/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _EXAMPLE_FILE_NAME_H    /* Guard against multiple inclusion */
#define _EXAMPLE_FILE_NAME_H


#include "system/ports/sys_ports.h"

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif


   
#define lcdReset PORTFbits.RF7
#define lcdSelect PORTEbits.RE4  //no need for this
#define SCLK PORTFbits.RF6
#define SDO PORTFbits.RF8

    void lcdInit(void);
    void lcdSendInst(unsigned char);
    void lcdSendData(unsigned char);
    void lcdDisplayPic(unsigned char*);
    void lcdSetPageAddr(unsigned char);
    void lcdSetColumnAddr(unsigned char);
    void lcdClear(void);
    void delay(int);
    void delay_ms(int);
    void lcdDisplayMenu(unsigned char ,unsigned char ,unsigned char ,unsigned char*);
    void lcdUpdate(unsigned char);
    void lcdUpdate2(unsigned char);
    void lcdUpdateSeq(unsigned char);
    void lcdUpdateFree(unsigned char);
    void lcdUpdateLoop(unsigned char);
    void lcdUpdateGame(unsigned char);
    void lcdUpdateRow(void);
    void lcdUpdateInst(void);
    void lcdUpdateIcon(void);
    void lcdUpdateIcon2(void);
    void lcdUpdateRate(void);
    void lcdUpdateRecord(void);
    void lcdUpdateScra(void);
    void lcdUpdateLe(void);
    void lcdUpdateLoopOption(unsigned char state);
    void lcdUpdateGetReady(void);
    void lcdUpdateRecording(void);
    void lcdUpdateRf(void);
    void lcdUpdateScale(void);
    
    unsigned char startup[1920];
    unsigned char freeplay[138];
    unsigned char game[92];
    unsigned char lp[130];
    unsigned char seq[180];
    unsigned char ss[150];
    unsigned char UI[1920];
    unsigned char trans[1920];
    unsigned char rows[184];
    unsigned char length[110];
    unsigned char UI2[1920];
    unsigned char pp[170];
    unsigned char inst[176];
    unsigned char bluetooth[10];
    unsigned char back[74];
    unsigned char arrow[28];
    unsigned char scratch[114];
    unsigned char rate[66];
    unsigned char scale[74];
    unsigned char music[12];
    unsigned char reset[82];
    unsigned char erase[80];
    unsigned char ml[12];
    unsigned char ms[12];
    unsigned char re[206];
    unsigned char record[110];
    unsigned char mg [12];
    unsigned char keep [72];
    unsigned char off[10];
    unsigned char le[180];
    unsigned char on[10];
    unsigned char rf[252];
    unsigned char getready[156];
    unsigned char recording[160];

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
