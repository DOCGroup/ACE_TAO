/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/posix/dirent.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_DIRENT_H_POSIX
#define PACE_DIRENT_H_POSIX

#include "pace/sys/types.h"
#include <dirent.h>

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

#ifndef PACE_DIR_T
#define PACE_DIR_T
typedef DIR PACE_DIR;
#endif /* PACE_DIR_T */

#ifndef PACE_DIRENT
#define PACE_DIRENT
typedef struct dirent pace_dirent;
#endif /* PACE_DIRENT */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_DIRENT_H_POSIX */
