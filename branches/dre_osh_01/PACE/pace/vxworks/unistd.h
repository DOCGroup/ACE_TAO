/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/vxworks/unistd.h
 *
 * = AUTHOR
 *    Joe Hoffert
 *
 * ============================================================================ */

#ifndef PACE_UNISTD_H_VXWORKS
#define PACE_UNISTD_H_VXWORKS

#include "pace/sys/types.h"
#include <unistd.h>

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

#define PACE_F_OK F_OK
#ifndef PACE_NULL
# define PACE_NULL NULL
#endif /* PACE_NULL */
#define PACE_R_OK R_OK
#define PACE_SEEK_CUR SEEK_CUR
#define PACE_SEEK_END SEEK_END
#define PACE_SEEK_SET SEEK_SET
#define PACE_STDERR_FILENO STDERR_FILENO
#define PACE_STDIN_FILENO STDIN_FILENO
#define PACE_STDOUT_FILENO STDOUT_FILENO
#define PACE_W_OK W_OK
#define PACE_X_OK X_OK

/* _POSIX _PC _SC Macros need to go here */

#ifndef PACE_SIZE_T
#define PACE_SIZE_T
typedef size_t pace_size_t;
#endif /* PACE_SIZE_T */

#ifndef PACE_SSIZE_T
#define PACE_SSIZE_T
typedef ssize_t pace_ssize_t;
#endif /* PACE_SSIZE_T */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_UNISTD_H_VXWORKS */
