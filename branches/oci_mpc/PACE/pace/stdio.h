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
#if (PACE_HAS_POSIX_DI_UOF)
  PACE_Export PACE_INLINE void pace_clearerr (PACE_FILE * stream);
#endif /* PACE_HAS_POSIX_DI_UOF */

  /**
     PACE's implementation of the POSIX function ctermid.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 4.7.1.
     */
#if (PACE_HAS_POSIX_DS_UOF)
  PACE_Export PACE_INLINE char * pace_ctermid (char * s);
#endif /* PACE_HAS_POSIX_DS_UOF */

  /**
     PACE's implementation of the POSIX function fclose.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
#if (PACE_HAS_POSIX_DI_UOF)
  PACE_Export PACE_INLINE int pace_fclose (PACE_FILE * stream);
#endif /* PACE_HAS_POSIX_DI_UOF */

  /**
     PACE's implementation of the POSIX function fdopen.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.2.2.
     */
#if (PACE_HAS_POSIX_DI_UOF)
  PACE_Export PACE_INLINE PACE_FILE * pace_fdopen (int fildes,
                                                   const char * type);
#endif /* PACE_HAS_POSIX_DI_UOF */

  /**
     PACE's implementation of the POSIX function ferror.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
#if (PACE_HAS_POSIX_DI_UOF)
  PACE_Export PACE_INLINE int pace_ferror (PACE_FILE * stream);
#endif /* PACE_HAS_POSIX_DI_UOF */

  /**
     PACE's implementation of the POSIX function feof.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
#if (PACE_HAS_POSIX_DI_UOF)
  PACE_Export PACE_INLINE int pace_feof (PACE_FILE * stream);
#endif /* PACE_HAS_POSIX_DI_UOF */

  /**
     PACE's implementation of the POSIX function fflush.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
#if (PACE_HAS_POSIX_DI_UOF)
  PACE_Export PACE_INLINE int pace_fflush (PACE_FILE * stream);
#endif /* PACE_HAS_POSIX_DI_UOF */

  /**
     PACE's implementation of the POSIX function fileno.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.2.1.
     */
#if (PACE_HAS_POSIX_DI_UOF)
  PACE_Export PACE_INLINE int pace_fileno (PACE_FILE * stream);
#endif /* PACE_HAS_POSIX_DI_UOF */

  /**
     PACE's implementation of the POSIX function fgetc.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
#if (PACE_HAS_POSIX_DI_UOF)
  PACE_Export PACE_INLINE int pace_fgetc (PACE_FILE * stream);
#endif /* PACE_HAS_POSIX_DI_UOF */

  /**
    C std 7.19.9.1
    */
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  PACE_Export PACE_INLINE int pace_fgetpos(PACE_FILE * stream,
                               pace_fpos_t * pos);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

  /**
     PACE's implementation of the POSIX function fgets.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
#if (PACE_HAS_POSIX_DI_UOF)
  PACE_Export PACE_INLINE char * pace_fgets (char * s, int n,
                                             PACE_FILE * stream);
#endif /* PACE_HAS_POSIX_DI_UOF */

  /**
     PACE's implementation of the POSIX function flockfile.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.2.6.
     */
#if (PACE_HAS_POSIX_FL_UOF)
  PACE_Export PACE_INLINE void pace_flockfile (PACE_FILE * file);
#endif /* PACE_HAS_POSIX_FL_UOF */
  /* Requires PACE_HAS_REENTRANT. */

  /**
     PACE's implementation of the POSIX function fopen.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
#if (PACE_HAS_POSIX_DI_UOF)
  PACE_Export PACE_INLINE PACE_FILE * pace_fopen (const char * filename,
                                                  const char * mode);
#endif /* PACE_HAS_POSIX_DI_UOF */

  /**
     PACE's implementation of the POSIX function fprintf.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
#if (PACE_HAS_POSIX_DI_UOF)
# define pace_fprintf fprintf
#endif /* PACE_HAS_POSIX_DI_UOF */
  /*  int pace_fprintf (PACE_FILE *stream, const char *format, ...);*/

  /**
     PACE's implementation of the POSIX function fputc.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
#if (PACE_HAS_POSIX_DI_UOF)
  PACE_Export PACE_INLINE int pace_fputc (int c, PACE_FILE * stream);
#endif /* PACE_HAS_POSIX_DI_UOF */

  /**
     PACE's implementation of the POSIX function fputs.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
#if (PACE_HAS_POSIX_DI_UOF)
  PACE_Export PACE_INLINE int pace_fputs (const char * s, PACE_FILE * stream);
#endif /* PACE_HAS_POSIX_DI_UOF */

  /**
     PACE's implementation of the POSIX function fread.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
#if (PACE_HAS_POSIX_DI_UOF)
  PACE_Export PACE_INLINE pace_size_t pace_fread (void * ptr,
                                                  pace_size_t size,
                                                  pace_size_t number_of_items,
                                                  PACE_FILE * stream);
#endif /* PACE_HAS_POSIX_DI_UOF */

  /**
     PACE's implementation of the POSIX function freopen.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
#if (PACE_HAS_POSIX_DI_UOF)
  PACE_Export PACE_INLINE FILE * pace_freopen (const char * filename,
                                               const char * mode,
                                               PACE_FILE * stream);
#endif /* PACE_HAS_POSIX_DI_UOF */

  /**
     PACE's implementation of the POSIX function fscanf.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
#if (PACE_HAS_POSIX_DI_UOF)
#  define pace_fscanf fscanf
#endif /* PACE_HAS_POSIX_DI_UOF */
  /* int pace_scanf (const char *format, ... ); */

  /**
     PACE's implementation of the POSIX function fseek.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
#if (PACE_HAS_POSIX_FM_UOF)
  PACE_Export PACE_INLINE int pace_fseek (PACE_FILE * stream,
                                          long offset,
                                          int whence);
#endif /* PACE_HAS_POSIX_FM_UOF */

  /**
    C std 7.19.9.3
    */
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  PACE_Export PACE_INLINE int pace_fsetpos(PACE_FILE *stream,
                                           const pace_fpos_t *pos);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

  /**
     PACE's implementation of the POSIX function ftell.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
#if (PACE_HAS_POSIX_FM_UOF)
  PACE_Export PACE_INLINE long pace_ftell (PACE_FILE * stream);
#endif /* PACE_HAS_POSIX_FM_UOF */

  /**
     PACE's implementation of the POSIX function ftrylockfile.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.2.6.
     */
#if (PACE_HAS_POSIX_FL_UOF)
  PACE_Export PACE_INLINE int pace_ftrylockfile (PACE_FILE * file);
#endif /* PACE_HAS_POSIX_FL_UOF */
  /* Requires PACE_HAS_REENTRANT. */

  /**
     PACE's implementation of the POSIX function funlockfile.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.2.6.
     */
#if (PACE_HAS_POSIX_FL_UOF)
  PACE_Export PACE_INLINE void pace_funlockfile (PACE_FILE * file);
#endif /* PACE_HAS_POSIX_FL_UOF */
  /* Requires PACE_HAS_REENTRANT. */

  /**
    C std 7.19.8.2
    */
#if (PACE_HAS_POSIX_DI_UOF)
  PACE_Export PACE_INLINE pace_size_t pace_fwrite(const void * ptr,
                                                  pace_size_t size,
                                                  pace_size_t nmemb,
                                                  PACE_FILE * stream);
#endif /* PACE_HAS_POSIX_DI_UOF */

  /**
     PACE's implementation of the POSIX function getc.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
#if (PACE_HAS_POSIX_DI_UOF)
  PACE_Export PACE_INLINE int pace_getc (PACE_FILE * stream);
#endif /* PACE_HAS_POSIX_DI_UOF */

  /**
     PACE's implementation of the POSIX function getc_unlocked.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.2.7.
     */
#if (PACE_HAS_POSIX_FL_UOF)
  PACE_Export PACE_INLINE int pace_getc_unlocked (PACE_FILE * stream);
#endif /* PACE_HAS_POSIX_FL_UOF */
  /* Requires PACE_HAS_REENTRANT. */

  /**
     PACE's implementation of the POSIX function getchar.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
#if (PACE_HAS_POSIX_DI_UOF)
  PACE_Export PACE_INLINE int pace_getchar ();
#endif /* PACE_HAS_POSIX_DI_UOF */

  /**
     PACE's implementation of the POSIX function getchar_unlocked.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.2.7.
     */
#if (PACE_HAS_POSIX_FL_UOF)
  PACE_Export PACE_INLINE int pace_getchar_unlocked ();
#endif /* PACE_HAS_POSIX_FL_UOF */
  /* Requires PACE_HAS_REENTRANT. */

  /**
     PACE's implementation of the POSIX function gets.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
#if (PACE_HAS_POSIX_DI_UOF)
  PACE_Export PACE_INLINE char *pace_gets (char * s);
#endif /* PACE_HAS_POSIX_DI_UOF */

  /**
     PACE's implementation of the POSIX function perror.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
#if (PACE_HAS_POSIX_DI_UOF)
  PACE_Export PACE_INLINE void pace_perror (const char * s);
#endif /* PACE_HAS_POSIX_DI_UOF */

  /**
     PACE's implementation of the POSIX function printf.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
#if (PACE_HAS_POSIX_DI_UOF)
# define pace_printf printf
#endif /* PACE_HAS_POSIX_DI_UOF */
  /*  int pace_printf (const char* format, ...); */

  /**
     PACE's implementation of the POSIX function putc.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
#if (PACE_HAS_POSIX_DI_UOF)
  PACE_Export PACE_INLINE int pace_putc (int c, PACE_FILE * stream);
#endif /* PACE_HAS_POSIX_DI_UOF */

  /**
     PACE's implementation of the POSIX function putc_unlocked.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.2.7.
     */
#if (PACE_HAS_POSIX_FL_UOF)
  PACE_Export PACE_INLINE int pace_putc_unlocked (int c, PACE_FILE * stream);
#endif /* PACE_HAS_POSIX_FL_UOF */
  /* Requires PACE_HAS_REENTRANT. */

  /**
     PACE's implementation of the POSIX function putchar.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
#if (PACE_HAS_POSIX_DI_UOF)
  PACE_Export PACE_INLINE int pace_putchar (int c);
#endif /* PACE_HAS_POSIX_DI_UOF */

  /**
     PACE's implementation of the POSIX function putchar_unlocked.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.2.7.
     */
#if (PACE_HAS_POSIX_FL_UOF)
  PACE_Export PACE_INLINE int pace_putchar_unlocked (int c);
#endif /* PACE_HAS_POSIX_FL_UOF */
  /* Requires PACE_HAS_REENTRANT. */

  /**
     PACE's implementation of the POSIX function puts.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
#if (PACE_HAS_POSIX_DI_UOF)
  PACE_Export PACE_INLINE int pace_puts (const char * s);
#endif /* PACE_HAS_POSIX_DI_UOF */

  /**
     PACE's implementation of the POSIX function remove.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
#if (PACE_HAS_POSIX_FS_UOF)
  PACE_Export PACE_INLINE int pace_remove (const char * path);
#endif /* PACE_HAS_POSIX_FS_UOF */

  /**
     PACE's implementation of the POSIX function rename.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 5.5.3.
     */
#if (PACE_HAS_POSIX_FS_UOF)
  PACE_Export PACE_INLINE int pace_rename (const char * old_name,
                                           const char * new_name);
#endif /* PACE_HAS_POSIX_FS_UOF */

  /**
     PACE's implementation of the POSIX function rewind.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
#if (PACE_HAS_POSIX_FM_UOF)
  PACE_Export PACE_INLINE void pace_rewind (PACE_FILE * stream);
#endif /* PACE_HAS_POSIX_FM_UOF */

  /**
     PACE's implementation of the POSIX function sprintf.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
#if (PACE_HAS_POSIX_DI_UOF)
# define pace_sprintf sprintf
#endif /* PACE_HAS_POSIX_DI_UOF */
  /*  int pace_sprintf (char* s, const char* format, ...); */

  /**
     PACE's implementation of the POSIX function scanf.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
#if (PACE_HAS_POSIX_DI_UOF)
# define pace_scanf scanf
#endif /* PACE_HAS_POSIX_DI_UOF */
  /* int pace_scanf (const char *format, ... ); */

  /**
     PACE's implementation of the POSIX function setbuf.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
#if (PACE_HAS_POSIX_DI_UOF)
  PACE_Export PACE_INLINE void pace_setbuf (PACE_FILE * stream, char * buf);
#endif /* PACE_HAS_POSIX_DI_UOF */


  /**
    C std 7.19.5.6
    */
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  PACE_Export PACE_INLINE int pace_setvbuf(PACE_FILE * stream,
                                           char * buf,
                                           int mode, pace_size_t size);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */
  /**
     PACE's implementation of the POSIX function sscanf.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
#if (PACE_HAS_POSIX_DI_UOF)
# define pace_sscanf sscanf
#endif /* PACE_HAS_POSIX_DI_UOF */
  /* int pace_sscanf (const char *s, const char *format, ... ); */

  /**
     PACE's implementation of the POSIX function tmpfile.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
#if (PACE_HAS_POSIX_FS_UOF)
  PACE_Export PACE_INLINE FILE * pace_tmpfile ();
#endif /* PACE_HAS_POSIX_FS_UOF */

  /**
     PACE's implementation of the POSIX function tmpnam.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
#if (PACE_HAS_POSIX_FS_UOF)
  PACE_Export PACE_INLINE char * pace_tmpnam (char * s);
#endif /* PACE_HAS_POSIX_FS_UOF */

  /**
     PACE's implementation of the POSIX function ungetc.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
#if (PACE_HAS_POSIX_DI_UOF)
  PACE_Export PACE_INLINE int pace_ungetc (int c, FILE * stream);
#endif /* PACE_HAS_POSIX_DI_UOF */

  /**
    C std 7.19.6.8
    */
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  PACE_Export PACE_INLINE int pace_vfprintf (PACE_FILE * stream,
                                             const char * format,
                                             va_list arg);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

  /**
    C std 7.19.6.10
    */
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  PACE_Export PACE_INLINE int pace_vprintf (const char * format,
                                            va_list arg);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

  /**
    C std 7.19.6.13
    */
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  PACE_Export PACE_INLINE int pace_vsprintf (char * s,
                                             const char * format,
                                             va_list arg);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if defined (PACE_HAS_INLINE)
#  if (PACE_HAS_POSIX)
#    include "pace/posix/stdio.inl"
#  elif (PACE_VXWORKS)
#    include "pace/vxworks/stdio.inl"
#  elif (PACE_WIN32)
#    include "pace/win32/stdio.inl"
#  endif
#endif /* PACE_HAS_INLINE */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_STDIO_H */
