// $Id$

#ifndef ACE_ADAPTER_OS_H
#define ACE_ADAPTER_OS_H
#include "ace/pre.h"

#include "../../OS.h"

class ACE_OS_W : public ACE_OS
{
public:
/*
  // = A set of wrappers for miscellaneous operations.
  static int atoi (const wchar_t *s);

  static ACE_TCHAR *getenv (const ACE_TCHAR *symbol);
  static int putenv (const ACE_TCHAR *string);
  static ACE_TCHAR *strenvdup (const ACE_TCHAR *str);
  static ACE_TCHAR *getenvstrings (void);

  static int argv_to_string (ACE_TCHAR **argv,
                             ACE_TCHAR *&buf,
                             int substitute_env_args = 1);
  static int string_to_argv (ACE_TCHAR *buf,
                             size_t &argc,
                             ACE_TCHAR **&argv,
                             int substitute_env_args = 1);

  // = A set of wrappers for condition variables.
  static int condattr_init (ACE_condattr_t &attributes,
                            int type = ACE_DEFAULT_SYNCH_TYPE);
  static int condattr_destroy (ACE_condattr_t &attributes);
  static int cond_broadcast (ACE_cond_t *cv);
  static int cond_destroy (ACE_cond_t *cv);
  static int cond_init (ACE_cond_t *cv,
                        short type = ACE_DEFAULT_SYNCH_TYPE,
                        const ACE_TCHAR *name = 0,
                        void *arg = 0);
  static int cond_init (ACE_cond_t *cv,
                        ACE_condattr_t &attributes,
                        const ACE_TCHAR *name = 0,
                        void *arg = 0);
  static int cond_signal (ACE_cond_t *cv);
  static int cond_timedwait (ACE_cond_t *cv,
                             ACE_mutex_t *m,
                             ACE_Time_Value *);
  static int cond_wait (ACE_cond_t *cv,
                        ACE_mutex_t *m);
  // = A set of wrappers for determining config info.
  static ACE_TCHAR *cuserid (ACE_TCHAR *user,
                             size_t maxlen = 32);
  static int uname (struct utsname *name);
  static long sysinfo (int cmd,
                       char *buf,
                       long count);
  static int hostname (ACE_TCHAR *name,
                       size_t maxnamelen);

  // = A set of wrappers for explicit dynamic linking.
  static int dlclose (ACE_SHLIB_HANDLE handle);

  static ACE_TCHAR *dlerror (void);
  static ACE_SHLIB_HANDLE dlopen (const ACE_TCHAR *filename,
                                  int mode = ACE_DEFAULT_SHLIB_MODE);
  static void *dlsym (ACE_SHLIB_HANDLE handle,
                      const ACE_TCHAR *symbol);

  static FILE *fopen (const ACE_TCHAR *filename, const ACE_TCHAR *mode);
  static FILE *fdopen (ACE_HANDLE handle, const ACE_TCHAR *mode);
  static ACE_TCHAR *fgets (ACE_TCHAR *buf, int size, FILE *fp);
  static int stat (const ACE_TCHAR *file, struct stat *);
  static int truncate (const ACE_TCHAR *filename, off_t length);

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
  static int fstat (ACE_HANDLE,
                    struct stat *);
  static int lstat (const char *,
                    struct stat *);
  static int ftruncate (ACE_HANDLE,
                        off_t);
  static size_t fwrite (const void *ptr,
                        size_t size,
                        size_t nitems,
                        FILE *fp);
  static void rewind (FILE *fp);

  // = Wrappers for searching and sorting.
  static void *bsearch (const void *key,
                        const void *base,
                        size_t nel,
                        size_t size,
                        ACE_COMPARE_FUNC);
  static void qsort (void *base,
                     size_t nel,
                     size_t width,
                     ACE_COMPARE_FUNC);

  // = A set of wrappers for file locks.
  static int flock_init (ACE_OS::ace_flock_t *lock,
                         int flags = 0,
                         const ACE_TCHAR *name = 0,
                         mode_t perms = 0);
  static int flock_destroy (ACE_OS::ace_flock_t *lock,
                            int unlink_file = 1);
  static int flock_rdlock (ACE_OS::ace_flock_t *lock,
                           short whence = 0,
                           off_t start = 0,
                           off_t len = 0);
  static int flock_tryrdlock (ACE_OS::ace_flock_t *lock,
                              short whence = 0,
                              off_t start = 0,
                              off_t len = 0);
  static int flock_trywrlock (ACE_OS::ace_flock_t *lock,
                              short whence = 0,
                              off_t start = 0,
                              off_t len = 0);
  static int flock_unlock (ACE_OS::ace_flock_t *lock,
                           short whence = 0,
                           off_t start = 0,
                           off_t len = 0);
  static int flock_wrlock (ACE_OS::ace_flock_t *lock,
                           short whence = 0,
                           off_t start = 0,
                           off_t len = 0);

  // = A set of wrappers for low-level process operations.
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
  static pid_t fork (const ACE_TCHAR *program_name);
  static pid_t fork_exec (ACE_TCHAR *argv[]);
  // Forks and exec's a process in a manner that works on Solaris and
  // NT.  argv[0] must be the full path name to the executable.

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
  static pid_t waitpid (pid_t pid,
                        ACE_exitcode *status = 0,
                        int wait_options = 0,
                        ACE_HANDLE handle = 0);
  // Calls <::waitpid> on UNIX/POSIX platforms and <::await> on
  // Chorus.  Does not work on Vxworks, or pSoS.
  // On Win32, <pid> is ignored if the <handle> is not equal to 0.
  // Passing the process <handle> is prefer on Win32 because using
  // <pid> to wait on the project doesn't always work correctly
  // if the waited process has already terminated.
  static pid_t wait (pid_t pid,
                     ACE_exitcode *status,
                     int wait_options = 0,
                     ACE_HANDLE handle = 0);
  // Calls <::WaitForSingleObject> on Win32 and <ACE::waitpid>
  // otherwise.  Returns the passed in <pid_t> on success and -1 on
  // failure.
  // On Win32, <pid> is ignored if the <handle> is not equal to 0.
  // Passing the process <handle> is prefer on Win32 because using
  // <pid> to wait on the project doesn't always work correctly
  // if the waited process has already terminated.
  static pid_t wait (int * = 0);
  // Calls OS <::wait> function, so it's only portable to UNIX/POSIX
  // platforms.

  // = A set of wrappers for timers and resource stats.
  static u_int alarm (u_int secs);
  static u_int ualarm (u_int usecs,
                       u_int interval = 0);
  static u_int ualarm (const ACE_Time_Value &tv,
                       const ACE_Time_Value &tv_interval = ACE_Time_Value::zero);
  static ACE_hrtime_t gethrtime (const ACE_HRTimer_Op = ACE_HRTIMER_GETTIME);
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


  static ACE_TCHAR *ctime_r (const time_t *clock, ACE_TCHAR *buf, int buflen);
  static size_t strftime (char *s,
                          size_t maxsize,
                          const char *format,
                          const struct tm *timeptr);

  // = A set of wrappers for System V message queues.
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

  // = A set of wrappers for memory mapped files.
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
                     LPSECURITY_ATTRIBUTES sa = 0);
  static int mprotect (void *addr,
                       size_t len,
                       int prot);
  static int msync (void *addr,
                    size_t len,
                    int sync);
  static int munmap (void *addr,
                     size_t len);

  // = A set of wrappers for recursive mutex locks.
  static int recursive_mutex_init (ACE_recursive_thread_mutex_t *m,
                                   const ACE_TCHAR *name = 0,
                                   ACE_mutexattr_t *arg = 0,
                                   LPSECURITY_ATTRIBUTES sa = 0);
  static int recursive_mutex_destroy (ACE_recursive_thread_mutex_t *m);
  static int recursive_mutex_lock (ACE_recursive_thread_mutex_t *m);
  static int recursive_mutex_trylock (ACE_recursive_thread_mutex_t *m);
  static int recursive_mutex_unlock (ACE_recursive_thread_mutex_t *m);

  // = A set of wrappers for mutex locks.
  static int mutex_init (ACE_mutex_t *m,
                         int type = ACE_DEFAULT_SYNCH_TYPE,
                         const ACE_TCHAR *name = 0,
                         ACE_mutexattr_t *arg = 0,
                         LPSECURITY_ATTRIBUTES sa = 0);
  static int mutex_destroy (ACE_mutex_t *m);
  static int mutex_lock (ACE_mutex_t *m);
  // Win32 note: Abandoned mutexes are not treated differently. 0 is
  // returned since the calling thread does get the ownership.
  static int mutex_lock (ACE_mutex_t *m,
                         int &abandoned);
  // This method is only implemented for Win32.  For abandoned
  // mutexes, <abandoned> is set to 1 and 0 is returned.
  static int mutex_trylock (ACE_mutex_t *m);
  // Win32 note: Abandoned mutexes are not treated differently. 0 is
  // returned since the calling thread does get the ownership.
  static int mutex_trylock (ACE_mutex_t *m,
                            int &abandoned);
  // This method is only implemented for Win32.  For abandoned
  // mutexes, <abandoned> is set to 1 and 0 is returned.
  static int mutex_unlock (ACE_mutex_t *m);

  // = A set of wrappers for mutex locks that only work within a
  // single process.
  static int thread_mutex_init (ACE_thread_mutex_t *m,
                                int type = ACE_DEFAULT_SYNCH_TYPE,
                                const ACE_TCHAR *name = 0,
                                ACE_mutexattr_t *arg = 0);
  static int thread_mutex_destroy (ACE_thread_mutex_t *m);
  static int thread_mutex_lock (ACE_thread_mutex_t *m);
  static int thread_mutex_trylock (ACE_thread_mutex_t *m);
  static int thread_mutex_unlock (ACE_thread_mutex_t *m);

  // = A set of wrappers for low-level file operations.
  static int access (const ACE_TCHAR *path, int amode);
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
  static int ioctl (ACE_HANDLE handle,
                    int cmd,
                    void * = 0);
  // UNIX-style <ioctl>.
  static int ioctl (ACE_HANDLE socket,
                    u_long io_control_code,
                    void *in_buffer_p,
                    u_long in_buffer,
                    void *out_buffer_p,
                    u_long out_buffer,
                    u_long *bytes_returned,
                    ACE_OVERLAPPED *overlapped,
                    ACE_OVERLAPPED_COMPLETION_FUNC func);
  // QoS-enabled <ioctl>.
  static int ioctl (ACE_HANDLE socket,
                    u_long io_control_code,
                    ACE_QoS &ace_qos,
                    u_long *bytes_returned,
                    void *buffer_p = 0,
                    u_long buffer = 0,
                    ACE_OVERLAPPED *overlapped = 0,
                    ACE_OVERLAPPED_COMPLETION_FUNC func = 0);
  // QoS-enabled <ioctl> when the I/O control code is either SIO_SET_QOS
  // or SIO_GET_QOS.
  static int isastream (ACE_HANDLE handle);
  static int isatty (int handle);
  static off_t lseek (ACE_HANDLE handle,
                      off_t offset,
                      int whence);*/
  
  static ACE_HANDLE open (const wchar_t *filename,
                          int mode,
                          int perms = 0,
                          LPSECURITY_ATTRIBUTES sa = 0)
  { return ACE_OS::open (ACE_TEXT_WCHAR_TO_CHAR (filename), mode, perms, sa); }

/*  
  // Receive <len> bytes into <buf> from <handle> (uses the
  // <ACE_OS::read> call, which uses the <read> system call on UNIX
  // and the <ReadFile> call on Win32). If errors occur, -1 is
  // returned.  If EOF occurs, 0 is returned.  Whatever data has been
  // transmitted will be returned to the caller through
  // <bytes_transferred>.
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
  static ssize_t write_n (ACE_HANDLE handle,
                          const void *buf,
                          size_t len,
                          size_t *bytes_transferred = 0);
  // Send <len> bytes from <buf> to <handle> (uses the <ACE_OS::write>
  // calls, which is uses the <write> system call on UNIX and the
  // <WriteFile> call on Win32).  If errors occur, -1 is returned.  If
  // EOF occurs, 0 is returned.  Whatever data has been transmitted
  // will be returned to the caller through <bytes_transferred>.
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

  // = A set of wrappers for event demultiplexing and IPC.
  static int select (int width,
                     fd_set *rfds,
                     fd_set *wfds,
                     fd_set *efds,
                     const ACE_Time_Value *tv = 0);
  static int select (int width,
                     fd_set *rfds,
                     fd_set *wfds,
                     fd_set *efds,
                     const ACE_Time_Value &tv);
  static int poll (struct pollfd *pollfds,
                   u_long len,
                   ACE_Time_Value *tv = 0);
  static int poll (struct pollfd *pollfds,
                   u_long len,
                   const ACE_Time_Value &tv);
  static int pipe (ACE_HANDLE handles[]);

  static ACE_HANDLE shm_open (const ACE_TCHAR *filename,
                              int mode,
                              int perms = 0,
                              LPSECURITY_ATTRIBUTES sa = 0);
  static int shm_unlink (const ACE_TCHAR *path);

  // = A set of wrappers for directory operations.
  static mode_t umask (mode_t cmask);
  static int chdir (const ACE_TCHAR *path);
  static int mkdir (const ACE_TCHAR *path,
                    mode_t mode = ACE_DEFAULT_DIR_PERMS);
  static int mkfifo (const ACE_TCHAR *file,
                     mode_t mode = ACE_DEFAULT_FILE_PERMS);
  static ACE_TCHAR *mktemp (ACE_TCHAR *t);
  static ACE_TCHAR *getcwd (ACE_TCHAR *, size_t);
  static int rename (const ACE_TCHAR *old_name,
                     const ACE_TCHAR *new_name,
                     int flags = -1);*/
  
  static int unlink (const wchar_t *path)
  { return ACE_OS::unlink (ACE_TEXT_WCHAR_TO_CHAR (path)); }

  /*  static ACE_TCHAR *tempnam (const ACE_TCHAR *dir = 0,
                             const ACE_TCHAR *pfx = 0);

  // = A set of wrappers for random number operations.
  static int rand (void);
  static int rand_r (ACE_RANDR_TYPE &seed);
  static void srand (u_int seed);

  // = A set of wrappers for readers/writer locks.
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

  // = A set of wrappers for auto-reset and manuaevents.
  static int event_init (ACE_event_t *event,
                         int manual_reset = 0,
                         int initial_state = 0,
                         int type = ACE_DEFAULT_SYNCH_TYPE,
                         const ACE_TCHAR *name = 0,
                         void *arg = 0,
                         LPSECURITY_ATTRIBUTES sa = 0);
  static int event_destroy (ACE_event_t *event);
  static int event_wait (ACE_event_t *event);
  static int event_timedwait (ACE_event_t *event,
                              ACE_Time_Value *timeout);
  static int event_signal (ACE_event_t *event);
  static int event_pulse (ACE_event_t *event);
  static int event_reset (ACE_event_t *event);

  // = A set of wrappers for semaphores.
  static int sema_destroy (ACE_sema_t *s);
  static int sema_init (ACE_sema_t *s,
                        u_int count,
                        int type = ACE_DEFAULT_SYNCH_TYPE,
                        const ACE_TCHAR *name = 0,
                        void *arg = 0,
                        int max = 0x7fffffff,
                        LPSECURITY_ATTRIBUTES sa = 0);
  static int sema_post (ACE_sema_t *s);
  static int sema_post (ACE_sema_t *s,
                        size_t release_count);
  static int sema_trywait (ACE_sema_t *s);
  static int sema_wait (ACE_sema_t *s);
  static int sema_wait (ACE_sema_t *s,
                        ACE_Time_Value &tv);

  // = A set of wrappers for System V semaphores.
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

  // = Thread scheduler interface.
  static int sched_params (const ACE_Sched_Params &, ACE_id_t id = ACE_SELF);
  // Set scheduling parameters.  An id of ACE_SELF indicates, e.g.,
  // set the parameters on the calling thread.

  // = A set of wrappers for System V shared memory.
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

  // = A set of wrappers for Signals.
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

  // = A set of wrappers for sockets.
  static ACE_HANDLE accept (ACE_HANDLE handle,
                            struct sockaddr *addr,
                            int *addrlen);
  // BSD-style <accept> (no QoS).
  static ACE_HANDLE accept (ACE_HANDLE handle,
                            struct sockaddr *addr,
                            int *addrlen,
                            const ACE_Accept_QoS_Params &qos_params);
  // QoS-enabled <accept>, which passes <qos_params> to <accept>.  If
  // the OS platform doesn't support QoS-enabled <accept> then the
  // <qos_params> are ignored and the BSD-style <accept> is called.
  static int bind (ACE_HANDLE s,
                   struct sockaddr *name,
                   int namelen);
  static int connect (ACE_HANDLE handle,
                      struct sockaddr *addr,
                      int addrlen);
  // BSD-style <connect> (no QoS).
  static int connect (ACE_HANDLE handle,
                      const sockaddr *addr,
                      int addrlen,
                      const ACE_QoS_Params &qos_params);
  // QoS-enabled <connect>, which passes <qos_params> to <connect>.
  // If the OS platform doesn't support QoS-enabled <connect> then the
  // <qos_params> are ignored and the BSD-style <connect> is called.

  static int closesocket (ACE_HANDLE s);
  static struct hostent *gethostbyaddr (const ACE_TCHAR *addr,
                                        int length,
                                        int type);
  static struct hostent *gethostbyname (const ACE_TCHAR *name);
  static struct hostent *gethostbyname2 (const ACE_TCHAR *name, int type);
  static struct hostent *gethostbyaddr_r (const ACE_TCHAR *addr,
                                          int length,
                                          int type,
                                          struct hostent *result,
                                          ACE_HOSTENT_DATA buffer,
                                          int *h_errnop);
  static struct hostent *gethostbyname_r (const ACE_TCHAR *name,
                                          struct hostent *result,
                                          ACE_HOSTENT_DATA buffer,
                                          int *h_errnop);
  static int getpeername (ACE_HANDLE handle,
                          struct sockaddr *addr,
                          int *addrlen);
  static struct protoent *getprotobyname (const ACE_TCHAR *name);
  static struct protoent *getprotobyname_r (const ACE_TCHAR *name,
                                            struct protoent *result,
                                            ACE_PROTOENT_DATA buffer);
  static struct protoent *getprotobynumber (int proto);
  static struct protoent *getprotobynumber_r (int proto,
                                              struct protoent *result,
                                              ACE_PROTOENT_DATA buffer);
  static struct servent *getservbyname (const ACE_TCHAR *svc,
                                        const ACE_TCHAR *proto);
  static struct servent *getservbyname_r (const ACE_TCHAR *svc,
                                          const ACE_TCHAR *proto,
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
  static long inet_addr (const ACE_TCHAR *name);
  static ACE_TCHAR *inet_ntoa (const struct in_addr addr);
  static int inet_aton (const ACE_TCHAR *strptr,
                        struct in_addr *addr);
  static const ACE_TCHAR *inet_ntop (int family,
                                     const void *addrptr,
                                     ACE_TCHAR *strptr,
                                     size_t len);
  static int inet_pton (int family,
                        const ACE_TCHAR *strptr,
                        void *addrptr);
  static int enum_protocols (int *protocols,
                             ACE_Protocol_Info *protocol_buffer,
                             u_long *buffer_length);
  // Retrieve information about available transport protocols
  // installed on the local machine.
  static ACE_HANDLE join_leaf (ACE_HANDLE socket,
                               const sockaddr *name,
                               int namelen,
                               const ACE_QoS_Params &qos_params);
  // Joins a leaf node into a QoS-enabled multi-point session.
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
  static int setsockopt (ACE_HANDLE handle,
                         int level,
                         int optname,
                         const char *optval,
                         int optlen);
  // QoS-enabled <ioctl> wrapper.
  static int shutdown (ACE_HANDLE handle,
                       int how);
  static ACE_HANDLE socket (int protocol_family,
                            int type,
                            int proto);

  // Create a BSD-style socket (no QoS).
  static ACE_HANDLE socket (int protocol_family,
                            int type,
                            int proto,
                            ACE_Protocol_Info *protocolinfo,
                            ACE_SOCK_GROUP g,
                            u_long flags);
  // Create a QoS-enabled socket.  If the OS platform doesn't support
  // QoS-enabled <socket> then the BSD-style <socket> is called.

  static int socketpair (int domain,
                         int type,
                         int protocol,
                         ACE_HANDLE sv[2]);
  static int socket_init (int version_high = 1,
                          int version_low = 1);
  // Initialize WinSock before first use (e.g., when a DLL is first
  // loaded or the first use of a socket() call.

  static int socket_fini (void);
  // Finalize WinSock after last use (e.g., when a DLL is unloaded).

  // = A set of wrappers for password routines.
  static void setpwent (void);
  static void endpwent (void);
  static struct passwd *getpwent (void);
  static struct passwd *getpwnam (const char *user);
  static struct passwd *getpwnam_r (const char *name,
                                    struct passwd *pwent,
                                    char *buffer,
                                    int buflen);

  // = A set of wrappers for regular expressions.
  static char *compile (const char *instring,
                        char *expbuf,
                        char *endbuf);
  static int step (const char *str,
                   char *expbuf);

  // @@ UNICODE: (brunsch) Can this be handled better?
  // The following WChar typedef and functions are used by TAO.  TAO
  // does not use wchar_t because the size of wchar_t is
  // platform-dependent. These are to be used for all
  // manipulate\ions of CORBA::WString.
  typedef ACE_UINT16 WChar;
  static u_int wslen (const WChar *);
  static WChar *wscpy (WChar *,
                       const WChar *);
  static int wscmp (const WChar *,
                    const WChar *);
  static int wsncmp (const WChar *,
                     const WChar *,
                     size_t len);

  // = A set of wrappers for threads

  // These are non-portable since they use ACE_thread_t and
  // ACE_hthread_t and will go away in a future release.
  static int thr_continue (ACE_hthread_t target_thread);
  static int thr_create (ACE_THR_FUNC func,
                         void *args,
                         long flags,
                         ACE_thread_t *thr_id,
                         ACE_hthread_t *t_handle = 0,
                         long priority = ACE_DEFAULT_THREAD_PRIORITY,
                         void *stack = 0,
                         size_t stacksize = 0,
                         ACE_Thread_Adapter *thread_adapter = 0);
  // Creates a new thread having <flags> attributes and running <func>
  // with <args> (if <thread_adapter> is non-0 then <func> and <args>
  // are ignored and are obtained from <thread_adapter>).  <thr_id>
  // and <t_handle> are set to the thread's ID and handle (?),
  // respectively.  The thread runs at <priority> priority (see
  // below).
  //
  // The <flags> are a bitwise-OR of the following:
  // = BEGIN<INDENT>
  // THR_CANCEL_DISABLE, THR_CANCEL_ENABLE, THR_CANCEL_DEFERRED,
  // THR_CANCEL_ASYNCHRONOUS, THR_BOUND, THR_NEW_LWP, THR_DETACHED,
  // THR_SUSPENDED, THR_DAEMON, THR_JOINABLE, THR_SCHED_FIFO,
  // THR_SCHED_RR, THR_SCHED_DEFAULT
  // = END<INDENT>
  //
  // By default, or if <priority> is set to
  // ACE_DEFAULT_THREAD_PRIORITY, an "appropriate" priority value for
  // the given scheduling policy (specified in <flags}>, e.g.,
  // <THR_SCHED_DEFAULT>) is used.  This value is calculated
  // dynamically, and is the median value between the minimum and
  // maximum priority values for the given policy.  If an explicit
  // value is given, it is used.  Note that actual priority values are
  // EXTREMEMLY implementation-dependent, and are probably best
  // avoided.
  //
  // Note that <thread_adapter> is always deleted by <thr_create>,
  // therefore it must be allocated with global operator new.

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
  static int thr_getspecific (ACE_thread_key_t key,
                              void **data);
  static int thr_keyfree (ACE_thread_key_t key);
  static int thr_key_detach (void *inst);
  static int thr_key_used (ACE_thread_key_t key);
  static size_t thr_min_stack (void);
  static int thr_setconcurrency (int hint);
  static int lwp_setparams (const ACE_Sched_Params &);
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

  static void unique_name (const void *object,
                           ACE_TCHAR *name,
                           size_t length);
  // This method uses process id and object pointer to come up with a
  // machine wide unique name.  The process ID will provide uniqueness
  // between processes on the same machine. The "this" pointer of the
  // <object> will provide uniqueness between other "live" objects in
  // the same process. The uniqueness of this name is therefore only
  // valid for the life of <object>.

  static ACE_thread_t NULL_thread;
  // This is necessary to deal with POSIX pthreads and their use of
  // structures for thread ids.

  static ACE_hthread_t NULL_hthread;
  // This is necessary to deal with POSIX pthreads and their use of
  // structures for thread handles.

  static ACE_thread_key_t NULL_key;
  // This is necessary to deal with POSIX pthreads and their use of
  // structures for TSS keys.

  static void mutex_lock_cleanup (void *mutex);
  // Handle asynchronous thread cancellation cleanup.

  static void cleanup_tss (const u_int main_thread);
  // Call TSS destructors for the current thread.  If the current
  // thread is the main thread, then the argument must be 1.
  // For private use of ACE_Object_Manager and ACE_Thread_Adapter only.

  static int scheduling_class (const char *class_name, ACE_id_t &);
  // Find the schedling class ID that corresponds to the class name.

  static int set_scheduling_params (const ACE_Sched_Params &,
                                    ACE_id_t id = ACE_SELF);
  // Friendly interface to <priocntl>(2).

  // Can't call the following priocntl, because that's a macro on Solaris.
  static int priority_control (ACE_idtype_t, ACE_id_t, int, void *);
  // Low-level interface to <priocntl>(2).*/
};

#include "ace/post.h"
#endif  /* ACE_ADAPTER_OS_H */
