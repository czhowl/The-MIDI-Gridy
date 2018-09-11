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

#ifndef _UI_H    /* Guard against multiple inclusion */
#define _UI_H

#include "lcd.h"


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif
    
    void encoderRightClick(void);
    void encoderRightIn(void);
    void encoderRightDe(void);
    void encoderLeftClick(void);
    void encoderLeftIn(void);
    void encoderLeftDe(void);
    void midiSamplePlay(void);
    
    uint8_t menu;
    uint8_t menuSelect;
    uint8_t interface;
    uint8_t menu2;
    uint8_t menu2Select;
    uint8_t menuSeq;
    uint8_t menuGame;
    uint8_t menuLoop;
    uint8_t menuFree;
    uint8_t freeze;
    uint8_t freeze2;
    

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
