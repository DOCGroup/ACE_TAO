/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/stdio.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_STDIO_H_WIN32
#define PACE_STDIO_H_WIN32

#include <stdio.h>
#include <stdarg.h>

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

  typedef FILE PACE_FILE;
  typedef fpos_t pace_fpos_t;

# ifndef PACE_BUSIZ
# define PACE_BUFSIZ BUFSIZ
# endif /* PACE_BUFSIZ */

# ifndef PACE_EOF
# define PACE_EOF EOF
# endif /* PACE_EOF */

# ifndef PACE_FILENAME_MAX
# define PACE_FILENAME_MAX FILENAME_MAX
# endif /* PACE_FILENAME_MAX */

# ifndef PACE_L_ctermid
# define PACE_L_ctermid L_ctermid
# endif /* PACE_L_ctermid */

# ifndef PACE_L_cuserid
# define PACE_L_cuserid L_cuserid
# endif /* PACE_L_cuserid */

# ifndef PACE_NULL
# define PACE_NULL NULL
# endif /* PACE_NULL */

# ifndef PACE_SEEK_CUR
# define PACE_SEEK_CUR SEEK_CUR
# endif /* PACE_SEEK_CUR */

# ifndef PACE_SEEK_END
# define PACE_SEEK_END SEEK_END
# endif /* PACE_SEEK_END */

# ifndef PACE_SEEK_SET
# define PACE_SEEK_SET SEEK_SET
# endif /* PACE_SEEK_SET */

# ifndef PACE_TMP_MAX
# define PACE_TMP_MAX TMP_MAX
# endif /* PACE_TMP_MAX */

# ifndef pace_stderr
# define pace_stderr stderr
# endif /* pace_stderr */

# ifndef pace_stdout
# define pace_stdout stdout
# endif /* pace_stdout */

# ifndef pace_stdin
# define pace_stdin stdin
# endif

#ifndef PACE_SIZE_T
#define PACE_SIZE_T 1
  typedef size_t pace_size_t;
#endif /* PACE_SIZE_T */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_STDIO_H_WIN32 */
