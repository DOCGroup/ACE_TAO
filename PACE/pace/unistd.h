/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/unistd.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_UNISTD_H
#define PACE_UNISTD_H

#include "pace/config/defines.h"

#if (PACE_HAS_POSIX)
# include "pace/posix/unistd.h"
#elif (PACE_VXWORKS)
# include "pace/vxworks/unistd.h"
#elif (PACE_WIN32)
# include "pace/win32/unistd.h"
#endif

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

  /**
     PACE's implementation of the POSIX function _exit.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.2.2.
     */
#if (PACE_HAS_POSIX_MP_UOF)
  PACE_Export PACE_INLINE void pace__exit (int status);
#endif /* PACE_HAS_POSIX_MP_UOF */

  /**
     PACE's implementation of the POSIX function access.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 5.6.3.
     */
#if (PACE_HAS_POSIX_FS_UOF)
  PACE_Export PACE_INLINE int pace_access (const char * path, int amode);
#endif /* PACE_HAS_POSIX_FS_UOF */

  /**
     PACE's implementation of the POSIX function alarm.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.4.1.
     */
#if (PACE_HAS_POSIX_SIG_UOF)
  PACE_Export PACE_INLINE unsigned int pace_alarm (unsigned int seconds);
#endif /* PACE_HAS_POSIX_SIG_UOF */

  /**
     PACE's implementation of the POSIX function chdir.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 5.2.1.
     */
#if (PACE_HAS_POSIX_FS_UOF)
  PACE_Export PACE_INLINE int pace_chdir (const char * path);
#endif /* PACE_HAS_POSIX_FS_UOF */

  /**
     PACE's implementation of the POSIX function chown.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 5.6.5.
     */
#if (PACE_HAS_POSIX_FA_UOF)
  PACE_Export PACE_INLINE int pace_chown (const char * path,
                                          pace_uid_t owner,
                                          pace_gid_t group);
#endif /* PACE_HAS_POSIX_FA_UOF */

  /**
     PACE's implementation of the POSIX function close.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 6.3.1.
     */
#if (PACE_HAS_POSIX_DI_UOF)
  PACE_Export PACE_INLINE int pace_close (PACE_HANDLE fildes);
#endif /* PACE_HAS_POSIX_DI_UOF */

  /**
     PACE's implementation of the POSIX function dup.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 6.2.1.
     */
#if (PACE_HAS_POSIX_FM_UOF)
  PACE_Export PACE_INLINE PACE_HANDLE pace_dup (PACE_HANDLE fildes);
#endif /* PACE_HAS_POSIX_FM_UOF */

  /**
     PACE's implementation of the POSIX function dup2.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 6.2.1.
     */
#if (PACE_HAS_POSIX_FM_UOF)
  PACE_Export PACE_INLINE PACE_HANDLE pace_dup2 (PACE_HANDLE fildes,
                                                 PACE_HANDLE fildes2);
#endif /* PACE_HAS_POSIX_FM_UOF */

  /**
     PACE's implementation of the POSIX function execl.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.1.2.
     */
#if (PACE_HAS_POSIX_MP_UOF)
# define pace_execl execl
#endif /* PACE_HAS_POSIX_MP_UOF */
  /*  PACE_Export PACE_INLINE int pace_execl (const char* path,
      const char* arg, ...); */

  /**
     PACE's implementation of the POSIX function execle.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.1.2.
     */
#if (PACE_HAS_POSIX_MP_UOF)
# define pace_execle execle
#endif /* PACE_HAS_POSIX_MP_UOF */
  /*  PACE_Export PACE_INLINE int pace_execle (const char* path,
      const char* arg, ...);*/

  /**
     PACE's implementation of the POSIX function execlp.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.1.2.
     */
#if (PACE_HAS_POSIX_MP_UOF)
# define pace_execlp execlp
#endif /* PACE_HAS_POSIX_MP_UOF */
  /* PACE_Export PACE_INLINE int pace_execlp (const char* file,
     const char* arg,  ...);*/

  /**
     PACE's implementation of the POSIX function execv.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.1.2.
     */
#if (PACE_HAS_POSIX_MP_UOF)
  PACE_Export PACE_INLINE int pace_execv (const char * path,
                                          char * const argv[]);
#endif /* PACE_HAS_POSIX_MP_UOF */

  /**
     PACE's implementation of the POSIX function execve.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.1.2.
     */
#if (PACE_HAS_POSIX_MP_UOF)
  PACE_Export PACE_INLINE int pace_execve (const char * path,
                                           char * const argv[],
                                           char * const envp[]);
#endif /* PACE_HAS_POSIX_MP_UOF */

  /**
     PACE's implementation of the POSIX function execvp.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.1.2.
     */
#if (PACE_HAS_POSIX_MP_UOF)
  PACE_Export PACE_INLINE int pace_execvp (const char * file,
                                           char * const argv[]);
#endif /* PACE_HAS_POSIX_MP_UOF */

  /**
     PACE's implementation of the POSIX function fdatasync.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 6.6.2.
     */
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  PACE_Export PACE_INLINE int pace_fdatasync (PACE_HANDLE fildes);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

  /**
     PACE's implementation of the POSIX function fork.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.1.1.
     */
#if (PACE_HAS_POSIX_MP_UOF)
  PACE_Export PACE_INLINE pace_pid_t pace_fork ();
#endif /* PACE_HAS_POSIX_MP_UOF */

  /**
     PACE's implementation of the POSIX function fpathconf.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 5.7.1.
     */
#if (PACE_HAS_POSIX_FS_UOF)
  PACE_Export PACE_INLINE long pace_fpathconf (PACE_HANDLE fildes, int name);
#endif /* PACE_HAS_POSIX_FS_UOF */

  /**
     PACE's implementation of the POSIX function fsync.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 6.6.1.
     */
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  PACE_Export PACE_INLINE int pace_fsync (PACE_HANDLE fildes);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

  /**
     PACE's implementation of the POSIX function ftruncate.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 5.6.7.
     */
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  PACE_Export PACE_INLINE int pace_ftruncate (PACE_HANDLE fildes,
                                              pace_off_t length);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

  /**
     PACE's implementation of the POSIX function getcwd.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 5.2.2.
     */
#if (PACE_HAS_POSIX_FS_UOF)
  PACE_Export PACE_INLINE char * pace_getcwd (char * buf, size_t size);
#endif /* PACE_HAS_POSIX_FS_UOF */

  /**
     PACE's implementation of the POSIX function getegid.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 4.2.1.
     */
#if (PACE_HAS_POSIX_UG_UOF)
  PACE_Export PACE_INLINE pace_uid_t pace_getegid ();
#endif /* PACE_HAS_POSIX_UG_UOF */

  /**
     PACE's implementation of the POSIX function geteuid.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 4.2.1.
     */
#if (PACE_HAS_POSIX_UG_UOF)
  PACE_Export PACE_INLINE pace_uid_t pace_geteuid ();
#endif /* PACE_HAS_POSIX_UG_UOF */

  /**
     PACE's implementation of the POSIX function getgid.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 4.2.1.
     */
#if (PACE_HAS_POSIX_UG_UOF)
  PACE_Export PACE_INLINE pace_uid_t pace_getgid ();
#endif /* PACE_HAS_POSIX_UG_UOF */

  /**
     PACE's implementation of the POSIX function getgroups.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 4.2.3.
     */
#if (PACE_HAS_POSIX_UG_UOF)
  PACE_Export PACE_INLINE int pace_getgroups (int gidsetsize,
                                              pace_gid_t grouplist[]);
#endif /* PACE_HAS_POSIX_UG_UOF */

  /**
     PACE's implementation of the POSIX function getlogin.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 4.2.4.
     */
#if (PACE_HAS_POSIX_UG_UOF)
  PACE_Export PACE_INLINE char * pace_getlogin ();
#endif /* PACE_HAS_POSIX_UG_UOF */

  /**
     PACE's implementation of the POSIX function getlogin_r.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 4.2.4.
     */
#if (PACE_HAS_POSIX_UGR_UOF)
  PACE_Export PACE_INLINE int pace_getlogin_r (char * name, size_t namesize);
#endif /* PACE_HAS_POSIX_UGR_UOF */
  /* Requires PACE_HAS_POSIX_PTHREAD_SEMANTICS. */

  /**
     PACE's implementation of the POSIX function getpgrp.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 4.3.1.
     */
#if (PACE_HAS_POSIX_UG_UOF)
  PACE_Export PACE_INLINE pace_pid_t pace_getpgrp ();
#endif /* PACE_HAS_POSIX_UG_UOF */

  /**
     PACE's implementation of the POSIX function getpid.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 4.1.1.
     */
#if (PACE_HAS_POSIX_MP_UOF)
  PACE_Export PACE_INLINE pace_pid_t pace_getpid ();
#endif /* PACE_HAS_POSIX_MP_UOF */

  /**
     PACE's implementation of the POSIX function getppid.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 4.1.1.
     */
#if (PACE_HAS_POSIX_MP_UOF)
  PACE_Export PACE_INLINE pace_pid_t pace_getppid ();
#endif /* PACE_HAS_POSIX_MP_UOF */

  /**
     PACE's implementation of the POSIX function getuid.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 4.2.1.
     */
#if (PACE_HAS_POSIX_UG_UOF)
  PACE_Export PACE_INLINE pace_uid_t pace_getuid ();
#endif /* PACE_HAS_POSIX_UG_UOF */

  /**
     PACE's implementation of the POSIX function isatty.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 4.7.2.
     */
#if (PACE_HAS_POSIX_DS_UOF)
  PACE_Export PACE_INLINE int pace_isatty (int fildes);
#endif /* PACE_HAS_POSIX_DS_UOF */

  /**
     PACE's implementation of the POSIX function link.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 5.3.4.
     */
#if (PACE_HAS_POSIX_FS_UOF)
  PACE_Export PACE_INLINE int pace_link (const char * existing,
                                         const char * new_link);
#endif /* PACE_HAS_POSIX_FS_UOF */

  /**
     PACE's implementation of the POSIX function lseek.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 6.5.3.
     */
#if (PACE_HAS_POSIX_FM_UOF)
  PACE_Export PACE_INLINE pace_off_t pace_lseek (PACE_HANDLE fildes,
                                                 off_t offset,
                                                 int whence);
#endif /* PACE_HAS_POSIX_FM_UOF */

  /**
     PACE's implementation of the POSIX function pathconf.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 5.7.1.
     */
#if (PACE_HAS_POSIX_FS_UOF)
  PACE_Export PACE_INLINE long pace_pathconf (const char * path, int name);
#endif /* PACE_HAS_POSIX_FS_UOF */

  /**
     PACE's implementation of the POSIX function pause.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.4.2.
     */
#if (PACE_HAS_POSIX_SIG_UOF)
  PACE_Export PACE_INLINE int pace_pause ();
#endif /* PACE_HAS_POSIX_SIG_UOF */

  /**
     PACE's implementation of the POSIX function pipe.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 6.1.1.
     */
#if (PACE_HAS_POSIX_P_UOF)
  PACE_Export PACE_INLINE int pace_pipe (PACE_HANDLE fildes[2]);
#endif /* PACE_HAS_POSIX_P_UOF */

  /**
     PACE's implementation of the POSIX function read.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 6.4.1.
     */
#if (PACE_HAS_POSIX_DI_UOF)
  PACE_Export PACE_INLINE pace_ssize_t pace_read (PACE_HANDLE fildes,
                                                  void * buf,
                                                  size_t nbyte);
#endif /* PACE_HAS_POSIX_DI_UOF */

  /**
     PACE's implementation of the POSIX function rmdir.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 5.5.2.
     */
#if (PACE_HAS_POSIX_FS_UOF)
  PACE_Export PACE_INLINE int pace_rmdir (const char * path);
#endif /* PACE_HAS_POSIX_FS_UOF */

  /**
     PACE's implementation of the POSIX function setgid.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 4.2.2.
     */
#if (PACE_HAS_POSIX_UG_UOF)
  PACE_Export PACE_INLINE int pace_setgid (pace_gid_t gid);
#endif /* PACE_HAS_POSIX_UG_UOF */

  /**
     PACE's implementation of the POSIX function setpgid.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 4.3.3.
     */
#if (PACE_HAS_POSIX_JC_UOF)
  PACE_Export PACE_INLINE int pace_setpgid (pace_pid_t pid, pace_pid_t pgid);
#endif /* PACE_HAS_POSIX_JC_UOF */

  /**
     PACE's implementation of the POSIX function setsid.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 4.3.2.
     */
#if (PACE_HAS_POSIX_UG_UOF)
  PACE_Export PACE_INLINE pace_pid_t pace_setsid ();
#endif /* PACE_HAS_POSIX_UG_UOF */

  /**
     PACE's implementation of the POSIX function setuid.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 4.2.2.
     */
#if (PACE_HAS_POSIX_UG_UOF)
  PACE_Export PACE_INLINE int pace_setuid (pace_uid_t uid);
#endif /* PACE_HAS_POSIX_UG_UOF */

  /**
     PACE's implementation of the POSIX function sleep.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.4.3.
     */
#if (PACE_HAS_POSIX_MP_UOF)
  PACE_Export PACE_INLINE unsigned pace_sleep (unsigned int seconds);
#endif /* PACE_HAS_POSIX_MP_UOF */

  /**
    PACE's implementation of the POSIX function sysconf.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 4.8.1.
     */
#if (PACE_HAS_POSIX_SP_UOF)
  PACE_Export PACE_INLINE long pace_sysconf (int name);
#endif /* PACE_HAS_POSIX_SP_UOF */

  /**
     PACE's implementation of the POSIX function ttyname.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 7.2.3.1.
     */
#if (PACE_HAS_POSIX_JC_UOF)
  PACE_Export PACE_INLINE pace_pid_t pace_tcgetpgrp (PACE_HANDLE fildes);
#endif /* PACE_HAS_POSIX_JC_UOF */

  /**
     PACE's implementation of the POSIX function ttyname.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 7.2.3.1.
     */
#if (PACE_HAS_POSIX_JC_UOF)
  PACE_Export PACE_INLINE int pace_tcsetpgrp (PACE_HANDLE fildes,
                                              pace_pid_t pgrp_id);
#endif /* PACE_HAS_POSIX_JC_UOF */

  /**
     PACE's implementation of the POSIX function ttyname.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 4.7.2.
     */
#if (PACE_HAS_POSIX_DS_UOF)
  PACE_Export PACE_INLINE char * pace_ttyname (PACE_HANDLE fildes);
#endif /* PACE_HAS_POSIX_DS_UOF */

  /**
     PACE's implementation of the POSIX function ttyname_r.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 4.7.2.
     */
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  PACE_Export PACE_INLINE int pace_ttyname_r (PACE_HANDLE fildes,
                                              char * name,
                                              size_t namesize);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */
  /* Requires PACE_HAS_POSIX_PTHREAD_SEMANTICS. */

  /**
     PACE's implementation of the POSIX function unlink.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 5.5.1.
     */
#if (PACE_HAS_POSIX_FS_UOF)
  PACE_Export PACE_INLINE int pace_unlink (const char * path);
#endif /* PACE_HAS_POSIX_FS_UOF */

  /**
     PACE's implementation of the POSIX function write.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 6.4.2.
     */
#if (PACE_HAS_POSIX_DI_UOF)
  PACE_Export PACE_INLINE pace_ssize_t pace_write (PACE_HANDLE fildes,
                                                   const void * buf,
                                                   size_t nbyte);
#endif /* PACE_HAS_POSIX_DI_UOF */

#if defined (PACE_HAS_INLINE)
#  if (PACE_HAS_POSIX)
#    include "pace/posix/unistd.inl"
#  elif (PACE_VXWORKS)
#    include "pace/vxworks/unistd.inl"
#  elif (PACE_WIN32)
#    include "pace/win32/unistd.inl"
#  endif
#endif /* PACE_HAS_INLINE */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_UNISTD_H */
