#include "lcd.h"
#include "ui.h"
#include "looping.h"

uint8_t modeSeq;
uint8_t modeGame;

uint8_t modeBle;
uint8_t pressSense;

void lcdInit(){
    lcdReset = 1;
    delay(1);
    lcdReset = 0;
    delay(1);
    lcdReset = 1;
    delay(1);
    lcdSelect = 1;
    lcdSendInst(0xE2); //system reset
    delay_ms(200);
    lcdSendInst(0x23); //multiples rate: 96, set temperature consenpation
    lcdSendInst(0xC8); //set LCD mapping control MX=0, MY=1;
    lcdSendInst(0xE8); //set bias:1/10.7bias
    lcdSendInst(0x81); //set gain and potentiometer
    lcdSendInst(0x14); //set contrast level
    lcdSendInst(0x2f); //power control:internal
    delay_ms(50);
    lcdSendInst(0x40); //set start line
    lcdSendInst(0x90); 
    lcdSendInst(0x89); //set RAM addr control
    lcdSendInst(0xAF);
    lcdSendInst(0xA4);
    //lcdSendInst(0xA5);
    delay_ms(200);
    lcdSendInst(0xA4);
    lcdClear();
    lcdDisplayPic(startup);
}

void lcdSendInst(unsigned char cmd){
    int i;
    //lcdSelect = 1;
    delay(1);
    SCLK=0;
    delay(1);
    SDO=0;   //SDO=0;CMD
    delay(1);
    SCLK=1;
    for(i = 0; i < 8; i++){
        delay(2);
        SCLK=0;
        delay(1);
        if(cmd & 0x80) SDO=1;
        else SDO=0;
        delay(1);
        SCLK=1;
        cmd <<= 1;
    }
    //lcdSelect = 0;
    delay(1);
}

void lcdSendData(unsigned char data){
    int i;
    //lcdSelect = 1;
    delay(1);
    SCLK=0;
    delay(1);
    SDO=1;   //SDO=1;DATA
    delay(1);
    SCLK=1;
    for(i = 0; i < 8; i++){
        delay(2);
        SCLK=0;
        delay(1);
        if(data & 0x80) SDO=1;
        else SDO=0;
        delay(1);
        SCLK=1;
        data <<= 1;
    }
    //lcdSelect = 0;
    delay(1);
}

void lcdSetPageAddr(unsigned char addr){
    lcdSendInst(0xB0 | addr);
}

void lcdSetColumnAddr(unsigned char addr){
    lcdSendInst(0x10 | (addr >> 4));
    lcdSendInst(0x0F & addr);
}

void lcdDisplayPic(unsigned char pic[]){
    unsigned char i, j, num = 0;
	for(i=0;i<8;i++){
	lcdSetPageAddr(i);   
    lcdSetColumnAddr(0x00);
        for(j=0;j<240;j++){
		    lcdSendData(pic[num*240+j]);
		}
		num++;
	}
    return;
}

void lcdClear(void){
    unsigned char i,j;
	for(i=0;i<8;i++){
	lcdSetPageAddr(i);	  
    lcdSetColumnAddr(0x00);
        for(j=0;j<240;j++){
		    lcdSendData(00);
		}
	}
 return;
}

void lcdDisplayMenu(unsigned char page,unsigned char column,unsigned char len,unsigned char ft1[])
{
    unsigned char k;

    lcdSetPageAddr(page);
    lcdSetColumnAddr(column);

    for(k=0;k<len;k++)
    {
        lcdSendData(ft1[k]);
    }
    lcdSetPageAddr(page+=1);
    lcdSetColumnAddr(column);

    for(k=len;k<len*2;k++)
    {
        lcdSendData(ft1[k]);
    }

    return;
}

void lcdUpdate(unsigned char state){
    lcdDisplayPic(UI);
    lcdUpdateIcon();
    if(state == 0){
        lcdDisplayMenu(1,170,46,game);
        lcdDisplayMenu(3,77,69,freeplay);
        lcdDisplayMenu(5,160,65,lp);
    }else if(state == 1){
        lcdDisplayMenu(1,148,90,seq);
        lcdDisplayMenu(3,77,46,game);
        lcdDisplayMenu(5,160,69,freeplay);
    }else if(state == 2){
        lcdDisplayMenu(1,160,65,lp);
        lcdDisplayMenu(3,77,90,seq);
        lcdDisplayMenu(5,170,46,game);
    }else if(state == 3){
        lcdDisplayMenu(1,160,69,freeplay);
        lcdDisplayMenu(3,77,65,lp);
        lcdDisplayMenu(5,148,90,seq);
    }
}

void lcdUpdateSeq(unsigned char state){
    lcdDisplayPic(UI);
    lcdUpdateIcon();
    if(state == 0){
        lcdDisplayMenu(1,140,37,back);
        lcdDisplayMenu(3,77,33,rate);
        lcdDisplayMenu(5,140,88,inst);
    }else if(state == 1){
        lcdDisplayMenu(1,140,75,ss);
        lcdDisplayMenu(3,77,37,back);
        lcdDisplayMenu(5,140,33,rate);
    }else if(state == 2){
        lcdDisplayMenu(1,140,84,rows);
        lcdDisplayMenu(3,77,75,ss);
        lcdDisplayMenu(5,140,37,back);
    }else if(state == 3){
        lcdDisplayMenu(1,140,85,pp);
        lcdDisplayMenu(3,77,84,rows);
        lcdDisplayMenu(5,140,75,ss);
    }else if(state == 4){
        lcdDisplayMenu(1,140,57,scratch);
        lcdDisplayMenu(3,77,85,pp);
        lcdDisplayMenu(5,140,84,rows);
    }else if(state == 5){
        lcdDisplayMenu(1,140,88,inst);
        lcdDisplayMenu(3,77,57,scratch);
        lcdDisplayMenu(5,140,85,pp);
    }else if(state == 6){
        lcdDisplayMenu(1,140,33,rate);
        lcdDisplayMenu(3,77,88,inst);
        lcdDisplayMenu(5,140,57,scratch);
    }
}

void lcdUpdateLoop(unsigned char state){
    lcdDisplayPic(UI);
    lcdUpdateIcon();
    if(state == 0){
        lcdDisplayMenu(1,137,103,re);
        lcdDisplayMenu(3,77,37,scale);
        lcdDisplayMenu(5,140,75,ss);
    }else if(state == 1){
        lcdDisplayMenu(1,140,85,pp);
        lcdDisplayMenu(3,77,103,re);
        lcdDisplayMenu(5,140,37,scale);
    }else if(state == 2){
        lcdDisplayMenu(1,140,37,back);
        lcdDisplayMenu(3,77,85,pp);
        lcdDisplayMenu(5,137,103,re);
    }else if(state == 3){
        lcdDisplayMenu(1,140,88,inst);
        lcdDisplayMenu(3,77,37,back);
        lcdDisplayMenu(5,140,85,pp);
    }else if(state == 4){
        lcdDisplayMenu(1,140,75,ss);
        lcdDisplayMenu(3,77,88,inst);
        lcdDisplayMenu(5,140,37,back);
    }else if(state == 5){
        lcdDisplayMenu(1,140,37,scale);
        lcdDisplayMenu(3,77,75,ss);
        lcdDisplayMenu(5,140,88,inst);
    }
}

void lcdUpdateGame(unsigned char state){
    lcdDisplayPic(UI);
    lcdUpdateIcon();
    if(state == 0){
        lcdDisplayMenu(1,140,85,pp);
        lcdDisplayMenu(3,77,75,ss);
        lcdDisplayMenu(5,140,37,back);
    }else if(state == 1){
        lcdDisplayMenu(1,140,37,back);
        lcdDisplayMenu(3,77,85,pp);
        lcdDisplayMenu(5,140,75,ss);
    }else if(state == 2){
        lcdDisplayMenu(1,140,75,ss);
        lcdDisplayMenu(3,77,37,back);
        lcdDisplayMenu(5,140,85,pp);
    }
}

void lcdUpdateFree(unsigned char state){
    lcdDisplayPic(UI);
    lcdUpdateIcon();
    if(state == 0){
        lcdDisplayMenu(1,140,84,rows);
        lcdDisplayMenu(3,77,88,inst);
        lcdDisplayMenu(5,140,37,scale);
    }else if(state == 1){
        lcdDisplayMenu(1,140,37,back);
        lcdDisplayMenu(3,77,84,rows);
        lcdDisplayMenu(5,140,88,inst);
    }else if(state == 2){
        lcdDisplayMenu(1,140,37,scale);
        lcdDisplayMenu(3,77,37,back);
        lcdDisplayMenu(5,140,84,rows);
    }else if(state == 3){
        lcdDisplayMenu(1,140,88,inst);
        lcdDisplayMenu(3,77,37,scale);
        lcdDisplayMenu(5,140,37,back);
    }
}

void lcdUpdate2(unsigned char state){
    lcdDisplayPic(UI2);
    lcdUpdateIcon2();
    if(state == 0) lcdDisplayMenu(2,5,14,arrow);
    else if(state == 1) lcdDisplayMenu(4,5,14,arrow);
    else if(state == 2) lcdDisplayMenu(2,90,14,arrow);
    else if(state == 3) lcdDisplayMenu(4,63,14,arrow);
    if(modeBle) lcdDisplayMenu(4,55,5,on);
    else lcdDisplayMenu(4,55,5,off);
    if(pressSense) lcdDisplayMenu(4,175,5,on);
    else lcdDisplayMenu(4,175,5,off);
}

void lcdUpdateRow(void){
    lcdDisplayPic(UI);
    lcdUpdateIcon();
    lcdDisplayMenu(3,77,84,rows);
}

void lcdUpdateInst(void){
    lcdDisplayPic(UI);
    lcdUpdateIcon();
    lcdDisplayMenu(3,77,88,inst);
}

void lcdUpdateRate(void){
    lcdDisplayPic(UI);
    lcdUpdateIcon();
    lcdDisplayMenu(3,77,33,rate);
}

void lcdUpdateIcon(){
    if(modeGame){
        lcdDisplayMenu(0,35,6,mg);
    }
    if(modeSeq){
        lcdDisplayMenu(0,25,6,ms);
    }
    if(loopStart || loopRecord){
        lcdDisplayMenu(0,15,6,ml);
    }
    if(modeBle){
        lcdDisplayMenu(0,5,5,bluetooth);
    }
}

void lcdUpdateIcon2(){
    if(modeGame){
        lcdDisplayMenu(0,200,6,mg);
    }
    if(modeSeq){
        lcdDisplayMenu(0,210,6,ms);
    }
    if(loopStart || loopRecord){
        lcdDisplayMenu(0,220,6,ml);
    }
    if(modeBle){
        lcdDisplayMenu(0,230,5,bluetooth);
    }
}

void lcdUpdateScra(){
    lcdDisplayPic(UI);
    lcdUpdateIcon();
    lcdDisplayMenu(3,77,57,scratch);
}

void lcdUpdateLe(){
    lcdDisplayPic(UI);
    lcdUpdateIcon();
    lcdDisplayMenu(3,77,90,le);
}

void lcdUpdateScale(){
    lcdDisplayPic(UI);
    lcdUpdateIcon();
    lcdDisplayMenu(3,77,37,scale);
}

void lcdUpdateLoopOption(unsigned char state){
    lcdDisplayPic(UI);
    lcdUpdateIcon();
    if(state == 0){
        lcdDisplayMenu(1,140,40,erase);
        lcdDisplayMenu(3,77,36,keep);
    }else if(state == 1){
        lcdDisplayMenu(3,77,40,erase);
        lcdDisplayMenu(5,140,36,keep);
    }
}

void lcdUpdateGetReady(){
    lcdDisplayPic(UI);
    lcdUpdateIcon();
    lcdDisplayMenu(3,77,78,getready);
}

void lcdUpdateRecording(){
    lcdDisplayPic(UI);
    lcdUpdateIcon();
    lcdDisplayMenu(3,77,80, recording);
}

void lcdUpdateRf(){
    lcdDisplayPic(UI);
    lcdUpdateIcon();
    lcdDisplayMenu(3,77,126, rf);
}

void delay(int t)
{
    int d = 0;
    for(d = 0; d < 10 * t; d++){
        asm("nop");
    }
}

void delay_ms(int t)
{
   int i;
   for(i = 0; i < t * 20017; i++)
   {
       asm("nop");
   }
}