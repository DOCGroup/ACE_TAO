/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/setjmp.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_SETJMP_H_WIN32
#define PACE_SETJMP_H_WIN32

#include <setjmp.h>

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

# define PACE_SIGJLEN 128

# ifndef PACE_JMP_BUF
# define PACE_JMP_BUF
  typedef jmp_buf pace_jmp_buf;
#endif /* PACE_JMP_BUF */

# ifndef PACE_SIGJMP_BUF
# define PACE_SIGJMP_BUF
  typedef long sigjmp_buf[PACE_SIGJLEN];
  typedef sigjmp_buf pace_sigjmp_buf;
#endif /* PACE_SIGJMP_BUF */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_SETJMP_H_WIN32 */
