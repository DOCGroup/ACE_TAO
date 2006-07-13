// $Id$

#include "ace/OS_NS_unistd.h"

ACE_RCSID(ace, OS_NS_unistd, "$Id$")

#if !defined (ACE_HAS_INLINED_OSCALLS)
# include "ace/OS_NS_unistd.inl"
#endif /* ACE_HAS_INLINED_OSCALLS */

#include "ace/Base_Thread_Adapter.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_ctype.h"
#include "ace/Default_Constants.h"
#include "ace/OS_Memory.h"
#include "ace/OS_NS_Thread.h"
#include "ace/Object_Manager_Base.h"
#include "ace/os_include/sys/os_pstat.h"
#include "ace/os_include/sys/os_sysctl.h"

#if defined (ACE_NEEDS_FTRUNCATE)
extern "C" int
ftruncate (ACE_HANDLE handle, long len)
{
  struct flock fl;
  fl.l_whence = 0;
  fl.l_len = 0;
  fl.l_start = len;
  fl.l_type = F_WRLCK;

  return ACE_OS::fcntl (handle, F_FREESP, reinterpret_cast <long> (&fl));
}
#endif /* ACE_NEEDS_FTRUNCATE */

/*****************************************************************************/

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

int
ACE_OS::argv_to_string (ACE_TCHAR **argv,
                        ACE_TCHAR *&buf,
                        bool substitute_env_args)
{
  if (argv == 0 || argv[0] == 0)
    return 0;

  size_t buf_len = 0;

  // Determine the length of the buffer.

  for (int i = 0; argv[i] != 0; i++)
    {
#if !defined (ACE_LACKS_ENV)
      // Account for environment variables.
      if (substitute_env_args && argv[i][0] == ACE_LIB_TEXT ('$'))
        {
#  if defined (ACE_WIN32) || !defined (ACE_HAS_WCHAR)
          ACE_TCHAR *temp = 0;
          // Win32 is the only platform with a wide-char ACE_OS::getenv().
          if ((temp = ACE_OS::getenv (&argv[i][1])) != 0)
            buf_len += ACE_OS::strlen (temp);
          else
            buf_len += ACE_OS::strlen (argv[i]);
#  else
          // This is an ACE_HAS_WCHAR platform and not ACE_WIN32.
          // Convert the env variable name for getenv(), then add
          // the length of the returned char *string. Later, when we
          // actually use the returned env variable value, convert it
          // as well.
          char *ctemp = ACE_OS::getenv (ACE_TEXT_ALWAYS_CHAR (&argv[i][1]));
          if (ctemp == 0)
            buf_len += ACE_OS::strlen (argv[i]);
          else
            buf_len += ACE_OS::strlen (ctemp);
#  endif /* ACE_WIN32 || !ACE_HAS_WCHAR */
        }
      else
#endif /* ACE_LACKS_ENV */
        buf_len += ACE_OS::strlen (argv[i]);

      // Add one for the extra space between each string.
      buf_len++;
    }

  // Step through all argv params and copy each one into buf; separate
  // each param with white space.

  ACE_NEW_RETURN (buf,
                  ACE_TCHAR[buf_len + 1],
                  0);

  // Initial null charater to make it a null string.
  buf[0] = '\0';
  ACE_TCHAR *end = buf;
  int j;

  for (j = 0; argv[j] != 0; j++)
    {

#if !defined (ACE_LACKS_ENV)
      // Account for environment variables.
      if (substitute_env_args && argv[j][0] == ACE_LIB_TEXT ('$'))
        {
#  if defined (ACE_WIN32) || !defined (ACE_HAS_WCHAR)
          // Win32 is the only platform with a wide-char ACE_OS::getenv().
          ACE_TCHAR *temp = ACE_OS::getenv (&argv[j][1]);
          if (temp != 0)
            end = ACE_OS::strecpy (end, temp);
          else
            end = ACE_OS::strecpy (end, argv[j]);
#  else
          // This is an ACE_HAS_WCHAR platform and not ACE_WIN32.
          // Convert the env variable name for getenv(), then convert
          // the returned char *string back to wchar_t.
          char *ctemp = ACE_OS::getenv (ACE_TEXT_ALWAYS_CHAR (&argv[j][1]));
          if (ctemp == 0)
            end = ACE_OS::strecpy (end, argv[j]);
          else
            end = ACE_OS::strecpy (end, ACE_TEXT_CHAR_TO_TCHAR (ctemp));
#  endif /* ACE_WIN32 || !ACE_HAS_WCHAR */
        }
      else
#endif /* ACE_LACKS_ENV */
        end = ACE_OS::strecpy (end, argv[j]);

      // Replace the null char that strecpy put there with white
      // space.
      end[-1] = ' ';
    }

  // Null terminate the string.
  *end = '\0';
  // The number of arguments.
  return j;
}

int
ACE_OS::execl (const char * /* path */, const char * /* arg0 */, ...)
{
  ACE_OS_TRACE ("ACE_OS::execl");
  ACE_NOTSUP_RETURN (-1);
  // Need to write this code.
  // ACE_OSCALL_RETURN (::execv (path, argv), int, -1);
}

int
ACE_OS::execle (const char * /* path */, const char * /* arg0 */, ...)
{
  ACE_OS_TRACE ("ACE_OS::execle");
  ACE_NOTSUP_RETURN (-1);
  // Need to write this code.
  //  ACE_OSCALL_RETURN (::execve (path, argv, envp), int, -1);
}

int
ACE_OS::execlp (const char * /* file */, const char * /* arg0 */, ...)
{
  ACE_OS_TRACE ("ACE_OS::execlp");
  ACE_NOTSUP_RETURN (-1);
  // Need to write this code.
  //  ACE_OSCALL_RETURN (::execvp (file, argv), int, -1);
}

pid_t
ACE_OS::fork (const ACE_TCHAR *program_name)
{
  ACE_OS_TRACE ("ACE_OS::fork");
# if defined (ACE_LACKS_FORK)
  ACE_UNUSED_ARG (program_name);
  ACE_NOTSUP_RETURN (pid_t (-1));
# else
  pid_t pid =
# if defined (ACE_HAS_STHREADS)
    ::fork1 ();
#else
    ::fork ();
#endif /* ACE_HAS_STHREADS */

#if !defined (ACE_HAS_MINIMAL_ACE_OS)
  if (pid == 0)
    ACE_Base_Thread_Adapter::sync_log_msg (program_name);
#endif /* ! ACE_HAS_MINIMAL_ACE_OS */

  return pid;
# endif /* ACE_WIN32 */
}

// Create a contiguous command-line argument buffer with each arg
// separated by spaces.

pid_t
ACE_OS::fork_exec (ACE_TCHAR *argv[])
{
# if defined (ACE_WIN32)
  ACE_TCHAR *buf;

  if (ACE_OS::argv_to_string (argv, buf) != -1)
    {
      PROCESS_INFORMATION process_info;
#   if !defined (ACE_HAS_WINCE)
      ACE_TEXT_STARTUPINFO startup_info;
      ACE_OS::memset ((void *) &startup_info,
                      0,
                      sizeof startup_info);
      startup_info.cb = sizeof startup_info;

      if (ACE_TEXT_CreateProcess (0,
                                  buf,
                                  0, // No process attributes.
                                  0,  // No thread attributes.
                                  TRUE, // Allow handle inheritance.
                                  0, // Don't create a new console window.
                                  0, // No environment.
                                  0, // No current directory.
                                  &startup_info,
                                  &process_info))
#   else
      if (ACE_TEXT_CreateProcess (0,
                                  buf,
                                  0, // No process attributes.
                                  0,  // No thread attributes.
                                  FALSE, // Can's inherit handles on CE
                                  0, // Don't create a new console window.
                                  0, // No environment.
                                  0, // No current directory.
                                  0, // Can't use startup info on CE
                                  &process_info))
#   endif /* ! ACE_HAS_WINCE */
        {
          // Free resources allocated in kernel.
          ACE_OS::close (process_info.hThread);
          ACE_OS::close (process_info.hProcess);
          // Return new process id.
          delete [] buf;
          return process_info.dwProcessId;
        }
    }

  // CreateProcess failed.
  return -1;
# else
      pid_t result = ACE_OS::fork ();

#   if defined (ACE_USES_WCHAR)
      // Wide-char builds need to convert the command-line args to
      // narrow char strings for execv().
      char **cargv;
      int arg_count;
#   endif /* ACE_HAS_WCHAR */

      switch (result)
        {
        case -1:
          // Error.
          return -1;
        case 0:
          // Child process.
#   if defined (ACE_USES_WCHAR)
          for (arg_count = 0; argv[arg_count] != 0; ++arg_count)
            ;
          ++arg_count;    // Need a 0-pointer end-of-array marker
          ACE_NEW_NORETURN (cargv, char*[arg_count]);
          if (cargv == 0)
            ACE_OS::exit (errno);
          --arg_count;    // Back to 0-indexed
          cargv[arg_count] = 0;
          while (--arg_count >= 0)
            cargv[arg_count] = ACE_Wide_To_Ascii::convert (argv[arg_count]);
          // Don't worry about freeing the cargv or the strings it points to.
          // Either the process will be replaced, or we'll exit.
          if (ACE_OS::execv (cargv[0], cargv) == -1)
            ACE_OS::exit (errno);
#   else
          if (ACE_OS::execv (argv[0], argv) == -1)
            {
              // The OS layer should not print stuff out
              // ACE_ERROR ((LM_ERROR,
              //             "%p Exec failed\n"));

              // If the execv fails, this child needs to exit.
              ACE_OS::exit (errno);
            }
#   endif /* ACE_HAS_WCHAR */

        default:
          // Server process.  The fork succeeded.
          return result;
        }
# endif /* ACE_WIN32 */
}

long
ACE_OS::num_processors (void)
{
  ACE_OS_TRACE ("ACE_OS::num_processors");

#if defined (ACE_HAS_PHARLAP)
  return 1;
#elif defined (ACE_WIN32)
  SYSTEM_INFO sys_info;
  ::GetSystemInfo (&sys_info);
  return sys_info.dwNumberOfProcessors;
#elif defined (_SC_NPROCESSORS_CONF)
  return ::sysconf (_SC_NPROCESSORS_CONF);
#elif defined (ACE_HAS_SYSCTL)
  int num_processors;
  int mib[2] = { CTL_HW, HW_NCPU };
  size_t len = sizeof (num_processors);

  sysctl(mib, 2, &num_processors, &len, NULL, 0);
  return num_processors;
#else
  ACE_NOTSUP_RETURN (-1);
#endif
}

long
ACE_OS::num_processors_online (void)
{
  ACE_OS_TRACE ("ACE_OS::num_processors_online");

#if defined (ACE_HAS_PHARLAP)
  return 1;
#elif defined (ACE_WIN32)
  SYSTEM_INFO sys_info;
  ::GetSystemInfo (&sys_info);
  return sys_info.dwNumberOfProcessors;
#elif defined (_SC_NPROCESSORS_ONLN)
  return ::sysconf (_SC_NPROCESSORS_ONLN);
#elif defined (ACE_HAS_SYSCTL)
  int num_processors;
  int mib[2] = { CTL_HW, HW_NCPU };
  size_t len = sizeof (num_processors);

  sysctl(mib, 2, &num_processors, &len, NULL, 0);
  return num_processors;
#elif defined (__hpux)
  struct pst_dynamic psd;
  if (::pstat_getdynamic (&psd, sizeof (psd), (size_t) 1, 0) != -1)
    return psd.psd_proc_cnt;
  else
    return -1;
#else
  ACE_NOTSUP_RETURN (-1);
#endif
}

ssize_t
ACE_OS::read_n (ACE_HANDLE handle,
                void *buf,
                size_t len,
                size_t *bt)
{
  size_t temp;
  size_t &bytes_transferred = bt == 0 ? temp : *bt;
  ssize_t n;

  for (bytes_transferred = 0;
       bytes_transferred < len;
       bytes_transferred += n)
    {
      n = ACE_OS::read (handle,
                        (char *) buf + bytes_transferred,
                        len - bytes_transferred);

      if (n == -1 || n == 0)
        return n;
    }

  return bytes_transferred;
}

ssize_t
ACE_OS::pread (ACE_HANDLE handle,
               void *buf,
               size_t nbytes,
               off_t offset)
{
# if defined (ACE_HAS_P_READ_WRITE)
#   if defined (ACE_WIN32)

  ACE_OS_GUARD

  // Remember the original file pointer position
  DWORD original_position = ::SetFilePointer (handle,
                                              0,
                                              0,
                                              FILE_CURRENT);

  if (original_position == 0xFFFFFFFF)
    return -1;

  // Go to the correct position
  DWORD altered_position = ::SetFilePointer (handle,
                                             offset,
                                             0,
                                             FILE_BEGIN);
  if (altered_position == 0xFFFFFFFF)
    return -1;

  DWORD bytes_read;

#     if defined (ACE_HAS_WINNT4) && (ACE_HAS_WINNT4 != 0)

  OVERLAPPED overlapped;
  overlapped.Internal = 0;
  overlapped.InternalHigh = 0;
  overlapped.Offset = offset;
  overlapped.OffsetHigh = 0;
  overlapped.hEvent = 0;

  BOOL result = ::ReadFile (handle,
                            buf,
                            static_cast <DWORD> (nbytes),
                            &bytes_read,
                            &overlapped);

  if (result == FALSE)
    {
      if (::GetLastError () != ERROR_IO_PENDING)
        return -1;

      else
        {
          result = ::GetOverlappedResult (handle,
                                          &overlapped,
                                          &bytes_read,
                                          TRUE);
          if (result == FALSE)
            return -1;
        }
    }

#     else /* ACE_HAS_WINNT4 && (ACE_HAS_WINNT4 != 0) */

  BOOL result = ::ReadFile (handle,
                            buf,
                            nbytes,
                            &bytes_read,
                            0);
  if (result == FALSE)
    return -1;

#     endif /* ACE_HAS_WINNT4 && (ACE_HAS_WINNT4 != 0) */

  // Reset the original file pointer position
  if (::SetFilePointer (handle,
                        original_position,
                        0,
                        FILE_BEGIN) == 0xFFFFFFFF)
    return -1;

  return (ssize_t) bytes_read;

#   else /* ACE_WIN32 */

  return ::pread (handle, buf, nbytes, offset);

#   endif /* ACE_WIN32 */

# else /* ACE_HAS_P_READ_WRITE */

  ACE_OS_GUARD

  // Remember the original file pointer position
  off_t original_position = ACE_OS::lseek (handle,
                                           0,
                                           SEEK_CUR);

  if (original_position == -1)
    return -1;

  // Go to the correct position
  off_t altered_position = ACE_OS::lseek (handle,
                                          offset,
                                          SEEK_SET);

  if (altered_position == -1)
    return -1;

  ssize_t bytes_read = ACE_OS::read (handle,
                                     buf,
                                     nbytes);

  if (bytes_read == -1)
    return -1;

  if (ACE_OS::lseek (handle,
                     original_position,
                     SEEK_SET) == -1)
    return -1;

  return bytes_read;

# endif /* ACE_HAD_P_READ_WRITE */
}

ssize_t
ACE_OS::pwrite (ACE_HANDLE handle,
                const void *buf,
                size_t nbytes,
                ACE_LOFF_T offset)
{
# if defined (ACE_HAS_P_READ_WRITE)
#   if defined (ACE_WIN32)

  ACE_OS_GUARD

  // Remember the original file pointer position
  DWORD original_position = ::SetFilePointer (handle,
                                              0,
                                              0,
                                              FILE_CURRENT);

  if (original_position == 0xFFFFFFFF)
    return -1;

  // Go to the correct position
  LARGE_INTEGER loffset;
  loffset.QuadPart = offset;
  DWORD altered_position = ::SetFilePointerEx (handle,
                                               loffset,
                                               0,
                                               FILE_BEGIN);
  if (altered_position == 0xFFFFFFFF)
    return -1;

  DWORD bytes_written;

#     if defined (ACE_HAS_WINNT4) && (ACE_HAS_WINNT4 != 0)

  OVERLAPPED overlapped;
  overlapped.Internal = 0;
  overlapped.InternalHigh = 0;
  overlapped.Offset = offset;
  overlapped.OffsetHigh = 0;
  overlapped.hEvent = 0;

  BOOL result = ::WriteFile (handle,
                             buf,
                             static_cast <DWORD> (nbytes),
                             &bytes_written,
                             &overlapped);

  if (result == FALSE)
    {
      if (::GetLastError () != ERROR_IO_PENDING)
        return -1;

      else
        {
          result = ::GetOverlappedResult (handle,
                                          &overlapped,
                                          &bytes_written,
                                          TRUE);
          if (result == FALSE)
            return -1;
        }
    }

#     else /* ACE_HAS_WINNT4 && (ACE_HAS_WINNT4 != 0) */

  BOOL result = ::WriteFile (handle,
                             buf,
                             nbytes,
                             &bytes_written,
                             0);
  if (result == FALSE)
    return -1;

#     endif /* ACE_HAS_WINNT4 && (ACE_HAS_WINNT4 != 0) */

  // Reset the original file pointer position
  if (::SetFilePointer (handle,
                        original_position,
                        0,
                        FILE_BEGIN) == 0xFFFFFFFF)
    return -1;

  return (ssize_t) bytes_written;

#   else /* ACE_WIN32 */

  return ::pwrite (handle, buf, nbytes, offset);
#   endif /* ACE_WIN32 */
# else /* ACE_HAS_P_READ_WRITE */

  ACE_OS_GUARD

  // Remember the original file pointer position
  off_t original_position = ACE_OS::lseek (handle,
                                           0,
                                           SEEK_CUR);
  if (original_position == -1)
    return -1;

  // Go to the correct position
  off_t altered_position = ACE_OS::lseek (handle,
                                          offset,
                                          SEEK_SET);
  if (altered_position == -1)
    return -1;

  ssize_t bytes_written = ACE_OS::write (handle,
                                         buf,
                                         nbytes);
  if (bytes_written == -1)
    return -1;

  if (ACE_OS::lseek (handle,
                     original_position,
                     SEEK_SET) == -1)
    return -1;

  return bytes_written;
# endif /* ACE_HAD_P_READ_WRITE */
}

int
ACE_OS::string_to_argv (ACE_TCHAR *buf,
                        int &argc,
                        ACE_TCHAR **&argv,
                        bool substitute_env_args)
{
  // Reset the number of arguments
  argc = 0;

  if (buf == 0)
    return -1;

  ACE_TCHAR *cp = buf;

  // First pass: count arguments.

  // '#' is the start-comment token..
  while (*cp != ACE_LIB_TEXT ('\0') && *cp != ACE_LIB_TEXT ('#'))
    {
      // Skip whitespace..
      while (ACE_OS::ace_isspace (*cp))
        cp++;

      // Increment count and move to next whitespace..
      if (*cp != ACE_LIB_TEXT ('\0'))
        argc++;

      while (*cp != ACE_LIB_TEXT ('\0') && !ACE_OS::ace_isspace (*cp))
        {
          // Grok quotes....
          if (*cp == ACE_LIB_TEXT ('\'') || *cp == ACE_LIB_TEXT ('"'))
            {
              ACE_TCHAR quote = *cp;

              // Scan past the string..
              for (cp++; *cp != ACE_LIB_TEXT ('\0') && *cp != quote; cp++)
                continue;

              // '\0' implies unmatched quote..
              if (*cp == ACE_LIB_TEXT ('\0'))
                {
                  argc--;
                  break;
                }
              else
                cp++;
            }
          else
            cp++;
        }
    }

  // Second pass: copy arguments.
  ACE_TCHAR arg[ACE_DEFAULT_ARGV_BUFSIZ];
  ACE_TCHAR *argp = arg;

  // Make sure that the buffer we're copying into is always large
  // enough.
  if (cp - buf >= ACE_DEFAULT_ARGV_BUFSIZ)
    ACE_NEW_RETURN (argp,
                    ACE_TCHAR[cp - buf + 1],
                    -1);

  // Make a new argv vector of argc + 1 elements.
  ACE_NEW_RETURN (argv,
                  ACE_TCHAR *[argc + 1],
                  -1);

  ACE_TCHAR *ptr = buf;

  for (int i = 0; i < argc; i++)
    {
      // Skip whitespace..
      while (ACE_OS::ace_isspace (*ptr))
        ptr++;

      // Copy next argument and move to next whitespace..
      cp = argp;
      while (*ptr != ACE_LIB_TEXT ('\0') && !ACE_OS::ace_isspace (*ptr))
        if (*ptr == ACE_LIB_TEXT ('\'') || *ptr == ACE_LIB_TEXT ('"'))
          {
            ACE_TCHAR quote = *ptr++;

            while (*ptr != ACE_LIB_TEXT ('\0') && *ptr != quote)
              *cp++ = *ptr++;

            if (*ptr == quote)
              ptr++;
          }
        else
          *cp++ = *ptr++;

      *cp = ACE_LIB_TEXT ('\0');

#if !defined (ACE_LACKS_ENV)
      // Check for environment variable substitution here.
      if (substitute_env_args) {
          argv[i] = ACE_OS::strenvdup(argp);

          if (argv[i] == 0)
            {
              if (argp != arg)
                delete [] argp;
              errno = ENOMEM;
              return -1;
            }
      }
      else
#endif /* ACE_LACKS_ENV */
        {
          argv[i] = ACE_OS::strdup(argp);

          if (argv[i] == 0)
            {
              if (argp != arg)
                delete [] argp;
              errno = ENOMEM;
              return -1;
            }
        }
    }

  if (argp != arg)
    delete [] argp;

  argv[argc] = 0;
  return 0;
}

// Write <len> bytes from <buf> to <handle> (uses the <write>
// system call on UNIX and the <WriteFile> call on Win32).

ssize_t
ACE_OS::write_n (ACE_HANDLE handle,
                 const void *buf,
                 size_t len,
                 size_t *bt)
{
  size_t temp;
  size_t &bytes_transferred = bt == 0 ? temp : *bt;
  ssize_t n;

  for (bytes_transferred = 0;
       bytes_transferred < len;
       bytes_transferred += n)
    {
      n = ACE_OS::write (handle,
                         (char *) buf + bytes_transferred,
                         len - bytes_transferred);

      if (n == -1 || n == 0)
        return n;
    }

  return bytes_transferred;
}

ACE_END_VERSIONED_NAMESPACE_DECL
