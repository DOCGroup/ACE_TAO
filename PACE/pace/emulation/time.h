/* $Id$ */

#ifndef PACE_EMU_TIME_H
#define PACE_EMU_TIME_H

#include "pace/signal.h"

typedef int pace_emu_clockid_t;
typedef int pace_emu_timer_t;
typedef struct emu_itimerspec {} pace_emu_itimerspec;
typedef struct emu_timespec {} pace_emu_timespec;

#ifndef PACE_SIGEVENT
#define PACE_SIGEVENT
  typedef struct sigevent pace_sigevent;
#endif /* PACE_SIGEVENT */
int pace_emu_clock_settime (pace_emu_clockid_t clock_id, const pace_emu_timespec* tp);
int pace_emu_clock_gettime (pace_emu_clockid_t clock_id, pace_emu_timespec* tp);
int pace_emu_clock_getres (pace_emu_clockid_t clock_id, pace_emu_timespec* res);

int pace_emu_timer_create (pace_emu_clockid_t clock_id,
                           pace_sigevent* evp,
                           pace_emu_timer_t* timerid);
int pace_emu_timer_delete (pace_emu_timer_t timerid);
int pace_emu_timer_settime (pace_emu_timer_t timerid,
                            int flags,
                            const pace_emu_itimerspec* value,
                            pace_emu_itimerspec* ovalue);
int pace_emu_timer_gettime (pace_emu_timer_t timerid,
                            const pace_emu_itimerspec* value);
int pace_emu_timer_getoverrun (pace_emu_timer_t timerid);

#endif /* PACE_EMU_TIME_H */
