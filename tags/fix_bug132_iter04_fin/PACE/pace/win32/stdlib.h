/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/stdlib.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_STDLIB_H_WIN32
#define PACE_STDLIB_H_WIN32

#include <stdlib.h>
#include "pace/unistd.h"

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

  typedef div_t pace_div_t;
  typedef ldiv_t pace_ldiv_t;

# ifndef PACE_EXIT_FAILURE
# define PACE_EXIT_FAILURE EXIT_FAILURE
# endif /* PACE_EXIT_FAILURE */

# ifndef PACE_EXIT_SUCCESS
# define PACE_EXIT_SUCCESS EXIT_SUCCESS
# endif /* PACE_EXIT_SUCCESS */

# ifndef PACE_MB_CUR_MAX
# define PACE_MB_CUR_MAX MB_CUR_MAX
# endif /* PACE_MB_CUR_MAX */

# ifndef PACE_NULL
# define PACE_NULL NULL
# endif /* PACE_NULL */

# ifndef PACE_RAND_MAX
# define PACE_RAND_MAX RAND_MAX
# endif /* PACE_RAND_MAX */

#ifndef PACE_SIZE_T
#define PACE_SIZE_T 1
  typedef size_t pace_size_t;
#endif /* PACE_SIZE_T */

#ifndef PACE_WCHAR_T
#define PACE_WCHAR_T 1
  typedef wchar_t pace_wchar_t;
#endif /* PACE_WCHAR_T */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_STDLIB_H_WIN32 */
