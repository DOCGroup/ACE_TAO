/* $Id$ -*- C -*-

 * ===========================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/stdio.inl
 *
 * = AUTHOR
 *    Luther Baker
 *
 * =========================================================================== */

#include <stdarg.h>

PACE_INLINE
void
pace_clearerr (FILE * stream)
{
  clearerr (stream);
  return;
}

PACE_INLINE
int
pace_fclose (FILE * stream)
{
  return fclose (stream);
}

PACE_INLINE
FILE *
pace_fdopen (int fildes,
             const char * type)
{
  return fdopen (fildes,
                 type);
}

PACE_INLINE
int
pace_ferror (FILE * stream)
{
  return ferror (stream);
}

PACE_INLINE
int
pace_feof (FILE * stream)
{
  return feof (stream);
}

PACE_INLINE
int
pace_fflush (FILE * stream)
{
  return fflush (stream);
}

PACE_INLINE
int
pace_fileno (FILE * stream)
{
  return fileno (stream);
}

PACE_INLINE
int
pace_fgetc (FILE * stream)
{
  return fgetc (stream);
}

PACE_INLINE
int
pace_fgetpos(PACE_FILE * stream,
             pace_fpos_t * pos)
{
  return fgetpos (stream, pos);
}

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

PACE_INLINE
FILE *
pace_fopen (const char * filename,
            const char * mode)
{
  return fopen (filename,
                mode);
}

PACE_INLINE
int
pace_fputc (int c,
            FILE * stream)
{
  return fputc (c,
                stream);
}

int
pace_fprintf (FILE *fp, const char *format, ...)
{
  int result = 0;
  va_list ap;
  va_start (ap, format);
  result = vfprintf (fp, format, ap);
  va_end (ap);
  return result;
}

PACE_INLINE
int
pace_fputs (const char * s,
            FILE * stream)
{
  return fputs (s,
                stream);
}

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

PACE_INLINE
int
pace_fsetpos(PACE_FILE *stream, const pace_fpos_t *pos)
{
  return fsetpos (stream, pos);
}

PACE_INLINE
long
pace_ftell (FILE * stream)
{
  return ftell (stream);
}

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

PACE_INLINE
pace_size_t
pace_fwrite(const void * ptr,
            pace_size_t size, pace_size_t nmemb,
            PACE_FILE * stream)
{
  return fwrite (ptr, size, nmemb, stream);
}

PACE_INLINE
int
pace_getc (FILE * stream)
{
  return getc (stream);
}

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

PACE_INLINE
int
pace_getchar ()
{
  return getchar ();
}

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

PACE_INLINE
char *
pace_gets (char * s)
{
  return gets (s);
}

PACE_INLINE
void
pace_perror (const char * s)
{
  perror (s);
  return;
}

int
pace_printf (const char* format, ...)
{
  int result = 0;
  va_list ap;
  va_start (ap, format);
  result = vprintf (format, ap);
  va_end (ap);
  return result;
}

PACE_INLINE
int
pace_putc (int c,
           FILE * stream)
{
  return putc (c, stream);
}

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

PACE_INLINE
int
pace_putchar (int c)
{
  return putchar (c);
}

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

PACE_INLINE
int
pace_puts (const char * s)
{
  return puts (s);
}

PACE_INLINE
int
pace_remove (const char * path)
{
  return remove (path);
}

PACE_INLINE
int
pace_rename (const char * old_name,
             const char * new_name)
{
  return rename (old_name,
                 new_name);
}

PACE_INLINE
void
pace_rewind (FILE * stream)
{
  rewind (stream);
  return;
}

PACE_INLINE
void
pace_setbuf (FILE * stream, char * buf)
{
  setbuf (stream, buf);
  return;
}

PACE_INLINE
int
pace_setvbuf(PACE_FILE * stream,
             char * buf,
             int mode, pace_size_t size)
{
  return setvbuf (stream, buf, mode, size);
}
int
pace_sprintf (char* s, const char* format, ...)
{
  int result = 0;
  va_list ap;
  va_start (ap, format);
  result = vsprintf (s, format, ap);
  va_end (ap);
  return result;
}

PACE_INLINE
FILE *
pace_tmpfile ()
{
 return tmpfile ();
}

PACE_INLINE
char *
pace_tmpnam (char * s)
{
 return tmpnam (s);
}

PACE_INLINE
int
pace_ungetc (int c, FILE * stream)
{
 return ungetc (c, stream);
}

PACE_INLINE
int
pace_vfprintf (PACE_FILE * stream,
               const char * format,
               va_list arg)
{
  /*  return vfprintf (stream, format, arg); */
  PACE_UNUSED_ARG (stream);
  PACE_UNUSED_ARG (format);
  PACE_UNUSED_ARG (arg);
  return -1;
}

PACE_INLINE
int
pace_vprintf (const char * format,
              va_list arg)
{
  /*  return vfprintf (format, arg); */
  PACE_UNUSED_ARG (format);
  PACE_UNUSED_ARG (arg);
  return -1;
}

PACE_INLINE
int
pace_vsprintf (char * s,
               const char * format,
               va_list arg)
{
  /*  return vsprintf (s, format, arg);*/
  PACE_UNUSED_ARG (s);
  PACE_UNUSED_ARG (format);
  PACE_UNUSED_ARG (arg);
  return -1;
}
