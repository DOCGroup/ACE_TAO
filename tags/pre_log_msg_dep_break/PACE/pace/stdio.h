/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    stdio.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */


#ifndef PACE_STDIO_H
#define PACE_STDIO_H


#include "pace/defines.h"
#include <stdio.h>
#include <stdarg.h>


# if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
# endif /* PACE_HAS_CPLUSPLUS */

  #define pace_stderr stderr;

  #define pace_stdout stdout;

  #define pace_stdin stdin;

  PACE_INLINE char * pace_ctermid (char * s);

  PACE_INLINE int pace_fclose (FILE * stream);

  PACE_INLINE FILE * pace_fdopen (int fildes, const char * type);

  PACE_INLINE int pace_fflush (FILE * stream);

  PACE_INLINE int pace_fileno (FILE * stream);

  PACE_INLINE int pace_fgetc (FILE * stream);

  PACE_INLINE char * pace_fgets (char * s, int n, FILE * stream);

  PACE_INLINE void pace_flockfile (FILE * file);
  /* Requires PACE_HAS_REENTRANT. */

  PACE_INLINE FILE * pace_fopen (const char * filename, const char * mode);

  #define pace_fprintf(X) fprintf X
  /* int fprintf (FILE *stream, const char *format, (*//* args *//*) ... );
   *
   * PLEASE PAY SPECIAL ATTENTION HERE!
   * This is a macro and requires an additional set of parenthesis
   * surrounding the arguments.
   */

  PACE_INLINE int pace_fputc (int c, FILE * stream);

  PACE_INLINE int pace_fputs (const char * s, FILE * stream);

  PACE_INLINE size_t pace_fread (void * ptr,
                                 size_t size,
                                 size_t number_of_items,
                                 FILE * stream);

  PACE_INLINE FILE * pace_freopen (const char * filename,
                                   const char * mode,
                                   FILE * stream);

  #define pace_fscanf(X) fscanf X
  /* int fscanf (FILE*stream, const char *format, ... );
   *
   * PLEASE PAY SPECIAL ATTENTION HERE!
   * This is a macro and requires an additional set of parenthesis
   * surrounding the arguments.
   */

  PACE_INLINE int pace_fseek (FILE * stream, long offset, int whence);

  PACE_INLINE int pace_fseek (FILE * stream, off_t offset, int whence);
  /* Not in POSIX spec. */

  PACE_INLINE long pace_ftell (FILE * stream);

  PACE_INLINE off_t pace_ftello (FILE * stream);
  /* Not in POSIX spec. */

  PACE_INLINE size_t pace_fwrite (const void * ptr,
  /* Not in POSIX spec. */        size_t size,
                                  size_t number_of_items,
                                  FILE * stream);

  PACE_INLINE int pace_trylockfile (FILE * file);
  /* Requires PACE_HAS_REENTRANT. */

  PACE_INLINE void pace_funlockfile (FILE * file);
  /* Requires PACE_HAS_REENTRANT. */

  PACE_INLINE int pace_getc (FILE * stream);

  PACE_INLINE int pace_getc_unlocked (FILE * stream);
  /* Requires PACE_HAS_REENTRANT. */

  PACE_INLINE int pace_getchar ();

  PACE_INLINE int pace_getchar_unlocked ();
  /* Requires PACE_HAS_REENTRANT. */

  PACE_INLINE char *PACE_gets (char * s);

  PACE_INLINE int pace_getw (FILE * stream);
  /* Not in POSIX spec. */

  PACE_INLINE void pace_perror (const char * s);

  #define pace_printf(X) printf X
  /* int printf (const char *format, (*//* args*//*) ... );
   *
   * PLEASE PAY SPECIAL ATTENTION HERE!
   * This is a macro and requires an additional set of parenthesis
   * surrounding the arguments.
   */

  PACE_INLINE int pace_putc (int c, FILE * stream);

  PACE_INLINE int pace_putc_unlocked (int c, FILE * stream);
  /* Requires PACE_HAS_REENTRANT. */

  PACE_INLINE int pace_putchar (int c);

  PACE_INLINE int pace_putchar_unlocked (int c);
  /* Requires PACE_HAS_REENTRANT. */

  PACE_INLINE int pace_puts (const char * s);

  PACE_INLINE int pace_putw (int w, FILE * stream);
  /* Not in POSIX spec. */

  PACE_INLINE int pace_rename (const char * old_name,
                               const char * new_name);

  PACE_INLINE void pace_rewind (FILE * stream);

  PACE_INLINE int pace_sprintf (char * buf, const char * format, ... );

  #define pace_scanf(X) scanf X
  /* int scanf (const char *format, ... );
   *
   * PLEASE PAY SPECIAL ATTENTION HERE!
   * This is a macro and requires an additional set of parenthesis
   * surrounding the arguments.
   */

  #define pace_sscanf(X) sscanf X
  /* int sscanf (const char *s, const char *format, ... );
   *
   * PLEASE PAY SPECIAL ATTENTION HERE!
   * This is a macro and requires an additional set of parenthesis
   * surrounding the arguments.
   */

  #define pace_snprintf(X) snprintf X
  /* int snprintf (char *s, size_t  n, const char *format, (*//* args*//*) ... );
   *
   * PLEASE PAY SPECIAL ATTENTION HERE!
   * This is a macro and requires an additional set of parenthesis
   * surrounding the arguments.
   */

  PACE_INLINE int pace_vsprintf (char * buffer,
                                 const char * format,
                                 va_list argptr);

  PACE_INLINE FILE * pace_tmpfile ();

# if defined (PACE_HAS_CPLUSPLUS)
}
# endif /* PACE_HAS_CPLUSPLUS */

# if defined (PACE_HAS_INLINE)
# include "pace/stdio.inl"
# endif /* PACE_HAS_INLINE */


#endif /* PACE_STDIO_H */
