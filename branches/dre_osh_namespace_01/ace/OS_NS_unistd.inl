// -*- C++ -*-
// $Id$

#include "ace/OS_NS_sys_utsname.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_errno.h"

ACE_INLINE int
ACE_OS::access (const char *path, int amode)
{
  ACE_OS_TRACE ("ACE_OS::access");
#if defined (ACE_LACKS_ACCESS)
#  if defined (ACE_HAS_WINCE)
  // @@ WINCE: There should be a Win32 API that can do this.
  // Hard coded read access here.
    FILE* handle = ACE_OS::fopen (ACE_TEXT_CHAR_TO_TCHAR(path), ACE_LIB_TEXT ("r"));
  ACE_UNUSED_ARG (amode);

  ACE_OS::fclose (handle);
  return (handle == ACE_INVALID_HANDLE ? -1 : 0);
#  elif defined (VXWORKS)
  FILE* handle = ACE_OS::fopen (ACE_TEXT_CHAR_TO_TCHAR(path), ACE_LIB_TEXT ("r"));
  ACE_UNUSED_ARG (amode);
  if (handle != 0)
    {
      ACE_OS::fclose (handle);
      return 0;
    }
  return (-1);
#  else
    ACE_UNUSED_ARG (path);
    ACE_UNUSED_ARG (amode);
    ACE_NOTSUP_RETURN (-1);
#  endif  // ACE_HAS_WINCE
#else
  ACE_OSCALL_RETURN (::access (path, amode), int, -1);
#endif /* ACE_LACKS_ACCESS */
}


#if defined (ACE_HAS_WCHAR)
ACE_INLINE int
ACE_OS::access (const wchar_t *path, int amode)
{
#if defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)
  ACE_OSCALL_RETURN (::_waccess (path, amode), int, -1);
#else /* ACE_WIN32 && !ACE_HAS_WINCE */
  return ACE_OS::access (ACE_Wide_To_Ascii (path).char_rep (), amode);
#endif /* ACE_WIN32 && !ACE_HAS_WINCE */
}
#endif /* ACE_HAS_WCHAR */

ACE_INLINE u_int
ACE_OS::alarm (u_int secs)
{
  ACE_OS_TRACE ("ACE_OS::alarm");
#if defined (ACE_WIN32) || defined (VXWORKS) || defined (CHORUS) || defined (ACE_PSOS)
  ACE_UNUSED_ARG (secs);

  ACE_NOTSUP_RETURN (0);
#else
  return ::alarm (secs);
#endif /* ACE_WIN32 || VXWORKS || CHORUS || ACE_PSOS */
}

ACE_INLINE int
ACE_OS::allocation_granularity (void)
{
#if defined (ACE_WIN32)
  SYSTEM_INFO sys_info;
  ::GetSystemInfo (&sys_info);
  return (int) sys_info.dwAllocationGranularity;
#else
  return ACE_OS::getpagesize ();
#endif /* ACE_WIN32 */
}

#if !defined (ACE_LACKS_CHDIR)
ACE_INLINE int
ACE_OS::chdir (const char *path)
{
  ACE_OS_TRACE ("ACE_OS::chdir");
#if defined (VXWORKS)
  ACE_OSCALL_RETURN (::chdir (ACE_const_cast (char *, path)), int, -1);

#elif defined (ACE_PSOS_LACKS_PHILE)
  ACE_UNUSED_ARG (path);
  ACE_NOTSUP_RETURN (-1);

#elif defined (ACE_PSOS)
    ACE_OSCALL_RETURN (ACE_ADAPT_RETVAL (::change_dir ((char *) path), ace_result_),
                     int, -1);

// This #elif seems weird... is Visual Age on NT not setting ACE_WIN32?
#elif !defined (ACE_WIN32) && !defined (AIX) && defined (__IBMCPP__) && (__IBMCPP__ >= 400)
  ACE_OSCALL_RETURN (::_chdir (path), int, -1);

#elif defined (ACE_HAS_WINCE)
  ACE_UNUSED_ARG (path);
  ACE_NOTSUP_RETURN (-1);

#else
  ACE_OSCALL_RETURN (::chdir (path), int, -1);

#endif /* VXWORKS */
}

#if defined (ACE_HAS_WCHAR)
ACE_INLINE int
ACE_OS::chdir (const wchar_t *path)
{
#if defined (ACE_WIN32)
  ACE_OSCALL_RETURN (::_wchdir (path), int, -1);
#else /* ACE_WIN32 */
  return ACE_OS::chdir (ACE_Wide_To_Ascii (path).char_rep ());
#endif /* ACE_WIN32 */
}
#endif /* ACE_HAS_WCHAR */
#endif /* ACE_LACKS_CHDIR */

// @todo: which 4 and why???  dhinton
// NOTE: The following four function definitions must appear before
// ACE_OS::sema_init ().

ACE_INLINE int
ACE_OS::close (ACE_HANDLE handle)
{
  ACE_OS_TRACE ("ACE_OS::close");
#if defined (ACE_WIN32)
  ACE_WIN32CALL_RETURN (ACE_ADAPT_RETVAL (::CloseHandle (handle), ace_result_), int, -1);
#elif defined (ACE_PSOS) && ! defined (ACE_PSOS_LACKS_PHILE)
  u_long result = ::close_f (handle);
  if (result != 0)
    {
      errno = result;
      return ACE_static_cast (int, -1);
    }
  return ACE_static_cast (int, 0);
#else
  ACE_OSCALL_RETURN (::close (handle), int, -1);
#endif /* ACE_WIN32 */
}

ACE_INLINE ACE_HANDLE
ACE_OS::dup (ACE_HANDLE handle)
{
  ACE_OS_TRACE ("ACE_OS::dup");
#if defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)
  ACE_HANDLE new_fd;
  if (::DuplicateHandle(::GetCurrentProcess (),
                        handle,
                        ::GetCurrentProcess(),
                        &new_fd,
                        0,
                        TRUE,
                        DUPLICATE_SAME_ACCESS))
    return new_fd;
  else
    ACE_FAIL_RETURN (ACE_INVALID_HANDLE);
  /* NOTREACHED */
#elif defined (VXWORKS) || defined (ACE_PSOS)
  ACE_UNUSED_ARG (handle);
  ACE_NOTSUP_RETURN (-1);
#elif defined (ACE_HAS_WINCE)
  ACE_UNUSED_ARG (handle);
  ACE_NOTSUP_RETURN (0);
#else
  ACE_OSCALL_RETURN (::dup (handle), ACE_HANDLE, ACE_INVALID_HANDLE);
#endif /* ACE_WIN32 && !ACE_HAS_WINCE */
}

ACE_INLINE int
ACE_OS::dup2 (ACE_HANDLE oldhandle, ACE_HANDLE newhandle)
{
  ACE_OS_TRACE ("ACE_OS::dup2");
#if defined (ACE_WIN32) || defined (VXWORKS) || defined (ACE_PSOS)
  // msvcrt has _dup2 ?!
  ACE_UNUSED_ARG (oldhandle);
  ACE_UNUSED_ARG (newhandle);

  ACE_NOTSUP_RETURN (-1);
#else
  ACE_OSCALL_RETURN (::dup2 (oldhandle, newhandle), int, -1);
#endif /* ACE_WIN32 || VXWORKS || ACE_PSOS */
}

ACE_INLINE int
ACE_OS::execv (const char *path,
               char *const argv[])
{
  ACE_OS_TRACE ("ACE_OS::execv");
#if defined (ACE_LACKS_EXEC)
  ACE_UNUSED_ARG (path);
  ACE_UNUSED_ARG (argv);

  ACE_NOTSUP_RETURN (-1);
#elif defined (CHORUS)
  KnCap cactorcap;
  int result = ::afexecv (path, &cactorcap, 0, argv);
  if (result != -1)
    ACE_OS::actorcaps_[result] = cactorcap;
  return result;
#elif defined (ACE_WIN32)
# if defined (__BORLANDC__) /* VSB */
  return ::execv (path, argv);
# elif defined (__MINGW32__)
  return ::_execv (path, (char *const *) argv);
# else
  return ::_execv (path, (const char *const *) argv);
# endif /* __BORLANDC__ */
#elif defined (ACE_LACKS_POSIX_PROTOTYPES)
  ACE_OSCALL_RETURN (::execv (path, (const char **) argv), int, -1);
#else
  ACE_OSCALL_RETURN (::execv (path, argv), int, -1);
#endif /* ACE_LACKS_EXEC */
}

ACE_INLINE int
ACE_OS::execve (const char *path,
                char *const argv[],
                char *const envp[])
{
  ACE_OS_TRACE ("ACE_OS::execve");
#if defined (ACE_LACKS_EXEC)
  ACE_UNUSED_ARG (path);
  ACE_UNUSED_ARG (argv);
  ACE_UNUSED_ARG (envp);

  ACE_NOTSUP_RETURN (-1);
#elif defined(CHORUS)
  KnCap cactorcap;
  int result = ::afexecve (path, &cactorcap, 0, argv, envp);
  if (result != -1)
    ACE_OS::actorcaps_[result] = cactorcap;
  return result;
#elif defined (ACE_WIN32)
# if defined (__BORLANDC__) /* VSB */
  return ::execve (path, argv, envp);
# elif defined (__MINGW32__)
  return ::_execve (path, (char *const *) argv, (char *const *) envp);
# else
  return ::_execve (path, (const char *const *) argv, (const char *const *) envp);
# endif /* __BORLANDC__ */
#elif defined (ACE_LACKS_POSIX_PROTOTYPES)
  ACE_OSCALL_RETURN (::execve (path, (const char **) argv, (char **) envp), int, -1);
#else
  ACE_OSCALL_RETURN (::execve (path, argv, envp), int, -1);
#endif /* ACE_LACKS_EXEC */
}

ACE_INLINE int
ACE_OS::execvp (const char *file,
                char *const argv[])
{
  ACE_OS_TRACE ("ACE_OS::execvp");
#if defined (ACE_LACKS_EXEC)
  ACE_UNUSED_ARG (file);
  ACE_UNUSED_ARG (argv);

  ACE_NOTSUP_RETURN (-1);
#elif defined(CHORUS)
  KnCap cactorcap;
  int result = ::afexecvp (file, &cactorcap, 0, argv);
  if (result != -1)
    ACE_OS::actorcaps_[result] = cactorcap;
  return result;
#elif defined (ACE_WIN32)
# if defined (__BORLANDC__) /* VSB */
  return ::execvp (file, argv);
# elif defined (__MINGW32__)
  return ::_execvp (file, (char *const *) argv);
# else
  return ::_execvp (file, (const char *const *) argv);
# endif /* __BORLANDC__ */
#elif defined (ACE_LACKS_POSIX_PROTOTYPES)
  ACE_OSCALL_RETURN (::execvp (file, (const char **) argv), int, -1);
#else
  ACE_OSCALL_RETURN (::execvp (file, argv), int, -1);
#endif /* ACE_LACKS_EXEC */
}

ACE_INLINE pid_t
ACE_OS::fork (void)
{
  ACE_OS_TRACE ("ACE_OS::fork");
#if defined (ACE_LACKS_FORK)
  ACE_NOTSUP_RETURN (pid_t (-1));
#else
  ACE_OSCALL_RETURN (::fork (), pid_t, -1);
#endif /* ACE_LACKS_FORK */
}

ACE_INLINE int
ACE_OS::fsync (ACE_HANDLE handle)
{
  ACE_OS_TRACE ("ACE_OS::fsync");
# if defined (ACE_LACKS_FSYNC)
  ACE_UNUSED_ARG (handle);
  ACE_NOTSUP_RETURN (-1);
# else
  ACE_OSCALL_RETURN (::fsync (handle), int, -1);
# endif /* ACE_LACKS_FSYNC */
}

ACE_INLINE int
ACE_OS::ftruncate (ACE_HANDLE handle, off_t offset)
{
  ACE_OS_TRACE ("ACE_OS::ftruncate");
#if defined (ACE_WIN32)
  if (::SetFilePointer (handle, offset, 0, FILE_BEGIN) != (unsigned) -1)
    ACE_WIN32CALL_RETURN (ACE_ADAPT_RETVAL (::SetEndOfFile (handle), ace_result_), int, -1);
  else
    ACE_FAIL_RETURN (-1);
  /* NOTREACHED */
#elif defined (ACE_PSOS_LACKS_PHILE)
  ACE_UNUSED_ARG (handle);
  ACE_UNUSED_ARG (offset);
  ACE_NOTSUP_RETURN (-1);
#elif defined (ACE_PSOS)
  ACE_OSCALL_RETURN (::ftruncate_f (handle, offset), int, -1);
#else
  ACE_OSCALL_RETURN (::ftruncate (handle, offset), int, -1);
#endif /* ACE_WIN32 */
}

ACE_INLINE ACE_TCHAR *
ACE_OS::getcwd (ACE_TCHAR *buf, size_t size)
{
  ACE_OS_TRACE ("ACE_OS::getcwd");
#if defined (ACE_PSOS_LACKS_PHILE)
  ACE_UNUSED_ARG (buf);
  ACE_UNUSED_ARG (size);
  ACE_NOTSUP_RETURN ( (char*)-1);
#elif defined (ACE_PSOS)

  static char pathbuf [BUFSIZ];

  // blank the path buffer
  ACE_OS::memset (pathbuf, '\0', BUFSIZ);

  // the following was suggested in the documentation for get_fn ()
  u_long result;
  char cur_dir_name [BUFSIZ] = ".";

  u_long cur_dir = 0, prev_dir = 0;
  while ((ACE_OS::strlen (pathbuf) < BUFSIZ) &&
         (ACE_OS::strlen (cur_dir_name) < BUFSIZ - ACE_OS::strlen ("/..")))
  {
    // get the current directory handle
    result = ::get_fn (cur_dir_name, &cur_dir);

    // check whether we're at the root: this test is
    // really lame, but the get_fn documentation says
    // *either* condition indicates you're trying to
    // move above the root.
    if ((result != 0) || ( cur_dir == prev_dir))
    {
      break;
    }

    // change name to the parent directory
    ACE_OS::strcat (cur_dir_name, "/..");

    // open the parent directory
    XDIR xdir;
    result = ::open_dir (cur_dir_name, &xdir);
    if (result != 0)
    {
      return 0;
    }

    // look for an entry that matches the current directory handle
    struct dirent dir_entry;
    while (1)
    {
      // get the next directory entry
      result = ::read_dir (&xdir, &dir_entry);
      if (result != 0)
      {
        return 0;
      }

      // check for a match
      if (dir_entry.d_filno == cur_dir)
      {
        // prefix the previous path with the entry's name and break
        if (ACE_OS::strlen (pathbuf) + ACE_OS::strlen (dir_entry.d_name) < BUFSIZ)
        {
          ACE_OS::strcpy (pathbuf + ACE_OS::strlen (dir_entry.d_name), pathbuf);
          ACE_OS::strcpy (pathbuf, dir_entry.d_name);
          break;
        }
        else
        {
          // we're out of room in the buffer
          return 0;
        }
      }
    }

    // close the parent directory
    result = ::close_dir (&xdir);
    if (result != 0)
    {
      return 0;
    }

    // save the current directory handle as the previous
    prev_dir =  cur_dir;
  }

  // return the path, if there is one
  return (ACE_OS::strlen (pathbuf) > 0) ? pathbuf : (char *) 0;
#elif defined (ACE_HAS_WINCE)
  ACE_UNUSED_ARG (buf);
  ACE_UNUSED_ARG (size);
  ACE_NOTSUP_RETURN (0);
#elif defined (ACE_WIN32)
#  if defined (ACE_USES_WCHAR)
  return ::_wgetcwd (buf, ACE_static_cast (int, size));
#  else
  return ::getcwd (buf, ACE_static_cast (int, size));
#  endif /* ACE_USES_WCHAR */
#else
  ACE_OSCALL_RETURN (::getcwd (buf, size), char *, 0);
#endif /* ACE_PSOS_LACKS_PHILE */
}

ACE_INLINE gid_t
ACE_OS::getgid (void)
{
  ACE_OS_TRACE ("ACE_OS::getgid");
#if defined (VXWORKS) || defined (ACE_PSOS) || defined (INTEGRITY)
  // getgid() is not supported:  just one user anyways
  return 0;
# elif defined (ACE_WIN32) || defined (CHORUS)
  ACE_NOTSUP_RETURN (ACE_static_cast (gid_t, -1));
# else
  ACE_OSCALL_RETURN (::getgid (), gid_t, (gid_t) -1);
# endif /* VXWORKS || ACE_PSOS */
}

#if !defined (ACE_WIN32)

ACE_INLINE int
ACE_OS::getopt (int argc, char *const *argv, const char *optstring)
{
  ACE_OS_TRACE ("ACE_OS::getopt");
#if defined (VXWORKS) || defined (ACE_PSOS) || defined (INTEGRITY)
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);
  ACE_UNUSED_ARG (optstring);
  ACE_NOTSUP_RETURN (-1);
# elif defined (ACE_LACKS_GETOPT_PROTO)
  ACE_OSCALL_RETURN (::getopt (argc, (char**) argv, optstring), int, -1);
# elif defined (ACE_LACKS_POSIX_PROTOTYPES)
  ACE_OSCALL_RETURN (::getopt (argc, (const char* const *) argv, optstring), int, -1);
# else
  ACE_OSCALL_RETURN (::getopt (argc, argv, optstring), int, -1);
# endif /* VXWORKS */
}

#else /* ACE_WIN32 */

ACE_INLINE int
ACE_OS::getopt (int argc, char *const *argv, const char *optstring)
{
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);
  ACE_UNUSED_ARG (optstring);

  ACE_OS_TRACE ("ACE_OS::getopt");
  ACE_NOTSUP_RETURN (-1);
}

#endif /* !ACE_WIN32 */

ACE_INLINE int
ACE_OS::getpagesize (void)
{
  ACE_OS_TRACE ("ACE_OS::getpagesize");
#if defined (ACE_WIN32) && !defined (ACE_HAS_PHARLAP)
  SYSTEM_INFO sys_info;
  ::GetSystemInfo (&sys_info);
  return (int) sys_info.dwPageSize;
#elif defined (_SC_PAGESIZE)
  return (int) ::sysconf (_SC_PAGESIZE);
#elif defined (ACE_HAS_GETPAGESIZE)
  return ::getpagesize ();
#else
  // Use the default set in config.h
  return ACE_PAGE_SIZE;
#endif /* ACE_WIN32 */
}

ACE_INLINE pid_t
ACE_OS::getpgid (pid_t pid)
{
  ACE_OS_TRACE ("ACE_OS::getpgid");
#if defined (ACE_LACKS_GETPGID)
  ACE_UNUSED_ARG (pid);
  ACE_NOTSUP_RETURN (-1);
#elif defined (VXWORKS) || defined (ACE_PSOS)
  // getpgid() is not supported, only one process anyway.
  ACE_UNUSED_ARG (pid);
  return 0;
#elif defined (linux) && __GLIBC__ > 1 && __GLIBC_MINOR__ >= 0
  // getpgid() is from SVR4, which appears to be the reason why GLIBC
  // doesn't enable its prototype by default.
  // Rather than create our own extern prototype, just use the one
  // that is visible (ugh).
  ACE_OSCALL_RETURN (::__getpgid (pid), pid_t, -1);
#else
  ACE_OSCALL_RETURN (::getpgid (pid), pid_t, -1);
#endif /* ACE_WIN32 */
}

ACE_INLINE pid_t
ACE_OS::getpid (void)
{
  // ACE_OS_TRACE ("ACE_OS::getpid");
#if defined (ACE_WIN32)
  return ::GetCurrentProcessId ();
#elif defined (VXWORKS) || defined (ACE_PSOS)
  // getpid() is not supported:  just one process anyways
  return 0;
#elif defined (CHORUS)
  return (pid_t) (::agetId ());
#else
  ACE_OSCALL_RETURN (::getpid (), int, -1);
#endif /* ACE_WIN32 */
}

ACE_INLINE pid_t
ACE_OS::getppid (void)
{
  ACE_OS_TRACE ("ACE_OS::getppid");
#if defined (ACE_LACKS_GETPPID)
  ACE_NOTSUP_RETURN (-1);
#elif defined (VXWORKS) || defined (ACE_PSOS)
  // getppid() is not supported, only one process anyway.
  return 0;
#else
  ACE_OSCALL_RETURN (::getppid (), pid_t, -1);
#endif /* ACE_LACKS_GETPPID */
}

ACE_INLINE uid_t
ACE_OS::getuid (void)
{
  ACE_OS_TRACE ("ACE_OS::getuid");
#if defined (VXWORKS) || defined (ACE_PSOS) || defined (INTEGRITY)
  // getuid() is not supported:  just one user anyways
  return 0;
# elif defined (ACE_WIN32) || defined (CHORUS)
  ACE_NOTSUP_RETURN (ACE_static_cast (uid_t, -1));
# else
  ACE_OSCALL_RETURN (::getuid (), uid_t, (uid_t) -1);
# endif /* VXWORKS || ACE_PSOS */
}

ACE_INLINE int
ACE_OS::hostname (char name[], size_t maxnamelen)
{
  ACE_OS_TRACE ("ACE_OS::hostname");
#if defined (ACE_HAS_PHARLAP)
  // PharLap only can do net stuff with the RT version.
#   if defined (ACE_HAS_PHARLAP_RT)
  // @@This is not at all reliable... requires ethernet and BOOTP to be used.
  // A more reliable way is to go thru the devices w/ EtsTCPGetDeviceCfg until
  // a legit IP address is found, then get its name w/ gethostbyaddr.
  ACE_SOCKCALL_RETURN (gethostname (name, maxnamelen), int, SOCKET_ERROR);
#   else
  ACE_UNUSED_ARG (name);
  ACE_UNUSED_ARG (maxnamelen);
  ACE_NOTSUP_RETURN (-1);
#   endif /* ACE_HAS_PHARLAP_RT */
#elif defined (VXWORKS) || defined (ACE_HAS_WINCE)
  ACE_OSCALL_RETURN (::gethostname (name, maxnamelen), int, -1);
#elif defined (ACE_WIN32)
  ACE_WIN32CALL_RETURN (ACE_ADAPT_RETVAL (::GetComputerNameA (name,
                                                        LPDWORD (&maxnamelen)),
                                          ace_result_), int, -1);
#elif defined (CHORUS)
  if (::gethostname (name, maxnamelen) == -1)
    return -1;
  else
    {
      if (ACE_OS::strlen (name) == 0)
        {
          // Try the HOST environment variable.
          ACE_TCHAR *const hostenv = ::getenv (ACE_LIB_TEXT ("HOST"));
          if (hostenv)
            ACE_OS::strsncpy (name, hostenv, maxnamelen);
        }
      return 0;
    }
#else /* ACE_HAS_PHARLAP */
  ACE_utsname host_info;

  if (ACE_OS::uname (&host_info) == -1)
    return -1;
  else
    {
      ACE_OS::strsncpy (name, host_info.nodename, maxnamelen);
      return 0;
    }
#endif /* ACE_HAS_PHARLAP */
}

#if defined (ACE_HAS_WCHAR)
ACE_INLINE int
ACE_OS::hostname (wchar_t name[], size_t maxnamelen)
{
#if defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)
  ACE_WIN32CALL_RETURN (ACE_ADAPT_RETVAL (GetComputerNameW (name,
                                                        LPDWORD (&maxnamelen)),
                                          ace_result_), int, -1);
#else /* ACE_WIN32 && !ACE_HAS_WINCE */
  // Emulate using the char version
  char *char_name = 0;
  int result = 0;

  ACE_NEW_RETURN (char_name, char[maxnamelen], -1);

  result = ACE_OS::hostname(char_name, maxnamelen);
  ACE_OS::strcpy (name, ACE_Ascii_To_Wide (char_name).wchar_rep ());

  delete [] char_name;
  return result;
#endif /* ACE_WIN32 && !ACE_HAS_WINCE */
}
#endif /* ACE_HAS_WCHAR */

ACE_INLINE int
ACE_OS::isatty (int handle)
{
#if defined (ACE_LACKS_ISATTY)
  ACE_UNUSED_ARG (handle);
  return 0;
# elif defined (ACE_WIN32)
  ACE_OS_TRACE ("ACE_OS::isatty");
  return ::_isatty (handle);
# else
  ACE_OS_TRACE ("ACE_OS::isatty");
  ACE_OSCALL_RETURN (::isatty (handle), int, -1);
# endif /* defined (ACE_LACKS_ISATTY) */
}

#if defined (ACE_WIN32)
ACE_INLINE int
ACE_OS::isatty (ACE_HANDLE handle)
{
#if defined (ACE_LACKS_ISATTY)
  ACE_UNUSED_ARG (handle);
  return 0;
#else
#  if defined (ACE_WIN64)
  int fd = ::_open_osfhandle (intptr_t (handle), 0);
#  else
  int fd = ::_open_osfhandle (long (handle), 0);
#  endif /* ACE_WIN64 */

  int status = ::_isatty (fd);
  ::_close (fd);
  return status;
#endif /* ACE_LACKS_ISATTY */
}

#endif /* ACE_WIN32 */

ACE_INLINE off_t
ACE_OS::lseek (ACE_HANDLE handle, off_t offset, int whence)
{
  ACE_OS_TRACE ("ACE_OS::lseek");
#if defined (ACE_WIN32)
# if SEEK_SET != FILE_BEGIN || SEEK_CUR != FILE_CURRENT || SEEK_END != FILE_END
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
      return ACE_static_cast (off_t, -1); // rather safe than sorry
    }
# endif  /* SEEK_SET != FILE_BEGIN || SEEK_CUR != FILE_CURRENT || SEEK_END != FILE_END */
  DWORD result = ::SetFilePointer (handle, offset, 0, whence);
  if (result == ACE_SYSCALL_FAILED)
    ACE_FAIL_RETURN (ACE_static_cast (off_t, -1));
  else
    return result;
#elif defined (ACE_PSOS)
# if defined (ACE_PSOS_LACKS_PHILE)
  ACE_UNUSED_ARG (handle);
  ACE_UNUSED_ARG (offset);
  ACE_UNUSED_ARG (whence);
  ACE_NOTSUP_RETURN (ACE_static_cast (off_t, -1));
# else
  unsigned long oldptr, newptr, result;
  // seek to the requested position
  result = ::lseek_f (handle, whence, offset, &oldptr);
  if (result != 0)
    {
      errno = result;
      return ACE_static_cast (off_t, -1);
    }
  // now do a dummy seek to the current position to obtain the position
  result = ::lseek_f (handle, SEEK_CUR, 0, &newptr);
  if (result != 0)
    {
      errno = result;
      return ACE_static_cast (off_t, -1);
    }
  return ACE_static_cast (off_t, newptr);
# endif /* defined (ACE_PSOS_LACKS_PHILE */
#else
  ACE_OSCALL_RETURN (::lseek (handle, offset, whence), off_t, -1);
#endif /* ACE_WIN32 */
}

#if defined (ACE_HAS_LLSEEK) || defined (ACE_HAS_LSEEK64)
ACE_INLINE ACE_LOFF_T
ACE_OS::llseek (ACE_HANDLE handle, ACE_LOFF_T offset, int whence)
{
  ACE_OS_TRACE ("ACE_OS::llseek");

#if ACE_SIZEOF_LONG == 8
  /* The native lseek is 64 bit.  Use it. */
  return ACE_OS::lseek (handle, offset, whence);
#elif defined (ACE_HAS_LLSEEK) && defined (ACE_HAS_LSEEK64)
# error Either ACE_HAS_LSEEK64 and ACE_HAS_LLSEEK should be defined, not both!
#elif defined (ACE_HAS_LSEEK64)
  ACE_OSCALL_RETURN (::lseek64 (handle, offset, whence), ACE_LOFF_T, -1);
#elif defined (ACE_HAS_LLSEEK)
  # if defined (ACE_WIN32)
  LARGE_INTEGER li;
  li.QuadPart = offset;
  li.LowPart = ::SetFilePointer (handle, li.LowPart, &li.HighPart, whence);
  if (li.LowPart == 0xFFFFFFFF && GetLastError() != NO_ERROR)
    li.QuadPart = -1;
  return li.QuadPart;
  # else
    ACE_OSCALL_RETURN (::llseek (handle, offset, whence), ACE_LOFF_T, -1);
  # endif /* WIN32 */
#endif
}
#endif /* ACE_HAS_LLSEEK || ACE_HAS_LSEEK64 */

ACE_INLINE ssize_t
ACE_OS::read (ACE_HANDLE handle, void *buf, size_t len)
{
  ACE_OS_TRACE ("ACE_OS::read");
#if defined (ACE_WIN32)
  DWORD ok_len;
  if (::ReadFile (handle, buf, ACE_static_cast (DWORD, len), &ok_len, 0))
    return (ssize_t) ok_len;
  else
    ACE_FAIL_RETURN (-1);
#elif defined (ACE_PSOS)
# if defined (ACE_PSOS_LACKS_PHILE)
  ACE_UNUSED_ARG (handle);
  ACE_UNUSED_ARG (buf);
  ACE_UNUSED_ARG (len);
  ACE_NOTSUP_RETURN (-1);
# else
  u_long count;
  u_long result = ::read_f (handle, buf, len, &count);
  if (result != 0)
    return ACE_static_cast (ssize_t, -1);
  else
    return ACE_static_cast (ssize_t, count == len ? count : 0);
# endif /* defined (ACE_PSOS_LACKS_PHILE */
#else

  int result;

# if defined (ACE_LACKS_POSIX_PROTOTYPES) || defined (ACE_HAS_CHARPTR_SOCKOPT)
  ACE_OSCALL (::read (handle, (char *) buf, len), ssize_t, -1, result);
# else
  ACE_OSCALL (::read (handle, buf, len), ssize_t, -1, result);
# endif /* ACE_LACKS_POSIX_PROTOTYPES */
  if (result == -1 && errno == EAGAIN)
    errno = EWOULDBLOCK;
  return result;
#endif /* ACE_WIN32 */
}

ACE_INLINE ssize_t
ACE_OS::read (ACE_HANDLE handle, void *buf, size_t len,
              ACE_OVERLAPPED *overlapped)
{
  ACE_OS_TRACE ("ACE_OS::read");
  overlapped = overlapped;
#if defined (ACE_WIN32)
  DWORD ok_len;
  DWORD short_len = ACE_static_cast (DWORD, len);
  if (::ReadFile (handle, buf, short_len, &ok_len, overlapped))
    return (ssize_t) ok_len;
  else
    ACE_FAIL_RETURN (-1);
#else
  return ACE_OS::read (handle, buf, len);
#endif /* ACE_WIN32 */
}

ACE_INLINE int
ACE_OS::readlink (const char *path, char *buf, size_t bufsiz)
{
  ACE_OS_TRACE ("ACE_OS::readlink");
# if defined (ACE_LACKS_READLINK) || \
     defined (ACE_HAS_WINCE) || defined (ACE_WIN32)
  ACE_UNUSED_ARG (path);
  ACE_UNUSED_ARG (buf);
  ACE_UNUSED_ARG (bufsiz);
  ACE_NOTSUP_RETURN (-1);
# else
#   if !defined(ACE_HAS_NONCONST_READLINK)
      ACE_OSCALL_RETURN (::readlink (path, buf, bufsiz), int, -1);
#   else
      ACE_OSCALL_RETURN (::readlink ((char *)path, buf, bufsiz), int, -1);
#   endif
# endif /* ACE_LACKS_READLINK */
}

#if !defined (ACE_WIN32)

ACE_INLINE int
ACE_OS::pipe (ACE_HANDLE fds[])
{
  ACE_OS_TRACE ("ACE_OS::pipe");
# if defined (VXWORKS) || defined (ACE_PSOS) || defined (INTEGRITY)
  ACE_UNUSED_ARG (fds);
  ACE_NOTSUP_RETURN (-1);
# else
  ACE_OSCALL_RETURN (::pipe (fds), int, -1);
# endif /* VXWORKS || ACE_PSOS */
}

#else /* ACE_WIN32 */

ACE_INLINE int
ACE_OS::pipe (ACE_HANDLE fds[])
{
# if !defined (ACE_HAS_WINCE) && !defined (__IBMCPP__)
  ACE_OS_TRACE ("ACE_OS::pipe");
  ACE_WIN32CALL_RETURN (ACE_ADAPT_RETVAL
                        (::CreatePipe (&fds[0], &fds[1], 0, 0),
                         ace_result_), int, -1);
# else
  ACE_NOTSUP_RETURN (-1);
# endif /* ACE_HAS_WINCE && !__IBMCPP__ */
}

#endif /* !ACE_WIN32 */

ACE_INLINE void *
ACE_OS::sbrk (int brk)
{
#if defined (ACE_LACKS_SBRK)
  ACE_UNUSED_ARG (brk);
  ACE_NOTSUP_RETURN (0);
#else
  ACE_OSCALL_RETURN (::sbrk (brk), void *, 0);
#endif /* VXWORKS */
}

ACE_INLINE int
ACE_OS::setgid (gid_t gid)
{
  ACE_OS_TRACE ("ACE_OS::setgid");
#if defined (VXWORKS) || defined (ACE_PSOS) || defined (INTEGRITY)
  // setgid() is not supported:  just one user anyways
  ACE_UNUSED_ARG (gid);
  return 0;
# elif defined (ACE_WIN32) || defined (CHORUS)
  ACE_UNUSED_ARG (gid);
  ACE_NOTSUP_RETURN (-1);
# else
  ACE_OSCALL_RETURN (::setgid (gid), int,  -1);
# endif /* VXWORKS || ACE_PSOS */
}

ACE_INLINE int
ACE_OS::setpgid (pid_t pid, pid_t pgid)
{
  ACE_OS_TRACE ("ACE_OS::setpgid");
#if defined (ACE_LACKS_SETPGID)
  ACE_UNUSED_ARG (pid);
  ACE_UNUSED_ARG (pgid);
  ACE_NOTSUP_RETURN (-1);
#elif defined (VXWORKS) || defined (ACE_PSOS)
  // <setpgid> is not supported, only one process anyway.
  ACE_UNUSED_ARG (pid);
  ACE_UNUSED_ARG (pgid);
  return 0;
#else
  ACE_OSCALL_RETURN (::setpgid (pid, pgid), int, -1);
#endif /* ACE_LACKS_SETPGID */
}

ACE_INLINE int
ACE_OS::setregid (gid_t rgid, gid_t egid)
{
  ACE_OS_TRACE ("ACE_OS::setregid");
#if defined (ACE_LACKS_SETREGID)
  ACE_UNUSED_ARG (rgid);
  ACE_UNUSED_ARG (egid);
  ACE_NOTSUP_RETURN (-1);
#elif defined (VXWORKS) || defined (ACE_PSOS)
  // <setregid> is not supported, only one process anyway.
  ACE_UNUSED_ARG (rgid);
  ACE_UNUSED_ARG (egid);
  return 0;
#else
  ACE_OSCALL_RETURN (::setregid (rgid, egid), int, -1);
#endif /* ACE_WIN32 */
}

ACE_INLINE int
ACE_OS::setreuid (uid_t ruid, uid_t euid)
{
  ACE_OS_TRACE ("ACE_OS::setreuid");
#if defined (ACE_LACKS_SETREUID)
  ACE_UNUSED_ARG (ruid);
  ACE_UNUSED_ARG (euid);
  ACE_NOTSUP_RETURN (-1);
#elif defined (VXWORKS) || defined (ACE_PSOS)
  // <setpgid> is not supported, only one process anyway.
  ACE_UNUSED_ARG (ruid);
  ACE_UNUSED_ARG (euid);
  return 0;
#else
  ACE_OSCALL_RETURN (::setreuid (ruid, euid), int, -1);
#endif /* ACE_WIN32 */
}

#if !defined (ACE_WIN32)

ACE_INLINE pid_t
ACE_OS::setsid (void)
{
  ACE_OS_TRACE ("ACE_OS::setsid");
# if defined (VXWORKS) || defined (CHORUS) || defined (ACE_PSOS) || defined (INTEGRITY)
  ACE_NOTSUP_RETURN (-1);
# else
  ACE_OSCALL_RETURN (::setsid (), int, -1);
# endif /* VXWORKS || CHORUS || ACE_PSOS */
}

#else /* ACE_WIN32 */

ACE_INLINE pid_t
ACE_OS::setsid (void)
{
  ACE_OS_TRACE ("ACE_OS::setsid");
  ACE_NOTSUP_RETURN (-1);
}

#endif /* WIN32 */

ACE_INLINE int
ACE_OS::setuid (uid_t uid)
{
  ACE_OS_TRACE ("ACE_OS::setuid");
#if defined (VXWORKS) || defined (ACE_PSOS) || defined (INTEGRITY)
  // setuid() is not supported:  just one user anyways
  ACE_UNUSED_ARG (uid);
  return 0;
# elif defined (ACE_WIN32) || defined(CHORUS)
  ACE_UNUSED_ARG (uid);
  ACE_NOTSUP_RETURN (-1);
# else
  ACE_OSCALL_RETURN (::setuid (uid), int,  -1);
# endif /* VXWORKS || ACE_PSOS */
}

ACE_INLINE int
ACE_OS::sleep (u_int seconds)
{
  ACE_OS_TRACE ("ACE_OS::sleep");
#if defined (ACE_WIN32)
  ::Sleep (seconds * ACE_ONE_SECOND_IN_MSECS);
  return 0;
#if 0
#elif defined (HPUX_10) && defined (ACE_HAS_PTHREADS_DRAFT4)
  // On HP-UX 10, _CMA_NOWRAPPERS_ disables the mapping from sleep to cma_sleep
  // which makes sleep() put the whole process to sleep, and keeps it from
  // noticing pending cancels.  So, in this case, use pthread_delay_np
  struct timespec rqtp;
  rqtp.tv_sec = seconds;
  rqtp.tv_nsec = 0L;
  return pthread_delay_np (&rqtp);
#endif /* 0 */
#elif defined (ACE_HAS_CLOCK_GETTIME)
  struct timespec rqtp;
  // Initializer doesn't work with Green Hills 1.8.7
  rqtp.tv_sec = seconds;
  rqtp.tv_nsec = 0L;
  ACE_OSCALL_RETURN (::nanosleep (&rqtp, 0), int, -1);
#elif defined (ACE_PSOS)
  timeval wait;
  wait.tv_sec = seconds;
  wait.tv_usec = 0;
  ACE_OSCALL_RETURN (::select (0, 0, 0, 0, &wait), int, -1);
#else
  ACE_OSCALL_RETURN (::sleep (seconds), int, -1);
#endif /* ACE_WIN32 */
}

ACE_INLINE int
ACE_OS::sleep (const ACE_Time_Value &tv)
{
  ACE_OS_TRACE ("ACE_OS::sleep");
#if defined (ACE_WIN32)
  ::Sleep (tv.msec ());
  return 0;
#else
# if defined (ACE_HAS_NONCONST_SELECT_TIMEVAL)
  // Copy the timeval, because this platform doesn't declare the timeval
  // as a pointer to const.
  timeval tv_copy = tv;
  ACE_OSCALL_RETURN (::select (0, 0, 0, 0, &tv_copy), int, -1);
# else  /* ! ACE_HAS_NONCONST_SELECT_TIMEVAL */
  const timeval *tvp = tv;
  ACE_OSCALL_RETURN (::select (0, 0, 0, 0, tvp), int, -1);
# endif /* ACE_HAS_NONCONST_SELECT_TIMEVAL */
#endif /* ACE_WIN32 */
}

ACE_INLINE long
ACE_OS::sysconf (int name)
{
  ACE_OS_TRACE ("ACE_OS::sysconf");
#if defined (ACE_WIN32) || defined (VXWORKS) || defined (ACE_PSOS) || defined (INTEGRITY)
  ACE_UNUSED_ARG (name);
  ACE_NOTSUP_RETURN (-1);
#else
  ACE_OSCALL_RETURN (::sysconf (name), long, -1);
#endif /* ACE_WIN32 || VXWORKS || ACE_PSOS */
}

ACE_INLINE long
ACE_OS::sysinfo (int cmd, char *buf, long count)
{
  ACE_OS_TRACE ("ACE_OS::sysinfo");
#if defined (ACE_HAS_SYSINFO)
  ACE_OSCALL_RETURN (::sysinfo (cmd, buf, count), long, -1);
#else
  ACE_UNUSED_ARG (cmd);
  ACE_UNUSED_ARG (buf);
  ACE_UNUSED_ARG (count);

  ACE_NOTSUP_RETURN (0);
#endif /* ACE_HAS_SYSINFO */
}

ACE_INLINE int
ACE_OS::truncate (const ACE_TCHAR *filename,
                  off_t offset)
{
  ACE_OS_TRACE ("ACE_OS::truncate");
#if defined (ACE_WIN32)
  ACE_HANDLE handle = ACE_OS::open (filename,
                                    O_WRONLY,
                                    ACE_DEFAULT_FILE_PERMS);
  if (handle == ACE_INVALID_HANDLE)
    ACE_FAIL_RETURN (-1);
  else if (::SetFilePointer (handle,
                             offset,
                             0,
                             FILE_BEGIN) != (unsigned) -1)
    {
      BOOL result = ::SetEndOfFile (handle);
      ::CloseHandle (handle);
      ACE_WIN32CALL_RETURN (ACE_ADAPT_RETVAL (result, ace_result_), int, -1);
    }
  else
    {
      ::CloseHandle (handle);
      ACE_FAIL_RETURN (-1);
    }
  /* NOTREACHED */
#elif !defined (ACE_LACKS_TRUNCATE)
  ACE_OSCALL_RETURN (::truncate (filename, offset), int, -1);
#else
  ACE_UNUSED_ARG (filename);
  ACE_UNUSED_ARG (offset);
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_WIN32 */
}

ACE_INLINE u_int
ACE_OS::ualarm (u_int usecs, u_int interval)
{
  ACE_OS_TRACE ("ACE_OS::ualarm");

#if defined (ACE_HAS_UALARM)
  return ::ualarm (usecs, interval);
#elif !defined (ACE_LACKS_UNIX_SIGNALS)
  ACE_UNUSED_ARG (interval);
  return ::alarm (usecs * ACE_ONE_SECOND_IN_USECS);
#else
  ACE_UNUSED_ARG (usecs);
  ACE_UNUSED_ARG (interval);
  ACE_NOTSUP_RETURN (0);
#endif /* ACE_HAS_UALARM */
}

ACE_INLINE u_int
ACE_OS::ualarm (const ACE_Time_Value &tv,
                const ACE_Time_Value &tv_interval)
{
  ACE_OS_TRACE ("ACE_OS::ualarm");

#if defined (ACE_HAS_UALARM)
  u_int usecs = (tv.sec () * ACE_ONE_SECOND_IN_USECS) + tv.usec ();
  u_int interval = (tv_interval.sec () * ACE_ONE_SECOND_IN_USECS) + tv_interval.usec ();
  return ::ualarm (usecs, interval);
#elif !defined (ACE_LACKS_UNIX_SIGNALS)
  ACE_UNUSED_ARG (tv_interval);
  return ::alarm (tv.sec ());
#else
  ACE_UNUSED_ARG (tv_interval);
  ACE_UNUSED_ARG (tv);
  ACE_NOTSUP_RETURN (0);
#endif /* ACE_HAS_UALARM */
}

ACE_INLINE int
ACE_OS::unlink (const ACE_TCHAR *path)
{
  ACE_OS_TRACE ("ACE_OS::unlink");
# if defined (VXWORKS)
    ACE_OSCALL_RETURN (::unlink (ACE_const_cast (char *, path)), int, -1);
# elif defined (ACE_PSOS) && ! defined (ACE_PSOS_LACKS_PHILE)
    ACE_OSCALL_RETURN (::remove_f ((char *) path), int , -1);
# elif defined (ACE_PSOS) && defined (ACE_PSOS_HAS_C_LIBRARY)
    ACE_OSCALL_RETURN (ACE_ADAPT_RETVAL (::remove ((char *) path),
                                                   ace_result_),
                       int, -1);
# elif defined (ACE_HAS_WINCE)
  // @@ The problem is, DeleteFile is not actually equals to unlink. ;(
  ACE_WIN32CALL_RETURN (ACE_ADAPT_RETVAL (::DeleteFile (path), ace_result_),
                        int, -1);
# elif defined (ACE_LACKS_UNLINK)
    ACE_UNUSED_ARG (path);
    ACE_NOTSUP_RETURN (-1);
# elif defined (ACE_WIN32) && defined (ACE_USES_WCHAR)
  ACE_OSCALL_RETURN (::_wunlink (path), int, -1);
# else
  ACE_OSCALL_RETURN (::unlink (path), int, -1);
# endif /* VXWORKS */
}

ACE_INLINE ssize_t
ACE_OS::write (ACE_HANDLE handle, const void *buf, size_t nbyte)
{
  ACE_OS_TRACE ("ACE_OS::write");
#if defined (ACE_WIN32)
  DWORD bytes_written; // This is set to 0 byte WriteFile.

  // Strictly correctly, we should loop writing all the data if more
  // than a DWORD length can hold.
  DWORD short_nbyte = ACE_static_cast (DWORD, nbyte);
  if (::WriteFile (handle, buf, short_nbyte, &bytes_written, 0))
    return (ssize_t) bytes_written;
  else
    ACE_FAIL_RETURN (-1);
#elif defined (ACE_PSOS)
# if defined (ACE_PSOS_LACKS_PHILE)
  ACE_UNUSED_ARG (handle);
  ACE_UNUSED_ARG (buf);
  ACE_UNUSED_ARG (nbyte);
  ACE_NOTSUP_RETURN (-1);
# else
  if(::write_f (handle, (void *) buf, nbyte) == 0)
    return (ssize_t) nbyte;
  else
    return -1;
# endif /* defined (ACE_PSOS_LACKS_PHILE) */
#else
# if defined (ACE_LACKS_POSIX_PROTOTYPES)
  ACE_OSCALL_RETURN (::write (handle, (const char *) buf, nbyte), ssize_t, -1);
# elif defined (ACE_PSOS)
  ACE_OSCALL_RETURN (::write_f(handle, (void *) buf, nbyte), ssize_t, -1);
# elif defined (ACE_HAS_CHARPTR_SOCKOPT)
  ACE_OSCALL_RETURN (::write (handle, (char *) buf, nbyte), ssize_t, -1);
# else
  ACE_OSCALL_RETURN (::write (handle, buf, nbyte), ssize_t, -1);
# endif /* ACE_LACKS_POSIX_PROTOTYPES */
#endif /* ACE_WIN32 */
}

ACE_INLINE ssize_t
ACE_OS::write (ACE_HANDLE handle,
               const void *buf,
               size_t nbyte,
               ACE_OVERLAPPED *overlapped)
{
  ACE_OS_TRACE ("ACE_OS::write");
  overlapped = overlapped;
#if defined (ACE_WIN32)
  DWORD bytes_written; // This is set to 0 byte WriteFile.

  DWORD short_nbyte = ACE_static_cast (DWORD, nbyte);
  if (::WriteFile (handle, buf, short_nbyte, &bytes_written, overlapped))
    return (ssize_t) bytes_written;
  else
    return -1;
#else
  return ACE_OS::write (handle, buf, nbyte);
#endif /* ACE_WIN32 */
}

