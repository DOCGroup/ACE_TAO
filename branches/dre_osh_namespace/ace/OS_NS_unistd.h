// -*- C++ -*-

//=============================================================================
/**
 *  @file   OS_NS_unistd.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 *  @author Jesper S. M|ller<stophph@diku.dk>
 *  @author and a cast of thousands...
 *
 *  Originally in OS.h.
 */
//=============================================================================

#ifndef ACE_OS_NS_UNISTD_H
# define ACE_OS_NS_UNISTD_H

# include /**/ "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#  pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/os_unistd.h"
#include "ace/Time_Value.h"
#include "ace/os_include/os_stdio.h"

// This should go in os_unistd.h, but since we don't yet implement any code
// at that level, we put it here.  It used to be in OS.i.
#if defined (ACE_NEEDS_FTRUNCATE)
extern "C" ACE_Export int ftruncate (ACE_HANDLE handle, long len);
#endif /* ACE_NEEDS_FTRUNCATE */

namespace ACE_OS {

  int access (const char *path, int amode);

#if defined (ACE_HAS_WCHAR)
  int access (const wchar_t *path, int amode);
#endif /* ACE_HAS_WCHAR */

  unsigned int alarm (u_int secs);

  int allocation_granularity (void);

  // used by ARGV::argv_to_string() and ACE_OS::fork_exec()
  int argv_to_string (ACE_TCHAR **argv,
                      ACE_TCHAR *&buf,
                      int substitute_env_args = 1);

#if !defined (ACE_LACKS_CHDIR)
  int chdir (const char *path);

#if defined (ACE_HAS_WCHAR)
  int chdir (const wchar_t *path);
#endif /* ACE_HAS_WCHAR */
#endif /* ACE_LACKS_CHDIR */

  int close (ACE_HANDLE handle);

  ACE_HANDLE dup (ACE_HANDLE handle);

  int dup2 (ACE_HANDLE oldfd,
            ACE_HANDLE newfd);

  int execl (const char *path,
             const char *arg0, ...);

  int execle (const char *path,
              const char *arg0, ...);

  int execlp (const char *file,
              const char *arg0, ...);

  int execv (const char *path,
             char *const argv[]);

  int execve (const char *path,
              char *const argv[],
              char *const envp[]);

  int execvp (const char *file,
              char *const argv[]);

  //@{
  /// Forks and exec's a process in a manner that works on Solaris and
  /// NT.  argv[0] must be the full path name to the executable.
  pid_t fork (void);

  // not in susv3
  pid_t fork (const ACE_TCHAR *program_name);

  pid_t fork_exec (ACE_TCHAR *argv[]);

  //@}

  int fsync (ACE_HANDLE handle);

  int ftruncate (ACE_HANDLE,
                 off_t);

  ACE_TCHAR *getcwd (ACE_TCHAR *, size_t);

  gid_t getgid (void);

  int getopt (int argc,
              char *const *argv,
              const char *optstring);

  int getpagesize (void);

  pid_t getpgid (pid_t pid);

  pid_t getpid (void);

  pid_t getppid (void);

  uid_t getuid (void);

  // should call gethostname()
  int hostname (char *name,
                size_t maxnamelen);

#if defined (ACE_HAS_WCHAR)
  int hostname (wchar_t *name,
                size_t maxnamelen);
#endif /* ACE_HAS_WCHAR */

  int isatty (int handle);

#if defined (ACE_WIN32)
  int isatty (ACE_HANDLE handle);
#endif /* ACE_WIN32 */

  off_t lseek (ACE_HANDLE handle,
               off_t offset,
               int whence);

#if defined (ACE_HAS_LLSEEK) || defined (ACE_HAS_LSEEK64)
  ACE_LOFF_T llseek (ACE_HANDLE handle, ACE_LOFF_T offset, int whence);
#endif /* ACE_HAS_LLSEEK */

  /// Get the number of CPUs configured in the machine.
  long num_processors (void);

  /// Get the number of CPUs currently online.
  long num_processors_online (void);

  int pipe (ACE_HANDLE handles[]);

  ssize_t pread (ACE_HANDLE handle,
                 void *buf,
                 size_t nbyte,
                 off_t offset);

  ssize_t pwrite (ACE_HANDLE handle,
                  const void *buf,
                  size_t nbyte,
                  off_t offset);

  ssize_t read (ACE_HANDLE handle,
                void *buf,
                size_t len);

  ssize_t read (ACE_HANDLE handle,
                void *buf,
                size_t len,
                ACE_OVERLAPPED *);

  /**
   * Receive <len> bytes into <buf> from <handle> (uses the
   * <ACE_OS::read> call, which uses the <read> system call on UNIX
   * and the <ReadFile> call on Win32). If errors occur, -1 is
   * returned.  If EOF occurs, 0 is returned.  Whatever data has been
   * read will be returned to the caller through<bytes_transferred>.
   *
   */
  ssize_t read_n (ACE_HANDLE handle,
                  void *buf,
                  size_t len,
                  size_t *bytes_transferred = 0);

  int readlink (const char *path,
                char *buf,
                size_t bufsiz);

  void *sbrk (int brk);

  int setgid (gid_t);

  int setpgid (pid_t pid, pid_t pgid);

  int setregid (gid_t rgid, gid_t egid);

  int setreuid (uid_t ruid, uid_t euid);

  pid_t setsid (void);

  int setuid (uid_t);

  int sleep (u_int seconds);

  int sleep (const ACE_Time_Value &tv);

  // used by ARGV::string_to_argv
  int string_to_argv (ACE_TCHAR *buf,
                      int &argc,
                      ACE_TCHAR **&argv,
                      int substitute_env_args = 1);

  long sysconf (int);

  // not in susv3
  long sysinfo (int cmd,
                char *buf,
                long count);

  int truncate (const ACE_TCHAR *filename, off_t length);

  u_int ualarm (u_int usecs,
                u_int interval = 0);

  u_int ualarm (const ACE_Time_Value &tv,
                const ACE_Time_Value &tv_interval = ACE_Time_Value::zero);

  int unlink (const ACE_TCHAR *path);

  ssize_t write (ACE_HANDLE handle,
                 const void *buf,
                 size_t nbyte);

  ssize_t write (ACE_HANDLE handle,
                 const void *buf,
                 size_t nbyte,
                 ACE_OVERLAPPED *);

  /**
   * Send <len> bytes from <buf> to <handle> (uses the <ACE_OS::write>
   * calls, which is uses the <write> system call on UNIX and the
   * <WriteFile> call on Win32).  If errors occur, -1 is returned.  If
   * EOF occurs, 0 is returned.  Whatever data has been transmitted
   * will be returned to the caller through <bytes_transferred>.
   */
  ssize_t write_n (ACE_HANDLE handle,
                   const void *buf,
                   size_t len,
                   size_t *bytes_transferred = 0);

} /* namespace ACE_OS */

# if defined (ACE_HAS_INLINED_OSCALLS)
#   if defined (ACE_INLINE)
#     undef ACE_INLINE
#   endif /* ACE_INLINE */
#   define ACE_INLINE inline
#   include "ace/OS_NS_unistd.inl"
# endif /* ACE_HAS_INLINED_OSCALLS */

# include /**/ "ace/post.h"
#endif /* ACE_OS_NS_UNISTD_H */
