/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/stdio.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_STDIO_H
#define PACE_STDIO_H

#include "pace/config/defines.h"

#if (PACE_HAS_POSIX)
# include "pace/posix/stdio.h"
#elif (PACE_VXWORKS)
# include "pace/vxworks/stdio.h"
#elif (PACE_WIN32)
# include "pace/win32/stdio.h"
#endif

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

  /**
     PACE's implementation of the POSIX function clearerr.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
  PACE_INLINE void pace_clearerr (PACE_FILE * stream);

  /**
     PACE's implementation of the POSIX function fclose.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
  PACE_INLINE int pace_fclose (PACE_FILE * stream);

  /**
     PACE's implementation of the POSIX function ctermid.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 4.7.1.
     */
  PACE_INLINE char * pace_ctermid (char * s);

  /**
     PACE's implementation of the POSIX function fdopen.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.2.2.
     */
  PACE_INLINE PACE_FILE * pace_fdopen (int fildes, const char * type);

  /**
     PACE's implementation of the POSIX function ferror.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
  PACE_INLINE int pace_ferror (PACE_FILE * stream);

  /**
     PACE's implementation of the POSIX function feof.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
  PACE_INLINE int pace_feof (PACE_FILE * stream);

  /**
     PACE's implementation of the POSIX function fflush.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
  PACE_INLINE int pace_fflush (PACE_FILE * stream);

  /**
     PACE's implementation of the POSIX function fileno.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.2.1.
     */
  PACE_INLINE int pace_fileno (PACE_FILE * stream);

  /**
     PACE's implementation of the POSIX function fgetc.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
  PACE_INLINE int pace_fgetc (PACE_FILE * stream);

  /**
    C std 7.19.9.1
    */
  PACE_INLINE int pace_fgetpos(PACE_FILE * stream,
                               pace_fpos_t * pos);

  /**
     PACE's implementation of the POSIX function fgets.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
  PACE_INLINE char * pace_fgets (char * s, int n, PACE_FILE * stream);

  /**
     PACE's implementation of the POSIX function flockfile.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.2.6.
     */
  PACE_INLINE void pace_flockfile (PACE_FILE * file);
  /* Requires PACE_HAS_REENTRANT. */

  /**
     PACE's implementation of the POSIX function fopen.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
  PACE_INLINE PACE_FILE * pace_fopen (const char * filename, const char * mode);

  /**
     PACE's implementation of the POSIX function fprintf.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
  int pace_fprintf (PACE_FILE *stream, const char *format, ...);

  /**
     PACE's implementation of the POSIX function fputc.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
  PACE_INLINE int pace_fputc (int c, PACE_FILE * stream);

  /**
     PACE's implementation of the POSIX function fputs.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
  PACE_INLINE int pace_fputs (const char * s, PACE_FILE * stream);

  /**
     PACE's implementation of the POSIX function fread.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
  PACE_INLINE pace_size_t pace_fread (void * ptr,
                                      pace_size_t size,
                                      pace_size_t number_of_items,
                                      PACE_FILE * stream);

  /**
     PACE's implementation of the POSIX function freopen.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
  PACE_INLINE FILE * pace_freopen (const char * filename,
                                   const char * mode,
                                   PACE_FILE * stream);

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
  PACE_INLINE int pace_fseek (PACE_FILE * stream, long offset, int whence);

  /**
    C std 7.19.9.3
    */
  PACE_INLINE int pace_fsetpos(PACE_FILE *stream, const pace_fpos_t *pos);

  /**
     PACE's implementation of the POSIX function ftell.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
  PACE_INLINE long pace_ftell (PACE_FILE * stream);

  /**
     PACE's implementation of the POSIX function ftrylockfile.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.2.6.
     */
  PACE_INLINE int pace_ftrylockfile (PACE_FILE * file);
  /* Requires PACE_HAS_REENTRANT. */

  /**
     PACE's implementation of the POSIX function funlockfile.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.2.6.
     */
  PACE_INLINE void pace_funlockfile (PACE_FILE * file);
  /* Requires PACE_HAS_REENTRANT. */

  /**
    C std 7.19.8.2
    */
  PACE_INLINE pace_size_t pace_fwrite(const void * ptr,
                       pace_size_t size, pace_size_t nmemb,
                       PACE_FILE * stream);

  /**
     PACE's implementation of the POSIX function getc.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
  PACE_INLINE int pace_getc (PACE_FILE * stream);

  /**
     PACE's implementation of the POSIX function getc_unlocked.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.2.7.
     */
  PACE_INLINE int pace_getc_unlocked (PACE_FILE * stream);
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
  PACE_INLINE int pace_putc (int c, PACE_FILE * stream);

  /**
     PACE's implementation of the POSIX function putc_unlocked.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.2.7.
     */
  PACE_INLINE int pace_putc_unlocked (int c, PACE_FILE * stream);
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
  PACE_INLINE void pace_rewind (PACE_FILE * stream);

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
  PACE_INLINE void pace_setbuf (PACE_FILE * stream, char * buf);


  /**
    C std 7.19.5.6
    */
  PACE_INLINE int pace_setvbuf(PACE_FILE * stream,
                               char * buf,
                               int mode, pace_size_t size);
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

  /**
    C std 7.19.6.8
    */
  PACE_INLINE int pace_vfprintf (PACE_FILE * stream,
                                 const char * format,
                                 va_list arg);

  /**
    C std 7.19.6.10
    */
  PACE_INLINE int pace_vprintf (const char * format,
                                va_list arg);

  /**
    C std 7.19.6.13
    */
  PACE_INLINE int pace_vsprintf (char * s,
                                 const char * format,
                                 va_list arg);
#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#if defined (PACE_HAS_INLINE)
#  if (PACE_HAS_POSIX)
#    include "pace/posix/stdio.inl"
#  elif (PACE_VXWORKS)
#    include "pace/vxworks/stdio.inl"
#  elif (PACE_WIN32)
#    include "pace/win32/stdio.inl"
#  endif
#endif /* PACE_HAS_INLINE */

#endif /* PACE_STDIO_H */
