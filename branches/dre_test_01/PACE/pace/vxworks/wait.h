/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/vxworks/wait.h
 *
 * = AUTHOR
 *    Joe Hoffert
 *
 * ============================================================================ */

#ifndef PACE_WAIT_H_VXWORKS
#define PACE_WAIT_H_VXWORKS

#include "pace/sys/types.h"

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

#define PACE_WEXITSTATUS WEXITSTATUS
#define PACE_WIFEXITED WIFEXITED
#define PACE_WIFSIGNALED WISIGNALED
#define PACE_WIFSTOPPED WIFSTOPPED
#define PACE_WNOHANG WNOHANG
#define PACE_WSTOPSIG WSTOPSIG
#define PACE_WTERMSIG WTERMSIG
#define PACE_WUNTRACED WUNTRACED

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_WAIT_H_VXWORKS */
