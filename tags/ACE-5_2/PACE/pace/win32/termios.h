/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/termios.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_SYS_TERMIOS_H_WIN32
#define PACE_SYS_TERMIOS_H_WIN32

#include "pace/sys/types.h"

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS) */

#ifndef PACE_CC_T
#define PACE_CC_T
  /* Temporary until we correctly emulate these funcs on WIN32. */
  typedef int cc_t;
  typedef cc_t pace_cc_t;
#endif /* PACE_CC_T */

#ifndef PACE_SPEED_T
#define PACE_SPEED_T
  /* Temporary until we correctly emulate these funcs on WIN32. */
  typedef int speed_t;
  typedef speed_t pace_speed_t;
#endif /* PACE_SPEED_T */

#ifndef PACE_TCFLAG_T
#define PACE_TCFLAG_T
  /* Temporary until we correctly emulate these funcs on WIN32. */
  typedef int tcflag_t;
  typedef tcflag_t pace_tcflag_t;
#endif /* PACE_TCFLAG_T */

#ifndef PACE_TERMIOS
#define PACE_TERMIOS
  /* Temporary until we correctly emulate these funcs on WIN32. */
  typedef struct termios { int a_; } pace_termios;
#endif /* PACE_TERMIOS */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_SYS_TERMIOS_H_WIN32 */
