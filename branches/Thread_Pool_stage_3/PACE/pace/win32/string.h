/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/string.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_STRING_H_WIN32
#define PACE_STRING_H_WIN32

#include "pace/unistd.h"

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

#if !defined (PACE_NULL)
# define PACE_NULL NULL
#endif /* NULL */

#ifndef PACE_SIZE_T
#define PACE_SIZE_T
  typedef size_t pace_size_t;
#endif /* PACE_SIZE_T */

#if (PACE_HAS_POSIX_CLSR_UOF)
char * strtok_r (char * s, const char * sep, char ** lasts);
#endif /* PACE_HAS_POSIX_CLSR_UOF */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_STRING_H_WIN32 */
