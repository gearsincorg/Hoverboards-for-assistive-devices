/* 
 * Author: Phil Malone
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef TIMERS_HEADER_H
#define	TIMERS_HEADER_H

#define BT_STARTTIME 10000
#define BT_TIMEOUT   30000

#include <xc.h> // include processor files - each processor file is guarded.  
#include "mcc_generated_files/tmr2.h"

void        initTimers(void);
void        timeKeeper(void);
void        sleep(uint32_t delay);
uint32_t    getTicks(void);
int32_t     getTicksSince(uint32_t from);
bool        oneSec(void);

void        resetBTTimer(void);
void        setBTTimeout(uint32_t timeout);
int32_t     BTTimeRemaining(void);
int32_t     BTTimeWaiting(void);


#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* TIMERS_HEADER_H */

