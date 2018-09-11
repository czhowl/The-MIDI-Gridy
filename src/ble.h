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

#ifndef _BLE_H    /* Guard against multiple inclusion */
#define _BLE_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */
#include <stdbool.h>
#include "driver/usart/drv_usart.h"

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

    
    bool outCommand(void);
    bool bleMidiEn(void);
    bool bleReset(void);
    bool bleMidiNoteOn(uint8_t, uint8_t);
    bool bleMidiNoteOff(uint8_t, uint8_t);
    
    char* bleCommand;
    char buffer[3];
    uint8_t modeBle;
    
    

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
