/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/posix/wait.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_WAIT_H_POSIX
#define PACE_WAIT_H_POSIX

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

#endif /* PACE_WAIT_H */
