/* $Id$ -*- C -*-

 * ===========================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/vxworks/stdio.inl
 *
 * = AUTHOR
 *    Joe Hoffert
 *
 * =========================================================================== */

#include <stdarg.h>

#if (PACE_HAS_POSIX_DI_UOF)
PACE_INLINE
void
pace_clearerr (FILE * stream)
{
  PACE_TRACE("pace_clearerr");

  clearerr (stream);
  return;
}
#endif /* PACE_HAS_POSIX_DI_UOF */

#if (PACE_HAS_POSIX_DS_UOF)
PACE_INLINE
char *
pace_ctermid (char * s)
{
  PACE_TRACE("pace_ctermid");

  PACE_UNUSED_ARG(s);
  PACE_ERRNO_NO_SUPPORT_RETURN ((char *) 0);
}
#endif /* PACE_HAS_POSIX_DS_UOF */

#if (PACE_HAS_POSIX_DI_UOF)
PACE_INLINE
int
pace_fclose (FILE * stream)
{
  PACE_TRACE("pace_fclose");

  return fclose (stream);
}
#endif /* PACE_HAS_POSIX_DI_UOF */

#if (PACE_HAS_POSIX_DI_UOF)
PACE_INLINE
FILE *
pace_fdopen (int fildes,
             const char * type)
{
  PACE_TRACE("pace_fdopen");

  return fdopen (fildes,
                 type);
}
#endif /* PACE_HAS_POSIX_DI_UOF */

#if (PACE_HAS_POSIX_DI_UOF)
PACE_INLINE
int
pace_ferror (FILE * stream)
{
  PACE_TRACE("pace_ferror");

  return ferror (stream);
}
#endif /* PACE_HAS_POSIX_DI_UOF */

#if (PACE_HAS_POSIX_DI_UOF)
PACE_INLINE
int
pace_feof (FILE * stream)
{
  PACE_TRACE("pace_feof");

  return feof (stream);
}
#endif /* PACE_HAS_POSIX_DI_UOF */

#if (PACE_HAS_POSIX_DI_UOF)
PACE_INLINE
int
pace_fflush (FILE * stream)
{
  PACE_TRACE("pace_fflush");

  return fflush (stream);
}
#endif /* PACE_HAS_POSIX_DI_UOF */

#if (PACE_HAS_POSIX_DI_UOF)
PACE_INLINE
int
pace_fileno (FILE * stream)
{
  PACE_TRACE("pace_fileno");

  return fileno (stream);
}
#endif /* PACE_HAS_POSIX_DI_UOF */

#if (PACE_HAS_POSIX_DI_UOF)
PACE_INLINE
int
pace_fgetc (FILE * stream)
{
  PACE_TRACE("pace_fgetc");

  return fgetc (stream);
}
#endif /* PACE_HAS_POSIX_DI_UOF */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_fgetpos (PACE_FILE * stream,
              pace_fpos_t * pos)
{
  PACE_TRACE("pace_fgetpos");

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
  PACE_TRACE("pace_fgets");

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
  PACE_TRACE("pace_flockfile");

  PACE_UNUSED_ARG (file);
  PACE_ERRNO_NO_SUPPORT ();
  return;
}
#endif /* PACE_HAS_POSIX_FL_UOF */

#if (PACE_HAS_POSIX_DI_UOF)
PACE_INLINE
FILE *
pace_fopen (const char * filename,
            const char * mode)
{
  PACE_TRACE("pace_fopen");

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
  PACE_TRACE("pace_fputc");

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
  PACE_TRACE("pace_fputs");

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
  PACE_TRACE("pace_fread");

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
  PACE_TRACE("pace_freopen");

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
  PACE_TRACE("pace_fseek");

  return fseek (stream,
                offset,
                whence);
}
#endif /* PACE_HAS_POSIX_FM_UOF */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_fsetpos (PACE_FILE *stream, const pace_fpos_t *pos)
{
  PACE_TRACE("pace_fsetpos");

  return fsetpos (stream, pos);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_FM_UOF)
PACE_INLINE
long
pace_ftell (FILE * stream)
{
  PACE_TRACE("pace_ftell");

  return ftell (stream);
}
#endif /* PACE_HAS_POSIX_FM_UOF */

#if (PACE_HAS_POSIX_FL_UOF)
PACE_INLINE
int
pace_ftrylockfile (FILE * file)
{
  PACE_TRACE("pace_ftrylockfile");

  PACE_UNUSED_ARG(file);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_FL_UOF */

#if (PACE_HAS_POSIX_FL_UOF)
PACE_INLINE
void
pace_funlockfile (FILE * file)
{
  PACE_TRACE("pace_funlockfile");

  PACE_UNUSED_ARG(file);
  PACE_ERRNO_NO_SUPPORT ();
  return;
}
#endif /* PACE_HAS_POSIX_FL_UOF */

#if (PACE_HAS_POSIX_DI_UOF)
PACE_INLINE
pace_size_t
pace_fwrite (const void * ptr,
             pace_size_t size, pace_size_t nmemb,
             PACE_FILE * stream)
{
  PACE_TRACE("pace_fwrite");

  return fwrite (ptr, size, nmemb, stream);
}
#endif /* PACE_HAS_POSIX_DI_UOF */

#if (PACE_HAS_POSIX_DI_UOF)
PACE_INLINE
int
pace_getc (FILE * stream)
{
  PACE_TRACE("pace_getc");

  return getc (stream);
}
#endif /* PACE_HAS_POSIX_DI_UOF */

#if (PACE_HAS_POSIX_FL_UOF)
PACE_INLINE
int
pace_getc_unlocked (FILE * stream)
{
  PACE_TRACE("pace_getc_unlocked");

  PACE_UNUSED_ARG(stream);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_FL_UOF */

#if (PACE_HAS_POSIX_DI_UOF)
PACE_INLINE
int
pace_getchar ()
{
  PACE_TRACE("pace_getchar");

  return getchar ();
}
#endif /* PACE_HAS_POSIX_DI_UOF */

#if (PACE_HAS_POSIX_FL_UOF)
PACE_INLINE
int
pace_getchar_unlocked ()
{
  PACE_TRACE("pace_getchar_unlocked");

  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_FL_UOF */

#if (PACE_HAS_POSIX_DI_UOF)
PACE_INLINE
char *
pace_gets (char * s)
{
  PACE_TRACE("pace_gets");

  return gets (s);
}
#endif /* PACE_HAS_POSIX_DI_UOF */

#if (PACE_HAS_POSIX_DI_UOF)
PACE_INLINE
void
pace_perror (const char * s)
{
  PACE_TRACE("pace_perror");

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
  PACE_TRACE("pace_putc");

  return putc (c, stream);
}
#endif /* PACE_HAS_POSIX_DI_UOF */

#if (PACE_HAS_POSIX_FL_UOF)
PACE_INLINE
int
pace_putc_unlocked (int c,
                    FILE * stream)
{
  PACE_TRACE("pace_putc_unlocked");

  PACE_UNUSED_ARG(c);
  PACE_UNUSED_ARG(stream);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_FL_UOF */

#if (PACE_HAS_POSIX_DI_UOF)
PACE_INLINE
int
pace_putchar (int c)
{
  PACE_TRACE("pace_putchar");

  return putchar (c);
}
#endif /* PACE_HAS_POSIX_DI_UOF */

#if (PACE_HAS_POSIX_FL_UOF)
PACE_INLINE
int
pace_putchar_unlocked (int c)
{
  PACE_TRACE("pace_putchar_unlocked");

  PACE_UNUSED_ARG(c);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_FL_UOF */

#if (PACE_HAS_POSIX_DI_UOF)
PACE_INLINE
int
pace_puts (const char * s)
{
  PACE_TRACE("pace_puts");

  return puts (s);
}
#endif /* PACE_HAS_POSIX_DI_UOF */

#if (PACE_HAS_POSIX_FS_UOF)
PACE_INLINE
int
pace_remove (const char * path)
{
  PACE_TRACE("pace_remove");

  return remove (path);
}
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_FS_UOF)
PACE_INLINE
int
pace_rename (const char * old_name,
             const char * new_name)
{
  PACE_TRACE("pace_rename");

  return rename (old_name,
                 new_name);
}
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_FM_UOF)
PACE_INLINE
void
pace_rewind (FILE * stream)
{
  PACE_TRACE("pace_rewind");

  rewind (stream);
  return;
}
#endif /* PACE_HAS_POSIX_FM_UOF */

#if (PACE_HAS_POSIX_DI_UOF)
PACE_INLINE
void
pace_setbuf (FILE * stream, char * buf)
{
  PACE_TRACE("pace_setbuf");

  setbuf (stream, buf);
  return;
}
#endif /* PACE_HAS_POSIX_DI_UOF */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_setvbuf (PACE_FILE * stream,
              char * buf,
              int mode, pace_size_t size)
{
  PACE_TRACE("pace_setvbuf");

  return setvbuf (stream, buf, mode, size);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_FS_UOF)
PACE_INLINE
FILE *
pace_tmpfile ()
{
  PACE_TRACE("pace_tmpfile");

  return tmpfile ();
}
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_FS_UOF)
PACE_INLINE
char *
pace_tmpnam (char * s)
{
  PACE_TRACE("pace_tmpnam");

  return tmpnam (s);
}
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_DI_UOF)
PACE_INLINE
int
pace_ungetc (int c, FILE * stream)
{
  PACE_TRACE("pace_ungetc");

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
  PACE_TRACE("pace_vfprintf");

  return vfprintf (stream, format, arg);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_vprintf (const char * format,
              va_list arg)
{
  PACE_TRACE("pace_vprintf");

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
  PACE_TRACE("pace_vsprintf");

  return vsprintf (s, format, arg);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */
