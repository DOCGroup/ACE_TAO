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

#define PACE_BUFSIZ BUFSIZ
#define PACE_EOF EOF
#define PACE_FILENAME_MAX FILENAME_MAX
#define PACE_L_ctermid L_ctermid
#define PACE_L_cuserid L_cuserid
#define PACE_NULL NULL
#define PACE_SEEK_CUR SEEK_CUR
#define PACE_SEEK_END SEEK_END
#define PACE_SEEK_SET SEEK_SET
#define PACE_TMP_MAX TMP_MAX
#define pace_stderr stderr
#define pace_stdout stdout
#define pace_stdin stdin

typedef fpos_t pace_fpos_t;

#ifndef PACE_SIZE_T
#define PACE_SIZE_T
  typedef size_t pace_size_t;
#endif /* PACE_SIZE_T */

typedef FILE PACE_FILE;

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_STDIO_H_WIN32 */
