/* $Id$ -*- C -*-

 * ===========================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/posix/stdio.inl
 *
 * = AUTHOR
 *    Luther Baker
 *
 * =========================================================================== */

#include <stdarg.h>

#if (PACE_HAS_POSIX_DI_UOF)
PACE_INLINE
void
pace_clearerr (FILE * stream)
{
  clearerr (stream);
  return;
}
#endif /* PACE_HAS_POSIX_DI_UOF */

#if (PACE_HAS_POSIX_DS_UOF)
PACE_INLINE
char *
pace_ctermid (char * s)
{
  return ctermid (s);
}
#endif /* PACE_HAS_POSIX_DS_UOF */

#if (PACE_HAS_POSIX_DI_UOF)
PACE_INLINE
int
pace_fclose (FILE * stream)
{
  return fclose (stream);
}
#endif /* PACE_HAS_POSIX_DI_UOF */

#if (PACE_HAS_POSIX_DI_UOF)
PACE_INLINE
FILE *
pace_fdopen (int fildes,
             const char * type)
{
  return fdopen (fildes,
                 type);
}
#endif /* PACE_HAS_POSIX_DI_UOF */

#if (PACE_HAS_POSIX_DI_UOF)
PACE_INLINE
int
pace_ferror (FILE * stream)
{
  return ferror (stream);
}
#endif /* PACE_HAS_POSIX_DI_UOF */

#if (PACE_HAS_POSIX_DI_UOF)
PACE_INLINE
int
pace_feof (FILE * stream)
{
  return feof (stream);
}
#endif /* PACE_HAS_POSIX_DI_UOF */

#if (PACE_HAS_POSIX_DI_UOF)
PACE_INLINE
int
pace_fflush (FILE * stream)
{
  return fflush (stream);
}
#endif /* PACE_HAS_POSIX_DI_UOF */

#if (PACE_HAS_POSIX_DI_UOF)
PACE_INLINE
int
pace_fileno (FILE * stream)
{
  return fileno (stream);
}
#endif /* PACE_HAS_POSIX_DI_UOF */

#if (PACE_HAS_POSIX_DI_UOF)
PACE_INLINE
int
pace_fgetc (FILE * stream)
{
  return fgetc (stream);
}
#endif /* PACE_HAS_POSIX_DI_UOF */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_fgetpos(PACE_FILE * stream,
             pace_fpos_t * pos)
{
  return fgetpos (stream, pos);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_DI_UOF)
PACE_INLINE
char *
pace_fgets (char * s,
            int n,
            FILE * stream)
{
  return fgets (s,
                n,
                stream);
}
#endif /* PACE_HAS_POSIX_DI_UOF */

#if (PACE_HAS_POSIX_FL_UOF)
PACE_INLINE
void
pace_flockfile (FILE * file)
{
#if defined (PACE_HAS_REENTRANT)
  flockfile (file);
#else /* ! PACE_HAS_REENTRANT */
  PACE_UNUSED_ARG (file);
  PACE_ERRNO_NO_SUPPORT ();
#endif /* ! PACE_HAS_REENTRANT */
  return;
}
#endif /* PACE_HAS_POSIX_FL_UOF */

#if (PACE_HAS_POSIX_DI_UOF)
PACE_INLINE
FILE *
pace_fopen (const char * filename,
            const char * mode)
{
  return fopen (filename,
                mode);
}
#endif /* PACE_HAS_POSIX_DI_UOF */

#if (PACE_HAS_POSIX_DI_UOF)
PACE_INLINE
int
pace_fputc (int c,
            FILE * stream)
{
  return fputc (c,
                stream);
}
#endif /* PACE_HAS_POSIX_DI_UOF */

#if (PACE_HAS_POSIX_DI_UOF)
PACE_INLINE
int
pace_fputs (const char * s,
            FILE * stream)
{
  return fputs (s,
                stream);
}
#endif /* PACE_HAS_POSIX_DI_UOF */

#if (PACE_HAS_POSIX_DI_UOF)
PACE_INLINE
size_t
pace_fread (void * ptr,
            size_t size,
            size_t number_of_items,
            FILE * stream)
{
  return fread (ptr,
                size,
                number_of_items,
                stream);
}
#endif /* PACE_HAS_POSIX_DI_UOF */

#if (PACE_HAS_POSIX_DI_UOF)
PACE_INLINE
FILE *
pace_freopen (const char * filename,
              const char * mode,
              FILE * stream)
{
  return freopen (filename,
                  mode,
                  stream);
}
#endif /* PACE_HAS_POSIX_DI_UOF */

#if (PACE_HAS_POSIX_FM_UOF)
PACE_INLINE
int
pace_fseek (FILE * stream,
            long offset,
            int whence)
{
  return fseek (stream,
                offset,
                whence);
}
#endif /* PACE_HAS_POSIX_FM_UOF */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_fsetpos(PACE_FILE *stream, const pace_fpos_t *pos)
{
  return fsetpos (stream, pos);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_FM_UOF)
PACE_INLINE
long
pace_ftell (FILE * stream)
{
  return ftell (stream);
}
#endif /* PACE_HAS_POSIX_FM_UOF */

#if (PACE_HAS_POSIX_FL_UOF)
PACE_INLINE
int
pace_ftrylockfile (FILE * file)
{
#if defined (PACE_HAS_REENTRANT)
  return ftrylockfile (file);
#else /* ! PACE_HAS_REENTRANT */
  PACE_UNUSED_ARG(file);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
#endif /* ! PACE_HAS_REENTRANT */
}
#endif /* PACE_HAS_POSIX_FL_UOF */

#if (PACE_HAS_POSIX_FL_UOF)
PACE_INLINE
void
pace_funlockfile (FILE * file)
{
#if defined (PACE_HAS_REENTRANT)
  funlockfile (file);
#else /* ! PACE_HAS_REENTRANT */
  PACE_UNUSED_ARG(file);
  PACE_ERRNO_NO_SUPPORT ();
#endif /* ! PACE_HAS_REENTRANT */
  return;
}
#endif /* PACE_HAS_POSIX_FL_UOF */

#if (PACE_HAS_POSIX_DI_UOF)
PACE_INLINE
pace_size_t
pace_fwrite(const void * ptr,
            pace_size_t size, pace_size_t nmemb,
            PACE_FILE * stream)
{
  return fwrite (ptr, size, nmemb, stream);
}
#endif /* PACE_HAS_POSIX_DI_UOF */

#if (PACE_HAS_POSIX_DI_UOF)
PACE_INLINE
int
pace_getc (FILE * stream)
{
  return getc (stream);
}
#endif /* PACE_HAS_POSIX_DI_UOF */

#if (PACE_HAS_POSIX_FL_UOF)
PACE_INLINE
int
pace_getc_unlocked (FILE * stream)
{
#if defined (PACE_HAS_REENTRANT)
  return getc_unlocked (stream);
#else /* ! PACE_HAS_POSIX */
  PACE_UNUSED_ARG(stream);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
#endif /* ! PACE_HAS_POSIX */
}
#endif /* PACE_HAS_POSIX_FL_UOF */

#if (PACE_HAS_POSIX_DI_UOF)
PACE_INLINE
int
pace_getchar ()
{
  return getchar ();
}
#endif /* PACE_HAS_POSIX_DI_UOF */

#if (PACE_HAS_POSIX_FL_UOF)
PACE_INLINE
int
pace_getchar_unlocked ()
{
#if defined (PACE_HAS_REENTRANT)
  return getchar_unlocked ();
#else /* ! PACE_HAS_REENTRANT */
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
#endif /* ! PACE_HAS_REENTRANT */
}
#endif /* PACE_HAS_POSIX_FL_UOF */

#if (PACE_HAS_POSIX_DI_UOF)
PACE_INLINE
char *
pace_gets (char * s)
{
  return gets (s);
}
#endif /* PACE_HAS_POSIX_DI_UOF */

#if (PACE_HAS_POSIX_DI_UOF)
PACE_INLINE
void
pace_perror (const char * s)
{
  perror (s);
  return;
}
#endif /* PACE_HAS_POSIX_DI_UOF */

#if (PACE_HAS_POSIX_DI_UOF)
PACE_INLINE
int
pace_putc (int c,
           FILE * stream)
{
  return putc (c, stream);
}
#endif /* PACE_HAS_POSIX_DI_UOF */

#if (PACE_HAS_POSIX_FL_UOF)
PACE_INLINE
int
pace_putc_unlocked (int c,
                    FILE * stream)
{
#if defined (PACE_HAS_REENTRANT)
  return putc_unlocked (c, stream);
#else /* ! PACE_HAS_REENTRANT */
  PACE_UNUSED_ARG(c);
  PACE_UNUSED_ARG(stream);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
#endif /* ! PACE_HAS_REENTRANT */
}
#endif /* PACE_HAS_POSIX_FL_UOF */

#if (PACE_HAS_POSIX_DI_UOF)
PACE_INLINE
int
pace_putchar (int c)
{
  return putchar (c);
}
#endif /* PACE_HAS_POSIX_DI_UOF */

#if (PACE_HAS_POSIX_FL_UOF)
PACE_INLINE
int
pace_putchar_unlocked (int c)
{
#if defined (PACE_HAS_REENTRANT)
  return putchar_unlocked (c);
#else /* ! PACE_HAS_REENTRANT */
  PACE_UNUSED_ARG(c);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
#endif /* ! PACE_HAS_REENTRANT */
}
#endif /* PACE_HAS_POSIX_FL_UOF */

#if (PACE_HAS_POSIX_DI_UOF)
PACE_INLINE
int
pace_puts (const char * s)
{
  return puts (s);
}
#endif /* PACE_HAS_POSIX_DI_UOF */

#if (PACE_HAS_POSIX_FS_UOF)
PACE_INLINE
int
pace_remove (const char * path)
{
  return remove (path);
}
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_FS_UOF)
PACE_INLINE
int
pace_rename (const char * old_name,
             const char * new_name)
{
  return rename (old_name,
                 new_name);
}
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_FM_UOF)
PACE_INLINE
void
pace_rewind (FILE * stream)
{
  rewind (stream);
  return;
}
#endif /* PACE_HAS_POSIX_FM_UOF */

#if (PACE_HAS_POSIX_DI_UOF)
PACE_INLINE
void
pace_setbuf (FILE * stream, char * buf)
{
  setbuf (stream, buf);
  return;
}
#endif /* PACE_HAS_POSIX_DI_UOF */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_setvbuf(PACE_FILE * stream,
             char * buf,
             int mode, pace_size_t size)
{
  return setvbuf (stream, buf, mode, size);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_FS_UOF)
PACE_INLINE
FILE *
pace_tmpfile ()
{
 return tmpfile ();
}
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_FS_UOF)
PACE_INLINE
char *
pace_tmpnam (char * s)
{
 return tmpnam (s);
}
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_DI_UOF)
PACE_INLINE
int
pace_ungetc (int c, FILE * stream)
{
 return ungetc (c, stream);
}
#endif /* PACE_HAS_POSIX_DI_UOF */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_vfprintf (PACE_FILE * stream,
               const char * format,
               va_list arg)
{
  return vfprintf (stream, format, arg);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_vprintf (const char * format,
              va_list arg)
{
  return vprintf (format, arg);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_vsprintf (char * s,
               const char * format,
               va_list arg)
{
  return vsprintf (s, format, arg);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */
