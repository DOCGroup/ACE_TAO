// $Id$

#define ACE_BUILD_DLL
#include "ace/IPC_SAP.h"
#include "ace/Handle_Set.h"
#include "ace/ACE.h"
#include "ace/Thread_Manager.h"
#include "ace/Reactor.h"
#include "ace/Auto_Ptr.h"
#include "ace/INET_Addr.h"
#include "ace/SString.h"
#include "ace/Process.h"

// Size of a VM page.
size_t ACE::pagesize_ = 0;

void
ACE::unique_name (const void *object,
                  LPTSTR name,
                  size_t length)
{
  // The process ID will provide uniqueness between processes on the
  // same machine. The "this" pointer of the <object> will provide
  // uniqueness between other "live" objects in the same process. The
  // uniqueness of this name is therefore only valid for the life of
  // <object>.
  TCHAR temp_name[ACE_UNIQUE_NAME_LEN];
  ACE_OS::sprintf (temp_name, __TEXT ("%d %d"), ACE_OS::getpid (), object);
  ACE_OS::strncpy (name, temp_name, length);
}

int
ACE::terminate_process (pid_t pid)
{
#if defined (ACE_WIN32)
  // Create a handle for the given process id.
  ACE_HANDLE process_handle =
    ::OpenProcess (PROCESS_TERMINATE,
                   FALSE, // New handle is not inheritable.
                   pid);

  if (process_handle == ACE_INVALID_HANDLE || process_handle == NULL)
    return -1;
  else
    {
      // Kill the process associated with process_handle.
      BOOL terminate_result = ::TerminateProcess (process_handle, 0);
      // Free up the kernel resources.
      ACE_OS::close (process_handle);
      return terminate_result;
    }
#elif defined (CHORUS)
  KnCap cap_;

  // Use the pid to find out the actor's capability, then kill it.
  if (::acap (AM_MYSITE, pid, &cap_) == 0)
    return ::akill (&cap_);
  else
    return -1;
#else
  return ACE_OS::kill (pid, 9);
#endif /* ACE_WIN32 */
}

int
ACE::process_active (pid_t pid)
{
#if !defined(ACE_WIN32)
  int retval = ACE_OS::kill (pid, 0);

  if (retval == 0)
    return 1;
  else if (errno == ESRCH)
    return 0;
  else
    return -1;
#else
  // Create a handle for the given process id.
  ACE_HANDLE process_handle = ::OpenProcess (PROCESS_QUERY_INFORMATION,
                                             FALSE,
                                             pid);
  if (process_handle == ACE_INVALID_HANDLE
      || process_handle == NULL)
    return 0;
  else
    {
      DWORD status;

      if (::GetExitCodeProcess (process_handle,
                                &status) == 0
          || status != STILL_ACTIVE)
        return 0;
      else
        return 1;
    }
#endif /* ACE_WIN32 */
}

int
ACE::register_stdin_handler (ACE_Event_Handler *eh,
                             ACE_Reactor *reactor,
                             ACE_Thread_Manager *thr_mgr,
                             int flags)
{
#if defined (ACE_WIN32)
  ACE_UNUSED_ARG (reactor);

  return thr_mgr->spawn (&ACE::read_adapter, (void *) eh, flags);
#else
  // Keep compilers happy.
  ACE_UNUSED_ARG (flags);
  ACE_UNUSED_ARG (thr_mgr);
  return reactor->register_handler (ACE_STDIN, eh, ACE_Event_Handler::READ_MASK);
#endif /* ACE_WIN32 */
}

// Used to read from non-socket ACE_HANDLEs in our own thread to work
// around Win32 limitations that don't allow us to select() on
// non-sockets (such as ACE_STDIN).  This is commonly used in
// situations where the Reactor is used to demultiplex read events on
// ACE_STDIN on UNIX.  Note that <event_handler> must be a subclass of
// <ACE_Event_Handler>.  If the <get_handle> method of this event
// handler returns <ACE_INVALID_HANDLE> we default to reading from
// ACE_STDIN.

void *
ACE::read_adapter (void *args)
{
  ACE_Event_Handler *this_ptr = (ACE_Event_Handler *) args;
  ACE_HANDLE handle = ACE_STDIN;

  while (this_ptr->handle_input (handle) != -1)
    continue;

  this_ptr->handle_close (handle, ACE_Event_Handler::READ_MASK);

  return 0;
}

const char *
ACE::execname (const char *old_name)
{
#if defined (ACE_HAS_WIN32)
  if (ACE_OS::strstr (old_name, ".exe") == 0)
    {
      char *new_name;

      ACE_NEW_RETURN (new_name, char[ACE_OS::strlen (old_name) +
                                     ACE_OS::strlen (".exe") +
                                     1], -1);
      char *end = new_name;
      end = ACE::strecpy (new_name, old_name);
      // Concatenate the .exe suffix onto the end of the executable.
      ACE_OS::strcpy (end, ".exe");
      return new_name;
    }
#endif /* ACE_HAS_WIN32 */
  return old_name;
}

u_long
ACE::hash_pjw (const char *str)
{
  u_long hash = 0;

  for (const char *temp = str; *temp != 0; temp++)
    {
      hash = (hash << 4) + (*temp * 13);

      u_long g = hash & 0xf0000000;

      if (g)
        {
          hash ^= (g >> 24);
          hash ^= g;
        }
    }

  return hash;
}

u_long
ACE::hash_pjw (const ACE_USHORT16 *str)
{
  u_long hash = 0;

  for (const ACE_USHORT16 *temp = str; *temp != 0; temp++)
    {
      hash = (hash << 4) + (*temp * 13);

      u_long g = hash & 0xf0000000;

      if (g)
        {
          hash ^= (g >> 24);
          hash ^= g;
        }
    }

  return hash;
}

size_t
ACE::strrepl (char *s, char search, char replace)
{
  ACE_TRACE ("ACE::strrepl");

  size_t replaced = 0;

  for (size_t i = 0; s[i] != '\0'; i++)
    if (s[i] == search)
      {
        s[i] = replace;
        replaced++;
      }

  return replaced;
}

char *
ACE::strenvdup (const char *str)
{
  ACE_TRACE ("ACE::strenvdup");

  char *temp;

  if (str[0] == '$'
      && (temp = ACE_OS::getenv (&str[1])) != 0)
    return ACE_OS::strdup (temp);
  else
    return ACE_OS::strdup (str);
}

/*

Examples:

Source               NT                       UNIX
===============================================================
netsvc               netsvc.dll               libnetsvc.so
                     (PATH will be evaluated) (LD_LIBRARY_PATH evaluated)

libnetsvc.dll        libnetsvc.dll            libnetsvc.dll + warning
netsvc.so            netsvc.so + warning      libnetsvc.so

..\../libs/netsvc    ..\..\libs\netsvc.dll     ../../libs/netsvc.so
                     (absolute path used)         (absolute path used)

*/

int
ACE::ldfind (const char filename[],
             char pathname[],
             size_t maxpathnamelen)
{
  ACE_TRACE ("ACE::ldfind");

  char tempcopy[MAXPATHLEN + 1];
  char searchpathname[MAXPATHLEN + 1];
  char searchfilename[MAXPATHLEN + 1];

  // Create a copy of filename to work with.
  if (ACE_OS::strlen (filename) + 1 > (sizeof tempcopy / sizeof (char)))
    {
      errno = ENOMEM;
      return -1;
    }
  else
    ACE_OS::strcpy (tempcopy, filename);

  // Insert canonical directory separators.
  char *separator_ptr;

  if (ACE_DIRECTORY_SEPARATOR_CHAR != '/')
    // Make all the directory separators ``canonical'' to simplify
    // subsequent code.
    ACE::strrepl (tempcopy, ACE_DIRECTORY_SEPARATOR_CHAR, '/');

  // Separate filename from pathname.
  separator_ptr = ACE_OS::strrchr (tempcopy, '/');

  // This is a relative path.
  if (separator_ptr == 0)
    {
      searchpathname[0] = '\0';
      ACE_OS::strcpy (searchfilename, tempcopy);
    }
  else // This is an absolute path.
    {
      ACE_OS::strcpy (searchfilename, separator_ptr + 1);
      separator_ptr[1] = '\0';
      ACE_OS::strcpy (searchpathname, tempcopy);
    }

  int got_suffix = 0;

  // Check to see if this has an appropriate DLL suffix for the OS
  // platform.
  char *s = ACE_OS::strrchr (searchfilename, '.');

  if (s != 0)
    {
      // Check whether this matches the appropriate platform-specific suffix.
      if (ACE_OS::strcmp (s, ACE_DLL_SUFFIX) == 0)
        got_suffix = 1;
      else
        ACE_ERROR ((LM_WARNING,
                    "Warning: improper suffix for a shared library on this platform: %s\n",
                    s));
    }

  // Make sure we've got enough space in searchfilename.
  if (ACE_OS::strlen (searchfilename) +
      ACE_OS::strlen (ACE_DLL_PREFIX) +
      got_suffix ? 0 : ACE_OS::strlen (ACE_DLL_SUFFIX) >= (sizeof searchfilename / sizeof (char)))
    {
      errno = ENOMEM;
      return -1;
    }

  // Use absolute pathname if there is one.
  if (ACE_OS::strlen (searchpathname) > 0)
    {
      if (ACE_OS::strlen (searchfilename)
          + ACE_OS::strlen (searchpathname) >= maxpathnamelen)
        {
          errno = ENOMEM;
          return -1;
        }
      else
        {
          if (ACE_DIRECTORY_SEPARATOR_CHAR != '/')
            // Revert to native path name separators
            ACE::strrepl (searchpathname, '/', ACE_DIRECTORY_SEPARATOR_CHAR);

          // First, try matching the filename *without* adding a
          // prefix.
          ACE_OS::sprintf (pathname, "%s%s%s",
                           searchpathname,
                           searchfilename,
                           got_suffix ? "" : ACE_DLL_SUFFIX);
          if (ACE_OS::access (pathname, F_OK) == 0)
            return 0;

          // Second, try matching the filename *with* adding a prefix.
          ACE_OS::sprintf (pathname, "%s%s%s%s",
                           searchpathname,
                           ACE_DLL_PREFIX,
                           searchfilename,
                           got_suffix ? "" : ACE_DLL_SUFFIX);
          if (ACE_OS::access (pathname, F_OK) == 0)
            return 0;
        }
    }

  // Use relative filenames via LD_LIBRARY_PATH or PATH (depending on
  // OS platform).
  else
    {
      char *ld_path = ACE_OS::getenv (ACE_LD_SEARCH_PATH);

      if (ld_path != 0 && (ld_path = ACE_OS::strdup (ld_path)) != 0)
        {
          // strtok has the strange behavior of not separating the
          // string ":/foo:/bar" into THREE tokens.  One would expect
          // that the first iteration the token would be an empty
          // string, the second iteration would be "/foo", and the
          // third iteration would be "/bar".  However, this is not
          // the case; one only gets two iterations: "/foo" followed
          // by "/bar".

          // This is especially a problem in parsing Unix paths
          // because it is permissible to specify 'the current
          // directory' as an empty entry.  So, we introduce the
          // following special code to cope with this:
              
          // Look at each dynamic lib directory in the search path.
          char *nextholder = 0;
          char *path_entry = ACE_OS::strsplit_r
            (ld_path, ACE_LD_SEARCH_PATH_SEPARATOR_STR, nextholder);

          int result = 0;

          while (path_entry != 0)
            {
              if (ACE_OS::strlen (path_entry)
                  + 1 + ACE_OS::strlen (searchfilename) >= maxpathnamelen)
                {
                  errno = ENOMEM;
                  result = -1;
                  break;
                }

              // This works around the issue where a path might have
              // an empty component indicating 'current directory'.
              // We need to do it here rather than anywhere else so
              // that the loop condition will still work.
              if (path_entry[0] == '\0')
                path_entry = ".";
              
              // First, try matching the filename *without* adding a
              // prefix.
              ACE_OS::sprintf (pathname, "%s%c%s%s",
                               path_entry,
                               ACE_DIRECTORY_SEPARATOR_CHAR,
                               searchfilename,
                               got_suffix ? "" : ACE_DLL_SUFFIX);
              if (ACE_OS::access (pathname, F_OK) == 0)
                break;

              // Second, try matching the filename *with* adding a
              // prefix.
              ACE_OS::sprintf (pathname, "%s%c%s%s%s",
                               path_entry,
                               ACE_DIRECTORY_SEPARATOR_CHAR,
                               ACE_DLL_PREFIX,
                               searchfilename,
                               got_suffix ? "" : ACE_DLL_SUFFIX);
              if (ACE_OS::access (pathname, F_OK) == 0)
                break;

              // Fetch the next item in the path
              path_entry = ACE_OS::strsplit_r
                (0, ACE_LD_SEARCH_PATH_SEPARATOR_STR, nextholder);
            }

          ACE_OS::free ((void *) ld_path);
          return result;
        }
    }

  errno = ENOENT;
  return -1;
}

FILE *
ACE::ldopen (const char *filename, const char *type)
{
  ACE_TRACE ("ACE::ldopen");
  char buf[MAXPATHLEN + 1];

  if (ACE::ldfind (filename, buf, sizeof buf) == -1)
    return 0;
  else
    return ACE_OS::fopen (buf, type);
}

const char *
ACE::basename (const char *pathname, char delim)
{
  ACE_TRACE ("ACE::basename");
  const char *temp = ::strrchr (pathname, delim);

  if (temp == 0)
    return pathname;
  else
    return temp + 1;
}

#if defined (ACE_HAS_UNICODE)
const wchar_t *
ACE::basename (const wchar_t *pathname, wchar_t delim)
{
  ACE_TRACE ("ACE::basename");
  const wchar_t *temp = ACE_OS::strrchr (pathname, delim);

  if (temp == 0)
    return pathname;
  else
    return temp + 1;
}
#endif /* ACE_HAS_UNICODE */

// Send N char *ptrs and int lengths.  Note that the char *'s precede
// the ints (basically, an varargs version of writev).  The count N is
// the *total* number of trailing arguments, *not* a couple of the
// number of tuple pairs!

ssize_t
ACE::send (ACE_HANDLE handle, size_t n, ...)
{
  ACE_TRACE ("ACE_SOCK_IO::send");

  va_list argp;  
  size_t total_tuples = n / 2;
  iovec *iovp;
#if defined (ACE_HAS_ALLOCA)
  iovp = (iovec *) alloca (total_tuples * sizeof (iovec));
#else 
  ACE_NEW_RETURN (iovp, iovec[total_tuples], -1);
#endif /* !defined (ACE_HAS_ALLOCA) */

  va_start (argp, n);

  for (size_t i = 0; i < total_tuples; i++)
    {
      iovp[i].iov_base = va_arg (argp, char *);
      iovp[i].iov_len  = va_arg (argp, int);
    }

  ssize_t result = ACE_OS::writev (handle, iovp, total_tuples);
#if !defined (ACE_HAS_ALLOCA)
  delete [] iovp;
#endif /* !defined (ACE_HAS_ALLOCA) */
  va_end (argp);
  return result;
}

// This is basically an interface to ACE_OS::readv, that doesn't use
// the struct iovec explicitly.  The ... can be passed as an arbitrary
// number of (char *ptr, int len) tuples.  However, the count N is the
// *total* number of trailing arguments, *not* a couple of the number
// of tuple pairs!

ssize_t
ACE::recv (ACE_HANDLE handle, size_t n, ...)
{
  ACE_TRACE ("ACE_SOCK_IO::recv");

  va_list argp;  
  size_t total_tuples = n / 2;
  iovec *iovp;
#if defined (ACE_HAS_ALLOCA)
  iovp = (iovec *) alloca (total_tuples * sizeof (iovec));
#else
  ACE_NEW_RETURN (iovp, iovec[total_tuples], -1);
#endif /* !defined (ACE_HAS_ALLOCA) */

  va_start (argp, n);

  for (size_t i = 0; i < total_tuples; i++)
    {
      iovp[i].iov_base = va_arg (argp, char *);
      iovp[i].iov_len  = va_arg (argp, int);
    }

  ssize_t result = ACE_OS::readv (handle, iovp, total_tuples);
#if !defined (ACE_HAS_ALLOCA)
  delete [] iovp;
#endif /* !defined (ACE_HAS_ALLOCA) */
  va_end (argp);
  return result;
}

// Miscellaneous static methods used throughout ACE.

ssize_t
ACE::send_n (ACE_HANDLE handle, const void *buf, size_t len)
{
  ACE_TRACE ("ACE::send_n");
  size_t bytes_written;
  ssize_t n = 0;

  for (bytes_written = 0; bytes_written < len; bytes_written += n)
    {
      n = ACE::send (handle, (const char *) buf + bytes_written,
                     len - bytes_written);
      if (n == -1)
        {
          if (errno != EWOULDBLOCK)
            return -1;
          else
            n = 0; // Keep trying to send.
        }
    }

  return bytes_written;
}

ssize_t
ACE::send_n (ACE_HANDLE handle,
             const void *buf,
             size_t len,
             int flags)
{
  ACE_TRACE ("ACE::send_n");
  size_t bytes_written;
  ssize_t n = 0;

  for (bytes_written = 0; bytes_written < len; bytes_written += n)
    {
      n = ACE_OS::send (handle, (const char *) buf + bytes_written,
                        len - bytes_written, flags);

      if (n == -1)
        {
          if (errno != EWOULDBLOCK)
            return -1;
          else
            n = 0; // Keep trying to send.
        }
    }

  return bytes_written;
}

// Receive <len> bytes into <buf> from <handle> (uses the <write>
// system call on UNIX and the <WriteFile> call on Win32).

ssize_t
ACE::write_n (ACE_HANDLE handle,
              const void *buf,
              size_t len)
{
  ACE_TRACE ("ACE::write_n");

  size_t bytes_written;
  ssize_t n = 0;

  for (bytes_written = 0; bytes_written < len; bytes_written += n)
    {
      n = ACE_OS::write (handle, (const char *) buf + bytes_written,
                         len - bytes_written);
      if (n == -1)
        {
          if (errno != EWOULDBLOCK)
            return -1;
          else
            n = 0; // Keep trying to send.
        }
    }

  return bytes_written;
}

ssize_t
ACE::recv_n (ACE_HANDLE handle, void *buf, size_t len)
{
  ACE_TRACE ("ACE::recv_n");
  size_t bytes_read;
  ssize_t n = 0;

  for (bytes_read = 0; bytes_read < len; bytes_read += n)
    {
      n = ACE::recv (handle, (char *) buf + bytes_read, len - bytes_read);

      if (n == -1)
        {
          if (errno != EWOULDBLOCK)
            return -1;
          else
            n = 0; // Keep trying to read.
        }
      else if (n == 0)
        break;
    }
  return bytes_read;
}

ssize_t
ACE::recv_n (ACE_HANDLE handle, void *buf, size_t len, int flags)
{
  ACE_TRACE ("ACE::recv_n");
  size_t bytes_read;
  ssize_t n = 0;

  for (bytes_read = 0; bytes_read < len; bytes_read += n)
    {
      n = ACE::recv (handle,
                     (char *) buf + bytes_read,
                     len - bytes_read,
                     flags);

      if (n == -1)
        {
          if (errno != EWOULDBLOCK)
            return -1;
          else
            n = 0; // Keep trying to read.
        }
      else if (n == 0)
        break;
    }

  return bytes_read;
}

// Receive <len> bytes into <buf> from <handle> (uses the <read>
// system call on UNIX and the <ReadFile> call on Win32).

ssize_t
ACE::read_n (ACE_HANDLE handle,
             void *buf,
             size_t len)
{
  ACE_TRACE ("ACE::read_n");

  size_t bytes_read;
  ssize_t n = 0;

  for (bytes_read = 0; bytes_read < len; bytes_read += n)
    {
      n = ACE_OS::read (handle, (char *) buf + bytes_read, len - bytes_read);

      if (n == -1)
        {
          if (errno != EWOULDBLOCK)
            return -1;
          else
            n = 0; // Keep trying to read.
        }
      else if (n == 0)
        break;
    }

  return bytes_read;
}

int 
ACE::enter_recv_timedwait (ACE_HANDLE handle,
                           const ACE_Time_Value *timeout,
                           int &val)
{
  // Give value a default value to keep Purify happy!
  val = 0;

  if (timeout == 0)
    return 0;

  ACE_Handle_Set handle_set;
  handle_set.set_bit (handle);

  // Wait for input to arrive or for the timeout to elapse.
  switch (ACE_OS::select (int (handle) + 1,
                          (fd_set *) handle_set, // read_fds.
                          (fd_set *) 0, // write_fds.
                          (fd_set *) 0, // exception_fds.
                          timeout))
    {
    case 1: // OK to read now.
      // We need to record whether we are already *in* nonblocking
      // mode, so that we can correctly reset the state when we're
      // done.
      val = ACE::get_flags (handle);

      if (ACE_BIT_ENABLED (val, ACE_NONBLOCK) == 0)
        // Set the descriptor into non-blocking mode if it's not
        // already in it.
        ACE::set_flags (handle, ACE_NONBLOCK);
      return 1;
    case 0: // Timer expired.
      errno = ETIME;
      /* FALLTHRU */
    default: // if we got here directly select() must have returned -1.
      return -1;
    }
}

void 
ACE::leave_recv_timedwait (ACE_HANDLE handle,
                           const ACE_Time_Value *timeout,
                           int val)
{
  if (timeout != 0 
      && ACE_BIT_ENABLED (val, ACE_NONBLOCK) == 0)
    {
      // We need to stash errno here because ACE::clr_flags() may
      // reset it.
      int error = errno;

      // Only disable ACE_NONBLOCK if we weren't in non-blocking mode
      // originally.
      ACE::clr_flags (handle, ACE_NONBLOCK);
      errno = error;
    }
}

int 
ACE::enter_send_timedwait (ACE_HANDLE handle,
                           const ACE_Time_Value* timeout,
                           int &val)
{
  // Give value a default value to keep Purify happy!
  val = 0;

  if (timeout==0)
    return 0;

  ACE_Handle_Set handle_set;
  handle_set.set_bit (handle);

  // On timed writes we always go into select(); only if the
  // descriptor is available for writing within the specified amount
  // of time do we put it in non-blocking mode

  switch (ACE_OS::select (int (handle) + 1,
                          (fd_set *) 0, 
                          (fd_set *) handle_set,
                          (fd_set *) 0,
                          timeout))
    {
    case 1: // Ok to write now.
      // We need to record whether we are already *in* nonblocking
      // mode, so that we can correctly reset the state when we're
      // done.
      val = ACE::get_flags (handle);

      if (ACE_BIT_ENABLED (val, ACE_NONBLOCK) == 0)
        // Set the descriptor into non-blocking mode if it's not
        // already in it.
        ACE::set_flags (handle, ACE_NONBLOCK);
      return 1;
    case 0: // Timer expired.
      errno = ETIME;
      /* FALLTHRU */
    default: // if we got here directly select() must have returned -1.
      return -1;
    }
}

void 
ACE::leave_send_timedwait (ACE_HANDLE handle,
                           const ACE_Time_Value *timeout,
                           int val)
{
  if (timeout != 0
      && ACE_BIT_ENABLED (val, ACE_NONBLOCK) == 0)
    {
      // We need to stash errno here because ACE::clr_flags() may
      // reset it.
      int error = errno;

      // Only disable ACE_NONBLOCK if we weren't in non-blocking mode
      // originally.
      ACE::clr_flags (handle, ACE_NONBLOCK);
      errno = error;
    }
}

ssize_t
ACE::sendto (ACE_HANDLE handle,
             const char *buf,
             int len, 
             int flags,
             const struct sockaddr *addr,
             int addrlen,
             const ACE_Time_Value *timeout)
{
  // ACE_TRACE ("ACE::sendto");
#if defined (ACE_HAS_SENDTO_TIMEDWAIT)
  if (timeout == 0)
     return ACE_OS::sendto (handle, buf, len, flags, addr, addrlen);
  else
    {
      ACE_Time_Value copy = *timeout;
      copy += ACE_OS::gettimeofday ();
      timespec_t ts = copy;
      return ::sendto_timedwait (handle, buf, len, flags, addr, addrlen, &ts);
    }
#else
  int val;
  if (ACE::enter_send_timedwait (handle, timeout, val) == -1)
    return -1;
  else 
    {
      int bytes_written = ACE_OS::sendto (handle, buf, len, flags, addr, addrlen);
      ACE::leave_send_timedwait (handle, timeout, val);
      return bytes_written;
    }
#endif /* ACE_HAS_SENDTO_TIMEDWAIT */
}

ssize_t
ACE::sendmsg (ACE_HANDLE handle, 
              const struct msghdr *msg,
              int flags,
              const ACE_Time_Value *timeout)
{
  // ACE_TRACE ("ACE::sendmsg");
#if defined (ACE_HAS_SENDMSG_TIMEDWAIT)
  if (timeout == 0)
     return ACE_OS::sendmsg (handle, msg, flags);
  else 
    {
      ACE_Time_Value copy = *timeout;
      copy += ACE_OS::gettimeofday ();
      timespec_t ts = copy;
      return ::sendmsg_timedwait (handle, msg, flags, &ts);
    }
#else
  int val;
  if (ACE::enter_send_timedwait (handle, timeout, val) == -1)
    return -1;
  else 
    {
      int bytes_written = ACE_OS::sendmsg (handle, msg, flags);
      ACE::leave_send_timedwait (handle, timeout, val);
      return bytes_written;
    }
#endif /* ACE_HAS_SENDMSG_TIMEDWAIT */
}

ssize_t
ACE::readv (ACE_HANDLE handle,
            struct iovec *iov,
            int iovcnt,
            const ACE_Time_Value *timeout)
{
  // ACE_TRACE ("ACE::readv");
#if defined (ACE_HAS_READV_TIMEDWAIT)
  if (timeout == 0)
     return ACE_OS::readv (handle, iov, iovcnt);
  else {
     ACE_Time_Value copy = *timeout;
     copy += ACE_OS::gettimeofday ();
     timespec_t ts = copy;
     return ::readv_timedwait (handle, iov, iovcnt, &ts);
  }
#else
  int val;
  if (ACE::enter_recv_timedwait (handle, timeout, val) == -1)
     return -1;
  else 
    {
      ssize_t bytes_read = ACE_OS::readv (handle, iov, iovcnt);
      ACE::leave_recv_timedwait (handle, timeout, val);
      return bytes_read;
    }
#endif /* ACE_HAS_READV_TIMEDWAIT */
}

ssize_t
ACE::writev (ACE_HANDLE handle,
             const struct iovec *iov,
             int iovcnt,
             const ACE_Time_Value *timeout)
{
  // ACE_TRACE ("ACE::writev");
#if defined (ACE_HAS_WRITEV_TIMEDWAIT)
  if (timeout == 0)
     return ACE_OS::writev (handle, iov, iovcnt);
  else {
     ACE_Time_Value copy = *timeout;
     copy += ACE_OS::gettimeofday ();
     timespec_t ts = copy;
     return ::writev_timedwait (handle, iov, iovcnt, &ts);
  }
#else
  int val;
  if (ACE::enter_send_timedwait (handle, timeout, val) == -1)
     return -1;
  else
    {
      ssize_t bytes_written = ACE_OS::writev (handle, iov, iovcnt);
      ACE::leave_send_timedwait (handle, timeout, val);
      return bytes_written;
    }
#endif /* ACE_HAS_WRITEV_TIMEDWAIT */
}

// Format buffer into printable format.  This is useful for debugging.
// Portions taken from mdump by J.P. Knight (J.P.Knight@lut.ac.uk)
// Modifications by Todd Montgomery.

int
ACE::format_hexdump (const char *buffer, int size, char *obuf, int obuf_sz)
{
  ACE_TRACE ("ACE::format_hexdump");

  u_char c;
  char textver[16 + 1];

  int maxlen = (obuf_sz / 68) * 16;

  if (size > maxlen)
    size = maxlen;

  int i;

  for (i = 0; i < (size >> 4); i++)
    {
      int j;

      for (j = 0 ; j < 16; j++)
        {
          c = buffer[(i << 4) + j];
          ACE_OS::sprintf (obuf, "%02x ", c);
          obuf += 3;
          if (j == 7)
            {
              ACE_OS::sprintf (obuf, " ");
              obuf++;
            }
          textver[j] = (c < 0x20 || c > 0x7e) ? '.' : c;
        }

      textver[j] = 0;

      ACE_OS::sprintf (obuf, "  %s\n", textver);

      while (*obuf != '\0')
        obuf++;
    }

  if (size % 16)
    {
      for (i = 0 ; i < size % 16; i++)
        {
          c = buffer[size - size % 16 + i];
          ACE_OS::sprintf (obuf,"%02x ",c);
          obuf += 3;
          if (i == 7)
            {
              ACE_OS::sprintf (obuf, " ");
              obuf++;
            }
          textver[i] = (c < 0x20 || c > 0x7e) ? '.' : c;
        }

      for (i = size % 16; i < 16; i++)
        {
          ACE_OS::sprintf (obuf, "   ");
          obuf += 3;
          textver[i] = ' ';
        }

      textver[i] = 0;
      ACE_OS::sprintf (obuf, "  %s\n", textver);
    }
  return size;
}

// Returns the current timestamp in the form
// "hour:minute:second:microsecond."  The month, day, and year are
// also stored in the beginning of the date_and_time array.  Returns 0
// if unsuccessful, else returns pointer to beginning of the "time"
// portion of <day_and_time>.

char *
ACE::timestamp (char date_and_time[], int date_and_timelen)
{
  //ACE_TRACE ("ACE::timestamp");

  if (date_and_timelen < 35)
    {
      errno = EINVAL;
      return 0;
    }

#if defined (WIN32)
  // @@ Jesper, I think Win32 supports all the UNIX versions below.
  // Therefore, we can probably remove this WIN32 ifdef altogether.
  SYSTEMTIME local;
  ::GetLocalTime (&local);

  ACE_OS::sprintf (date_and_time, "%02d/%02d/%04d %02d.%02d.%02d.%06d",
                   (int) local.wMonth, // new, also the %02d in sprintf
                   (int) local.wDay,   // new, also the %02d in sprintf
                   (int) local.wYear,  // new, also the %02d in sprintf
                   (int) local.wHour,
                   (int) local.wMinute,
                   (int) local.wSecond,
                   (int) local.wMilliseconds * 1000);
#else  /* UNIX */
  char timebuf[26]; // This magic number is based on the ctime(3c) man page.
  ACE_Time_Value cur_time = ACE_OS::gettimeofday ();
  time_t secs = cur_time.sec ();
  ACE_OS::ctime_r (&secs, timebuf, sizeof timebuf);
  ACE_OS::strncpy (date_and_time, timebuf, date_and_timelen);
  ACE_OS::sprintf (&date_and_time[19], ".%06d", cur_time.usec ());
#endif /* WIN32 */
  date_and_time[26] = '\0';
  return &date_and_time[11];
}

// This function rounds the request to a multiple of the page size.

size_t
ACE::round_to_pagesize (off_t len)
{
  ACE_TRACE ("ACE::round_to_pagesize");

  if (ACE::pagesize_ == 0)
    {
#if defined (ACE_WIN32)
      SYSTEM_INFO sys_info;
      ::GetSystemInfo (&sys_info);
      ACE::pagesize_ = (size_t) sys_info.dwPageSize;
#elif defined (_SC_PAGESIZE)
      ACE::pagesize_ = (size_t) ::sysconf (_SC_PAGESIZE);
#elif defined (ACE_HAS_GETPAGESIZE)
      ACE::pagesize_ = (size_t) ::getpagesize ();
#else
      // Use the default set in config.h
      ACE::pagesize_ = ACE_PAGE_SIZE;
#endif /* ACE_WIN32 */
    }

  return (len + (ACE::pagesize_ - 1)) & ~(ACE::pagesize_ - 1);
}

ACE_HANDLE
ACE::handle_timed_complete (ACE_HANDLE h,
                            ACE_Time_Value *timeout,
                            int is_tli)
{
  ACE_TRACE ("ACE::handle_timed_complete");
  ACE_Handle_Set rd_handles;
  ACE_Handle_Set wr_handles;
  int need_to_check;

#if defined (ACE_WIN32)
  ACE_Handle_Set ex_handles;
  ex_handles.set_bit (h);
#endif /* ACE_WIN32 */
  rd_handles.set_bit (h);
  wr_handles.set_bit (h);

#if defined (ACE_WIN32)
  int n = ACE_OS::select (int (h) + 1,
                          rd_handles,
                          wr_handles,
                          ex_handles,
                          timeout);
#else
  int n = ACE_OS::select (int (h) + 1,
                          rd_handles,
                          wr_handles,
                          0, timeout);
#endif /* ACE_WIN32 */

  // If we failed to connect within the time period allocated by the
  // caller, then we fail (e.g., the remote host might have been too
  // busy to accept our call).
  if (n <= 0)
    {
      if (n == 0)
        errno = ETIME;
      return ACE_INVALID_HANDLE;
    }

  // Check if the handle is ready for reading and the handle is *not*
  // ready for writing, which may indicate a problem.  But we need to
  // make sure...
#if defined (ACE_WIN32)
  need_to_check = (rd_handles.is_set (h) || ex_handles.is_set (h));
#elif defined (VXWORKS)
  ACE_UNUSED_ARG (is_tli);

  // Force the check on VxWorks.  The read handle for "h" is not set,
  // so "need_to_check" is false at this point.  The write handle is
  // set, for what it's worth.
  need_to_check = 1;
#else
  if (is_tli)
    need_to_check = (rd_handles.is_set (h) && !wr_handles.is_set (h));
  else
    need_to_check = rd_handles.is_set (h);
#endif /* ACE_WIN32 */

  if (need_to_check)
    {
      char dummy;

      // The following recv() won't block provided that the
      // ACE_NONBLOCK flag has not been turned off .
      n = ACE::recv (h, &dummy, 1, MSG_PEEK);

      if (n <= 0)
        {
          if (n == 0)
            errno = ECONNREFUSED;
          return ACE_INVALID_HANDLE;
        }
    }

  // 1. The HANDLE is ready for writing or 2. recv() returned that
  // there are data to be read, which indicates the connection was
  // successfully established.
  return h;
}

ACE_HANDLE
ACE::handle_timed_open (ACE_Time_Value *timeout,
                        LPCTSTR name,
                        int flags,
                        int perms)
{
  ACE_TRACE ("ACE::handle_timed_open");

  if (timeout != 0)
    {
      // Open the named pipe or file using non-blocking mode...
      ACE_HANDLE handle = ACE_OS::open (name,
                                        flags | ACE_NONBLOCK,
                                        perms);
      if (handle == ACE_INVALID_HANDLE
          && (errno == EWOULDBLOCK
              && (timeout->sec () > 0 || timeout->usec () > 0)))
        // This expression checks if we were polling.
        errno = ETIMEDOUT;

      return handle;
    }
  else
    return ACE_OS::open (name, flags, perms);
}

// Wait up to <timeout> amount of time to accept a connection.

int
ACE::handle_timed_accept (ACE_HANDLE listener,
                          ACE_Time_Value *timeout,
                          int restart)
{
  ACE_TRACE ("ACE::handle_timed_accept");
  // Make sure we don't bomb out on erroneous values.
  if (listener == ACE_INVALID_HANDLE)
    return -1;

  // Use the select() implementation rather than poll().
  ACE_Handle_Set rd_handle;
  rd_handle.set_bit (listener);

  // We need a loop here if <restart> is enabled.

  for (;;)
    {
      switch (ACE_OS::select (int (listener) + 1,
                              rd_handle, 0, 0,
                              timeout))
        {
        case -1:
          if (errno == EINTR && restart)
            continue;
          else
            return -1;
          /* NOTREACHED */
        case 0:
          if (timeout != 0 && timeout->sec() == 0 && timeout->usec() == 0)
            errno = EWOULDBLOCK;
          else
            errno = ETIMEDOUT;
          return -1;
          /* NOTREACHED */
        case 1:
          return 0;
          /* NOTREACHED */
        default:
          errno = EINVAL;
          return -1;
          /* NOTREACHED */
        }
    }
  /* NOTREACHED */
  return 0;
}

// Bind socket to an unused port.

int
ACE::bind_port (ACE_HANDLE handle)
{
  ACE_TRACE ("ACE::bind_port");
  sockaddr_in sin;
  // This should be a constant, so I hope they never change the number
  // of bits in a port number!
  static u_short upper_limit = ACE_MAX_DEFAULT_PORT;
  int lower_limit = IPPORT_RESERVED;
  int round_trip = upper_limit;

  ACE_OS::memset ((void *) &sin, 0, sizeof sin);
  sin.sin_family = AF_INET;
#if defined (ACE_HAS_SIN_LEN)
  sin.sin_len = sizeof sin;
#endif /* ACE_HAS_SIN_LEN */
  sin.sin_addr.s_addr = INADDR_ANY;

  for (;;)
    {
      sin.sin_port = htons (upper_limit);

      if (ACE_OS::bind (handle, (sockaddr *) &sin, sizeof sin) >= 0)
        {
#if defined (ACE_WIN32)
          upper_limit--;
#endif
          return 0;
        }
      else if (errno != EADDRINUSE)
        return -1;
      else
        {
          upper_limit--;

          // Wrap back around when we reach the bottom.
          if (upper_limit <= lower_limit)
            upper_limit = ACE_MAX_DEFAULT_PORT;

          // See if we have already gone around once!
          if (upper_limit == round_trip)
            {
              errno = EAGAIN;
              return -1;
            }
        }
    }
}

// Make the current process a UNIX daemon.  This is based on Stevens
// code from APUE.

int
ACE::daemonize (const char pathname[])
{
  ACE_TRACE ("ACE::daemonize");
#if !defined (ACE_LACKS_FORK)
  pid_t pid = ACE_OS::fork ();

  if (pid == -1)
    return -1;
  else if (pid != 0)
    ACE_OS::exit (0); // Parent exits.

  // 1st child continues.
  ACE_OS::setsid (); // Become session leader.

  ACE_OS::signal (SIGHUP, SIG_IGN);

  pid = ACE_OS::fork ();

  if (pid != 0)
    ACE_OS::exit (0); // First child terminates.

  // Second child continues.

  ACE_OS::chdir (pathname); // change working directory.

  ACE_OS::umask (0); // clear our file mode creation mask.

  // Close down the files.
  for (int i = ACE::max_handles () - 1; i >= 0; i--)
    ACE_OS::close (i);
  return 0;
#else
  ACE_UNUSED_ARG (pathname);
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_WIN32 */
}

int
ACE::max_handles (void)
{
  ACE_TRACE ("ACE::set_handle_limit");
#if defined (_SC_OPEN_MAX)
  return ACE_OS::sysconf (_SC_OPEN_MAX);
#elif defined (RLIMIT_NOFILE)
  rlimit rl;
  ACE_OS::getrlimit (RLIMIT_NOFILE, &rl);
  return rl.rlim_cur;
#else
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_WIN32 */
}

// Set the number of currently open handles in the process.
//
// If NEW_LIMIT == -1 set the limit to the maximum allowable.
// Otherwise, set it to be the value of NEW_LIMIT.

int
ACE::set_handle_limit (int new_limit)
{
  ACE_TRACE ("ACE::set_handle_limit");
#if defined (RLIMIT_NOFILE)
  struct rlimit rl;

  int max_handles = ACE::max_handles ();

  if (new_limit < 0 || new_limit > max_handles)
    rl.rlim_cur = max_handles;
  else
    rl.rlim_cur = new_limit;

  return ACE_OS::setrlimit (RLIMIT_NOFILE, &rl);
#else
  new_limit = new_limit;
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_WIN32 */
}

// Flags are file status flags to turn on.

int
ACE::set_flags (ACE_HANDLE handle, int flags)
{
  ACE_TRACE ("ACE::set_flags");
#if defined (ACE_WIN32) || defined (VXWORKS)
  switch (flags)
    {
    case ACE_NONBLOCK:
      // nonblocking argument (1)
      // blocking:            (0)
      {
        u_long nonblock = 1;
        return ACE_OS::ioctl (handle, FIONBIO, &nonblock);
      }
    default:
      ACE_NOTSUP_RETURN (-1);
    }
#else
  int val = ACE_OS::fcntl (handle, F_GETFL, 0);

  if (val == -1)
    return -1;

  // Turn on flags.
  ACE_SET_BITS (val, flags);

  if (ACE_OS::fcntl (handle, F_SETFL, val) == -1)
    return -1;
  else
    return 0;
#endif /* ACE_WIN32 && VXWORKS */
}

// Flags are the file status flags to turn off.

int
ACE::clr_flags (ACE_HANDLE handle, int flags)
{
  ACE_TRACE ("ACE::clr_flags");

#if defined (ACE_WIN32) || defined (VXWORKS)
  switch (flags)
    {
    case ACE_NONBLOCK:
      // nonblocking argument (1)
      // blocking:            (0)
      {
        u_long nonblock = 0;
        return ACE_OS::ioctl (handle, FIONBIO, &nonblock);
      }
    default:
      ACE_NOTSUP_RETURN (-1);
    }
#else
  int val = ACE_OS::fcntl (handle, F_GETFL, 0);

  if (val == -1)
    return -1;

  // Turn flags off.
  ACE_CLR_BITS (val, flags);

  if (ACE_OS::fcntl (handle, F_SETFL, val) == -1)
    return -1;
  else
    return 0;
#endif /* ACE_WIN32 || VXWORKS */
}

int
ACE::map_errno (int error)
{
  switch (error)
    {
#if defined (ACE_WIN32)
    case WSAEWOULDBLOCK:
      return EAGAIN; // Same as UNIX errno EWOULDBLOCK.
#endif /* ACE_WIN32 */
    }

  return error;
}

ssize_t
ACE::send (ACE_HANDLE handle,
           const void *buf,
           size_t n,
           int flags,
           const ACE_Time_Value *timeout)
{
#if defined (ACE_HAS_SEND_TIMEDWAIT)
  if (timeout == 0)
    return ACE::send (handle, buf, n, flags);
  else 
    {
      ACE_Time_Value copy = *timeout;
      copy += ACE_OS::gettimeofday();
      timespec_t ts = copy;
      return ::send_timedwait (handle, buf, n, flags, &ts);
    }
#else
  int val;

  if (ACE::enter_send_timedwait (handle, timeout, val) == -1)
    return -1;
  else 
    {
      ssize_t bytes_written = ACE::send (handle, buf, n, flags);
      ACE::leave_send_timedwait (handle, timeout, val);
      return bytes_written;
    }
#endif /* ACE_HAS_SEND_TIMEDWAIT */
}

ssize_t
ACE::send (ACE_HANDLE handle,
           const void *buf,
           size_t n,
           const ACE_Time_Value *timeout)
{
  // ACE_TRACE ("ACE_OS::write");
#if defined (ACE_HAS_WRITE_TIMEDWAIT)
  if (timeout == 0)
    return ACE::send (handle, buf, n);
  else
    {
      ACE_Time_Value copy = *timeout;
      copy += ACE_OS::gettimeofday ();
      timespec_t ts = copy;
      return ::write_timedwait (handle, buf, n, &ts);
    }
#else
  int val;

  if (ACE::enter_send_timedwait (handle, timeout, val) == -1)
    return -1;
  else 
    {
      ssize_t bytes_written = ACE::send (handle, buf, n);
      ACE::leave_send_timedwait (handle, timeout, val);
      return bytes_written;
    }
#endif /* ACE_HAS_WRITE_TIMEDWAIT */
}

ssize_t
ACE::send_n (ACE_HANDLE handle,
             const void *buf,
             size_t n,
             int flags,
             const ACE_Time_Value *timeout)
{
  // Total number of bytes written.
  size_t bytes_written;

  // Actual number of bytes written in each <send> attempt.
  ssize_t i = 0;

  for (bytes_written = 0; 
       bytes_written < n;
       bytes_written += i)
    {
      i = ACE::send (handle, (char *) buf + bytes_written,
                     n - bytes_written, flags, timeout);
      if (i == -1)
        break;
    }

  return bytes_written;
}

ssize_t
ACE::recvfrom (ACE_HANDLE handle,
               char *buf,
               int len, 
               int flags,
               struct sockaddr *addr,
               int *addrlen,
               const ACE_Time_Value *timeout)
{
  // ACE_TRACE ("ACE::recvfrom");
#if defined (ACE_HAS_RECVFROM_TIMEDWAIT)
  if (timeout == 0)
     return ACE_OS::recvfrom (handle, buf, len, flags, addr, addrlen);
  else
    {
      ACE_Time_Value copy = *timeout;
      copy += ACE_OS::gettimeofday ();
      timespec_t ts = copy;
      return ::recvfrom_timedwait (handle, buf, len, flags, addr, addrlen, &ts);
    }
#else
  int val;
  if (ACE::enter_recv_timedwait (handle, timeout, val) == -1)
     return -1;
  else 
    {
      int bytes_read = ACE_OS::recvfrom (handle, buf, len, flags, addr, addrlen);
      ACE::leave_recv_timedwait (handle, timeout, val);
      return bytes_read;
    }
#endif /* ACE_HAS_RECVFROM_TIMEDWAIT */
}

ssize_t
ACE::recvmsg (ACE_HANDLE handle,
              struct msghdr *msg,
              int flags,
              const ACE_Time_Value *timeout)
{
  // ACE_TRACE ("ACE::recvmsg");
#if defined (ACE_HAS_RECVMSG_TIMEDWAIT)
  if (timeout == 0)
     return ACE_OS::recvmsg (handle, msg, flags);
  else 
    {
      ACE_Time_Value copy = *timeout;
      copy += ACE_OS::gettimeofday ();
      timespec_t ts = copy;
      return ::recvmsg_timedwait (handle, msg, flags, &ts);
    }
#else
  int val;
  if (ACE::enter_recv_timedwait (handle, timeout, val) == -1)
    return -1;
  else 
    {
      int bytes_read = ACE_OS::recvmsg (handle, msg, flags);
      ACE::leave_recv_timedwait (handle, timeout, val);
      return bytes_read;
    }
#endif /* ACE_HAS_RECVMSG_TIMEDWAIT */
}

ssize_t
ACE::recv (ACE_HANDLE handle,
           void *buf,
           size_t n,
           const ACE_Time_Value *timeout)
{
  // ACE_TRACE ("ACE::read");
#if defined (ACE_HAS_READ_TIMEDWAIT)
  if (timeout == 0)
     return ACE::recv (handle, buf, n);
  else 
    {
      ACE_Time_Value copy = *timeout;
      copy += ACE_OS::gettimeofday ();
      timespec_t ts = copy;
      return ::read_timedwait (handle, buf, n, &ts);
    }
#else
  int val;

  if (ACE::enter_recv_timedwait (handle, timeout, val) == -1)
    return -1;
  else
    {
      ssize_t bytes_read = ACE::recv (handle, buf, n);
      ACE::leave_recv_timedwait (handle, timeout, val);
      return bytes_read;
    }
#endif /* ACE_HAS_READ_TIMEDWAIT */
}

ssize_t 
ACE::recv (ACE_HANDLE handle,
           void *buf,
           size_t len,
           int flags,
           const ACE_Time_Value *timeout)
{
  // ACE_TRACE ("ACE::recv");
#if defined (ACE_HAS_RECV_TIMEDWAIT)
  if (timeout == 0)
     return ACE::recv (handle, buf, len, flags);
  else
    {
      ACE_Time_Value copy = *timeout;
      copy += ACE_OS::gettimeofday ();
      timespec_t ts = copy;
      return ::recv_timedwait (handle, buf, len, flags, &ts);
    }
#else
  int val;
  if (ACE::enter_recv_timedwait (handle, timeout, val)==-1)
     return -1;
  else
    {
      ssize_t bytes_recv = ACE::recv (handle, buf, len, flags);
      ACE::leave_recv_timedwait (handle, timeout, val);
      return bytes_recv;
    }
#endif /* ACE_HAS_RECV_TIMEDWAIT */
}

ssize_t
ACE::recv_n (ACE_HANDLE handle,
             void *buf,
             size_t n,
             int flags,
             const ACE_Time_Value *timeout)
{
  size_t bytes_received;

  // Actual number of bytes read in each attempt.
  ssize_t i = 0;

  for (bytes_received = 0; bytes_received < n; bytes_received += i)
    {
      i = ACE::recv (handle, (char *) buf + bytes_received,
                     n - bytes_received, flags, timeout);

      if (i == -1 || i == 0)
        break;
    }

  return bytes_received;
}

u_long
ACE::is_prime (const u_long n,
               const u_long min_factor,
               const u_long max_factor)
{
  if (n > 3)
    {
      for (u_long factor = min_factor; factor <= max_factor; ++factor)
        if (n / factor * factor == n)
          return factor;

      return 0;
    }
  else
    return 0;
}

const char *
ACE::sock_error (int error)
{
#if defined (ACE_WIN32)
  switch (error)
    {
    case WSAVERNOTSUPPORTED:
      return "version of WinSock not supported";
      /* NOTREACHED */
    case WSASYSNOTREADY:
      return "WinSock not present or not responding";
      /* NOTREACHED */
    case WSAEINVAL:
      return "app version not supported by DLL";
      /* NOTREACHED */
    case WSAHOST_NOT_FOUND:
      return "Authoritive: Host not found";
      /* NOTREACHED */
    case WSATRY_AGAIN:
      return "Non-authoritive: host not found or server failure";
      /* NOTREACHED */
    case WSANO_RECOVERY:
      return "Non-recoverable: refused or not implemented";
      /* NOTREACHED */
    case WSANO_DATA:
      return "Valid name, no data record for type";
      /* NOTREACHED */
      /*
        case WSANO_ADDRESS:
        return "Valid name, no MX record";
          */
    case WSANOTINITIALISED:
      return "WSA Startup not initialized";
      /* NOTREACHED */
    case WSAENETDOWN:
      return "Network subsystem failed";
      /* NOTREACHED */
    case WSAEINPROGRESS:
      return "Blocking operation in progress";
      /* NOTREACHED */
    case WSAEINTR:
      return "Blocking call cancelled";
      /* NOTREACHED */
    case WSAEAFNOSUPPORT:
      return "address family not supported";
      /* NOTREACHED */
    case WSAEMFILE:
      return "no file descriptors available";
      /* NOTREACHED */
    case WSAENOBUFS:
      return "no buffer space available";
      /* NOTREACHED */
    case WSAEPROTONOSUPPORT:
      return "specified protocol not supported";
      /* NOTREACHED */
    case WSAEPROTOTYPE:
      return "protocol wrong type for this socket";
      /* NOTREACHED */
    case WSAESOCKTNOSUPPORT:
      return "socket type not supported for address family";
      /* NOTREACHED */
    case WSAENOTSOCK:
      return "descriptor is not a socket";
      /* NOTREACHED */
    case WSAEWOULDBLOCK:
      return "socket marked as non-blocking and SO_LINGER set not 0";
      /* NOTREACHED */
    case WSAEADDRINUSE:
      return "address already in use";
      /* NOTREACHED */
    case WSAECONNABORTED:
      return "connection aborted";
      /* NOTREACHED */
    case WSAECONNRESET:
      return "connection reset";
      /* NOTREACHED */
    case WSAENOTCONN:
      return "not connected";
      /* NOTREACHED */
    case WSAETIMEDOUT:
      return "connection timed out";
      /* NOTREACHED */
    case WSAECONNREFUSED:
      return "connection refused";
      /* NOTREACHED */
    case WSAEHOSTDOWN:
      return "host down";
      /* NOTREACHED */
    case WSAEHOSTUNREACH:
      return "host unreachable";
      /* NOTREACHED */
    case WSAEADDRNOTAVAIL:
      return "address not available";
      /* NOTREACHED */
    default:
      return "unknown error";
      /* NOTREACHED */
    }
#else
  ACE_UNUSED_ARG (error);
  ACE_NOTSUP_RETURN (0);
#endif /* ACE_WIN32 */
}

int
ACE::get_bcast_addr (ACE_UINT32 &bcast_addr,
                     const char *host_name,
                     ACE_UINT32 host_addr,
                     ACE_HANDLE handle)
{
  ACE_TRACE ("ACE::get_bcast_addr");

#if !defined(ACE_WIN32)
  ACE_HANDLE s = handle;

  if (s == ACE_INVALID_HANDLE)
    s = ACE_OS::socket (AF_INET, SOCK_STREAM, 0);

  if (s == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ACE_OS::socket"), -1);

  struct ifconf ifc;
  char buf[BUFSIZ];

  ifc.ifc_len = sizeof buf;
  ifc.ifc_buf = buf;

  // Get interface structure and initialize the addresses using UNIX
  // techniques
  if (ACE_OS::ioctl (s, SIOCGIFCONF, (char *) &ifc) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n",
                      "ACE::get_bcast_addr: ioctl (get interface configuration)"),
                      -1);

  struct ifreq *ifr = ifc.ifc_req;

  struct sockaddr_in ip_addr;

  // Get host ip address if necessary.
  if (host_name)
    {
      hostent *hp = ACE_OS::gethostbyname (host_name);

      if (hp == 0)
        return -1;
      else
        ACE_OS::memcpy ((char *) &ip_addr.sin_addr.s_addr,
                        (char *) hp->h_addr,
                        hp->h_length);
    }
  else
    {
      ACE_OS::memset ((void *) &ip_addr, 0, sizeof ip_addr);
      ACE_OS::memcpy ((void *) &ip_addr.sin_addr,
                      (void*) &host_addr,
                      sizeof ip_addr.sin_addr);
    }

  for (int n = ifc.ifc_len / sizeof (struct ifreq);
       n > 0;
       n--, ifr++)
    {
      struct sockaddr_in if_addr;

      // Compare host ip address with interface ip address.
      ACE_OS::memcpy (&if_addr, &ifr->ifr_addr, sizeof if_addr);

      if (ip_addr.sin_addr.s_addr != if_addr.sin_addr.s_addr)
        continue;

      if (ifr->ifr_addr.sa_family != AF_INET)
        {
          ACE_ERROR ((LM_ERROR, "%p\n",
                      "ACE::get_bcast_addr: Not AF_INET"));
          continue;
        }

      struct ifreq flags = *ifr;
      struct ifreq if_req = *ifr;

      if (ACE_OS::ioctl (s, SIOCGIFFLAGS, (char *) &flags) == -1)
        {
          ACE_ERROR ((LM_ERROR, "%p\n",
                     "ACE::get_bcast_addr: ioctl (get interface flags)"));
          continue;
        }

      if (ACE_BIT_ENABLED (flags.ifr_flags, IFF_UP) == 0)
        {
          ACE_ERROR ((LM_ERROR, "%p\n",
                     "ACE::get_bcast_addr: Network interface is not up"));
          continue;
        }

      if (ACE_BIT_ENABLED (flags.ifr_flags, IFF_LOOPBACK))
        continue;

      if (ACE_BIT_ENABLED (flags.ifr_flags, IFF_BROADCAST))
        {
          if (ACE_OS::ioctl (s, SIOCGIFBRDADDR, (char *) &if_req) == -1)
            ACE_ERROR ((LM_ERROR, "%p\n",
                       "ACE::get_bcast_addr: ioctl (get broadaddr)"));
          else
            {
              ACE_OS::memcpy ((struct sockaddr_in *) &ip_addr,
                              (struct sockaddr_in *) &if_req.ifr_broadaddr,
                              sizeof if_req.ifr_broadaddr);

              ACE_OS::memcpy ((void *) &host_addr,
                              (void *) &ip_addr.sin_addr,
                              sizeof host_addr);

              if (handle == ACE_INVALID_HANDLE)
                ACE_OS::close (s);
              bcast_addr = host_addr;
              return 0;
            }
        }
      else
        ACE_ERROR ((LM_ERROR, "%p\n",
                   "ACE::get_bcast_addr: Broadcast is not enable for this interface."));

      if (handle == ACE_INVALID_HANDLE)
        ACE_OS::close (s);
      bcast_addr = host_addr;
      return 0;
    }

  return 0;
#else
  ACE_UNUSED_ARG (handle);
  ACE_UNUSED_ARG (host_addr);
  ACE_UNUSED_ARG (host_name);
  bcast_addr = (ACE_UINT32 (INADDR_BROADCAST));
  return 0;
#endif /* !ACE_WIN32 */
}

// Helper routine for get_ip_interfaces, differs by UNIX platform so
// put into own subroutine.  perform some ioctls to retrieve ifconf
// list of ifreq structs.

int
ACE::count_interfaces (ACE_HANDLE handle,
                       size_t &how_many)
{
#if defined (sparc) && defined (SIOCGIFNUM)
  if (ACE_OS::ioctl (handle, SIOCGIFNUM, (caddr_t) &how_many) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "ACE::get_ip_interfaces:ioctl - SIOCGIFNUM failed"), -1);
   return 0;
#elif defined (__unix)  /* Note: DEC CXX doesn't define "unix" */
// BSD compatible OS: HP UX, AIX, SunOS 4.x perform some ioctls to
// retrieve ifconf list of ifreq structs no SIOCGIFNUM on SunOS 4.x,
// so use guess and scan algorithm

  const int MAX_IF = 50; // probably hard to put this many ifs in a unix box..
  int num_ifs = MAX_IF; // HACK - set to an unreasonable number
  struct ifconf ifcfg;
  struct ifreq *p_ifs = NULL;
  size_t ifreq_size = 0;

  ifreq_size = num_ifs * sizeof (struct ifreq);
  p_ifs = (struct ifreq *) ACE_OS::malloc (ifreq_size);

  if (!p_ifs)
    {
      errno = ENOMEM;
      return -1;
    }

  ACE_OS::memset (p_ifs, 0, ifreq_size);
  ACE_OS::memset (&ifcfg, 0, sizeof (struct ifconf));
  ifcfg.ifc_req = p_ifs;
  ifcfg.ifc_len = ifreq_size;

  if (ACE_OS::ioctl (handle, SIOCGIFCONF, (caddr_t) &ifcfg) == -1)
    {
      ACE_OS::free (ifcfg.ifc_req);
      ACE_ERROR_RETURN ((LM_ERROR, "count_interfaces:ioctl - SIOCGIFCONF failed"), -1);
    }

  int if_count = 0, i ;

  // get if address out of ifreq buffers.
  // ioctl puts a blank-named interface to mark the end of the
  // returned interfaces.
  for (i = 0; i < num_ifs; p_ifs++, i++)
    {
      if (p_ifs->ifr_name[0] == '\0')
          break;
      if_count++;
    }

  ACE_OS::free (ifcfg.ifc_req);
  how_many = if_count;
  return 0;
#else
   ACE_UNUSED_ARG (handle);
   ACE_UNUSED_ARG (how_many);
   ACE_NOTSUP_RETURN (-1);; // no implmentation
#endif /* __SVR4 */
}

// Routine to return a descriptor from which ioctl() requests can be
// made.

ACE_HANDLE
ACE::get_handle (void)
{
// Solaris 2.x
  ACE_HANDLE handle = ACE_INVALID_HANDLE;
#if defined (sparc)
  handle = ACE_OS::open ("/dev/udp", O_RDONLY);
#elif defined (__unix)  /* Note: DEC CXX doesn't define "unix" */
// BSD compatible OS: HP UX, AIX, SunOS 4.x
  handle = ACE_OS::socket (PF_INET, SOCK_DGRAM, 0);
#endif /* __SVR4 */
  return handle;
}

#if defined (ACE_WIN32)
// Return value in buffer for a key/name pair from the Windows
// Registry up to buf_len size.

static int
get_reg_value (const TCHAR *key,
               const TCHAR *name,
               TCHAR *buffer,
               DWORD &buf_len)
{
  HKEY hk;
  DWORD buf_type;
  LONG rc = ::RegOpenKeyEx (HKEY_LOCAL_MACHINE, key, 0, KEY_READ, &hk);

  // 1. open key that defines the interfaces used for TCP/IP?
  if (rc != ERROR_SUCCESS)
    // print_error_string(TEXT("RegOpenKeyEx"), rc);
    return -1;

  rc = ::RegQueryValueEx (hk, name, 0, &buf_type,
                          (u_char *) buffer, &buf_len);

  if (rc != ERROR_SUCCESS)
    {
      // print_error_string(TEXT("RegEnumKeyEx"), rc);
      RegCloseKey (hk);
      return -2;
    }

  ::RegCloseKey (hk);
  return 0;
}
#endif /* ACE_WIN32 */

// return an array of all configured IP interfaces on this host, count
// rc = 0 on success (count == number of interfaces else -1 caller is
// responsible for calling delete [] on parray

int
ACE::get_ip_interfaces (size_t &count,
                        ACE_INET_Addr *&addrs)
{
  ACE_TRACE ("ACE::get_ip_interfaces");

#if defined (ACE_WIN32)
  const TCHAR *SVCS_KEY1 =  __TEXT ("SYSTEM\\CurrentControlSet\\Services\\");
  const TCHAR *LINKAGE_KEY1 =
    __TEXT ("SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Linkage");
  const TCHAR *TCP_PARAM_SUBKEY = __TEXT ("\\Parameters\\Tcpip");
  const TCHAR *BIND_NAME_ID =  __TEXT ("Bind");
  const TCHAR *IPADDR_NAME_ID = __TEXT ("IPAddress");
  const TCHAR *INVALID_TCPIP_DEVICE_ADDR = __TEXT ("0.0.0.0");
  const int MAX_STRING_SZ = 4096;

  TCHAR raw_buffer[ACE_MAX_FULLY_QUALIFIED_NAME_LEN + 1];
  DWORD raw_buflen = ACE_MAX_FULLY_QUALIFIED_NAME_LEN + 1;
  TCHAR buffer[ACE_MAX_FULLY_QUALIFIED_NAME_LEN + 1];
  DWORD buf_len = ACE_MAX_FULLY_QUALIFIED_NAME_LEN + 1;

  if (::get_reg_value (LINKAGE_KEY1,
                       BIND_NAME_ID,
                       raw_buffer,
                       raw_buflen))
    return -1;
  // return buffer contains NULL delimited strings

  ACE_Tokenizer dev_names (raw_buffer);
  dev_names.delimiter (__TEXT('\0'));
  int n_interfaces = 0;

  // Count the number of interfaces
  while (dev_names.next () != NULL)
    n_interfaces ++;

  // case 1. no interfaces present, empty string? OS version change?
  if (n_interfaces == 0)
    return 0;

  ACE_NEW_RETURN (addrs, ACE_INET_Addr[n_interfaces], -2);

  count = 0;
  for (int i = 0; i < n_interfaces; i++)
    {

      // a. construct name to access IPAddress for this interface
      ACE_TEXT_STRING ifdevkey (SVCS_KEY1);
      ACE_TEXT_STRING the_dev = dev_names.next ();

      // chop off the "\Device" and keep last name.
      if (the_dev.length() < 8)
        return -3;              // Something's wrong
      else
        {
          the_dev = the_dev.substring (8);  // rest of string from offset 8
          ifdevkey += the_dev;
          ifdevkey += TCP_PARAM_SUBKEY;

          // b. extract value
          if (get_reg_value (ifdevkey.rep(), IPADDR_NAME_ID, buffer, buf_len))
            return -4;

          if (ACE_OS::strcmp (buffer, INVALID_TCPIP_DEVICE_ADDR) == 0)
            continue;           // Don't count this device

          // c. store in hostinfo object array and up the counter
          addrs[count++] = ACE_INET_Addr ((u_short) 0,
                                          ACE_MULTIBYTE_STRING (buffer));
        }
    }
  return 0;
#elif defined(__unix)
  //  COMMON (SVR4 and BSD) UNIX CODE

  size_t num_ifs;

  ACE_HANDLE handle = get_handle();     // call specific routine as necessary

  if (handle == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ACE::get_ip_interfaces:open"), -1);

  if (ACE::count_interfaces (handle, num_ifs))
    {
      ACE_OS::close (handle);
      return -1;
    }

  // ioctl likes to have an extra ifreq structure to mark the end of what it
  // returned, so increase the num_ifs by one.
  ++num_ifs;

  struct ifreq *ifs;
  ACE_NEW_RETURN (ifs, struct ifreq[num_ifs], -1);
  ACE_OS::memset (ifs, 0, num_ifs * sizeof (struct ifreq));
  ACE_Auto_Array_Ptr<struct ifreq> p_ifs (ifs);

  if (p_ifs.get() == 0)
    {
      ACE_OS::close (handle);
      errno = ENOMEM;
      return -1;
    }

  struct ifconf ifcfg;
  ACE_OS::memset (&ifcfg, 0, sizeof (struct ifconf));
  ifcfg.ifc_req = p_ifs.get ();
  ifcfg.ifc_len = num_ifs * sizeof (struct ifreq);

  if (ACE_OS::ioctl (handle, SIOCGIFCONF, (caddr_t) &ifcfg) == -1)
    {
      ACE_OS::close (handle);
      ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "is_address_local:ioctl - SIOCGIFCONF failed"), -1);
    }

  ACE_OS::close (handle);

  // ------------ now create and initialize output array -------------

  ACE_NEW_RETURN (addrs, ACE_INET_Addr[num_ifs], -1); // caller must free

  struct ifreq *pcur = p_ifs.get ();
  // Pull the address out of each INET interface.  Not every interface is
  // for IP, so be careful to count properly.  When setting the INET_Addr,
  // note that the 3rd arg (0) says to leave the byte order (already in net
  // byte order from the interface structure) as is.
  count = 0;
  for (size_t i = 0; i < num_ifs; pcur++, i++)
    {
      if (pcur->ifr_addr.sa_family == AF_INET)
        {
          struct sockaddr_in *_addr_ = (struct sockaddr_in *)&(pcur->ifr_addr);
          addrs[count].set ((u_short)0, _addr_->sin_addr.s_addr, 0);
          count++;
        }
    }
  return 0;
#else
  ACE_UNUSED_ARG (count);
  ACE_UNUSED_ARG (addrs);
  ACE_NOTSUP_RETURN (-1);;                      // no implementation
#endif /* ACE_WIN32 */
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Auto_Array_Ptr<struct ifreq>;
template class ACE_Auto_Basic_Array_Ptr<struct ifreq>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Auto_Array_Ptr<struct ifreq>
#pragma instantiate ACE_Auto_Basic_Array_Ptr<struct ifreq>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
