/* $Id$
 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/emulation/time.h
 *
 * = AUTHOR
 *    John Heitmann
 *
 * ============================================================================ */

#ifndef PACE_EMU_TIME_H
#define PACE_EMU_TIME_H

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

#ifndef PACE_EMU_CLOCKID_T
#define PACE_EMU_CLOCKID_T
  typedef int pace_emu_clockid_t;
#endif /* PACE_EMU_CLOCKID_T */

#ifndef PACE_EMU_TIMER_T
#define PACE_EMU_TIMER_T
  typedef int pace_emu_timer_t;
#endif /* PACE_EMU_TIMER_T */

#ifndef PACE_EMU_ITIMERSPEC
#define PACE_EMU_ITIMERSPEC
  typedef struct emu_itimerspec {} pace_emu_itimerspec;
#endif /* PACE_EMU_ITIMERSPEC */

#ifndef PACE_EMU_TIMESPEC
#define PACE_EMU_TIMESPEC
  typedef struct emu_timespec {} pace_emu_timespec;
#endif /* PACE_EMU_TIMESPEC */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_EMU_TIME_H */
