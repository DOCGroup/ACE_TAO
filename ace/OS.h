// -*- C++ -*-

//=============================================================================
/**
 *  @file   OS.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 *  @author Jesper S. M|ller<stophph@diku.dk>
 *  @author and a cast of thousands...
 */
//=============================================================================

#ifndef ACE_OS_H
# define ACE_OS_H

# include "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

# include "ace/ace_dlfcn.h" // needed for ACE_SHLIB_HANDLE, etc...
# include "ace/ace_fcntl.h" // needed for flock, etc...
# include "ace/ace_poll.h" // needed for poll
# include "ace/ace_pwd.h"  // needed for password stuff
# include "ace/ace_signal.h" 
# include "ace/ace_stdio.h"
# include "ace/ace_stdlib.h" // needed for ACE_RANDR_TYPE
# include "ace/ace_stropts.h" // needed for strbuf, etc...
# include "ace/ace_threads.h" // ace specific that includes pthreads, etc...
# include "ace/ace_time.h"  // needed for clock_t on windows
# include "ace/ace_sys_resource.h"
# include "ace/ace_sys_sem.h"
# include "ace/ace_sys_socket.h"
# include "ace/ace_sys_stat.h"
# include "ace/ace_sys_uio.h"
# include "ace/ace_sys_utsname.h"
# include "ace/ace_sys_wait.h" // process control

// Include the split up ACE_OS classes
# include "ace/OS_Dirent.h"
# include "ace/OS_String.h"
# include "ace/OS_Memory.h"
# include "ace/OS_TLI.h"
# include "ace/OS_Errno.h"

# include "ace/Time_Value.h"

class ACE_Timeout_Manager;

// Here are all ACE-specific default constants, needed throughout ACE
// and its applications.  The values can be over written by user
// specific values in config.h files.
# include "ace/Default_Constants.h"

// Here are all ACE-specific global declarations needed throughout
// ACE.
# include "ace/Global_Macros.h"

// include the ACE min()/max() functions.
# include "ace/Min_Max.h"

// Once the PSOS stuff has been incorporated into the various ace_*.h
// headers, this can be removed.
# if defined (ACE_PSOS)
#   include "ace_psos.h"
# endif /* ACE_PSOS */

// The following are includes that have not been moved/examined yet.
# if defined (ACE_HAS_PROC_FS)
#   include /**/ <sys/procfs.h>
# endif /* ACE_HAS_PROC_FS */

# if defined ACE_HAS_BYTESEX_H
#   include /**/ <bytesex.h>
# endif /* ACE_HAS_BYTESEX_H */

# include "ace/Basic_Types.h"

// Need to look into moving this. (where?)
# if defined (__QNX__)
    include /**/ <unix.h>
    typedef long fd_mask;
# endif /* ! VXWORKS */

# if defined (ACE_HAS_ALLOCA_H)
#   include /**/ <alloca.h>
# endif /* ACE_HAS_ALLOCA_H */

// Create some useful typedefs.

# if defined (ACE_HAS_CHARPTR_DL)
typedef ACE_TCHAR * ACE_DL_TYPE;
# else
typedef const ACE_TCHAR * ACE_DL_TYPE;
# endif /* ACE_HAS_CHARPTR_DL */

/////////////////////////////////////////
// file stuff  move to ace_stdio.h?
# if defined (ACE_HAS_SYS_FILIO_H)
#   include /**/ <sys/filio.h>
# endif /* ACE_HAS_SYS_FILIO_H */

// fstat, lstat, stat in ACE_OS
# if defined (ACE_WIN32) && !defined (ACE_HAS_WINCE) && !defined (__BORLANDC__)
    typedef struct _stat ACE_stat;
# else
    typedef struct stat ACE_stat;
# endif /* ACE_WIN32 */

// and where should this go? only used in ACE_OS::bsearch()
// We need this for MVS...
extern "C" {
  typedef int (*ACE_COMPARE_FUNC)(const void *, const void *);
}

///////////////////////////////////////////////////////////////////
// Don't know where to put these, but since they are only used in OS.i, 
// they can stay here for now.
/// Helper for the ACE_OS::timezone() function
/**
 * We put all the timezone stuff that used to be in ACE_OS::timezone()
 * here because on some platforms "timezone" is a macro.  Because of this,
 * the name ACE_OS::timezone will cause errors.  So in order to use the
 * macro as it is defined but also keep the name ACE_OS::timezone, we
 * use timezone first here in this inline function, and then undefine
 * timezone.
 */
inline long ace_timezone()
{
# if !defined (ACE_HAS_WINCE) && !defined (VXWORKS) && !defined (ACE_PSOS) \
    && !defined (CHORUS)
#   if defined (ACE_WIN32)
  return _timezone;  // For Win32.
#   elif defined (__Lynx__) || defined (__FreeBSD__) || defined (ACE_HAS_SUNOS4_GETTIMEOFDAY)
  long result = 0;
  struct timeval time;
  struct timezone zone;
  ACE_UNUSED_ARG (result);
  ACE_OSCALL (::gettimeofday (&time, &zone), int, -1, result);
  return zone.tz_minuteswest * 60;
#   else  /* __Lynx__ || __FreeBSD__ ... */
  return timezone;
#   endif /* __Lynx__ || __FreeBSD__ ... */
# else
  ACE_NOTSUP_RETURN (0);
# endif /* !ACE_HAS_WINCE && !VXWORKS && !ACE_PSOS */
}

# if !defined (ACE_LACKS_DIFFTIME)
/// Helper for the ACE_OS::difftime() function
/**
 * We moved the difftime code that used to be in ACE_OS::difftime()
 * here because on some platforms "difftime" is a macro.  Because of this,
 * the name ACE_OS::difftime will cause errors.  So in order to use the
 * macro as it is defined but also keep the name ACE_OS::difftime, we
 * use difftime first here in this inline function, and then undefine
 * it.
 */
inline double ace_difftime(time_t t1, time_t t0)
{
#   if defined (ACE_PSOS) && ! defined (ACE_PSOS_HAS_TIME)
  // simulate difftime ; just subtracting ; ACE_PSOS case
  return ((double)t1) - ((double)t0);
#   else
  return difftime (t1, t0);
#   endif /* defined (ACE_PSOS) && ! defined (ACE_PSOS_HAS_TIME) */
}
# endif /* !ACE_LACKS_DIFFTIME */

/// Helper for the ACE_OS::cuserid() function
/**
 * On some platforms cuserid is a macro.  Defining ACE_OS::cuserid()
 * becomes really hard, as there is no way to save the macro
 * definition using the pre-processor.
 * This inline function achieves the same effect, without namespace
 * pollution or performance penalties.
 *
 * @todo We maybe should move a lot of the code in ACE_OS::cuserid here so
 *       it is treated the same as the above ace_difftime and ace_timezone.
 *       But since there is a good deal more code in ACE_OS::cuserid, we
 *       probably need to move some of it off into some sort of emulation
 *       function.
 */
# if !defined (ACE_LACKS_CUSERID) && !defined(ACE_HAS_ALT_CUSERID) \
    && !defined(ACE_WIN32) && !defined (VXWORKS)
inline char *ace_cuserid(char *user)
{
  return cuserid(user);
}
# endif /* !ACE_LACKS_CUSERID && !ACE_HAS_ALT_CUSERID && ... */

///////////////////////////////////////////////////////////////
class ACE_QoS;
class ACE_QoS_Params;
class ACE_Accept_QoS_Params;
class ACE_Base_Thread_Adapter;
class ACE_Thread_Hook;

#include "ace/Sched_Params.h"

/**
 * @class ACE_OS
 *
 * @brief This class defines an OS independent programming API that
 *     shields developers from nonportable aspects of writing
 *     efficient system programs on Win32, POSIX and other versions
 *     of UNIX, and various real-time operating systems.
 *
 * This class encapsulates the differences between various OS
 * platforms.  When porting ACE to a new platform, this class is
 * the place to focus on.  Once this file is ported to a new
 * platform, pretty much everything else comes for "free."  See
 * <www.cs.wustl.edu/~schmidt/ACE_wrappers/etc/ACE-porting.html>
 * for instructions on porting ACE.  Please see the README file
 * in this directory for complete information on the meaning of
 * the various macros.
 */
class ACE_OS_Export ACE_OS
  : public ACE_OS_Dirent,
    public ACE_OS_String,
    public ACE_OS_Memory,
    public ACE_OS_TLI
{
  ACE_CLASS_IS_NAMESPACE (ACE_OS);
public:
  friend class ACE_Timeout_Manager;

# if defined (ACE_WIN32)
  // = Default Win32 Security Attributes definition.
  static LPSECURITY_ATTRIBUTES default_win32_security_attributes (LPSECURITY_ATTRIBUTES);

  // = Win32 OS version determination function.
  /// Return the win32 OSVERSIONINFO structure.
  static const OSVERSIONINFO &get_win32_versioninfo (void);

  // = A pair of functions for modifying ACE's Win32 resource usage.
  /// Return the handle of the module containing ACE's resources. By
  /// default, for a DLL build of ACE this is a handle to the ACE DLL
  /// itself, and for a static build it is a handle to the executable.
  static HINSTANCE get_win32_resource_module (void);

  /// Allow an application to modify which module contains ACE's
  /// resources. This is mainly useful for a static build of ACE where
  /// the required resources reside somewhere other than the executable.
  static void set_win32_resource_module (HINSTANCE);

# endif /* ACE_WIN32 */

  // = A set of wrappers for miscellaneous operations.
  static int atoi (const char *s);

# if defined (ACE_HAS_WCHAR)
  static int atoi (const wchar_t *s);
# endif /* ACE_HAS_WCHAR */

  /// This method computes the largest integral value not greater than x.
  static double floor (double x);

  /// This method computes the smallest integral value not less than x.
  static double ceil (double x);

  static char *getenv (const char *symbol);
# if defined (ACE_HAS_WCHAR) && defined (ACE_WIN32)
  static wchar_t *getenv (const wchar_t *symbol);
# endif /* ACE_HAS_WCHAR && ACE_WIN32 */
  static int putenv (const ACE_TCHAR *string);
  static ACE_TCHAR *strenvdup (const ACE_TCHAR *str);
  static ACE_TCHAR *getenvstrings (void);

  static int getopt (int argc,
                     char *const *argv,
                     const char *optstring);
  static int argv_to_string (ACE_TCHAR **argv,
                             ACE_TCHAR *&buf,
                             int substitute_env_args = 1);
  static int string_to_argv (ACE_TCHAR *buf,
                             size_t &argc,
                             ACE_TCHAR **&argv,
                             int substitute_env_args = 1);
  static long sysconf (int);

  //@{ @name A set of wrappers for condition variables.
  static int condattr_init (ACE_condattr_t &attributes,
                            int type = ACE_DEFAULT_SYNCH_TYPE);
  static int condattr_destroy (ACE_condattr_t &attributes);
  static int cond_broadcast (ACE_cond_t *cv);
  static int cond_destroy (ACE_cond_t *cv);
  static int cond_init (ACE_cond_t *cv,
                        short type = ACE_DEFAULT_SYNCH_TYPE,
                        const char *name = 0,
                        void *arg = 0);
  static int cond_init (ACE_cond_t *cv,
                        ACE_condattr_t &attributes,
                        const char *name = 0,
                        void *arg = 0);
# if defined (ACE_HAS_WCHAR)
  static int cond_init (ACE_cond_t *cv,
                        short type,
                        const wchar_t *name,
                        void *arg = 0);
  static int cond_init (ACE_cond_t *cv,
                        ACE_condattr_t &attributes,
                        const wchar_t *name,
                        void *arg = 0);
# endif /* ACE_HAS_WCHAR */
  static int cond_signal (ACE_cond_t *cv);
  static int cond_timedwait (ACE_cond_t *cv,
                             ACE_mutex_t *m,
                             ACE_Time_Value *);
  static int cond_wait (ACE_cond_t *cv,
                        ACE_mutex_t *m);
# if defined (ACE_WIN32) && defined (ACE_HAS_WTHREADS)
  static int cond_timedwait (ACE_cond_t *cv,
                             ACE_thread_mutex_t *m,
                             ACE_Time_Value *);
  static int cond_wait (ACE_cond_t *cv,
                        ACE_thread_mutex_t *m);
# endif /* ACE_WIN32 && ACE_HAS_WTHREADS */
  //@}


  //@{ @name Wrappers to obtain the current user id
# if !defined (ACE_LACKS_CUSERID)
#   if defined(cuserid)
#     undef cuserid
#   endif /* cuserid */

  // default maxlen is asking for trouble.  the caller
  // needs to allocate user and pass it's size...
  static char *cuserid (char *user,
                        size_t maxlen = L_cuserid);

#   if defined (ACE_HAS_WCHAR)
  // Also, this version won't accept a null user, where the other will.
  // I think it's better not to accept a null user, thus it's reentrant.
  static wchar_t *cuserid (wchar_t *user,
                           size_t maxlen = ACE_MAX_USERID);
#   endif /* ACE_HAS_WCHAR */
# endif /* ACE_LACKS_CUSERID */
  //@}

  //@{ @name Wrappers to obtain configuration info
  static int uname (ACE_utsname *name);
  static long sysinfo (int cmd,
                       char *buf,
                       long count);
  static int hostname (char *name,
                       size_t maxnamelen);

# if defined (ACE_HAS_WCHAR)
  static int hostname (wchar_t *name,
                       size_t maxnamelen);
# endif /* ACE_HAS_WCHAR */
  //@}

  //@{ @name A set of wrappers for explicit dynamic linking.
  static int dlclose (ACE_SHLIB_HANDLE handle);

  static ACE_TCHAR *dlerror (void);
  static ACE_SHLIB_HANDLE dlopen (const ACE_TCHAR *filename,
                                  int mode = ACE_DEFAULT_SHLIB_MODE);
  static void *dlsym (ACE_SHLIB_HANDLE handle,
                      const ACE_TCHAR *symbol);
  //@}

  //{@ @name A set of wrappers for stdio file operations.
  static int last_error (void);
  static void last_error (int);
  static int set_errno_to_last_error (void);
  static int set_errno_to_wsa_last_error (void);
  static int fclose (FILE *fp);
  static int fcntl (ACE_HANDLE handle,
                    int cmd,
                    long arg = 0);
  static int fdetach (const char *file);

  static int fsync (ACE_HANDLE handle);

# if defined (ACE_USES_WCHAR)
  // If fp points to the Unicode format file, the file pointer will be moved right next
  // to the Unicode header (2 types).  Otherwise, file pointer will be at the beginning.
  static void checkUnicodeFormat (FILE* fp);
# endif  // ACE_USES_WCHAR

  static FILE *fopen (const ACE_TCHAR *filename, const ACE_TCHAR *mode);
  static FILE *freopen (const ACE_TCHAR *filename, const ACE_TCHAR *mode, FILE* stream);
# if defined (fdopen)
#   undef fdopen
# endif /* fdopen */
  static FILE *fdopen (ACE_HANDLE handle, const ACE_TCHAR *mode);
  static ACE_TCHAR *fgets (ACE_TCHAR *buf, int size, FILE *fp);
  static int stat (const ACE_TCHAR *file, ACE_stat *);
  static int truncate (const ACE_TCHAR *filename, off_t length);

  static int fprintf (FILE *fp, const char *format, ...);
  static int sprintf (char *buf, const char *format, ...);
  static int vsprintf (char *buffer, const char *format, va_list argptr);
  static int printf (const char *format, ...);
# if defined (ACE_HAS_WCHAR)
  static int sprintf (wchar_t *buf, const wchar_t *format, ...);
  static int fprintf (FILE *fp, const wchar_t *format, ...);
  static int vsprintf (wchar_t *buffer, const wchar_t *format, va_list argptr);
# endif /* ACE_HAS_WCHAR */

  static void perror (const ACE_TCHAR *s);

  // The old gets () which directly maps to the evil, unprotected
  // gets () has been deprecated.  If you really need gets (),
  // consider the following one.

  // A better gets ().
  //   If n == 0, input is swallowed, but NULL is returned.
  //   Otherwise, reads up to n-1 bytes (not including the newline),
  //              then swallows rest up to newline
  //              then swallows newline
  static char *gets (char *str, int n = 0);
  static int puts (const ACE_TCHAR *s);
  static int fputs (const ACE_TCHAR *s,
                    FILE *stream);

  static int fflush (FILE *fp);
  static size_t fread (void *ptr,
                       size_t size,
                       size_t nelems,
                       FILE *fp);
  static int fseek (FILE *fp,
                    long offset,
                    int ptrname);
  static long ftell (FILE* fp);
  static int  fgetpos (FILE* fp, fpos_t* pos);
  static int  fsetpos (FILE* fp, fpos_t* pos);
  static int fstat (ACE_HANDLE,
                    ACE_stat *);
  static int lstat (const char *,
                    ACE_stat *);
  static int ftruncate (ACE_HANDLE,
                        off_t);
  static size_t fwrite (const void *ptr,
                        size_t size,
                        size_t nitems,
                        FILE *fp);
  static void rewind (FILE *fp);
  //@}

  //@{ @name Wrappers for searching and sorting.
  static void *bsearch (const void *key,
                        const void *base,
                        size_t nel,
                        size_t size,
                        ACE_COMPARE_FUNC);
  static void qsort (void *base,
                     size_t nel,
                     size_t width,
                     ACE_COMPARE_FUNC);
  //@}

  //@{ @name A set of wrappers for file locks.
  static int flock_init (ace_flock_t *lock,
                         int flags = 0,
                         const ACE_TCHAR *name = 0,
                         mode_t perms = 0);
  static int flock_destroy (ace_flock_t *lock,
                            int unlink_file = 1);
# if defined (ACE_WIN32)
  static void adjust_flock_params (ace_flock_t *lock,
                                   short whence,
                                   off_t &start,
                                   off_t &len);
# endif /* ACE_WIN32 */
  static int flock_rdlock (ace_flock_t *lock,
                           short whence = 0,
                           off_t start = 0,
                           off_t len = 0);
  static int flock_tryrdlock (ace_flock_t *lock,
                              short whence = 0,
                              off_t start = 0,
                              off_t len = 0);
  static int flock_trywrlock (ace_flock_t *lock,
                              short whence = 0,
                              off_t start = 0,
                              off_t len = 0);
  static int flock_unlock (ace_flock_t *lock,
                           short whence = 0,
                           off_t start = 0,
                           off_t len = 0);
  static int flock_wrlock (ace_flock_t *lock,
                           short whence = 0,
                           off_t start = 0,
                           off_t len = 0);
  //@}

  //@{ @name A set of wrappers for low-level process operations.
  static int atexit (ACE_EXIT_HOOK func);
  static int execl (const char *path,
                    const char *arg0, ...);
  static int execle (const char *path,
                     const char *arg0, ...);
  static int execlp (const char *file,
                     const char *arg0, ...);
  static int execv (const char *path,
                    char *const argv[]);
  static int execvp (const char *file,
                     char *const argv[]);
  static int execve (const char *path,
                     char *const argv[],
                     char *const envp[]);
  static void _exit (int status = 0);
  static void exit (int status = 0);
  static void abort (void);
  static pid_t fork (void);

  //@{
  /// Forks and exec's a process in a manner that works on Solaris and
  /// NT.  argv[0] must be the full path name to the executable.
  static pid_t fork (const ACE_TCHAR *program_name);
  static pid_t fork_exec (ACE_TCHAR *argv[]);
  //@}

  static int getpagesize (void);
  static int allocation_granularity (void);

  static gid_t getgid (void);
  static int setgid (gid_t);
  static pid_t getpid (void);
  static pid_t getpgid (pid_t pid);
  static pid_t getppid (void);
  static uid_t getuid (void);
  static int setuid (uid_t);
  static pid_t setsid (void);
  static int setpgid (pid_t pid, pid_t pgid);
  static int setreuid (uid_t ruid, uid_t euid);
  static int setregid (gid_t rgid, gid_t egid);
  static int system (const ACE_TCHAR *s);
  //@}

  /**
   * Calls <::waitpid> on UNIX/POSIX platforms and <::await> on
   * Chorus.  Does not work on Vxworks, or pSoS.
   * On Win32, <pid> is ignored if the <handle> is not equal to 0.
   * Passing the process <handle> is prefer on Win32 because using
   * <pid> to wait on the project doesn't always work correctly
   * if the waited process has already terminated.
   */
  static pid_t waitpid (pid_t pid,
                        ACE_exitcode *status = 0,
                        int wait_options = 0,
                        ACE_HANDLE handle = 0);

  /**
   * Calls <::WaitForSingleObject> on Win32 and <ACE::waitpid>
   * otherwise.  Returns the passed in <pid_t> on success and -1 on
   * failure.
   * On Win32, <pid> is ignored if the <handle> is not equal to 0.
   * Passing the process <handle> is prefer on Win32 because using
   * <pid> to wait on the project doesn't always work correctly
   * if the waited process has already terminated.
   */
  static pid_t wait (pid_t pid,
                     ACE_exitcode *status,
                     int wait_options = 0,
                     ACE_HANDLE handle = 0);

  /// Calls OS <::wait> function, so it's only portable to UNIX/POSIX
  /// platforms.
  static pid_t wait (int * = 0);

  //@{ @name A set of wrappers for timers and resource stats.
  static u_int alarm (u_int secs);
  static u_int ualarm (u_int usecs,
                       u_int interval = 0);
  static u_int ualarm (const ACE_Time_Value &tv,
                       const ACE_Time_Value &tv_interval = ACE_Time_Value::zero);
  static ACE_hrtime_t gethrtime (int = ACE_HRTIMER_GETTIME);
# if defined (ACE_HAS_POWERPC_TIMER) && (defined (ghs) || defined (__GNUG__))
  static void readPPCTimeBase (u_long &most,
                               u_long &least);
# endif /* ACE_HAS_POWERPC_TIMER  &&  (ghs or __GNUG__) */
  static int clock_gettime (clockid_t,
                            struct timespec *);
  static ACE_Time_Value gettimeofday (void);
  static int getrusage (int who,
                        struct rusage *rusage);
  static int getrlimit (int resource,
                        struct rlimit *rl);
  static int setrlimit (int resource,
                        ACE_SETRLIMIT_TYPE *rl);
  static int sleep (u_int seconds);
  static int sleep (const ACE_Time_Value &tv);
  static int nanosleep (const struct timespec *requested,
                        struct timespec *remaining = 0);

# if defined (ACE_HAS_BROKEN_R_ROUTINES)
#   undef ctime_r
#   undef asctime_r
#   undef rand_r
#   undef getpwnam_r
# endif /* ACE_HAS_BROKEN_R_ROUTINES */
  //@}

  //@{ @name A set of wrappers for operations on time.
# if !defined (ACE_HAS_WINCE)
  static time_t mktime (struct tm *timeptr);
# endif /* !ACE_HAS_WINCE */

  // wrapper for time zone information.
  static void tzset (void);

# if defined (timezone)
#   undef timezone
# endif /* timezone */
  static long timezone (void);

# if defined (difftime)
#   undef difftime
# endif /* difftime */
  static double difftime (time_t t1,
                          time_t t0);
  static time_t time (time_t *tloc = 0);
  static struct tm *localtime (const time_t *clock);
  static struct tm *localtime_r (const time_t *clock,
                                 struct tm *res);
  static struct tm *gmtime (const time_t *clock);
  static struct tm *gmtime_r (const time_t *clock,
                              struct tm *res);
  static char *asctime (const struct tm *tm);
  static char *asctime_r (const struct tm *tm,
                          char *buf, int buflen);
  static ACE_TCHAR *ctime (const time_t *t);
  static ACE_TCHAR *ctime_r (const time_t *clock, ACE_TCHAR *buf, int buflen);
  static size_t strftime (char *s,
                          size_t maxsize,
                          const char *format,
                          const struct tm *timeptr);
  //@}

  //@{ @name A set of wrappers for System V message queues.
  static int msgctl (int msqid,
                     int cmd,
                     struct msqid_ds *);
  static int msgget (key_t key,
                     int msgflg);
  static int msgrcv (int int_id,
                     void *buf,
                     size_t len,
                     long type,
                     int flags);
  static int msgsnd (int int_id,
                     const void *buf,
                     size_t len,
                     int flags);
  //@}

  //@{ @name A set of wrappers for memory mapped files.
  static int madvise (caddr_t addr,
                      size_t len,
                      int advice);
  static void *mmap (void *addr,
                     size_t len,
                     int prot,
                     int flags,
                     ACE_HANDLE handle,
                     off_t off = 0,
                     ACE_HANDLE *file_mapping = 0,
                     LPSECURITY_ATTRIBUTES sa = 0,
                     const ACE_TCHAR *file_mapping_name = 0);
  static int mprotect (void *addr,
                       size_t len,
                       int prot);
  static int msync (void *addr,
                    size_t len,
                    int sync);
  static int munmap (void *addr,
                     size_t len);
  //@}

  //@{ @name A set of wrappers for recursive mutex locks.
  static int recursive_mutex_init (ACE_recursive_thread_mutex_t *m,
                                   const ACE_TCHAR *name = 0,
                                   ACE_mutexattr_t *arg = 0,
                                   LPSECURITY_ATTRIBUTES sa = 0);
  static int recursive_mutex_destroy (ACE_recursive_thread_mutex_t *m);
  static int recursive_mutex_lock (ACE_recursive_thread_mutex_t *m);
  static int recursive_mutex_trylock (ACE_recursive_thread_mutex_t *m);
  static int recursive_mutex_unlock (ACE_recursive_thread_mutex_t *m);
  //@}

  //@{ @name A set of wrappers for mutex locks.
  static int mutex_init (ACE_mutex_t *m,
                         int type = ACE_DEFAULT_SYNCH_TYPE,
                         const char *name = 0,
                         ACE_mutexattr_t *arg = 0,
                         LPSECURITY_ATTRIBUTES sa = 0);
# if defined (ACE_HAS_WCHAR)
  static int mutex_init (ACE_mutex_t *m,
                         int type,
                         const wchar_t *name,
                         ACE_mutexattr_t *arg = 0,
                         LPSECURITY_ATTRIBUTES sa = 0);
# endif /* ACE_HAS_WCHAR */
  static int mutex_destroy (ACE_mutex_t *m);

  /// Win32 note: Abandoned mutexes are not treated differently. 0 is
  /// returned since the calling thread does get the ownership.
  static int mutex_lock (ACE_mutex_t *m);

  /// This method is only implemented for Win32.  For abandoned
  /// mutexes, <abandoned> is set to 1 and 0 is returned.
  static int mutex_lock (ACE_mutex_t *m,
                         int &abandoned);

  /**
   * This method attempts to acquire a lock, but gives up if the lock
   * has not been acquired by the given time.  If the lock is not
   * acquired within the given amount of time, then this method
   * returns -1 with an <ETIME> errno on platforms that actually
   * support timed mutexes.  The timeout should be an absolute time.
   * Note that the mutex should not be a recursive one, i.e., it
   * should only be a standard mutex or an error checking mutex.
   */
  static int mutex_lock (ACE_mutex_t *m,
                         const ACE_Time_Value &timeout);

  /**
   * If <timeout> == 0, calls <ACE_OS::mutex_lock(m)>.  Otherwise,
   * this method attempts to acquire a lock, but gives up if the lock
   * has not been acquired by the given time, in which case it returns
   * -1 with an <ETIME> errno on platforms that actually support timed
   * mutexes.  The timeout should be an absolute time.  Note that the
   * mutex should not be a recursive one, i.e., it should only be a
   * standard mutex or an error checking mutex.
   */
  static int mutex_lock (ACE_mutex_t *m,
                         const ACE_Time_Value *timeout);

  /// Win32 note: Abandoned mutexes are not treated differently. 0 is
  /// returned since the calling thread does get the ownership.
  static int mutex_trylock (ACE_mutex_t *m);

  /// This method is only implemented for Win32.  For abandoned
  /// mutexes, <abandoned> is set to 1 and 0 is returned.
  static int mutex_trylock (ACE_mutex_t *m,
                            int &abandoned);

  static int mutex_unlock (ACE_mutex_t *m);
  //@}

  //@{ @name A set of wrappers for mutex locks that only work within a single process.
  static int thread_mutex_init (ACE_thread_mutex_t *m,
                                int type = ACE_DEFAULT_SYNCH_TYPE,
                                const char *name = 0,
                                ACE_mutexattr_t *arg = 0);
# if defined (ACE_HAS_WCHAR)
  static int thread_mutex_init (ACE_thread_mutex_t *m,
                                int type,
                                const wchar_t *name,
                                ACE_mutexattr_t *arg = 0);
# endif /* ACE_HAS_WCHAR */
  static int thread_mutex_destroy (ACE_thread_mutex_t *m);
  static int thread_mutex_lock (ACE_thread_mutex_t *m);
  static int thread_mutex_lock (ACE_thread_mutex_t *m,
                                const ACE_Time_Value &timeout);
  static int thread_mutex_lock (ACE_thread_mutex_t *m,
                                const ACE_Time_Value *timeout);
  static int thread_mutex_trylock (ACE_thread_mutex_t *m);
  static int thread_mutex_unlock (ACE_thread_mutex_t *m);
  //@}

  //@{ @name A set of wrappers for low-level file operations.
  static int access (const char *path, int amode);
# if defined (ACE_HAS_WCHAR)
  static int access (const wchar_t *path, int amode);
# endif /* ACE_HAS_WCHAR */

  static int close (ACE_HANDLE handle);
  static ACE_HANDLE creat (const ACE_TCHAR *filename,
                           mode_t mode);
  static ACE_HANDLE dup (ACE_HANDLE handle);
  static int dup2 (ACE_HANDLE oldfd,
                   ACE_HANDLE newfd);
  static int fattach (int handle,
                      const char *path);
  static long filesize (ACE_HANDLE handle);
  static long filesize (const ACE_TCHAR *handle);
  static int getmsg (ACE_HANDLE handle,
                     struct strbuf *ctl,
                     struct strbuf
                     *data, int *flags);
  static int getpmsg (ACE_HANDLE handle,
                      struct strbuf *ctl,
                      struct strbuf
                      *data,
                      int *band,
                      int *flags);

  /// UNIX-style <ioctl>.
  static int ioctl (ACE_HANDLE handle,
                    int cmd,
                    void * = 0);

# if !defined (ACE_HAS_WINCE)
  /// QoS-enabled <ioctl>.
  static int ioctl (ACE_HANDLE socket,
                    u_long io_control_code,
                    void *in_buffer_p,
                    u_long in_buffer,
                    void *out_buffer_p,
                    u_long out_buffer,
                    u_long *bytes_returned,
                    ACE_OVERLAPPED *overlapped,
                    ACE_OVERLAPPED_COMPLETION_FUNC func);

  /// QoS-enabled <ioctl> when the I/O control code is either
  /// SIO_SET_QOS or SIO_GET_QOS.
  static int ioctl (ACE_HANDLE socket,
                    u_long io_control_code,
                    ACE_QoS &ace_qos,
                    u_long *bytes_returned,
                    void *buffer_p = 0,
                    u_long buffer = 0,
                    ACE_OVERLAPPED *overlapped = 0,
                    ACE_OVERLAPPED_COMPLETION_FUNC func = 0);
# endif  // ACE_HAS_WINCE

  static int isastream (ACE_HANDLE handle);
  static int isatty (int handle);
# if defined (ACE_WIN32)
  static int isatty (ACE_HANDLE handle);
# endif /* ACE_WIN32 */
  static off_t lseek (ACE_HANDLE handle,
                      off_t offset,
                      int whence);
# if defined (ACE_HAS_LLSEEK) || defined (ACE_HAS_LSEEK64)
  static ACE_LOFF_T llseek (ACE_HANDLE handle, ACE_LOFF_T offset, int whence);
# endif /* ACE_HAS_LLSEEK */

  // It used to be that the <perms> argument default was 0 on all
  // platforms. Further, the ACE_OS::open implementations ignored <perms>
  // for Win32 and always supplied read|write|delete. To preserve
  // backward compatibility and allow users to pass in values
  // that are used as desired, the defaults are now what the default
  // action used to be on Win32. The implementation now obeys what is passed.
# if defined (ACE_WIN32)
#   if defined (ACE_HAS_WINNT4) && (ACE_HAS_WINNT4 == 1)
#     define ACE_DEFAULT_OPEN_PERMS (FILE_SHARE_READ | FILE_SHARE_WRITE | \
                                    FILE_SHARE_DELETE)
#   else
#     define ACE_DEFAULT_OPEN_PERMS (FILE_SHARE_READ | FILE_SHARE_WRITE)
#   endif /* ACE_HAS_WINCE */
# else
#   define ACE_DEFAULT_OPEN_PERMS 0
# endif  /* ACE_WIN32 */

  /// The O_APPEND flag is only partly supported on Win32. If you specify
  /// O_APPEND, then the file pointer will be positioned at the end of
  /// the file initially during open, but it is not re-positioned at
  /// the end prior to each write, as specified by POSIX.  This
  /// is generally good enough for typical situations, but it is ``not
  /// quite right'' in its semantics.
  static ACE_HANDLE open (const char *filename,
                          int mode,
                          int perms = ACE_DEFAULT_OPEN_PERMS,
                          LPSECURITY_ATTRIBUTES sa = 0);
# if defined (ACE_HAS_WCHAR)
  static ACE_HANDLE open (const wchar_t *filename,
                          int mode,
                          int perms = ACE_DEFAULT_OPEN_PERMS,
                          LPSECURITY_ATTRIBUTES sa = 0);
# endif /* ACE_HAS_WCHAR */
  static int putmsg (ACE_HANDLE handle,
                     const struct strbuf *ctl,
                     const struct strbuf *data,
                     int flags);
  static int putpmsg (ACE_HANDLE handle,
                      const struct strbuf *ctl,
                      const struct strbuf *data,
                      int band,
                      int flags);
  static ssize_t read (ACE_HANDLE handle,
                       void *buf,
                       size_t len);
  static ssize_t read (ACE_HANDLE handle,
                       void *buf,
                       size_t len,
                       ACE_OVERLAPPED *);
  /**
   * Receive <len> bytes into <buf> from <handle> (uses the
   * <ACE_OS::read> call, which uses the <read> system call on UNIX
   * and the <ReadFile> call on Win32). If errors occur, -1 is
   * returned.  If EOF occurs, 0 is returned.  Whatever data has been
   * transmitted will be returned to the caller through
   * <bytes_transferred>.
   */
  static ssize_t read_n (ACE_HANDLE handle,
                         void *buf,
                         size_t len,
                         size_t *bytes_transferred = 0);

  static int readlink (const char *path,
                       char *buf,
                       size_t bufsiz);
  static ssize_t pread (ACE_HANDLE handle,
                        void *buf,
                        size_t nbyte,
                        off_t offset);
  static int recvmsg (ACE_HANDLE handle,
                      struct msghdr *msg,
                      int flags);
  static int sendmsg (ACE_HANDLE handle,
                      const struct msghdr *msg,
                      int flags);
  static ssize_t write (ACE_HANDLE handle,
                        const void *buf,
                        size_t nbyte);
  static ssize_t write (ACE_HANDLE handle,
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
  static ssize_t write_n (ACE_HANDLE handle,
                          const void *buf,
                          size_t len,
                          size_t *bytes_transferred = 0);

  static ssize_t pwrite (ACE_HANDLE handle,
                         const void *buf,
                         size_t nbyte,
                         off_t offset);
  static ssize_t readv (ACE_HANDLE handle,
                        iovec *iov,
                        int iovlen);
  static ssize_t writev (ACE_HANDLE handle,
                         const iovec *iov,
                         int iovcnt);
  static ssize_t recvv (ACE_HANDLE handle,
                        iovec *iov,
                        int iovlen);
  static ssize_t sendv (ACE_HANDLE handle,
                        const iovec *iov,
                        int iovcnt);
  //@}

  //@{ @name A set of wrappers for event demultiplexing and IPC.
  static int select (int width,
                     fd_set *rfds,
                     fd_set *wfds = 0,
                     fd_set *efds = 0,
                     const ACE_Time_Value *tv = 0);
  static int select (int width,
                     fd_set *rfds,
                     fd_set *wfds,
                     fd_set *efds,
                     const ACE_Time_Value &tv);
  static int poll (struct pollfd *pollfds,
                   u_long len,
                   const ACE_Time_Value *tv = 0);
  static int poll (struct pollfd *pollfds,
                   u_long len,
                   const ACE_Time_Value &tv);
  static int pipe (ACE_HANDLE handles[]);

  static ACE_HANDLE shm_open (const ACE_TCHAR *filename,
                              int mode,
                              int perms = 0,
                              LPSECURITY_ATTRIBUTES sa = 0);
  static int shm_unlink (const ACE_TCHAR *path);
  //@}

  //@{ @name A set of wrappers for directory operations.
  static mode_t umask (mode_t cmask);

# if !defined (ACE_LACKS_CHDIR)
  static int chdir (const char *path);

#   if defined (ACE_HAS_WCHAR)
  static int chdir (const wchar_t *path);
#   endif /* ACE_HAS_WCHAR */
# endif /* ACE_LACKS_CHDIR */

  static int mkdir (const ACE_TCHAR *path,
                    mode_t mode = ACE_DEFAULT_DIR_PERMS);
  static int mkfifo (const ACE_TCHAR *file,
                     mode_t mode = ACE_DEFAULT_FILE_PERMS);
  static ACE_TCHAR *mktemp (ACE_TCHAR *t);
  static ACE_HANDLE mkstemp (ACE_TCHAR *t);
  static ACE_TCHAR *getcwd (ACE_TCHAR *, size_t);
  static int rename (const ACE_TCHAR *old_name,
                     const ACE_TCHAR *new_name,
                     int flags = -1);
  static int unlink (const ACE_TCHAR *path);
  static ACE_TCHAR *tempnam (const ACE_TCHAR *dir = 0,
                             const ACE_TCHAR *pfx = 0);
  //@}

  //@{ @name A set of wrappers for random number operations.
  static int rand (void);
  static int rand_r (ACE_RANDR_TYPE &seed);
  static void srand (u_int seed);
  //@}

  //@{ @name A set of wrappers for readers/writer locks.
  static int rwlock_init (ACE_rwlock_t *rw,
                          int type = ACE_DEFAULT_SYNCH_TYPE,
                          const ACE_TCHAR *name = 0,
                          void *arg = 0);
  static int rwlock_destroy (ACE_rwlock_t *rw);
  static int rw_rdlock (ACE_rwlock_t *rw);
  static int rw_wrlock (ACE_rwlock_t *rw);
  static int rw_tryrdlock (ACE_rwlock_t *rw);
  static int rw_trywrlock (ACE_rwlock_t *rw);
  static int rw_trywrlock_upgrade (ACE_rwlock_t *rw);
  static int rw_unlock (ACE_rwlock_t *rw);
  //@}

  //@{ @name A set of wrappers for auto-reset and manual events.
  static int event_init (ACE_event_t *event,
                         int manual_reset = 0,
                         int initial_state = 0,
                         int type = ACE_DEFAULT_SYNCH_TYPE,
                         const char *name = 0,
                         void *arg = 0,
                         LPSECURITY_ATTRIBUTES sa = 0);
# if defined (ACE_HAS_WCHAR)
  static int event_init (ACE_event_t *event,
                         int manual_reset,
                         int initial_state,
                         int type,
                         const wchar_t *name,
                         void *arg = 0,
                         LPSECURITY_ATTRIBUTES sa = 0);
# endif /* ACE_HAS_WCHAR */
  static int event_destroy (ACE_event_t *event);
  static int event_wait (ACE_event_t *event);
  static int event_timedwait (ACE_event_t *event,
                              ACE_Time_Value *timeout,
                              int use_absolute_time = 1);
  static int event_signal (ACE_event_t *event);
  static int event_pulse (ACE_event_t *event);
  static int event_reset (ACE_event_t *event);
  //@}

  //@{ @name A set of wrappers for semaphores.
  static int sema_destroy (ACE_sema_t *s);
  static int sema_init (ACE_sema_t *s,
                        u_int count,
                        int type = ACE_DEFAULT_SYNCH_TYPE,
                        const char *name = 0,
                        void *arg = 0,
                        int max = 0x7fffffff,
                        LPSECURITY_ATTRIBUTES sa = 0);
# if defined (ACE_HAS_WCHAR)
  static int sema_init (ACE_sema_t *s,
                        u_int count,
                        int type,
                        const wchar_t *name,
                        void *arg = 0,
                        int max = 0x7fffffff,
                        LPSECURITY_ATTRIBUTES sa = 0);
# endif /* ACE_HAS_WCHAR */
  static int sema_post (ACE_sema_t *s);
  static int sema_post (ACE_sema_t *s,
                        size_t release_count);
  static int sema_trywait (ACE_sema_t *s);
  static int sema_wait (ACE_sema_t *s);
  static int sema_wait (ACE_sema_t *s,
                        ACE_Time_Value &tv);
  static int sema_wait (ACE_sema_t *s,
                        ACE_Time_Value *tv);
  //@}

  //@{ @name A set of wrappers for System V semaphores.
  static int semctl (int int_id,
                     int semnum,
                     int cmd,
                     semun);
  static int semget (key_t key,
                     int nsems,
                     int flags);
  static int semop (int int_id,
                    struct sembuf *sops,
                    size_t nsops);
  //@}

  //@{ @name Thread scheduler interface.
  /// Set scheduling parameters.  An id of ACE_SELF indicates, e.g.,
  /// set the parameters on the calling thread.
  static int sched_params (const ACE_Sched_Params &, ACE_id_t id = ACE_SELF);
  //@}

  //@{ @name A set of wrappers for System V shared memory.
  static void *shmat (int int_id,
                      void *shmaddr,
                      int shmflg);
  static int shmctl (int int_id,
                     int cmd,
                     struct shmid_ds *buf);
  static int shmdt (void *shmaddr);
  static int shmget (key_t key,
                     int size,
                     int flags);
  ///@}

  //@{ @name A set of wrappers for Signals.
  static int kill (pid_t pid,
                   int signum);
  static int sigaction (int signum,
                        const struct sigaction *nsa,
                        struct sigaction *osa);
  static int sigaddset (sigset_t *s,
                        int signum);
  static int sigdelset (sigset_t *s,
                        int signum);
  static int sigemptyset (sigset_t *s);
  static int sigfillset (sigset_t *s);
  static int sigismember (sigset_t *s,
                          int signum);
  static ACE_SignalHandler signal (int signum,
                                   ACE_SignalHandler);
  static int sigsuspend (const sigset_t *set);
  static int sigprocmask (int how,
                          const sigset_t *nsp,
                          sigset_t *osp);

  static int pthread_sigmask (int how,
                              const sigset_t *nsp,
                              sigset_t *osp);
  //@}

  //@{ @name A set of wrappers for sockets.
  /// BSD-style <accept> (no QoS).
  static ACE_HANDLE accept (ACE_HANDLE handle,
                            struct sockaddr *addr,
                            int *addrlen);

# if !defined (ACE_HAS_WINCE)
  /**
   * QoS-enabled <accept>, which passes <qos_params> to <accept>.  If
   * the OS platform doesn't support QoS-enabled <accept> then the
   * <qos_params> are ignored and the BSD-style <accept> is called.
   */
  static ACE_HANDLE accept (ACE_HANDLE handle,
                            struct sockaddr *addr,
                            int *addrlen,
                            const ACE_Accept_QoS_Params &qos_params);
# endif  // ACE_HAS_WINCE

  /// BSD-style <connect> (no QoS).
  static int connect (ACE_HANDLE handle,
                      struct sockaddr *addr,
                      int addrlen);

# if !defined (ACE_HAS_WINCE)
  /**
   * QoS-enabled <connect>, which passes <qos_params> to <connect>.
   * If the OS platform doesn't support QoS-enabled <connect> then the
   * <qos_params> are ignored and the BSD-style <connect> is called.
   */
  static int connect (ACE_HANDLE handle,
                      const sockaddr *addr,
                      int addrlen,
                      const ACE_QoS_Params &qos_params);
# endif  // ACE_HAS_WINCE

  static int bind (ACE_HANDLE s,
                   struct sockaddr *name,
                   int namelen);

  static int closesocket (ACE_HANDLE s);
  static struct hostent *gethostbyaddr (const char *addr,
                                        int length,
                                        int type);
  static struct hostent *gethostbyname (const char *name);
  static struct hostent *getipnodebyname (const char *name, int family,
                                          int flags = 0);
  static struct hostent *getipnodebyaddr (const void *src, size_t len,
                                          int family);
  static struct hostent *gethostbyaddr_r (const char *addr,
                                          int length,
                                          int type,
                                          struct hostent *result,
                                          ACE_HOSTENT_DATA buffer,
                                          int *h_errnop);
  static struct hostent *gethostbyname_r (const char *name,
                                          struct hostent *result,
                                          ACE_HOSTENT_DATA buffer,
                                          int *h_errnop);
  static int getpeername (ACE_HANDLE handle,
                          struct sockaddr *addr,
                          int *addrlen);
  static struct protoent *getprotobyname (const char *name);
  static struct protoent *getprotobyname_r (const char *name,
                                            struct protoent *result,
                                            ACE_PROTOENT_DATA buffer);
  static struct protoent *getprotobynumber (int proto);
  static struct protoent *getprotobynumber_r (int proto,
                                              struct protoent *result,
                                              ACE_PROTOENT_DATA buffer);
  static struct servent *getservbyname (const char *svc,
                                        const char *proto);
  static struct servent *getservbyname_r (const char *svc,
                                          const char *proto,
                                          struct servent *result,
                                          ACE_SERVENT_DATA buf);
  static int getsockname (ACE_HANDLE handle,
                          struct sockaddr *addr,
                          int *addrlen);
  static int getsockopt (ACE_HANDLE handle,
                         int level,
                         int optname,
                         char *optval,
                         int *optlen);
  static unsigned long inet_addr (const char *name);
  static char *inet_ntoa (const struct in_addr addr);
  static int inet_aton (const char *strptr,
                        struct in_addr *addr);
  static const char *inet_ntop (int family,
                                const void *addrptr,
                                char *strptr,
                                 size_t len);
  static int inet_pton (int family,
                        const char *strptr,
                        void *addrptr);
  /// Retrieve information about available transport protocols
  /// installed on the local machine.
  static int enum_protocols (int *protocols,
                             ACE_Protocol_Info *protocol_buffer,
                             u_long *buffer_length);

# if !defined (ACE_HAS_WINCE)
  /// Joins a leaf node into a QoS-enabled multi-point session.
  static ACE_HANDLE join_leaf (ACE_HANDLE socket,
                               const sockaddr *name,
                               int namelen,
                               const ACE_QoS_Params &qos_params);
# endif  // ACE_HAS_WINCE

  static int listen (ACE_HANDLE handle,
                     int backlog);
  static int recv (ACE_HANDLE handle,
                   char *buf,
                   int len,
                   int flags = 0);
  static int recvfrom (ACE_HANDLE handle,
                       char *buf,
                       int len,
                       int flags,
                       struct sockaddr *addr,
                       int *addrlen);
  static int recvfrom (ACE_HANDLE handle,
                       iovec *buffers,
                       int buffer_count,
                       size_t &number_of_bytes_recvd,
                       int &flags,
                       struct sockaddr *addr,
                       int *addrlen,
                       ACE_OVERLAPPED *overlapped,
                       ACE_OVERLAPPED_COMPLETION_FUNC func);
  static int send (ACE_HANDLE handle,
                   const char *buf,
                   int len,
                   int flags = 0);
  static int sendto (ACE_HANDLE handle,
                     const char *buf,
                     int len,
                     int flags,
                     const struct sockaddr *addr,
                     int addrlen);
  static int sendto (ACE_HANDLE handle,
                     const iovec *buffers,
                     int buffer_count,
                     size_t &number_of_bytes_sent,
                     int flags,
                     const struct sockaddr *addr,
                     int addrlen,
                     ACE_OVERLAPPED *overlapped,
                     ACE_OVERLAPPED_COMPLETION_FUNC func);

  /// Manipulate the options associated with a socket.
  static int setsockopt (ACE_HANDLE handle,
                         int level,
                         int optname,
                         const char *optval,
                         int optlen);
  static int shutdown (ACE_HANDLE handle,
                       int how);

  /// Create a BSD-style socket (no QoS).
  static ACE_HANDLE socket (int protocol_family,
                            int type,
                            int proto);

  /// Create a QoS-enabled socket.  If the OS platform doesn't support
  /// QoS-enabled <socket> then the BSD-style <socket> is called.
  static ACE_HANDLE socket (int protocol_family,
                            int type,
                            int proto,
                            ACE_Protocol_Info *protocolinfo,
                            ACE_SOCK_GROUP g,
                            u_long flags);

  static int socketpair (int domain,
                         int type,
                         int protocol,
                         ACE_HANDLE sv[2]);

  /// Initialize WinSock before first use (e.g., when a DLL is first
  /// loaded or the first use of a socket() call.
  static int socket_init (int version_high = 1,
                          int version_low = 1);

  /// Finalize WinSock after last use (e.g., when a DLL is unloaded).
  static int socket_fini (void);
  //@}

  //@{ @name A set of wrappers for password routines.
  static void setpwent (void);
  static void endpwent (void);
  static struct passwd *getpwent (void);
  static struct passwd *getpwnam (const char *user);
  static struct passwd *getpwnam_r (const char *name,
                                    struct passwd *pwent,
                                    char *buffer,
                                    int buflen);
  //@}

  //@{ @name A set of wrappers for regular expressions.
  static char *compile (const char *instring,
                        char *expbuf,
                        char *endbuf);
  static int step (const char *str,
                   char *expbuf);
  //@}


# if 0
  //@{ @name A set of wrappers for threads (these are portable since they use the ACE_Thread_ID).
  static int thr_continue (const ACE_Thread_ID &thread);
  static int thr_create (ACE_THR_FUNC,
                         void *args,
                         long flags,
                         ACE_Thread_ID *,
                         long priority = ACE_DEFAULT_THREAD_PRIORITY,
                         void *stack = 0,
                         size_t stacksize = 0);
  static int thr_getprio (ACE_Thread_ID thr_id,
                          int &prio,
                          int *policy = 0);
  static int thr_join (ACE_Thread_ID waiter_id,
                       void **status);
  static int thr_kill (ACE_Thread_ID thr_id,
                       int signum);
  static ACE_Thread_ID thr_self (void);
  static int thr_setprio (ACE_Thread_ID thr_id,
                          int prio);
  static int thr_setprio (const ACE_Sched_Priority prio);
  static int thr_suspend (ACE_Thread_ID target_thread);
  static int thr_cancel (ACE_Thread_ID t_id);
  //@}
# endif /* 0 */

  //@{ @name A set of wrappers for threads

  // These are non-portable since they use ACE_thread_t and
  // ACE_hthread_t and will go away in a future release.
  static int thr_continue (ACE_hthread_t target_thread);

  /*
   * Creates a new thread having <flags> attributes and running <func>
   * with <args> (if <thread_adapter> is non-0 then <func> and <args>
   * are ignored and are obtained from <thread_adapter>).  <thr_id>
   * and <t_handle> are set to the thread's ID and handle (?),
   * respectively.  The thread runs at <priority> priority (see
   * below).
   *
   * The <flags> are a bitwise-OR of the following:
   * = BEGIN<INDENT>
   * THR_CANCEL_DISABLE, THR_CANCEL_ENABLE, THR_CANCEL_DEFERRED,
   * THR_CANCEL_ASYNCHRONOUS, THR_BOUND, THR_NEW_LWP, THR_DETACHED,
   * THR_SUSPENDED, THR_DAEMON, THR_JOINABLE, THR_SCHED_FIFO,
   * THR_SCHED_RR, THR_SCHED_DEFAULT, THR_EXPLICIT_SCHED,
   * THR_SCOPE_SYSTEM, THR_SCOPE_PROCESS
   * = END<INDENT>
   *
   * By default, or if <priority> is set to
   * ACE_DEFAULT_THREAD_PRIORITY, an "appropriate" priority value for
   * the given scheduling policy (specified in <flags}>, e.g.,
   * <THR_SCHED_DEFAULT>) is used.  This value is calculated
   * dynamically, and is the median value between the minimum and
   * maximum priority values for the given policy.  If an explicit
   * value is given, it is used.  Note that actual priority values are
   * EXTREMEMLY implementation-dependent, and are probably best
   * avoided.
   *
   * Note that <thread_adapter> is always deleted by <thr_create>,
   * therefore it must be allocated with global operator new.
   */
  static int thr_create (ACE_THR_FUNC func,
                         void *args,
                         long flags,
                         ACE_thread_t *thr_id,
                         ACE_hthread_t *t_handle = 0,
                         long priority = ACE_DEFAULT_THREAD_PRIORITY,
                         void *stack = 0,
                         size_t stacksize = 0,
                         ACE_Base_Thread_Adapter *thread_adapter = 0);

  static int thr_getprio (ACE_hthread_t thr_id,
                          int &prio);
  static int thr_join (ACE_hthread_t waiter_id,
                       void **status);
  static int thr_join (ACE_thread_t waiter_id,
                       ACE_thread_t *thr_id,
                       void **status);
  static int thr_kill (ACE_thread_t thr_id,
                       int signum);
  static ACE_thread_t thr_self (void);
  static void thr_self (ACE_hthread_t &);
  static int thr_setprio (ACE_hthread_t thr_id,
                          int prio);
  static int thr_setprio (const ACE_Sched_Priority prio);
  static int thr_suspend (ACE_hthread_t target_thread);
  static int thr_cancel (ACE_thread_t t_id);

  static int thr_cmp (ACE_hthread_t t1,
                      ACE_hthread_t t2);
  static int thr_equal (ACE_thread_t t1,
                        ACE_thread_t t2);
  static void thr_exit (void *status = 0);
  static int thr_getconcurrency (void);
  static int lwp_getparams (ACE_Sched_Params &);
# if defined (ACE_HAS_TSS_EMULATION) && defined (ACE_HAS_THREAD_SPECIFIC_STORAGE)
  static int thr_getspecific (ACE_OS_thread_key_t key,
                              void **data);
# endif /* ACE_HAS_TSS_EMULATION && ACE_HAS_THREAD_SPECIFIC_STORAGE */
  static int thr_getspecific (ACE_thread_key_t key,
                              void **data);
  static int thr_keyfree (ACE_thread_key_t key);
  static int thr_key_detach (void *inst);
# if defined (ACE_HAS_THR_C_DEST)
#   if defined (ACE_HAS_TSS_EMULATION) && defined (ACE_HAS_THREAD_SPECIFIC_STORAGE)
  static int thr_keycreate (ACE_OS_thread_key_t *key,
                            ACE_THR_C_DEST,
                            void *inst = 0);
#   endif /* ACE_HAS_TSS_EMULATION && ACE_HAS_THREAD_SPECIFIC_STORAGE */
  static int thr_keycreate (ACE_thread_key_t *key,
                            ACE_THR_C_DEST,
                            void *inst = 0);
# else
#   if defined (ACE_HAS_TSS_EMULATION) && defined (ACE_HAS_THREAD_SPECIFIC_STORAGE)
  static int thr_keycreate (ACE_OS_thread_key_t *key,
                            ACE_THR_DEST,
                            void *inst = 0);
#   endif /* ACE_HAS_TSS_EMULATION && ACE_HAS_THREAD_SPECIFIC_STORAGE */
  static int thr_keycreate (ACE_thread_key_t *key,
                            ACE_THR_DEST,
                            void *inst = 0);
# endif /* ACE_HAS_THR_C_DEST */
  static int thr_key_used (ACE_thread_key_t key);
  static size_t thr_min_stack (void);
  static int thr_setconcurrency (int hint);
  static int lwp_setparams (const ACE_Sched_Params &);
# if defined (ACE_HAS_TSS_EMULATION) && defined (ACE_HAS_THREAD_SPECIFIC_STORAGE)
  static int thr_setspecific (ACE_OS_thread_key_t key,
                              void *data);
# endif /* ACE_HAS_TSS_EMULATION && ACE_HAS_THREAD_SPECIFIC_STORAGE */
  static int thr_setspecific (ACE_thread_key_t key,
                              void *data);
  static int thr_sigsetmask (int how,
                             const sigset_t *nsm,
                             sigset_t *osm);
  static int thr_setcancelstate (int new_state,
                                 int *old_state);
  static int thr_setcanceltype (int new_type,
                                int *old_type);
  static int sigwait (sigset_t *set,
                      int *sig = 0);
  static int sigtimedwait (const sigset_t *set,
                           siginfo_t *info,
                           const ACE_Time_Value *timeout);
  static void thr_testcancel (void);
  static void thr_yield (void);

  /**
   * This method uses process id and object pointer to come up with a
   * machine wide unique name.  The process ID will provide uniqueness
   * between processes on the same machine. The "this" pointer of the
   * <object> will provide uniqueness between other "live" objects in
   * the same process. The uniqueness of this name is therefore only
   * valid for the life of <object>.
   */
  static void unique_name (const void *object,
                           ACE_TCHAR *name,
                           size_t length);

  /// This is necessary to deal with POSIX pthreads and their use of
  /// structures for thread ids.
  static ACE_thread_t NULL_thread;

  /// This is necessary to deal with POSIX pthreads and their use of
  /// structures for thread handles.
  static ACE_hthread_t NULL_hthread;

  /// This is necessary to deal with POSIX pthreads and their use of
  /// structures for TSS keys.
  static ACE_thread_key_t NULL_key;

# if defined (CHORUS)
  /// This is used to map an actor's id into a KnCap for killing and
  /// waiting actors.
  static KnCap actorcaps_[ACE_CHORUS_MAX_ACTORS];
# endif /* CHORUS */
  //@}

# if defined (ACE_WIN32)
  /// Keeps track of whether we've already initialized WinSock...
  static int socket_initialized_;
# endif /* ACE_WIN32 */

  /// Handle asynchronous thread cancellation cleanup.
  static void mutex_lock_cleanup (void *mutex);

  /**
   * Call TSS destructors for the current thread.  If the current
   * thread is the main thread, then the argument must be 1.
   * For private use of ACE_Object_Manager and ACE_Thread_Adapter only.
   */
  static void cleanup_tss (const u_int main_thread);

# if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0) && defined (ACE_LACKS_NETDB_REENTRANT_FUNCTIONS)
  static int netdb_acquire (void);
  static int netdb_release (void);
# endif /* defined (ACE_MT_SAFE) && ACE_LACKS_NETDB_REENTRANT_FUNCTIONS */

  /// Find the schedling class ID that corresponds to the class name.
  static int scheduling_class (const char *class_name, ACE_id_t &);

  /// Friendly interface to <priocntl>(2).
  static int set_scheduling_params (const ACE_Sched_Params &,
                                    ACE_id_t id = ACE_SELF);

  /// Low-level interface to <priocntl>(2).
  /**
   * Can't call the following priocntl, because that's a macro on
   * Solaris.
   */
  static int priority_control (ACE_idtype_t, ACE_id_t, int, void *);

# if defined (ACE_HAS_STRPTIME)
  static char *strptime (char *buf,
                         const char *format,
                         struct tm *tm);

#   if defined (ACE_LACKS_NATIVE_STRPTIME)
  static int strptime_getnum (char *buf, int *num, int *bi,
                              int *fi, int min, int max);
#   endif /* ACE_LACKS_NATIVE_STRPTIME */
# endif /* ACE_HAS_STRPTIME */

private:

# if defined (ACE_LACKS_WRITEV)
  static int writev_emulation (ACE_HANDLE handle,
                               ACE_WRITEV_TYPE *iov,
                               int iovcnt);
# endif /* ACE_LACKS_WRITEV */

# if defined (ACE_LACKS_READV)
  static ssize_t readv_emulation (ACE_HANDLE handle,
                                  ACE_READV_TYPE *iov,
                                  int iovcnt);
# endif /* ACE_LACKS_READV */

  /// Function that is called by <ACE_OS::exit>, if non-null.
  static ACE_EXIT_HOOK exit_hook_;

  /// For use by ACE_Object_Manager only, to register its exit hook..
  static ACE_EXIT_HOOK set_exit_hook (ACE_EXIT_HOOK hook);

  /// Allow the ACE_OS_Object_Manager to call set_exit_hook.
  friend class ACE_OS_Object_Manager;

# if defined (ACE_WIN32)
#   if defined (ACE_HAS_WINCE)
  /// Supporting data for ctime and ctime_r functions on WinCE.
  static const wchar_t *day_of_week_name[7];
  static const wchar_t *month_name[12];
#   endif /* ACE_HAS_WINCE */

  /// Translate fopen's mode char to open's mode.  This helper function
  /// is here to avoid maintaining several pieces of identical code.
  static void fopen_mode_to_open_mode_converter (ACE_TCHAR x, int &hmode);

  static OSVERSIONINFO win32_versioninfo_;

  static HINSTANCE win32_resource_module_;

# endif /* ACE_WIN32 */

# if defined (ACE_HAS_VIRTUAL_TIME)
  static clock_t times (struct tms *buf);
# endif /* ACE_HAS_VIRTUAL_TIME */

  //changed for ACE_HAS_VIRTUAL_TIME changes.

  static int cond_timedwait_i (ACE_cond_t *cv,
                       ACE_mutex_t *m,
                       ACE_Time_Value *);

  static u_int alarm_i (u_int secs);

  static u_int ualarm_i (u_int usecs, u_int interval = 0);

  static u_int ualarm_i (const ACE_Time_Value &tv,
                         const ACE_Time_Value &tv_interval = ACE_Time_Value::zero);

  static int sleep_i (u_int seconds);

  static int sleep_i (const ACE_Time_Value &tv);

  static int nanosleep_i (const struct timespec *requested,
                          struct timespec *remaining = 0);

  static int select_i (int width,
                       fd_set *rfds,
                       fd_set *wfds,
                       fd_set *efds,
                       const ACE_Time_Value *tv = 0);

  static int select_i (int width,
                       fd_set *rfds,
                       fd_set *wfds,
                       fd_set *efds,
                       const ACE_Time_Value &tv);

  static int poll_i (struct pollfd *pollfds,
                     u_long len,
                     const ACE_Time_Value *tv = 0);

  static int poll_i (struct pollfd *pollfds,
                     u_long len,
                     const ACE_Time_Value &tv);

  static int sema_wait_i (ACE_sema_t *s);

  static int sema_wait_i (ACE_sema_t *s,
                          ACE_Time_Value &tv);

  static int sigtimedwait_i (const sigset_t *set,
                             siginfo_t *info,
                             const ACE_Time_Value *timeout);

  static ACE_Time_Value gettimeofday_i (void);
};

/////////////////////////////////////////////////////////////////////////
// These need to be moved out...
// these are only used in the ctor for AOM and should be hidden in the 
// cpp once the stuff is moved.
# if defined (ACE_WIN32)
// Default WIN32 structured exception handler.
int ACE_SEH_Default_Exception_Selector (void *);
int ACE_SEH_Default_Exception_Handler (void *);
# endif /* ACE_WIN32 */

//////////////////////////////////////////////////////////////////////////
// This is all TSS stuff and needs to be moved.
# if defined (ACE_HAS_TSS_EMULATION)
    // Allow config.h to set the default number of thread keys.
#   if !defined (ACE_DEFAULT_THREAD_KEYS)
#     define ACE_DEFAULT_THREAD_KEYS 64
#   endif /* ! ACE_DEFAULT_THREAD_KEYS */

// forward declaration
class ACE_TSS_Keys;

/**
 * @class ACE_TSS_Emulation
 *
 * @brief Thread-specific storage emulation.
 *
 * This provides a thread-specific storage implementation.
 * It is intended for use on platforms that don't have a
 * native TSS, or have a TSS with limitations such as the
 * number of keys or lack of support for removing keys.
 */
class ACE_OS_Export ACE_TSS_Emulation
{
public:
  typedef void (*ACE_TSS_DESTRUCTOR)(void *value) /* throw () */;

  /// Maximum number of TSS keys allowed over the life of the program.
  enum { ACE_TSS_THREAD_KEYS_MAX = ACE_DEFAULT_THREAD_KEYS };

  /// Returns the total number of keys allocated so far.
  static u_int total_keys ();

  /// Sets the argument to the next available key.  Returns 0 on success,
  /// -1 if no keys are available.
  static int next_key (ACE_thread_key_t &key);

  /// Release a key that was used. This way the key can be given out in a
  /// new request. Returns 0 on success, 1 if the key was not reserved.
  static int release_key (ACE_thread_key_t key);

  /// Returns the exit hook associated with the key.  Does _not_ check
  /// for a valid key.
  static ACE_TSS_DESTRUCTOR tss_destructor (const ACE_thread_key_t key);

  /// Associates the TSS destructor with the key.  Does _not_ check
  /// for a valid key.
  static void tss_destructor (const ACE_thread_key_t key,
                              ACE_TSS_DESTRUCTOR destructor);

  /// Accesses the object referenced by key in the current thread's TSS array.
  /// Does _not_ check for a valid key.
  static void *&ts_object (const ACE_thread_key_t key);

  /**
   * Setup an array to be used for local TSS.  Returns the array
   * address on success.  Returns 0 if local TSS had already been
   * setup for this thread.  There is no corresponding tss_close ()
   * because it is not needed.
   * NOTE: tss_open () is called by ACE for threads that it spawns.
   * If your application spawns threads without using ACE, and it uses
   * ACE's TSS emulation, each of those threads should call tss_open
   * ().  See the ace_thread_adapter () implementaiton for an example.
   */
  static void *tss_open (void *ts_storage[ACE_TSS_THREAD_KEYS_MAX]);

  /// Shutdown TSS emulation.  For use only by ACE_OS::cleanup_tss ().
  static void tss_close ();

private:
  // Global TSS structures.
  /// Contains the possible value of the next key to be allocated. Which key
  /// is actually allocated is based on the tss_keys_used
  static u_int total_keys_;

  /// Array of thread exit hooks (TSS destructors) that are called for each
  /// key (that has one) when the thread exits.
  static ACE_TSS_DESTRUCTOR tss_destructor_ [ACE_TSS_THREAD_KEYS_MAX];

  /// TSS_Keys instance to administrate whether a specific key is in used
  /// or not
  static ACE_TSS_Keys tss_keys_used_;

#   if defined (ACE_HAS_THREAD_SPECIFIC_STORAGE)
  /// Location of current thread's TSS array.
  static void **tss_base (void* ts_storage[] = 0, u_int *ts_created = 0);
#   else  /* ! ACE_HAS_THREAD_SPECIFIC_STORAGE */
  /// Location of current thread's TSS array.
  static void **&tss_base ();
#   endif /* ! ACE_HAS_THREAD_SPECIFIC_STORAGE */

#   if defined (ACE_HAS_THREAD_SPECIFIC_STORAGE)
  // Rely on native thread specific storage for the implementation,
  // but just use one key.
  static ACE_OS_thread_key_t native_tss_key_;

  // Used to indicate if native tss key has been allocated
  static int key_created_;
#   endif /* ACE_HAS_THREAD_SPECIFIC_STORAGE */
};

# else   /* ! ACE_HAS_TSS_EMULATION */
#   if defined (TLS_MINIMUM_AVAILABLE)
    // WIN32 platforms define TLS_MINIMUM_AVAILABLE natively.
#     define ACE_DEFAULT_THREAD_KEYS TLS_MINIMUM_AVAILABLE
#   endif /* TSL_MINIMUM_AVAILABLE */

# endif /* ACE_HAS_TSS_EMULATION */

// moved ACE_TSS_Ref, ACE_TSS_Info, and ACE_TSS_Keys class
// declarations from OS.cpp so they are visible to the single
// file of template instantiations.
# if defined (ACE_WIN32) || defined (ACE_HAS_TSS_EMULATION) || (defined (ACE_PSOS) && defined (ACE_PSOS_HAS_TSS))
/**
 * @class ACE_TSS_Ref
 *
 * @brief "Reference count" for thread-specific storage keys.
 *
 * Since the <ACE_Unbounded_Stack> doesn't allow duplicates, the
 * "reference count" is the identify of the thread_id.
 */
class ACE_TSS_Ref
{
public:
  /// Constructor
  ACE_TSS_Ref (ACE_thread_t id);

  /// Default constructor
  ACE_TSS_Ref (void);

  /// Check for equality.
  int operator== (const ACE_TSS_Ref &) const;

  /// Check for inequality.
  int operator!= (const ACE_TSS_Ref &) const;

// private:

  /// ID of thread using a specific key.
  ACE_thread_t tid_;
};

/**
 * @class ACE_TSS_Info
 *
 * @brief Thread Specific Key management.
 *
 * This class maps a key to a "destructor."
 */
class ACE_TSS_Info
{
public:
  /// Constructor
  ACE_TSS_Info (ACE_thread_key_t key,
                void (*dest)(void *) = 0,
                void *tss_inst = 0);

  /// Default constructor
  ACE_TSS_Info (void);

  /// Returns 1 if the key is in use, 0 if not.
  int key_in_use (void) const { return thread_count_ != -1; }

  /// Mark the key as being in use if the flag is non-zero, or
  /// not in use if the flag is 0.
  void key_in_use (int flag) { thread_count_ = flag == 0  ?  -1  :  1; }

  /// Check for equality.
  int operator== (const ACE_TSS_Info &) const;

  /// Check for inequality.
  int operator!= (const ACE_TSS_Info &) const;

  /// Dump the state.
  void dump (void);

private:
  /// Key to the thread-specific storage item.
  ACE_thread_key_t key_;

  /// "Destructor" that gets called when the item is finally released.
  void (*destructor_)(void *);

  /// Pointer to ACE_TSS<xxx> instance that has/will allocate the key.
  void *tss_obj_;

  /// Count of threads that are using this key.  Contains -1 when the
  /// key is not in use.
  int thread_count_;

  friend class ACE_TSS_Cleanup;
};

/**
 * @class ACE_TSS_Keys
 *
 * @brief Collection of in-use flags for a thread's TSS keys.
 * For internal use only by ACE_TSS_Cleanup; it is public because
 * some compilers can't use nested classes for template instantiation
 * parameters.
 *
 * Wrapper around array of whether each key is in use.  A simple
 * typedef doesn't work with Sun C++ 4.2.
 */
class ACE_TSS_Keys
{
public:
  /// Default constructor, to initialize all bits to zero (unused).
  ACE_TSS_Keys (void);

  /// Mark the specified key as being in use, if it was not already so marked.
  /// Returns 1 if the had already been marked, 0 if not.
  int test_and_set (const ACE_thread_key_t key);

  /// Mark the specified key as not being in use, if it was not already so
  /// cleared.  Returns 1 if the key had already been cleared, 0 if not.
  int test_and_clear (const ACE_thread_key_t key);

  /// Return whether the specific key is marked as in use.
  /// Returns 1 if the key is been marked, 0 if not.
  int is_set (const ACE_thread_key_t key) const;

private:
  /// For a given key, find the word and bit number that represent it.
  static void find (const u_int key, u_int &word, u_int &bit);

  enum
    {
#   if ACE_SIZEOF_LONG == 8
      ACE_BITS_PER_WORD = 64,
#   elif ACE_SIZEOF_LONG == 4
      ACE_BITS_PER_WORD = 32,
#   else
#     error ACE_TSS_Keys only supports 32 or 64 bit longs.
#   endif /* ACE_SIZEOF_LONG == 8 */
      ACE_WORDS = (ACE_DEFAULT_THREAD_KEYS - 1) / ACE_BITS_PER_WORD + 1
    };

  /// Bit flag collection.  A bit value of 1 indicates that the key is in
  /// use by this thread.
  u_long key_bit_words_[ACE_WORDS];
};

# endif /* defined (ACE_WIN32) || defined (ACE_HAS_TSS_EMULATION) */

//////////////////////////////////////////////////////////////////////////
// This is only used in OS.i and OS.cpp right now.
// Support non-scalar thread keys, such as with some POSIX
// implementations, e.g., MVS.
# if defined (ACE_HAS_NONSCALAR_THREAD_KEY_T)
#   define ACE_KEY_INDEX(OBJ,KEY) \
  u_int OBJ; \
  ACE_OS::memcpy (&OBJ, &KEY, sizeof (u_int))
# else
#   define ACE_KEY_INDEX(OBJ,KEY) u_int OBJ = KEY

# endif /* ACE_HAS_NONSCALAR_THREAD_KEY_T */

//////////////////////////////////////////////////////////////////////////
// These seem to be used only in OS.i and Timer_Queue_Adapters.cpp
# if defined (ACE_HAS_THR_C_FUNC)
// This is necessary to work around nasty problems with MVS C++.
extern "C" ACE_OS_Export void ace_mutex_lock_cleanup_adapter (void *args);
#   define ACE_PTHREAD_CLEANUP_PUSH(A) pthread_cleanup_push (ace_mutex_lock_cleanup_adapter, (void *) A);
#   define ACE_PTHREAD_CLEANUP_POP(A) pthread_cleanup_pop(A)
# elif defined (ACE_HAS_PTHREADS) && !defined (ACE_LACKS_PTHREAD_CLEANUP)
// Though we are defining a extern "C" function to match the prototype of
// pthread_cleanup_push, it is undone by the Solaris header file
// /usr/include/pthread.h. So this macro generates a warning under Solaris
// with SunCC. This is a bug in the Solaris header file.
extern "C" ACE_OS_Export void ace_mutex_lock_cleanup_adapter (void *args);
#   define ACE_PTHREAD_CLEANUP_PUSH(A) pthread_cleanup_push (ace_mutex_lock_cleanup_adapter, (void *) A);
#   define ACE_PTHREAD_CLEANUP_POP(A) pthread_cleanup_pop(A)
# else
#   define ACE_PTHREAD_CLEANUP_PUSH(A)
#   define ACE_PTHREAD_CLEANUP_POP(A)
# endif /* ACE_HAS_THR_C_FUNC */

/////////////////////////////////////////////////////////////////////////////
// These don't seem to be used anywhere... except in the tests!!!
// looks like Global_Macros.h or Default_Constants might be a better place...
# if !defined (ACE_DEFAULT_MUTEX_A)
#   define ACE_DEFAULT_MUTEX_A "ACE_MUTEX"
# endif /* ACE_DEFAULT_MUTEX_A */

# if defined (ACE_HAS_WCHAR)
#   define ACE_DEFAULT_MUTEX_W ACE_TEXT_WIDE(ACE_DEFAULT_MUTEX_A)
# endif /* ACE_HAS_WCHAR */

# define ACE_DEFAULT_MUTEX ACE_LIB_TEXT (ACE_DEFAULT_MUTEX_A)

//////////////////////////////////////////////////////////////////////////////
// Need to look into getting rid of this--or moving it to a move apropriate place.
# if !defined (ACE_HAS_MINIMAL_ACE_OS)
#   include "ace/Trace.h"
# endif /* ! ACE_HAS_MINIMAL_ACE_OS */

///////////////////////////////////////////////////////////////////////
// From here down should be stable.
# if defined (ACE_HAS_INLINED_OSCALLS)
#   if defined (ACE_INLINE)
#     undef ACE_INLINE
#   endif /* ACE_INLINE */
#   define ACE_INLINE inline
#   include "ace/OS.i"
# endif /* ACE_HAS_INLINED_OSCALLS */

# if defined (ACE_LEGACY_MODE)
#   include "ace/Log_Msg.h"
#   include "ace/Thread_Hook.h"
#   include "ace/Thread_Adapter.h"
#   include "ace/Thread_Exit.h"
#   include "ace/Thread_Control.h"
# endif  /* ACE_LEGACY_MODE */

// This has to be at the bottom... Well, at least after OM_Base...
# include "ace/Main_Macros.h"

# include "ace/post.h"
#endif  /* ACE_OS_H */
