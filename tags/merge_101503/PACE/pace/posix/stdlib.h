/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/posix/stdlib.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_STDLIB_H_POSIX
#define PACE_STDLIB_H_POSIX

#include <stdlib.h>

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

#define PACE_EXIT_FAILURE EXIT_FAILURE
#define PACE_EXIT_SUCCESS EXIT_SUCCESS
#define PACE_MB_CUR_MAX MB_CUR_MAX
#ifndef PACE_NULL
# define PACE_NULL NULL
#endif /* PACE_NULL */
#define PACE_RAND_MAX RAND_MAX

#ifndef PACE_DIV_T
#define PACE_DIV_T
  typedef div_t pace_div_t;
#endif /* PACE_DIV_T */

#ifndef PACE_LDIV_T
#define PACE_LDIV_T
  typedef ldiv_t pace_ldiv_t;
#endif /* PACE_LDIV_T */

#ifndef PACE_SIZE_T
#define PACE_SIZE_T
  typedef size_t pace_size_t;
#endif /* PACE_SIZE_T */

#ifndef PACE_WCHAR_T
#define PACE_WCHAR_T
  typedef wchar_t pace_wchar_t;
#endif /* PACE_WCHAR_T */

#if defined (PACE_HAS_CPLUSPLUS)
# ifndef PACE_BSEARCH_PF
# define PACE_BSEARCH_PF
  typedef int (*pace_bsearch_pf) (const void*, const void*);
# endif /* PACE_BSEARCH_PF */
#endif /* PACE_HAS_CPLUSPLUS */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_STDLIB_H_POSIX */
