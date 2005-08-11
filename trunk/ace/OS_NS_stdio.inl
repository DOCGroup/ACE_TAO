// -*- C++ -*-
//
// $Id$

#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_fcntl.h"
#include "ace/OS_NS_errno.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_pwd.h"
#include "ace/OS_NS_macros.h"
#include "ace/OS_Memory.h"

#if defined (ACE_HAS_CHARPTR_SPRINTF)
#  define ACE_SPRINTF_ADAPTER(X) ACE_OS::strlen (X)
#else
#  define ACE_SPRINTF_ADAPTER(X) X
#endif /* ACE_HAS_CHARPTR_SPRINTF */

#if defined (ACE_PSOS)
ACE_INLINE int
isatty (int h)
{
  return ACE_OS::isatty (h);
}
#if defined (fileno)
#undef fileno
#endif /* defined (fileno)*/
ACE_INLINE ACE_HANDLE
fileno (FILE *fp)
{
  return (ACE_HANDLE) fp;
}
#endif /* defined (ACE_PSOS) */

/*****************************************************************************/

#if defined (ACE_WIN32)
ACE_INLINE void
ACE_OS::flock_adjust_params (ACE_OS::ace_flock_t *lock,
                             short whence,
                             off_t &start,
                             off_t &len)
{
  switch (whence)
    {
    case SEEK_SET:
      break;
    case SEEK_CUR:
      start += SetFilePointer (lock->handle_, 0, 0, FILE_CURRENT);
      break;
    case SEEK_END:
      start += ::GetFileSize (lock->handle_, 0);
      break;
    }
  lock->overlapped_.Offset = start;
  if (len == 0)
    len = ::GetFileSize (lock->handle_,
                         0) - start;
}
#endif /* ACE_WIN32 */

ACE_INLINE int
ACE_OS::flock_init (ACE_OS::ace_flock_t *lock,
                    int flags,
                    const ACE_TCHAR *name,
                    mode_t perms)
{
  ACE_OS_TRACE ("ACE_OS::flock_init");
#if defined (CHORUS)
  lock->lockname_ = 0;
  // Let's see if it already exists.
  lock->handle_ = ACE_OS::shm_open (name,
                                    flags | O_CREAT | O_EXCL,
                                    perms);
  if (lock->handle_ == ACE_INVALID_HANDLE)
    {
      if (errno == EEXIST)
        // It's already there, so we'll just open it.
        lock->handle_ = ACE_OS::shm_open (name,
                                          flags | O_CREAT,
                                          ACE_DEFAULT_FILE_PERMS);
      else
        return -1;
    }
  else
    {
      // We own this shared memory object!  Let's set its size.
      if (ACE_OS::ftruncate (lock->handle_,
                             sizeof (ACE_mutex_t)) == -1)
        return -1;
      // Note that only the owner can destroy a file lock...
      ACE_ALLOCATOR_RETURN (lock->lockname_,
                            ACE_OS::strdup (name),
                            -1);
    }
  if (lock->handle_ == ACE_INVALID_HANDLE)
    return -1;

  lock->process_lock_ =
    (ACE_mutex_t *) ACE_OS::mmap (0,
                                  sizeof (ACE_mutex_t),
                                  PROT_RDWR,
                                  MAP_SHARED,
                                  lock->handle_,
                                  0);
  if (lock->process_lock_ == MAP_FAILED)
    return -1;

  if (lock->lockname_
      // Only initialize it if we're the one who created it.
      && ACE_OS::mutex_init (lock->process_lock_,
                             USYNC_PROCESS,
                             name,
                             0) != 0)
        return -1;
  return 0;
#else
#if defined (ACE_WIN32)
  // Once initialized, these values are never changed.
  lock->overlapped_.Internal = 0;
  lock->overlapped_.InternalHigh = 0;
  lock->overlapped_.OffsetHigh = 0;
  lock->overlapped_.hEvent = 0;
#endif /* ACE_WIN32 */
  lock->handle_ = ACE_INVALID_HANDLE;
  lock->lockname_ = 0;

  if (name != 0)
    {
      ACE_OSCALL (ACE_OS::open (name, flags, perms),
                  ACE_HANDLE,
                  ACE_INVALID_HANDLE,
                  lock->handle_);
      lock->lockname_ = ACE_OS::strdup (name);
      return lock->handle_ == ACE_INVALID_HANDLE ? -1 : 0;
    }
  else
    return 0;
#endif /* CHORUS */
}

ACE_INLINE int
ACE_OS::flock_unlock (ACE_OS::ace_flock_t *lock,
                      short whence,
                      off_t start,
                      off_t len)
{
  ACE_OS_TRACE ("ACE_OS::flock_unlock");
#if defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)
  ACE_OS::flock_adjust_params (lock, whence, start, len);
  ACE_WIN32CALL_RETURN (ACE_ADAPT_RETVAL (::UnlockFile (lock->handle_,
                                                        lock->overlapped_.Offset,
                                                        0,
                                                        len,
                                                        0),
                                          ace_result_), int, -1);
#elif defined (CHORUS)
  ACE_UNUSED_ARG (whence);
  ACE_UNUSED_ARG (start);
  ACE_UNUSED_ARG (len);
  return ACE_OS::mutex_unlock (lock->process_lock_);
#elif defined (ACE_LACKS_FILELOCKS)
  ACE_UNUSED_ARG (lock);
  ACE_UNUSED_ARG (whence);
  ACE_UNUSED_ARG (start);
  ACE_UNUSED_ARG (len);
  ACE_NOTSUP_RETURN (-1);
#else
  lock->lock_.l_whence = whence;
  lock->lock_.l_start = start;
  lock->lock_.l_len = len;
  lock->lock_.l_type = F_UNLCK;   // Unlock file.

  // release lock
  ACE_OSCALL_RETURN (ACE_OS::fcntl (lock->handle_, F_SETLK,
                                    reinterpret_cast<long> (&lock->lock_)),
                     int, -1);
#endif /* ACE_WIN32 */
}

ACE_INLINE int
ACE_OS::flock_destroy (ACE_OS::ace_flock_t *lock,
                       int unlink_file)
{
  ACE_OS_TRACE ("ACE_OS::flock_destroy");
  if (lock->handle_ != ACE_INVALID_HANDLE)
    {
      ACE_OS::flock_unlock (lock);
      // Close the handle.
      ACE_OS::close (lock->handle_);
      lock->handle_ = ACE_INVALID_HANDLE;
#if defined (CHORUS)
      // Are we the owner?
      if (lock->process_lock_ && lock->lockname_ != 0)
        {
          // Only destroy the lock if we're the owner
          ACE_OS::mutex_destroy (lock->process_lock_);
          ACE_OS::munmap (lock->process_lock_,
                          sizeof (ACE_mutex_t));
          if (unlink_file)
            ACE_OS::shm_unlink (lock->lockname_);
          ACE_OS::free (
            static_cast<void *> (const_cast<ACE_TCHAR *> (lock->lockname_)));
        }
      else if (lock->process_lock_)
        // Just unmap the memory.
        ACE_OS::munmap (lock->process_lock_,
                        sizeof (ACE_mutex_t));
#else
      if (lock->lockname_ != 0)
        {
          if (unlink_file)
            ACE_OS::unlink (lock->lockname_);
          ACE_OS::free (
            static_cast<void *> (const_cast<ACE_TCHAR *> (lock->lockname_)));
        }
#endif /* CHORUS */
      lock->lockname_ = 0;
    }
  return 0;
}

ACE_INLINE int
ACE_OS::flock_rdlock (ACE_OS::ace_flock_t *lock,
                      short whence,
                      off_t start,
                      off_t len)
{
  ACE_OS_TRACE ("ACE_OS::flock_rdlock");
#if defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)
  ACE_OS::flock_adjust_params (lock, whence, start, len);
#  if defined (ACE_HAS_WINNT4) && (ACE_HAS_WINNT4 != 0)
  ACE_WIN32CALL_RETURN (ACE_ADAPT_RETVAL (::LockFileEx (lock->handle_,
                                                        0,
                                                        0,
                                                        len,
                                                        0,
                                                        &lock->overlapped_),
                                          ace_result_), int, -1);
#  else /* ACE_HAS_WINNT4 && (ACE_HAS_WINNT4 != 0) */
  ACE_WIN32CALL_RETURN (ACE_ADAPT_RETVAL (::LockFile (lock->handle_,
                                                      lock->overlapped_.Offset,
                                                      0,
                                                      len,
                                                      0),
                                          ace_result_), int, -1);
#  endif /* ACE_HAS_WINNT4 && (ACE_HAS_WINNT4 != 0) */
#elif defined (CHORUS)
  ACE_UNUSED_ARG (whence);
  ACE_UNUSED_ARG (start);
  ACE_UNUSED_ARG (len);
  return ACE_OS::mutex_lock (lock->process_lock_);
#elif defined (ACE_LACKS_FILELOCKS)
  ACE_UNUSED_ARG (lock);
  ACE_UNUSED_ARG (whence);
  ACE_UNUSED_ARG (start);
  ACE_UNUSED_ARG (len);
  ACE_NOTSUP_RETURN (-1);
#else
  lock->lock_.l_whence = whence;
  lock->lock_.l_start = start;
  lock->lock_.l_len = len;
  lock->lock_.l_type = F_RDLCK;         // set read lock
  // block, if no access
  ACE_OSCALL_RETURN (ACE_OS::fcntl (lock->handle_, F_SETLKW,
                                    reinterpret_cast<long> (&lock->lock_)),
                     int, -1);
#endif /* ACE_WIN32 */
}

ACE_INLINE int
ACE_OS::flock_tryrdlock (ACE_OS::ace_flock_t *lock,
                         short whence,
                         off_t start,
                         off_t len)
{
  ACE_OS_TRACE ("ACE_OS::ace_flock_tryrdlock");
#if defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)
#  if defined (ACE_HAS_WINNT4) && (ACE_HAS_WINNT4 != 0)
  ACE_OS::flock_adjust_params (lock, whence, start, len);
  ACE_WIN32CALL_RETURN (ACE_ADAPT_RETVAL (::LockFileEx (lock->handle_,
                                                        LOCKFILE_FAIL_IMMEDIATELY,
                                                        0,
                                                        len,
                                                        0,
                                                        &lock->overlapped_),
                                          ace_result_), int, -1);
#  else /* ACE_HAS_WINNT4 && (ACE_HAS_WINNT4 != 0) */
  ACE_UNUSED_ARG (lock);
  ACE_UNUSED_ARG (whence);
  ACE_UNUSED_ARG (start);
  ACE_UNUSED_ARG (len);
  ACE_NOTSUP_RETURN (-1);
#  endif /* ACE_HAS_WINNT4 && (ACE_HAS_WINNT4 != 0) */
#elif defined (CHORUS)
  ACE_UNUSED_ARG (whence);
  ACE_UNUSED_ARG (start);
  ACE_UNUSED_ARG (len);
  return ACE_OS::mutex_trylock (lock->process_lock_);
#elif defined (ACE_LACKS_FILELOCKS)
  ACE_UNUSED_ARG (lock);
  ACE_UNUSED_ARG (whence);
  ACE_UNUSED_ARG (start);
  ACE_UNUSED_ARG (len);
  ACE_NOTSUP_RETURN (-1);
#else
  lock->lock_.l_whence = whence;
  lock->lock_.l_start = start;
  lock->lock_.l_len = len;
  lock->lock_.l_type = F_RDLCK;         // set read lock

  int result = 0;
  // Does not block, if no access, returns -1 and set errno = EBUSY;
  ACE_OSCALL (ACE_OS::fcntl (lock->handle_, F_SETLK,
                             reinterpret_cast<long> (&lock->lock_)),
              int, -1, result);

# if ! defined (ACE_PSOS)
  if (result == -1 && (errno == EACCES || errno == EAGAIN))
    errno = EBUSY;
# endif /* ! defined (ACE_PSOS) */

  return result;
#endif /* ACE_WIN32 */
}

ACE_INLINE int
ACE_OS::flock_trywrlock (ACE_OS::ace_flock_t *lock,
                         short whence,
                         off_t start,
                         off_t len)
{
  ACE_OS_TRACE ("ACE_OS::ace_flock_trywrlock");
#if defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)
#  if defined (ACE_HAS_WINNT4) && (ACE_HAS_WINNT4 != 0)
  ACE_OS::flock_adjust_params (lock, whence, start, len);
  ACE_WIN32CALL_RETURN (ACE_ADAPT_RETVAL (::LockFileEx (lock->handle_,
                                                        LOCKFILE_FAIL_IMMEDIATELY | LOCKFILE_EXCLUSIVE_LOCK,
                                                        0,
                                                        len,
                                                        0,
                                                        &lock->overlapped_),
                                          ace_result_), int, -1);
#  else /* ACE_HAS_WINNT4 && (ACE_HAS_WINNT4 != 0) */
  ACE_UNUSED_ARG (lock);
  ACE_UNUSED_ARG (whence);
  ACE_UNUSED_ARG (start);
  ACE_UNUSED_ARG (len);
  ACE_NOTSUP_RETURN (-1);
#  endif /* ACE_HAS_WINNT4 && (ACE_HAS_WINNT4 != 0) */
#elif defined (CHORUS)
  ACE_UNUSED_ARG (whence);
  ACE_UNUSED_ARG (start);
  ACE_UNUSED_ARG (len);
  return ACE_OS::mutex_trylock (lock->process_lock_);
#elif defined (ACE_LACKS_FILELOCKS)
  ACE_UNUSED_ARG (lock);
  ACE_UNUSED_ARG (whence);
  ACE_UNUSED_ARG (start);
  ACE_UNUSED_ARG (len);
  ACE_NOTSUP_RETURN (-1);
#else
  lock->lock_.l_whence = whence;
  lock->lock_.l_start = start;
  lock->lock_.l_len = len;
  lock->lock_.l_type = F_WRLCK;         // set write lock

  int result = 0;
  // Does not block, if no access, returns -1 and set errno = EBUSY;
  ACE_OSCALL (ACE_OS::fcntl (lock->handle_,
                             F_SETLK,
                             reinterpret_cast<long> (&lock->lock_)),
              int, -1, result);

# if ! defined (ACE_PSOS)
  if (result == -1 && (errno == EACCES || errno == EAGAIN))
    errno = EBUSY;
# endif /* ! defined (ACE_PSOS) */

  return result;
#endif /* ACE_WIN32 */
}

ACE_INLINE int
ACE_OS::flock_wrlock (ACE_OS::ace_flock_t *lock,
                      short whence,
                      off_t start,
                      off_t len)
{
  ACE_OS_TRACE ("ACE_OS::flock_wrlock");
#if defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)
  ACE_OS::flock_adjust_params (lock, whence, start, len);
#  if defined (ACE_HAS_WINNT4) && (ACE_HAS_WINNT4 != 0)
  ACE_WIN32CALL_RETURN (ACE_ADAPT_RETVAL (::LockFileEx (lock->handle_,
                                                        LOCKFILE_EXCLUSIVE_LOCK,
                                                        0,
                                                        len,
                                                        0,
                                                        &lock->overlapped_),
                                          ace_result_), int, -1);
#  else /* ACE_HAS_WINNT4 && (ACE_HAS_WINNT4 != 0) */
  ACE_WIN32CALL_RETURN (ACE_ADAPT_RETVAL (::LockFile (lock->handle_,
                                                      lock->overlapped_.Offset,
                                                      0,
                                                      len,
                                                      0),
                                          ace_result_), int, -1);
#  endif /* ACE_HAS_WINNT4 && (ACE_HAS_WINNT4 != 0) */
#elif defined (CHORUS)
  ACE_UNUSED_ARG (whence);
  ACE_UNUSED_ARG (start);
  ACE_UNUSED_ARG (len);
  return ACE_OS::mutex_lock (lock->process_lock_);
#elif defined (ACE_LACKS_FILELOCKS)
  ACE_UNUSED_ARG (lock);
  ACE_UNUSED_ARG (whence);
  ACE_UNUSED_ARG (start);
  ACE_UNUSED_ARG (len);
  ACE_NOTSUP_RETURN (-1);
#else
  lock->lock_.l_whence = whence;
  lock->lock_.l_start = start;
  lock->lock_.l_len = len;
  lock->lock_.l_type = F_WRLCK;         // set write lock
  // block, if no access
  ACE_OSCALL_RETURN (ACE_OS::fcntl (lock->handle_, F_SETLKW,
                                    reinterpret_cast<long> (&lock->lock_)),
                     int, -1);
#endif /* ACE_WIN32 */
}

#if defined (ACE_WIN32)
ACE_INLINE void
ACE_OS::fopen_mode_to_open_mode_converter (ACE_TCHAR x, int &hmode)
{
    switch (x)
      {
      case ACE_LIB_TEXT ('r'):
        if (ACE_BIT_DISABLED (hmode, _O_RDWR))
          {
            ACE_CLR_BITS (hmode, _O_WRONLY);
            ACE_SET_BITS (hmode, _O_RDONLY);
          }
        break;
      case ACE_LIB_TEXT ('w'):
        if (ACE_BIT_DISABLED (hmode, _O_RDWR))
          {
            ACE_CLR_BITS (hmode, _O_RDONLY);
            ACE_SET_BITS (hmode, _O_WRONLY);
          }
        ACE_SET_BITS (hmode, _O_CREAT | _O_TRUNC);
        break;
      case ACE_LIB_TEXT ('a'):
        if (ACE_BIT_DISABLED (hmode, _O_RDWR))
          {
            ACE_CLR_BITS (hmode, _O_RDONLY);
            ACE_SET_BITS (hmode, _O_WRONLY);
          }
        ACE_SET_BITS (hmode, _O_CREAT | _O_APPEND);
        break;
      case ACE_LIB_TEXT ('+'):
        ACE_CLR_BITS (hmode, _O_RDONLY | _O_WRONLY);
        ACE_SET_BITS (hmode, _O_RDWR);
        break;
      case ACE_LIB_TEXT ('t'):
        ACE_CLR_BITS (hmode, _O_BINARY);
        ACE_SET_BITS (hmode, _O_TEXT);
        break;
      case ACE_LIB_TEXT ('b'):
        ACE_CLR_BITS (hmode, _O_TEXT);
        ACE_SET_BITS (hmode, _O_BINARY);
        break;
      }
}
#endif /* ACE_WIN32 */

#if !defined (ACE_LACKS_CLEARERR)
ACE_INLINE void
ACE_OS::clearerr (FILE* fp)
{
#if defined (__ace_clearerr_hack)
  __ace_clearerr(fp);
#else
   ::clearerr(fp);
#endif /* __ace_clearerr_hack */
}
#endif /* !ACE_LACKS_CLEARERR */

#if !defined (ACE_LACKS_CUSERID)
ACE_INLINE char *
ACE_OS::cuserid (char *user, size_t maxlen)
{
  ACE_OS_TRACE ("ACE_OS::cuserid");
#if defined (VXWORKS)
  ACE_UNUSED_ARG (maxlen);
  if (user == 0)
    {
      // Require that the user field be non-null, i.e., don't
      // allocate or use static storage.
      ACE_NOTSUP_RETURN (0);
    }
  else
    {
      ::remCurIdGet (user, 0);
      return user;
    }
#elif defined (CHORUS) || defined (ACE_PSOS) || defined (__QNXNTO__)
  ACE_UNUSED_ARG (user);
  ACE_UNUSED_ARG (maxlen);
  ACE_NOTSUP_RETURN (0);
#elif defined (ACE_WIN32)
  BOOL result = GetUserNameA (user, (u_long *) &maxlen);
  if (result == FALSE)
    ACE_FAIL_RETURN (0);
  else
    return user;
#elif defined (ACE_HAS_ALT_CUSERID)
#  if defined (ACE_LACKS_PWD_FUNCTIONS)
#    error Cannot use alternate cuserid() without POSIX password functions!
#  endif  /* ACE_LACKS_PWD_FUNCTIONS */

  // POSIX.1 dropped the cuserid() function.
  // GNU GLIBC and other platforms correctly deprecate the cuserid()
  // function.

  if (maxlen == 0)
    {
      // It doesn't make sense to have a zero length user ID.
      errno = EINVAL;
      return 0;
    }

  struct passwd *pw = 0;

  // Make sure the file pointer is at the beginning of the password file
  ::setpwent ();
  // Should use ACE_OS::setpwent() but I didn't want to move this
  // method after it.

  // Use the effective user ID to determine the user name.
  pw = ::getpwuid (::geteuid ());

  // Make sure the password file is closed.
  ::endpwent ();

  size_t max_length = 0;
  char *userid = 0;

  if (user == 0)
    {
      // Not reentrant/thread-safe, but nothing else can be done if a
      // zero pointer was passed in as the destination.

#if defined (_POSIX_SOURCE)
      const size_t ACE_L_cuserid = L_cuserid;
#else
      const size_t ACE_L_cuserid = 9;  // 8 character user ID + NULL
#endif  /* _POSIX_SOURCE */

      static char tmp[ACE_L_cuserid] = { '\0' };
      max_length = ACE_L_cuserid - 1; // Do not include NULL in length

      userid = tmp;
    }
  else
    {
      max_length = maxlen;
      userid = user;
    }

  // Extract the user name from the passwd structure.
  if (ACE_OS::strlen (pw->pw_name) <= max_length)
    {
      return ACE_OS::strcpy (userid, pw->pw_name);
    }
  else
    {
      errno = ENOSPC;  // Buffer is not large enough.
      return 0;
    }
#else
  // Hackish because of missing buffer size!
  ACE_UNUSED_ARG (maxlen);
  ACE_OSCALL_RETURN (::ace_cuserid(user), char*, 0);
#endif /* VXWORKS */
}

#if defined (ACE_HAS_WCHAR)
ACE_INLINE wchar_t *
ACE_OS::cuserid (wchar_t *user, size_t maxlen)
{
# if defined (ACE_WIN32)
  BOOL result = GetUserNameW (user, (u_long *) &maxlen);
  if (result == FALSE)
    ACE_FAIL_RETURN (0);
  else
    return user;
# else /* ACE_WIN32 */
  char *char_user;
  wchar_t *result = 0;

  ACE_NEW_RETURN (char_user, char[maxlen + 1], 0);

  if (ACE_OS::cuserid (char_user, maxlen))
    {
      ACE_OS::strcpy (user, ACE_Ascii_To_Wide (char_user).wchar_rep ());
      result = user;
    }

  delete [] char_user;

  return result;
# endif /* ACE_WIN32 */
}
#endif /* ACE_HAS_WCHAR  */
#endif /* ACE_LACKS_CUSERID */

ACE_INLINE int
ACE_OS::fclose (FILE *fp)
{
  ACE_OS_TRACE ("ACE_OS::fclose");
  ACE_OSCALL_RETURN (ACE_STD_NAMESPACE::fclose (fp), int, -1);
}

ACE_INLINE FILE *
ACE_OS::fdopen (ACE_HANDLE handle, const ACE_TCHAR *mode)
{
  ACE_OS_TRACE ("ACE_OS::fdopen");
# if defined (ACE_HAS_WINCE)
  ACE_OSCALL_RETURN (::_wfdopen (handle, mode), FILE*, 0);
# elif defined (ACE_WIN32)
  // kernel file handle -> FILE* conversion...
  // Options: _O_APPEND, _O_RDONLY and _O_TEXT are lost

  FILE *file = 0;

#  if defined (ACE_WIN64)
  int crt_handle = ::_open_osfhandle (intptr_t (handle), 0);
#  else
  int crt_handle = ::_open_osfhandle (long (handle), 0);
#  endif /* ACE_WIN64 */

  if (crt_handle != -1)
    {
#   if defined(__BORLANDC__) /* VSB */
      file = ::_fdopen (crt_handle, (char *) mode);
#   elif defined (ACE_USES_WCHAR)
      file = ::_wfdopen (crt_handle, mode);
#   else
      file = ::_fdopen (crt_handle, mode);
#   endif /* __BORLANDC__ */

      if (!file)
        {
#   if (defined(__BORLANDC__) && __BORLANDC__ >= 0x0530)
          ::_rtl_close (crt_handle);
#   else
          ::_close (crt_handle);
#   endif /* (defined(__BORLANDC__) && __BORLANDC__ >= 0x0530) */
        }
    }

  return file;
# elif defined (ACE_PSOS)
  // @@ it may be possible to implement this for pSOS,
  // but it isn't obvious how to do this (perhaps via
  // f_stat to glean the default volume, and then open_fn ?)
  ACE_UNUSED_ARG (handle);
  ACE_UNUSED_ARG (mode);
  ACE_NOTSUP_RETURN (0);
# else
  ACE_OSCALL_RETURN
    (::fdopen (handle, ACE_TEXT_ALWAYS_CHAR (mode)), FILE *, 0);
# endif /* ACE_HAS_WINCE */
}

ACE_INLINE int
ACE_OS::fflush (FILE *fp)
{
  ACE_OS_TRACE ("ACE_OS::fflush");
#if defined (VXWORKS)
  if (fp == 0)
    {
      // Do not allow fflush(0) on VxWorks
      return 0;
    }
#endif /* VXWORKS */

  ACE_OSCALL_RETURN (ACE_STD_NAMESPACE::fflush (fp), int, -1);
}

ACE_INLINE int
ACE_OS::fgetc (FILE* fp)
{
  ACE_OSCALL_RETURN (ACE_STD_NAMESPACE::fgetc (fp), int, -1);
}

ACE_INLINE int
ACE_OS::fgetpos (FILE* fp, fpos_t* pos)
{
  ACE_OSCALL_RETURN (ACE_STD_NAMESPACE::fgetpos (fp, pos), int, -1);
}

ACE_INLINE char *
ACE_OS::fgets (char *buf, int size, FILE *fp)
{
  ACE_OS_TRACE ("ACE_OS::fgets");
  ACE_OSCALL_RETURN (ACE_STD_NAMESPACE::fgets (buf, size, fp), char *, 0);
}

#if defined (ACE_HAS_WCHAR) && !defined(ACE_LACKS_FGETWS)
ACE_INLINE wchar_t *
ACE_OS::fgets (wchar_t *buf, int size, FILE *fp)
{
  ACE_OS_TRACE ("ACE_OS::fgets");
  ACE_OSCALL_RETURN (ACE_STD_NAMESPACE::fgetws (buf, size, fp), wchar_t *, 0);
}
#endif /* ACE_HAS_WCHAR && !ACE_LACKS_FGETWS */

#if !defined (ACE_WIN32)
// Win32 implementation of fopen () is in OS_NS_stdio.cpp.
ACE_INLINE FILE *
ACE_OS::fopen (const char *filename, const ACE_TCHAR *mode)
{
  ACE_OS_TRACE ("ACE_OS::fopen");
  ACE_OSCALL_RETURN
    (::fopen (filename, ACE_TEXT_ALWAYS_CHAR (mode)), FILE *, 0);
}

#if defined (ACE_HAS_WCHAR)
ACE_INLINE FILE *
ACE_OS::fopen (const wchar_t *filename, const ACE_TCHAR *mode)
{
  ACE_OS_TRACE ("ACE_OS::fopen");
  // Non-Windows doesn't use wchar_t file systems.
  ACE_Wide_To_Ascii n_filename (filename);
  ACE_OSCALL_RETURN
    (::fopen (n_filename.char_rep (), ACE_TEXT_ALWAYS_CHAR (mode)), FILE*, 0);
}
#endif /* ACE_HAS_WCHAR */

#endif /* ACE_WIN32 */

ACE_INLINE int
ACE_OS::fputs (const char *s, FILE *stream)
{
  ACE_OS_TRACE ("ACE_OS::fputs");
  ACE_OSCALL_RETURN (ACE_STD_NAMESPACE::fputs (s, stream), int, -1);
}

#if defined (ACE_HAS_WCHAR) && !defined(ACE_LACKS_FPUTWS)
ACE_INLINE int
ACE_OS::fputs (const wchar_t *s, FILE *stream)
{
  ACE_OS_TRACE ("ACE_OS::fputs");
  ACE_OSCALL_RETURN (ACE_STD_NAMESPACE::fputws (s, stream), int, -1);
}
#endif /* ACE_HAS_WCHAR && !ACE_LACKS_FPUTWS */

ACE_INLINE size_t
ACE_OS::fread (void *ptr, size_t size, size_t nelems, FILE *fp)
{
  ACE_OS_TRACE ("ACE_OS::fread");
  ACE_OSCALL_RETURN (ACE_STD_NAMESPACE::fread (ptr, size, nelems, fp), int, 0);
}

ACE_INLINE FILE *
ACE_OS::freopen (const ACE_TCHAR *filename, const ACE_TCHAR *mode, FILE* stream)
{
  ACE_OS_TRACE ("ACE_OS::freopen");
#if defined (ACE_WIN32) && defined (ACE_USES_WCHAR)
  ACE_OSCALL_RETURN (::_wfreopen (filename, mode, stream), FILE *, 0);
#else
  ACE_OSCALL_RETURN
    (ACE_STD_NAMESPACE::freopen (ACE_TEXT_ALWAYS_CHAR (filename),
                                 ACE_TEXT_ALWAYS_CHAR (mode),
                                 stream),
     FILE *, 0);
#endif /* ACE_WIN32 && ACE_USES_WCHAR */
}

ACE_INLINE int
ACE_OS::fseek (FILE *fp, long offset, int whence)
{
# if defined (ACE_WIN32)
#   if SEEK_SET != FILE_BEGIN || SEEK_CUR != FILE_CURRENT || SEEK_END != FILE_END
  //#error Windows NT is evil AND rude!
  switch (whence)
    {
    case SEEK_SET:
      whence = FILE_BEGIN;
      break;
    case SEEK_CUR:
      whence = FILE_CURRENT;
      break;
    case SEEK_END:
      whence = FILE_END;
      break;
    default:
      errno = EINVAL;
      return -1; // rather safe than sorry
    }
#   endif  /* SEEK_SET != FILE_BEGIN || SEEK_CUR != FILE_CURRENT || SEEK_END != FILE_END */
# endif   /* ACE_WIN32 */
  ACE_OSCALL_RETURN (ACE_STD_NAMESPACE::fseek (fp, offset, whence), int, -1);
}

ACE_INLINE int
ACE_OS::fsetpos (FILE* fp, fpos_t* pos)
{
  ACE_OSCALL_RETURN (::fsetpos (fp, pos), int, -1);
}

ACE_INLINE long
ACE_OS::ftell (FILE* fp)
{
  ACE_OSCALL_RETURN (ACE_STD_NAMESPACE::ftell (fp), long, -1);
}

ACE_INLINE size_t
ACE_OS::fwrite (const void *ptr, size_t size, size_t nitems, FILE *fp)
{
  ACE_OS_TRACE ("ACE_OS::fwrite");
  ACE_OSCALL_RETURN (ACE_STD_NAMESPACE::fwrite (ptr, size, nitems, fp), int, 0);
}

ACE_INLINE void
ACE_OS::perror (const char *s)
{
  ACE_OS_TRACE ("ACE_OS::perror");
#if defined (ACE_HAS_WINCE)
  // @@ WINCE: How should this be handled
  ACE_UNUSED_ARG (s);
#else
  ::perror (s);
#endif /* ACE_HAS_WINCE */
}

#if defined (ACE_HAS_WCHAR)
ACE_INLINE void
ACE_OS::perror (const wchar_t *s)
{
  ACE_OS_TRACE ("ACE_OS::perror");
#if defined (ACE_HAS_WINCE)
  // @@ WINCE: How should this be handled
  ACE_UNUSED_ARG (s);
#elif defined (ACE_WIN32)
  ::_wperror (s);
#else
  ACE_Wide_To_Ascii n_s (s);
  ::perror (n_s.char_rep ());
#endif /* ACE_HAS_WINCE */
}
#endif /* ACE_HAS_WCHAR */

ACE_INLINE int
ACE_OS::puts (const char *s)
{
  ACE_OS_TRACE ("ACE_OS::puts");
  ACE_OSCALL_RETURN (::puts (s), int, -1);
}

#if defined (ACE_HAS_WCHAR)
ACE_INLINE int
ACE_OS::puts (const wchar_t *s)
{
  ACE_OS_TRACE ("ACE_OS::puts");
#if defined (ACE_WIN32)
  ACE_OSCALL_RETURN (::_putws (s), int, -1);
#else /* ACE_WIN32 */
  // There's no putws()...
  ACE_Wide_To_Ascii n_s (s);
  ACE_OSCALL_RETURN (::puts (n_s.char_rep ()), int, -1);
#endif /* ACE_WIN32 */
}
#endif /* ACE_HAS_WCHAR */

ACE_INLINE int
ACE_OS::rename (const char *old_name,
                const char *new_name,
                int flags)
{
# if defined (ACE_LACKS_RENAME)
  ACE_UNUSED_ARG (old_name);
  ACE_UNUSED_ARG (new_name);
  ACE_UNUSED_ARG (flags);
  ACE_NOTSUP_RETURN (-1);
# elif defined (ACE_HAS_WINCE)
  // Win CE is always wide-char.
  ACE_UNUSED_ARG (flags);
  if (0 != MoveFile (ACE_TEXT_CHAR_TO_TCHAR (old_name),
                     ACE_TEXT_CHAR_TO_TCHAR (new_name)))
    ACE_FAIL_RETURN (-1);
  return 0;
# elif defined (ACE_WIN32) && defined (ACE_HAS_WINNT4) && (ACE_HAS_WINNT4 == 1)
  // NT4 (and up) provides a way to rename/move a file with similar semantics
  // to what's usually done on UNIX - if there's an existing file with
  // <new_name> it is removed before the file is renamed/moved. The
  // MOVEFILE_COPY_ALLOWED is specified to allow such a rename across drives.
  if (flags == -1)
    flags = MOVEFILE_COPY_ALLOWED | MOVEFILE_REPLACE_EXISTING;
  if (::MoveFileExA (old_name, new_name, flags) == 0)
    ACE_FAIL_RETURN (-1);
  return 0;
# else /* ACE_LACKS_RENAME */
  ACE_UNUSED_ARG (flags);
  ACE_OSCALL_RETURN (::rename (old_name, new_name), int, -1);
# endif /* ACE_LACKS_RENAME */
}

#if defined (ACE_HAS_WCHAR)
ACE_INLINE int
ACE_OS::rename (const wchar_t *old_name,
                const wchar_t *new_name,
                int flags)
{
# if defined (ACE_LACKS_RENAME)
  ACE_UNUSED_ARG (old_name);
  ACE_UNUSED_ARG (new_name);
  ACE_UNUSED_ARG (flags);
  ACE_NOTSUP_RETURN (-1);
# elif defined (ACE_HAS_WINCE)
  ACE_UNUSED_ARG (flags);
  if (MoveFile (old_name, new_name) != 0)
    ACE_FAIL_RETURN (-1);
  return 0;
# elif defined (ACE_WIN32) && defined (ACE_HAS_WINNT4) && (ACE_HAS_WINNT4 == 1)
  // NT4 (and up) provides a way to rename/move a file with similar semantics
  // to what's usually done on UNIX - if there's an existing file with
  // <new_name> it is removed before the file is renamed/moved. The
  // MOVEFILE_COPY_ALLOWED is specified to allow such a rename across drives.
  if (flags == -1)
    flags = MOVEFILE_COPY_ALLOWED | MOVEFILE_REPLACE_EXISTING;
  if (::MoveFileExW (old_name, new_name, flags) == 0)
    ACE_FAIL_RETURN (-1);
  return 0;
# elif defined (ACE_WIN32)
  ACE_UNUSED_ARG (flags);
  ACE_OSCALL_RETURN (::_wrename (old_name, new_name), int, -1);
# else /* ACE_LACKS_RENAME */
  ACE_Wide_To_Ascii nold_name (old_name);
  ACE_Wide_To_Ascii nnew_name (new_name);
  return ACE_OS::rename (nold_name.char_rep (), nnew_name.char_rep (), flags);
# endif /* ACE_LACKS_RENAME */
}
#endif /* ACE_HAS_WCHAR */

ACE_INLINE void
ACE_OS::rewind (FILE *fp)
{
#if !defined (ACE_HAS_WINCE)
  ACE_OS_TRACE ("ACE_OS::rewind");
  ::rewind (fp);
#else
  // In WinCE, "FILE *" is actually a HANDLE.
  ::SetFilePointer (fp, 0L, 0L, FILE_BEGIN);
#endif /* ACE_HAS_WINCE */
}

ACE_INLINE char *
ACE_OS::tempnam (const char *dir, const char *pfx)
{
  ACE_OS_TRACE ("ACE_OS::tempnam");
#if defined (ACE_LACKS_TEMPNAM)
  ACE_UNUSED_ARG (dir);
  ACE_UNUSED_ARG (pfx);
  ACE_NOTSUP_RETURN (0);
#elif defined (ACE_PSOS)
  // pSOS only considers the directory prefix
  ACE_UNUSED_ARG (pfx);
  ACE_OSCALL_RETURN (::tmpnam (const_cast <char *> (dir)), char *, 0);
#elif (defined (ACE_WIN32) && ((defined (__BORLANDC__) && (__BORLANDC__ < 0x600)) || defined (__DMC__)))
  ACE_OSCALL_RETURN (::_tempnam (const_cast <char *> (dir), const_cast<char *> (pfx)), char *, 0);
#else /* ACE_LACKS_TEMPNAM */
  ACE_OSCALL_RETURN (ACE_STD_NAMESPACE::tempnam (dir, pfx), char *, 0);
#endif /* ACE_LACKS_TEMPNAM */
}

#if defined (ACE_HAS_WCHAR)
ACE_INLINE wchar_t *
ACE_OS::tempnam (const wchar_t *dir, const wchar_t *pfx)
{
  ACE_OS_TRACE ("ACE_OS::tempnam");
#if defined (ACE_LACKS_TEMPNAM)
  ACE_UNUSED_ARG (dir);
  ACE_UNUSED_ARG (pfx);
  ACE_NOTSUP_RETURN (0);
#elif defined(ACE_WIN32)
#  if (defined (__BORLANDC__) && (__BORLANDC__ < 0x600)) || defined (__DMC__)
  ACE_OSCALL_RETURN (::_wtempnam (const_cast <wchar_t*> (dir), const_cast <wchar_t*> (pfx)), wchar_t *, 0);
#  else
  ACE_OSCALL_RETURN (::_wtempnam (dir, pfx), wchar_t *, 0);
#  endif /* __BORLANDC__ */
#else /* ACE_LACKS_TEMPNAM */
  // No native wide-char support; convert to narrow and call the char* variant.
  char *ndir = ACE_Wide_To_Ascii (dir).char_rep ();
  char *npfx = ACE_Wide_To_Ascii (pfx).char_rep ();
  char *name = ACE_OS::tempnam (ndir, npfx);
  // ACE_OS::tempnam returns a pointer to a malloc()-allocated space.
  // Convert that string to wide-char and free() the original.
  wchar_t *wname = 0;
  if (name != 0)
    {
      size_t namelen = ACE_OS::strlen (name) + 1;
      wname = reinterpret_cast<wchar_t *>
        (ACE_OS::malloc (namelen * sizeof (wchar_t)));
      if (wname != 0)
        ACE_OS::strcpy (wname, ACE_Ascii_To_Wide (name).wchar_rep ());
      ACE_OS::free (name);
    }
  return wname;
#endif /* ACE_LACKS_TEMPNAM */
}
#endif /* ACE_HAS_WCHAR */

ACE_INLINE int
ACE_OS::vsprintf (char *buffer, const char *format, va_list argptr)
{
  return ACE_SPRINTF_ADAPTER (::vsprintf (buffer, format, argptr));
}

ACE_INLINE int
ACE_OS::vsnprintf (char *buffer, size_t maxlen, const char *format, va_list ap)
{
#if defined (ACE_HAS_SNPRINTF)

#  if !defined (ACE_WIN32) \
   || (defined (__BORLANDC__) && (__BORLANDC__ >= 0x600))
  return ACE_SPRINTF_ADAPTER (::vsnprintf (buffer, maxlen, format, ap));
#  else

  int result =
    ACE_SPRINTF_ADAPTER (::_vsnprintf (buffer, maxlen, format, ap));

  // Win32 doesn't regard a full buffer with no 0-terminate as an
  // overrun.
  if (result == static_cast<int> (maxlen))
    result = -1;

  // Win32 doesn't 0-terminate the string if it overruns maxlen.
  if (result == -1)
    buffer[maxlen-1] = '\0';

  return result;

# endif
#else
  ACE_UNUSED_ARG (buffer);
  ACE_UNUSED_ARG (maxlen);
  ACE_UNUSED_ARG (format);
  ACE_UNUSED_ARG (ap);
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_SNPRINTF */
}

#if defined (ACE_HAS_WCHAR)
ACE_INLINE int
ACE_OS::vsprintf (wchar_t *buffer, const wchar_t *format, va_list argptr)
{
# if (defined _XOPEN_SOURCE && (_XOPEN_SOURCE - 0) >= 500) || \
     (defined (sun) && !(defined(_XOPEN_SOURCE) && (_XOPEN_VERSION-0==4))) || \
     (defined (ACE_HAS_DINKUM_STL) || defined (__DMC__))

  // The XPG4/UNIX98/C99 signature of the wide-char sprintf has a
  // maxlen argument. Since this method doesn't supply one, pass in
  // a length that works (ULONG_MAX doesn't on all platform since some check
  // to see if the operation will remain in bounds). If this isn't ok, use
  // ACE_OS::snprintf().
  return vswprintf (buffer, 4096, format, argptr);

# elif defined (ACE_WIN32)
  // Windows has vswprintf, but the signature is from the older ISO C
  // standard. Also see ACE_OS::snprintf() for more info on this.

  return vswprintf (buffer, format, argptr);

# else
  ACE_UNUSED_ARG (buffer);
  ACE_UNUSED_ARG (format);
  ACE_UNUSED_ARG (argptr);
  ACE_NOTSUP_RETURN (-1);

# endif /* XPG5 || ACE_HAS_DINKUM_STL */
}
#endif /* ACE_HAS_WCHAR */

#if defined (ACE_HAS_WCHAR)
ACE_INLINE int
ACE_OS::vsnprintf (wchar_t *buffer, size_t maxlen, const wchar_t *format, va_list ap)
{
  ACE_UNUSED_ARG (buffer);
  ACE_UNUSED_ARG (maxlen);
  ACE_UNUSED_ARG (format);
  ACE_UNUSED_ARG (ap);
  ACE_NOTSUP_RETURN (-1);
}
#endif /* ACE_HAS_WCHAR */

#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
#if defined (ACE_WIN32)
ACE_INLINE const OSVERSIONINFO &
ACE_OS::get_win32_versioninfo ()
{
  return ACE_OS::win32_versioninfo_;
}

ACE_INLINE HINSTANCE
ACE_OS::get_win32_resource_module ()
{
  return ACE_OS::win32_resource_module_;
}

ACE_INLINE void
ACE_OS::set_win32_resource_module (HINSTANCE instance)
{
  ACE_OS::win32_resource_module_ = instance;
}

ACE_INLINE LPSECURITY_ATTRIBUTES
ACE_OS::default_win32_security_attributes (LPSECURITY_ATTRIBUTES sa)
{
#if defined (ACE_DEFINES_DEFAULT_WIN32_SECURITY_ATTRIBUTES)
  if (sa == 0)
    {
      // @@ This is a good place to use pthread_once.
      static SECURITY_ATTRIBUTES default_sa;
      static SECURITY_DESCRIPTOR sd;
      InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
      SetSecurityDescriptorDacl(&sd, TRUE, 0, FALSE);
      default_sa.nLength = sizeof(SECURITY_ATTRIBUTES);
      default_sa.lpSecurityDescriptor = &sd;
      default_sa.bInheritHandle       = TRUE;
      sa = &default_sa;
    }
  return sa;
#else /* !ACE_DEFINES_DEFAULT_WIN32_SECURITY_ATTRIBUTES */
  return sa;
#endif /* ACE_DEFINES_DEFAULT_WIN32_SECURITY_ATTRIBUTES */
}

ACE_INLINE LPSECURITY_ATTRIBUTES
ACE_OS::default_win32_security_attributes_r (LPSECURITY_ATTRIBUTES sa,
                                             LPSECURITY_ATTRIBUTES sa_buffer,
                                             SECURITY_DESCRIPTOR* sd_buffer)
{
#if defined (ACE_DEFINES_DEFAULT_WIN32_SECURITY_ATTRIBUTES)
  if (sa == 0)
    {
      if (sa_buffer != 0 && sd_buffer != 0)
        {
          InitializeSecurityDescriptor
            (sd_buffer, SECURITY_DESCRIPTOR_REVISION);
          SetSecurityDescriptorDacl (sd_buffer, TRUE, NULL, FALSE);
          sa_buffer->nLength = sizeof(SECURITY_ATTRIBUTES);
          sa_buffer->lpSecurityDescriptor = sd_buffer;
          sa_buffer->bInheritHandle       = TRUE;
          sa = sa_buffer;
        }
    }
  return sa;
#else /* !ACE_DEFINES_DEFAULT_WIN32_SECURITY_ATTRIBUTES */
  ACE_UNUSED_ARG(sa_buffer);
  ACE_UNUSED_ARG(sd_buffer);
  return sa;
#endif /* ACE_DEFINES_DEFAULT_WIN32_SECURITY_ATTRIBUTES */
}

#endif /* ACE_WIN32 */
#endif
