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

#ifndef PACE_STDIO_H
#define PACE_STDIO_H

#include <stdio.h>
#include <stdarg.h>

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

  #define pace_stderr stderr;

  #define pace_stdout stdout;

  #define pace_stdin stdin;

  /** 
     PACE's implementation of the POSIX function clearerr.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
  PACE_INLINE void pace_clearerr (FILE * stream);

  /** 
     PACE's implementation of the POSIX function ctermid.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 4.7.1.
   */
  PACE_INLINE char * pace_ctermid (char * s);

  /** 
     PACE's implementation of the POSIX function fclose.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
  PACE_INLINE int pace_fclose (FILE * stream);

  /** 
     PACE's implementation of the POSIX function fdopen.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.2.2.
   */
  PACE_INLINE FILE * pace_fdopen (int fildes, const char * type);

  /** 
     PACE's implementation of the POSIX function ferror.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
  PACE_INLINE int pace_ferror (FILE * stream);

  /** 
     PACE's implementation of the POSIX function feof.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
  PACE_INLINE int pace_feof (FILE * stream);

  /** 
     PACE's implementation of the POSIX function fflush.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
  PACE_INLINE int pace_fflush (FILE * stream);

  /** 
     PACE's implementation of the POSIX function fileno.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.2.1.
   */
  PACE_INLINE int pace_fileno (FILE * stream);

  /** 
     PACE's implementation of the POSIX function fgetc.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
  PACE_INLINE int pace_fgetc (FILE * stream);

  /** 
     PACE's implementation of the POSIX function fgets.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
  PACE_INLINE char * pace_fgets (char * s, int n, FILE * stream);

  /** 
     PACE's implementation of the POSIX function flockfile.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.2.6.
   */
  PACE_INLINE void pace_flockfile (FILE * file);
  /* Requires PACE_HAS_REENTRANT. */

  /** 
     PACE's implementation of the POSIX function fopen.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
  PACE_INLINE FILE * pace_fopen (const char * filename, const char * mode);

  /** 
     PACE's implementation of the POSIX function fprintf.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
  int pace_fprintf (FILE *stream, const char *format, ...);

  /** 
     PACE's implementation of the POSIX function fputc.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
  PACE_INLINE int pace_fputc (int c, FILE * stream);

  /** 
     PACE's implementation of the POSIX function fputs.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
  PACE_INLINE int pace_fputs (const char * s, FILE * stream);

  /** 
     PACE's implementation of the POSIX function fread.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
  PACE_INLINE size_t pace_fread (void * ptr,
                                 size_t size,
                                 size_t number_of_items,
                                 FILE * stream);

  /** 
     PACE's implementation of the POSIX function freopen.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
  PACE_INLINE FILE * pace_freopen (const char * filename,
                                   const char * mode,
                                   FILE * stream);

  /** 
     PACE's implementation of the POSIX function fscanf.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
  #define pace_fscanf (X) fscanf X
  /* int pace_scanf (const char *format, ... );
  *
  * PLEASE PAY SPECIAL ATTENTION HERE!
  * This is a macro and requires an additional set of parenthesis
  * surrounding the arguments.
  */

  /** 
     PACE's implementation of the POSIX function fseek.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
  PACE_INLINE int pace_fseek (FILE * stream, long offset, int whence);

  /** 
     PACE's implementation of the POSIX function ftell.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
  PACE_INLINE long pace_ftell (FILE * stream);

  /** 
     PACE's implementation of the POSIX function ftrylockfile.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.2.6.
   */
  PACE_INLINE int pace_ftrylockfile (FILE * file);
  /* Requires PACE_HAS_REENTRANT. */

  /** 
     PACE's implementation of the POSIX function funlockfile.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.2.6.
   */
  PACE_INLINE void pace_funlockfile (FILE * file);
  /* Requires PACE_HAS_REENTRANT. */

  /** 
     PACE's implementation of the POSIX function getc.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
  PACE_INLINE int pace_getc (FILE * stream);

  /** 
     PACE's implementation of the POSIX function getc_unlocked.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.2.7.
   */
  PACE_INLINE int pace_getc_unlocked (FILE * stream);
  /* Requires PACE_HAS_REENTRANT. */

  /** 
     PACE's implementation of the POSIX function getchar.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
  PACE_INLINE int pace_getchar ();

  /** 
     PACE's implementation of the POSIX function getchar_unlocked.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.2.7.
   */
  PACE_INLINE int pace_getchar_unlocked ();
  /* Requires PACE_HAS_REENTRANT. */

  /** 
     PACE's implementation of the POSIX function gets.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
  PACE_INLINE char *pace_gets (char * s);

  /** 
     PACE's implementation of the POSIX function perror.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
  PACE_INLINE void pace_perror (const char * s);

  /** 
     PACE's implementation of the POSIX function printf.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
  int pace_printf (const char* format, ...);

  /** 
     PACE's implementation of the POSIX function putc.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
  PACE_INLINE int pace_putc (int c, FILE * stream);

  /** 
     PACE's implementation of the POSIX function putc_unlocked.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.2.7.
   */
  PACE_INLINE int pace_putc_unlocked (int c, FILE * stream);
  /* Requires PACE_HAS_REENTRANT. */

  /** 
     PACE's implementation of the POSIX function putchar.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
  PACE_INLINE int pace_putchar (int c);

  /** 
     PACE's implementation of the POSIX function putchar_unlocked.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.2.7.
   */
  PACE_INLINE int pace_putchar_unlocked (int c);
  /* Requires PACE_HAS_REENTRANT. */

  /** 
     PACE's implementation of the POSIX function puts.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
  PACE_INLINE int pace_puts (const char * s);

  /** 
     PACE's implementation of the POSIX function remove.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
  PACE_INLINE int pace_remove (const char * path);

  /** 
     PACE's implementation of the POSIX function rename.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 5.5.3.
   */
  PACE_INLINE int pace_rename (const char * old_name,
                               const char * new_name);

  /** 
     PACE's implementation of the POSIX function rewind.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
  PACE_INLINE void pace_rewind (FILE * stream);

  /** 
     PACE's implementation of the POSIX function sprintf.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
  int pace_sprintf (char* s, const char* format, ...);

  /** 
     PACE's implementation of the POSIX function scanf.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
  #define pace_scanf (X) scanf X
  /* int pace_scanf (const char *format, ... );
  *
  * PLEASE PAY SPECIAL ATTENTION HERE!
  * This is a macro and requires an additional set of parenthesis
  * surrounding the arguments.
  */

  /** 
     PACE's implementation of the POSIX function setbuf.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
  PACE_INLINE void pace_setbuf (FILE * stream, char * buf);

  /** 
     PACE's implementation of the POSIX function sscanf.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
  #define pace_sscanf (X) sscanf X
  /* int pace_sscanf (const char *s, const char *format, ... );
  *
  * PLEASE PAY SPECIAL ATTENTION HERE!
  * This is a macro and requires an additional set of parenthesis
  * surrounding the arguments.
  */

  /** 
     PACE's implementation of the POSIX function sscanf.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
  PACE_INLINE FILE * pace_tmpfile ();

  /** 
     PACE's implementation of the POSIX function tmpnam.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
  PACE_INLINE char * pace_tmpnam (char * s);

  /** 
     PACE's implementation of the POSIX function ungetc.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
  PACE_INLINE int pace_ungetc (int c, FILE * stream);

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#if defined (PACE_HAS_INLINE)
# include "pace/posix/stdio.inl"
#endif /* PACE_HAS_INLINE */

#endif /* PACE_STDIO_H */
