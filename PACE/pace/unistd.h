/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    unistd.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */


#ifndef PACE_UNISTD_H
#define PACE_UNISTD_H


#include "pace/defines.h"
#include "pace/sys/types.h"
#include <stropts.h>


# if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
# endif /* PACE_HAS_CPLUSPLUS */

  PACE_INLINE void pace__exit (int status);

  PACE_INLINE unsigned int pace_alarm (unsigned int seconds);

  PACE_INLINE int pace_access (const char * path, int amode);

  PACE_INLINE int pace_chdir (const char * path);

  PACE_INLINE int pace_chown (const char * path, uid_t owner, gid_t group);

  PACE_INLINE int pace_close (int fildes);

  PACE_INLINE int pace_dup (int fildes);

  PACE_INLINE int pace_dup2 (int fildes, int fildes2);

  #define pace_execl(X)  execl X
  #define pace_execle(X) execle X
  #define pace_execlp(X) execlp X
  /* int execl (const char *path, const char *arg0, ...,
   *            const char *argn, char * (*//*NULL*//*) );
   * int execle (const char *path, char *const arg0[], ...,
   *             const char *argn, char * (*//*NULL*//*), char *const envp[]);
   * int execlp (const char *file, const char  *arg0,  ...,
   *             const char *argn, char * (*//*NULL*//*));
   *
   * PLEASE PAY SPECIAL ATTENTION HERE!
   * This is a macro and requires an additional set of parenthesis
   * surrounding the arguments.
   */

  PACE_INLINE int pace_execv (const char * path, char * const argv[]);

  PACE_INLINE int pace_execve (const char * path, char * const argv[],
                               char * const envp[]);

  PACE_INLINE int pace_execvp (const char * file, char * const argv[]);

  PACE_INLINE int pace_fdatasync (int fildes);

  PACE_INLINE pid_t pace_fork ();

  PACE_INLINE long pace_fpathconf (int fildes, int name);

  PACE_INLINE int pace_fsync (int fildes);

  PACE_INLINE int pace_ftruncate (int fildes, off_t length);

  PACE_INLINE char * pace_getcwd (char * buf, size_t size);

  PACE_INLINE uid_t pace_getegid ();

  PACE_INLINE uid_t pace_geteuid ();

  PACE_INLINE uid_t pace_getgid ();

  PACE_INLINE int pace_getgroups (int gidsetsize, gid_t grouplist[]);

  PACE_INLINE char * pace_getlogin ();

  PACE_INLINE int pace_getlogin_r (char * name, size_t namesize);
  /* Requires PACE_HAS_POSIX_PTHREAD_SEMANTICS. */

  PACE_INLINE pid_t pace_getpgrp ();

  PACE_INLINE pid_t pace_getpid ();

  PACE_INLINE pid_t pace_getppid ();

  PACE_INLINE uid_t pace_getuid ();

  #define pace_ioctl(X) ioctl X
  /* int ioctl (int fildes, int request, (*//* args *//*))
   *
   * PLEASE PAY SPECIAL ATTENTION HERE!
   * This is a macro and requires an additional set of parenthesis
   * surrounding the arguments.
   */


  PACE_INLINE int pace_isatty (int fildes);

  PACE_INLINE int pace_link (const char * existing, const char * new_link);

  PACE_INLINE off_t pace_lseek (int fildes, off_t offset, int whence);

  PACE_INLINE long pace_pathconf (const char * path, int name);

  PACE_INLINE int pace_pause ();

  PACE_INLINE int pace_pipe (int fildes[2]);

  PACE_INLINE ssize_t pace_read (int fildes, void * buf, size_t nbyte);

  PACE_INLINE int pace_rmdir (const char * path);

  PACE_INLINE int pace_setgid (gid_t gid);

  PACE_INLINE int pace_setpgid (pid_t pid, pid_t pgid);

  PACE_INLINE pid_t pace_setsid ();

  PACE_INLINE int pace_setuid (uid_t uid);

  PACE_INLINE unsigned pace_sleep (unsigned int seconds);

  PACE_INLINE long pace_sysconf (int name);

  PACE_INLINE char * pace_ttyname (int fildes);

  PACE_INLINE int pace_ttyname_r (int fildes, char * name, size_t namesize);
  /* Requires PACE_HAS_POSIX_PTHREAD_SEMANTICS. */

  PACE_INLINE int pace_unlink (const char * path);

  PACE_INLINE ssize_t pace_write (int fildes, const void * buf, size_t nbyte);

# if defined (PACE_HAS_CPLUSPLUS)
}
# endif /* PACE_HAS_CPLUSPLUS */

# if defined (PACE_HAS_INLINE)
# include "unistd.inl"
# endif /* PACE_HAS_INLINE */


#endif /* PACE_UNISTD_H */
