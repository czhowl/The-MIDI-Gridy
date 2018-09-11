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

#ifndef _LOOPING_H    /* Guard against multiple inclusion */
#define _LOOPING_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */
#include "driver/tmr/drv_tmr.h"


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif
    
    typedef struct {
        uint32_t time;
        uint8_t onOff;
        uint8_t velocity;
        uint8_t pos;
    } loopingNote;
    
    typedef struct _loopingConfig {
        uint8_t inst[4];
        uint8_t scale[4];
    } loopingConfig;
    
    typedef struct _looping {
        loopingConfig loopSetting;
        loopingNote note[300];
        int usedLength;
        uint32_t length;
    } looping;

    void loopingRecord(void);
    void loopClear(void);
    void loopPlay(void);
    
    
    uint32_t loopCounter;
    uint8_t loopStart;
    looping loop;
    uint8_t loopN;
    uint8_t loopRecord;
    uint8_t loopExist;
    uint8_t modeLoop;
    uint8_t loopStartPause;

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
