/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/posix/unistd.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_UNISTD_H
#define PACE_UNISTD_H

#include "pace/sys/types.h"
#include <stropts.h>
#include <unistd.h>

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

  /**
     PACE's implementation of the POSIX function _exit.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.2.2.
   */
  PACE_INLINE void pace__exit (int status);

  /**
     PACE's implementation of the POSIX function alarm.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.4.1.
   */
  PACE_INLINE unsigned int pace_alarm (unsigned int seconds);

  /**
     PACE's implementation of the POSIX function access.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 5.6.3.
   */
  PACE_INLINE int pace_access (const char * path, int amode);

  /**
     PACE's implementation of the POSIX function chdir.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 5.2.1.
   */
  PACE_INLINE int pace_chdir (const char * path);

  /**
     PACE's implementation of the POSIX function chown.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 5.6.5.
   */
  PACE_INLINE int pace_chown (const char * path, uid_t owner, pace_gid_t group);

  /**
     PACE's implementation of the POSIX function close.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 6.3.1.
   */
  PACE_INLINE int pace_close (int fildes);

  /**
     PACE's implementation of the POSIX function ctermid.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 4.7.1.
     */
  PACE_INLINE char * pace_ctermid (char * s);


  /**
     PACE's implementation of the POSIX function dup.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 6.2.1.
   */
  PACE_INLINE int pace_dup (int fildes);

  /**
     PACE's implementation of the POSIX function dup2.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 6.2.1.
   */
  PACE_INLINE int pace_dup2 (int fildes, int fildes2);

  /**
     PACE's implementation of the POSIX function execl.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.1.2.
   */
  int pace_execl (const char* path, const char* arg, ...);

  /**
     PACE's implementation of the POSIX function execle.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.1.2.
   */
  int pace_execle (const char* path, const char* arg, ...);

  /**
     PACE's implementation of the POSIX function execlp.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.1.2.
   */
  int pace_execlp (const char* file, const char* arg,  ...);

  /**
     PACE's implementation of the POSIX function execv.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.1.2.
   */
  PACE_INLINE int pace_execv (const char * path, char * const argv[]);

  /**
     PACE's implementation of the POSIX function execve.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.1.2.
   */
  PACE_INLINE int pace_execve (const char * path, char * const argv[],
                               char * const envp[]);

  /**
     PACE's implementation of the POSIX function execvp.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.1.2.
   */
  PACE_INLINE int pace_execvp (const char * file, char * const argv[]);

  /**
     PACE's implementation of the POSIX function fdatasync.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 6.6.2.
   */
  PACE_INLINE int pace_fdatasync (int fildes);

  /**
     PACE's implementation of the POSIX function fork.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.1.1.
   */
  PACE_INLINE pace_pid_t pace_fork ();

  /**
     PACE's implementation of the POSIX function fpathconf.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 5.7.1.
   */
  PACE_INLINE long pace_fpathconf (int fildes, int name);

  /**
     PACE's implementation of the POSIX function fsync.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 6.6.1.
   */
  PACE_INLINE int pace_fsync (int fildes);

  /**
     PACE's implementation of the POSIX function ftruncate.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 5.6.7.
   */
  PACE_INLINE int pace_ftruncate (int fildes, pace_off_t length);

  /**
     PACE's implementation of the POSIX function getcwd.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 5.2.2.
   */
  PACE_INLINE char * pace_getcwd (char * buf, size_t size);

  /**
     PACE's implementation of the POSIX function getegid.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 4.2.1.
   */
  PACE_INLINE uid_t pace_getegid ();

  /**
     PACE's implementation of the POSIX function geteuid.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 4.2.1.
   */
  PACE_INLINE uid_t pace_geteuid ();

  /**
     PACE's implementation of the POSIX function getgid.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 4.2.1.
   */
  PACE_INLINE uid_t pace_getgid ();

  /**
     PACE's implementation of the POSIX function getgroups.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 4.2.3.
   */
  PACE_INLINE int pace_getgroups (int gidsetsize, pace_gid_t grouplist[]);

  /**
     PACE's implementation of the POSIX function getlogin.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 4.2.4.
   */
  PACE_INLINE char * pace_getlogin ();

  /**
     PACE's implementation of the POSIX function getlogin_r.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 4.2.4.
   */
  PACE_INLINE int pace_getlogin_r (char * name, size_t namesize);
  /* Requires PACE_HAS_POSIX_PTHREAD_SEMANTICS. */

  /**
     PACE's implementation of the POSIX function getpgrp.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 4.3.1.
   */
  PACE_INLINE pace_pid_t pace_getpgrp ();

  /**
     PACE's implementation of the POSIX function getpid.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 4.1.1.
   */
  PACE_INLINE pace_pid_t pace_getpid ();

  /**
     PACE's implementation of the POSIX function getppid.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 4.1.1.
   */
  PACE_INLINE pace_pid_t pace_getppid ();

  /**
     PACE's implementation of the POSIX function getuid.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 4.2.1.
   */
  PACE_INLINE uid_t pace_getuid ();

  /**
     PACE's implementation of the POSIX function isatty.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 4.7.2.
   */
  PACE_INLINE int pace_isatty (int fildes);

  /**
     PACE's implementation of the POSIX function link.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 5.3.4.
   */
  PACE_INLINE int pace_link (const char * existing, const char * new_link);

  /**
     PACE's implementation of the POSIX function lseek.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 6.5.3.
   */
  PACE_INLINE pace_off_t pace_lseek (int fildes, off_t offset, int whence);

  /**
     PACE's implementation of the POSIX function pathconf.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 5.7.1.
   */
  PACE_INLINE long pace_pathconf (const char * path, int name);

  /**
     PACE's implementation of the POSIX function pause.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.4.2.
   */
  PACE_INLINE int pace_pause ();

  /**
     PACE's implementation of the POSIX function pipe.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 6.1.1.
   */
  PACE_INLINE int pace_pipe (int fildes[2]);

  /**
     PACE's implementation of the POSIX function read.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 6.4.1.
   */
  PACE_INLINE ssize_t pace_read (int fildes, void * buf, size_t nbyte);

  /**
     PACE's implementation of the POSIX function rmdir.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 5.5.2.
   */
  PACE_INLINE int pace_rmdir (const char * path);

  /**
     PACE's implementation of the POSIX function setgid.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 4.2.2.
   */
  PACE_INLINE int pace_setgid (pace_gid_t gid);

  /**
     PACE's implementation of the POSIX function setpgid.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 4.3.3.
   */
  PACE_INLINE int pace_setpgid (pace_pid_t pid, pid_t pgid);

  /**
     PACE's implementation of the POSIX function setsid.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 4.3.2.
   */
  PACE_INLINE pace_pid_t pace_setsid ();

  /**
     PACE's implementation of the POSIX function setuid.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 4.2.2.
   */
  PACE_INLINE int pace_setuid (uid_t uid);

  /**
     PACE's implementation of the POSIX function sleep.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.4.3.
   */
  PACE_INLINE unsigned pace_sleep (unsigned int seconds);

  /**
     PACE's implementation of the POSIX function sysconf.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 4.8.1.
   */
  PACE_INLINE long pace_sysconf (int name);

  /**
     PACE's implementation of the POSIX function ttyname.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 4.7.2.
   */
  PACE_INLINE char * pace_ttyname (int fildes);

  /**
     PACE's implementation of the POSIX function ttyname_r.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 4.7.2.
   */
  PACE_INLINE int pace_ttyname_r (int fildes, char * name, size_t namesize);
  /* Requires PACE_HAS_POSIX_PTHREAD_SEMANTICS. */

  /**
     PACE's implementation of the POSIX function unlink.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 5.5.1.
   */
  PACE_INLINE int pace_unlink (const char * path);

  /**
     PACE's implementation of the POSIX function write.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 6.4.2.
   */
  PACE_INLINE ssize_t pace_write (int fildes, const void * buf, size_t nbyte);

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#if defined (PACE_HAS_INLINE)
# include "pace/posix/unistd.inl"
#endif /* PACE_HAS_INLINE */

#endif /* PACE_UNISTD_H */
