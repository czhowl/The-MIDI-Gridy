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

#ifndef _GAME_H    /* Guard against multiple inclusion */
#define _GAME_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */
#include <stdint.h>
/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif
    
    typedef struct {
        const uint32_t time;
        const uint8_t onOff;
        const uint8_t pos;
    } gameNote;

    uint8_t modeGame;
    uint8_t gameScale[48];
    uint8_t gameInst[4];
    int gameN;
    uint32_t gameCounter;
    uint64_t gameMask;
    uint64_t gameSwitch;
    uint8_t gameStart;
    gameNote gameTest[510];
    
    void gamePlay(void);
    void gameClear(void);
    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
