/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/vxworks/termios.h
 *
 * = AUTHOR
 *    Joe Hoffert
 *
 * ============================================================================ */

#ifndef PACE_SYS_TERMIOS_H_VXWORKS
#define PACE_SYS_TERMIOS_H_VXWORKS

#include "pace/sys/types.h"

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS) */

#ifndef PACE_CC_T
#define PACE_CC_T
typedef int pace_cc_t;
#endif /* PACE_CC_T */

#ifndef PACE_SPEED_T
#define PACE_SPEED_T
typedef int pace_speed_t;
#endif /* PACE_SPEED_T */

#ifndef PACE_TCFLAG_T
#define PACE_TCFLAG_T
typedef int pace_tcflag_t;
#endif /* PACE_TCFLAG_T */

#ifndef PACE_TERMIOS
#define PACE_TERMIOS
typedef int pace_termios;
#endif /* PACE_TERMIOS */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_SYS_TERMIOS_H_VXWORKS */
