/* $Id$ -*- C -*-

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/posix/stdio.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_STDIO_H_POSIX
#define PACE_STDIO_H_POSIX

#include <stdarg.h>
#include <stdio.h>

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

#define PACE_BUFSIZ BUFSIZ
#define PACE_EOF EOF
#define PACE_FILENAME_MAX FILENAME_MAX
#define PACE_L_ctermid L_ctermid
#define PACE_L_cuserid L_cuserid
#ifndef PACE_NULL
#define PACE_NULL NULL
#endif /* PACE_NULL */
#define PACE_SEEK_CUR SEEK_CUR
#define PACE_SEEK_END SEEK_END
#define PACE_SEEK_SET SEEK_SET
#define PACE_TMP_MAX TMP_MAX
#define pace_stderr stderr
#define pace_stdout stdout
#define pace_stdin stdin

#ifndef PACE_FPOS_T
#define PACE_FPOS_T
typedef fpos_t pace_fpos_t;
#endif /* PACE_FPOS_T */

#ifndef PACE_SIZE_T
#define PACE_SIZE_T
  typedef size_t pace_size_t;
#endif /* PACE_SIZE_T */

typedef FILE PACE_FILE;

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_STDIO_H_POSIX */
