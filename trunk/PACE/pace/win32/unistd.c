/* $Id$

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/unistd.c
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

#include "pace/unistd.h"

#if !defined (PACE_HAS_INLINE)
# include "pace/win32/unistd.inl"
#endif /* ! PACE_HAS_INLINE */

#if (PACE_HAS_POSIX_DI_UOF)
int
pace_win32_close (PACE_HANDLE fildes)
{
  PACE_WIN32CALL_RETURN
    (PACE_ADAPT_RETVAL
     (CloseHandle (fildes), pace_result_), int, -1);
}
#endif /* PACE_HAS_POSIX_DI_UOF */

#if (PACE_HAS_POSIX_FM_UOF)
PACE_HANDLE
pace_win32_dup (PACE_HANDLE fildes)
{
  PACE_HANDLE new_fd;
  if (DuplicateHandle(GetCurrentProcess (),
                      fildes,
                      GetCurrentProcess(),
                      &new_fd,
                      0,
                      TRUE,
                      DUPLICATE_SAME_ACCESS))
    {
      return new_fd;
    }
  else
    {
      PACE_FAIL_RETURN (PACE_INVALID_HANDLE);
    }
}
#endif /* PACE_HAS_POSIX_FM_UOF */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
int
pace_win32_ftruncate (PACE_HANDLE fildes, pace_off_t length)
{
  if (SetFilePointer (fildes, length, NULL, FILE_BEGIN) != (unsigned) -1)
    {
      PACE_WIN32CALL_RETURN
        (PACE_ADAPT_RETVAL
         (SetEndOfFile (fildes), pace_result_), int, -1);
    }
  else
    {
      PACE_FAIL_RETURN (-1);
    }
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_FM_UOF)
pace_off_t
pace_win32_lseek (PACE_HANDLE fildes, pace_off_t offset, int whence)
{
# if SEEK_SET != FILE_BEGIN \
  || SEEK_CUR != FILE_CURRENT \
  || SEEK_END != FILE_END

  /* #error Windows NT is evil AND rude! */
  switch (whence) {

    case SEEK_SET: {
      whence = FILE_BEGIN;
      break;
    }
    case SEEK_CUR: {
      whence = FILE_CURRENT;
      break;
    }
    case SEEK_END: {
      whence = FILE_END;
      break;
    }
    default: {
      errno = EINVAL;
      return (off_t)-1; // rather safe than sorry
    }
  }
  PACE_OSCALL_RETURN (lseek (handle, offset, whence), off_t, -1);
# endif  /* SEEK_SET != FILE_BEGIN || SEEK_CUR != FILE_CURRENT || SEEK_END != FILE_END */
  DWORD result = SetFilePointer (fildes, offset, NULL, whence);
  if (result == PACE_SYSCALL_FAILED)
    {
      off_t retval = -1;
      PACE_FAIL_RETURN (retval);
    }
  else
    {
      return result;
    }
}
#endif /* PACE_HAS_POSIX_FM_UOF */

#if (PACE_HAS_POSIX_DI_UOF)
pace_ssize_t
pace_win32_write (PACE_HANDLE fildes, const void * buf, size_t nbyte)
{
  DWORD bytes_written; /* This is set to 0 byte WriteFile. */

  if (WriteFile (fildes, buf, nbyte, &bytes_written, 0))
    {
      return (pace_ssize_t) bytes_written;
    }
  else
    {
      PACE_FAIL_RETURN (-1);
    }
}
#endif /* PACE_HAS_POSIX_DI_UOF */

#if (PACE_HAS_POSIX_DI_UOF)
pace_ssize_t
pace_win32_read (PACE_HANDLE fildes, void * buf, size_t nbyte)
{
  DWORD ok_len;

  if (ReadFile (fildes, buf, nbyte, &ok_len, 0))
    {
      return (pace_ssize_t) ok_len;
    }
  else
    {
      PACE_FAIL_RETURN (-1);
    }
}
#endif /* PACE_HAS_POSIX_DI_UOF */
