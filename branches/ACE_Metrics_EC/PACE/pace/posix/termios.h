/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/posix/termios.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_SYS_TERMIOS_H_POSIX
#define PACE_SYS_TERMIOS_H_POSIX

#include "pace/sys/types.h"
#include <termios.h>

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS) */

#ifndef PACE_CC_T
#define PACE_CC_T
typedef cc_t pace_cc_t;
#endif /* PACE_CC_T */

#ifndef PACE_SPEED_T
#define PACE_SPEED_T
typedef speed_t pace_speed_t;
#endif /* PACE_SPEED_T */

#ifndef PACE_TCFLAG_T
#define PACE_TCFLAG_T
typedef tcflag_t pace_tcflag_t;
#endif /* PACE_TCFLAG_T */

#ifndef PACE_TERMIOS
#define PACE_TERMIOS
typedef struct termios pace_termios;
#endif /* PACE_TERMIOS */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_SYS_TERMIOS_H_POSIX */
