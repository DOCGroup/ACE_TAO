/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/vxworks/unistd.inl
 *
 * = AUTHOR
 *    Joe Hoffert
 *
 * ============================================================================= */

#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#if (PACE_HAS_POSIX_MP_UOF)
PACE_INLINE
void
pace__exit (int status)
{
  exit (status);
  return;
}
#endif /* PACE_HAS_POSIX_MP_UOF */

#if (PACE_HAS_POSIX_FS_UOF)
PACE_INLINE
int
pace_access (const char * path, int amode)
{
  PACE_UNUSED_ARG(path);
  PACE_UNUSED_ARG(amode);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_SIG_UOF)
PACE_INLINE
unsigned int
pace_alarm (unsigned int seconds)
{
  PACE_UNUSED_ARG(seconds);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_SIG_UOF */

#if (PACE_HAS_POSIX_FS_UOF)
PACE_INLINE
int
pace_chdir (const char * path)
{
  return chdir (PACE_NONCONST_ARG_CAST (char *) path);
}
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_FA_UOF)
PACE_INLINE
int
pace_chown (const char * path, uid_t owner, pace_gid_t group)
{
  PACE_UNUSED_ARG(path);
  PACE_UNUSED_ARG(owner);
  PACE_UNUSED_ARG(group);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_FA_UOF */

#if (PACE_HAS_POSIX_DI_UOF)
PACE_INLINE
int
pace_close (PACE_HANDLE fildes)
{
  return close (fildes);
}
#endif /* PACE_HAS_POSIX_DI_UOF */

#if (PACE_HAS_POSIX_FM_UOF)
PACE_INLINE
PACE_HANDLE
pace_dup (PACE_HANDLE fildes)
{
  PACE_UNUSED_ARG(fildes);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_FM_UOF */

#if (PACE_HAS_POSIX_FM_UOF)
PACE_INLINE
PACE_HANDLE
pace_dup2 (PACE_HANDLE fildes, PACE_HANDLE fildes2)
{
  PACE_UNUSED_ARG(fildes);
  PACE_UNUSED_ARG(fildes2);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_FM_UOF */

#if (PACE_HAS_POSIX_MP_UOF)
PACE_INLINE
int
pace_execv (const char * path,
            char * const argv[])
{
  PACE_UNUSED_ARG(path);
  PACE_UNUSED_ARG(argv);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
  /* if successful, this operation does NOT return */
}
#endif /* PACE_HAS_POSIX_MP_UOF */

#if (PACE_HAS_POSIX_MP_UOF)
PACE_INLINE
int
pace_execve (const char * path,
             char * const argv[],
             char * const envp[])
{
  PACE_UNUSED_ARG(path);
  PACE_UNUSED_ARG(argv);
  PACE_UNUSED_ARG(envp);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
  /* if successful, this operation does NOT return */
}
#endif /* PACE_HAS_POSIX_MP_UOF */

#if (PACE_HAS_POSIX_MP_UOF)
PACE_INLINE
int
pace_execvp (const char * file,
             char * const argv[])
{
  PACE_UNUSED_ARG(file);
  PACE_UNUSED_ARG(argv);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
  /* if successful, this operation does NOT return */
}
#endif /* PACE_HAS_POSIX_MP_UOF */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_fdatasync (PACE_HANDLE fildes)
{
  PACE_UNUSED_ARG(fildes);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_MP_UOF)
PACE_INLINE
pid_t
pace_fork ()
{
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_MP_UOF */

#if (PACE_HAS_POSIX_FS_UOF)
PACE_INLINE
long
pace_fpathconf (PACE_HANDLE fildes, int name)
{
  PACE_UNUSED_ARG(fildes);
  PACE_UNUSED_ARG(name);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_fsync (PACE_HANDLE fildes)
{
  PACE_UNUSED_ARG(fildes);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_ftruncate (PACE_HANDLE fildes, pace_off_t length)
{
  return ftruncate (fildes, length);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_FS_UOF)
PACE_INLINE
char *
pace_getcwd (char * buf, size_t size)
{
  return getcwd (buf, size);
}
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_UG_UOF)
PACE_INLINE
uid_t
pace_getegid ()
{
  /* getegid() is not supported:  just one user anyway */
  return 0;
}
#endif /* PACE_HAS_POSIX_UGR_UOF */

#if (PACE_HAS_POSIX_UG_UOF)
PACE_INLINE
uid_t
pace_geteuid ()
{
  /* geteuid() is not supported:  just one user anyway */
  return 0;
}
#endif /* PACE_HAS_POSIX_UGR_UOF */

#if (PACE_HAS_POSIX_UG_UOF)
PACE_INLINE
int
pace_getgroups (int gidsetsize, pace_gid_t grouplist[])
{
  /* getgroups() is not supported:  just one user anyway */
  PACE_UNUSED_ARG(gidsetsize);
  PACE_UNUSED_ARG(grouplist);
  return 0;
}
#endif /* PACE_HAS_POSIX_UG_UOF */

#if (PACE_HAS_POSIX_UG_UOF)
PACE_INLINE
uid_t
pace_getgid ()
{
  /* getgid() is not supported:  just one user anyway */
  return 0;
}
#endif /* PACE_HAS_POSIX_UGR_UOF */

#if (PACE_HAS_POSIX_UG_UOF)
PACE_INLINE
char*
pace_getlogin ()
{
  PACE_ERRNO_NO_SUPPORT_RETURN ((char *) 0);
}
#endif /* PACE_HAS_POSIX_UGR_UOF */

#if (PACE_HAS_POSIX_UGR_UOF)
PACE_INLINE
int
pace_getlogin_r (char * name, size_t namesize)
{
  PACE_UNUSED_ARG(name);
  PACE_UNUSED_ARG(namesize);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_UGR_UOF */

#if (PACE_HAS_POSIX_UG_UOF)
PACE_INLINE
pid_t
pace_getpgrp ()
{
  /* getpgrp() is not supported:  just one user anyway */
  return 0;
}
#endif /* PACE_HAS_POSIX_UG_UOF */

#if (PACE_HAS_POSIX_MP_UOF)
PACE_INLINE
pid_t
pace_getpid ()
{
  /* getpid() is not supported:  just one user anyway */
  return 0;
}
#endif /* PACE_HAS_POSIX_MP_UOF */

#if (PACE_HAS_POSIX_MP_UOF)
PACE_INLINE
pid_t
pace_getppid ()
{
  /* getppid() is not supported:  just one user anyway */
  return 0;
}
#endif /* PACE_HAS_POSIX_MP_UOF */

#if (PACE_HAS_POSIX_UG_UOF)
PACE_INLINE
uid_t
pace_getuid ()
{
  /* getuid() is not supported:  just one user anyways */
  return 0;
}
#endif /* PACE_HAS_POSIX_UG_UOF */

#if (PACE_HAS_POSIX_DS_UOF)
PACE_INLINE
int
pace_isatty (int fildes)
{
  return isatty (fildes);
}
#endif /* PACE_HAS_POSIX_DS_UOF */

#if (PACE_HAS_POSIX_FS_UOF)
PACE_INLINE
int
pace_link (const char * existing, const char * new_link)
{
  PACE_UNUSED_ARG(existing);
  PACE_UNUSED_ARG(new_link);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_FM_UOF)
PACE_INLINE
pace_off_t
pace_lseek (PACE_HANDLE fildes, pace_off_t offset, int whence)
{
  return lseek (fildes, offset, whence);
}
#endif /* PACE_HAS_POSIX_FM_UOF */

#if (PACE_HAS_POSIX_FS_UOF)
PACE_INLINE
long
pace_pathconf (const char * path, int name)
{
  PACE_UNUSED_ARG(path);
  PACE_UNUSED_ARG(name);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_SIG_UOF)
PACE_INLINE
int
pace_pause ()
{
  return pause ();
}
#endif /* PACE_HAS_POSIX_SIG_UOF */

#if (PACE_HAS_POSIX_P_UOF)
PACE_INLINE
int
pace_pipe (PACE_HANDLE fildes[2])
{
  PACE_UNUSED_ARG(fildes);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_P_UOF */

#if (PACE_HAS_POSIX_DI_UOF)
PACE_INLINE
ssize_t
pace_read (PACE_HANDLE fildes, void * buf, size_t nbyte)
{
  return read (fildes, PACE_NONCONST_ARG_CAST (char *) buf, nbyte);
}
#endif /* PACE_HAS_POSIX_DI_UOF */

#if (PACE_HAS_POSIX_FS_UOF)
PACE_INLINE
int
pace_rmdir (const char * path)
{
  return rmdir (PACE_NONCONST_ARG_CAST (char *) path);
}
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_UG_UOF)
PACE_INLINE
int
pace_setgid (pace_gid_t gid)
{
  /* setuid() is not supported:  just one user anyways */
  return 0;
}
#endif /* PACE_HAS_POSIX_UG_UOF */

#if (PACE_HAS_POSIX_JC_UOF)
PACE_INLINE
int
pace_setpgid (pid_t pid, pid_t pgid)
{
  /* setpgid() is not supported:  just one user anyways */
  return 0;
}
#endif /* PACE_HAS_POSIX_JC_UOF */

#if (PACE_HAS_POSIX_UG_UOF)
PACE_INLINE
pid_t
pace_setsid ()
{
  /* setsid() is not supported:  just one user anyways */
  return 0;
}
#endif /* PACE_HAS_POSIX_UG_UOF */

#if (PACE_HAS_POSIX_UG_UOF)
PACE_INLINE
int
pace_setuid (uid_t uid)
{
  /* setuid() is not supported:  just one user anyways */
  return 0;
}
#endif /* PACE_HAS_POSIX_UG_UOF */

#if (PACE_HAS_POSIX_MP_UOF)
PACE_INLINE
unsigned int
pace_sleep (unsigned int seconds)
{
  struct timespec rqtp;
  /* Initializer doesn't work with Green Hills 1.8.7 */
  rqtp.tv_sec = seconds;
  rqtp.tv_nsec = 0L;
  return nanosleep (&rqtp, 0);
}
#endif /* PACE_HAS_POSIX_MP_UOF */

#if (PACE_HAS_POSIX_SP_UOF)
PACE_INLINE
long
pace_sysconf (int name)
{
  PACE_UNUSED_ARG(name);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_SP_UOF */

#if (PACE_HAS_POSIX_JC_UOF)
PACE_INLINE
pid_t
pace_tcgetpgrp (PACE_HANDLE fildes)
{
  PACE_UNUSED_ARG(fildes);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_JC_UOF */

#if (PACE_HAS_POSIX_JC_UOF)
PACE_INLINE
int
pace_tcsetpgrp (PACE_HANDLE fildes, pid_t pgrp_id)
{
  PACE_UNUSED_ARG(fildes);
  PACE_UNUSED_ARG(pgrp_id);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_JC_UOF */

#if (PACE_HAS_POSIX_DS_UOF)
PACE_INLINE
char *
pace_ttyname (PACE_HANDLE fildes)
{
  PACE_UNUSED_ARG(fildes);
  PACE_ERRNO_NO_SUPPORT_RETURN ((char *) 0);
}
#endif /* PACE_HAS_POSIX_DS_UOF */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_ttyname_r (PACE_HANDLE fildes,
                char * name,
                size_t namesize)
{
  PACE_UNUSED_ARG(fildes);
  PACE_UNUSED_ARG(name);
  PACE_UNUSED_ARG(namesize);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_FS_UOF)
PACE_INLINE
int
pace_unlink (const char * path)
{
  return unlink (PACE_NONCONST_ARG_CAST (char *) path);
}
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_DI_UOF)
PACE_INLINE
ssize_t
pace_write (PACE_HANDLE fildes, const void * buf, size_t nbyte)
{
  return write (fildes, PACE_NONCONST_ARG_CAST (char *) buf, nbyte);
}
#endif /* PACE_HAS_POSIX_DI_UOF */
