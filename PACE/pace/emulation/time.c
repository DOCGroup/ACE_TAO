/* $Id$ */

#include "pace/emulation/time.h"

int pace_emu_clock_settime (pace_emu_clockid_t clock_id, const pace_timespec* tp)
{
  errno = ENOSYS;
  return -1;
}

int pace_emu_clock_gettime (pace_emu_clockid_t clock_id, pace_timespec* tp)
{
  errno = ENOSYS;
  return -1;
}

int pace_emu_clock_getres (pace_emu_clockid_t clock_id, pace_timespec* res)
{
  errno = ENOSYS;
  return -1;
}

int pace_emu_timer_create (pace_emu_clockid_t clock_id,
                           pace_sigevent* evp,
                           pace_emu_timer_t* timerid)
{
  errno = ENOSYS;
  return -1;
}

int pace_emu_timer_delete (pace_emu_timer_t timerid)
{
  errno = ENOSYS;
  return -1;
}

int pace_emu_timer_settime (pace_emu_timer_t timerid,
                            int flags,
                            const pace_itimerspec* value,
                            pace_itimerspec* ovalue)
{
  errno = ENOSYS;
  return -1;
}

int pace_emu_timer_gettime (pace_emu_timer_t timerid,
                            const pace_itimerspec* value)
{
  errno = ENOSYS;
  return -1;
}

int pace_emu_timer_getoverrun (pace_emu_timer_t timerid)
{
  errno = ENOSYS;
  return -1;
}
