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

#ifndef _SEQ_H    /* Guard against multiple inclusion */
#define _SEQ_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */
#include <stdint.h>
#include "settings.h"

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

    
    
    uint8_t modeSeq;
    uint64_t sequenceSwitch;
    uint64_t seqRowMask;
    uint64_t ring;
    uint8_t ringPos;
    uint64_t sequenceNote;
    uint8_t seqSound;
    uint64_t sequenceCol;
    uint64_t seqMask;
    uint8_t seqCounter;
    uint8_t modeSeqPause;
    uint8_t modeScra;
    uint64_t ringi;
    uint8_t seqInst[4];
    
    void seqPlaySound(void);
    void seqUpdate(void);
    void seqClear(void);

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
