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

#ifndef _SETTINGS_H    /* Guard against multiple inclusion */
#define _SETTINGS_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */
#include <stdint.h>
#include "peripheral/ports/plib_ports.h"
#include "seq.h"
#include "looping.h"
#include "ui.h"

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

    

    
    uint8_t level;
    uint8_t vol;
    uint8_t seqRate;
    uint8_t scaleChanged;
    uint8_t numFlag;
    uint8_t rateCollection[12];
    uint8_t scaleCollection[12];;
    uint8_t volCollection[12];
    uint8_t scaleConfig[4];
    uint8_t scaleSetFlag;
    uint8_t scaleRowSelect;
    
    uint16_t instLed;
    uint8_t instSetFlag;
    uint8_t instRowSelect;
    uint8_t instConfig[4];
    uint8_t instChanged;
    
    uint8_t rowSetFlag;
    uint16_t rowLed;
    char rowConfig[4];
    
    uint64_t numSwitch;
    
    void rowSet(void);
    void instSet(void);
    void numSet(void);   //volume 0, rate 2
    void scaleSet(void);


    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
