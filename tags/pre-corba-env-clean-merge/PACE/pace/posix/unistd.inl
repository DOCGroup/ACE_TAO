/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/posix/unistd.inl
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

#include <unistd.h>
#include <stdarg.h>
#include <string.h>

#if (PACE_HAS_POSIX_MP_UOF)
PACE_INLINE
void
pace__exit (int status)
{
 _exit (status);
 return;
}
#endif /* PACE_HAS_POSIX_MP_UOF */

#if (PACE_HAS_POSIX_FS_UOF)
PACE_INLINE
int
pace_access (const char * path, int amode)
{
  return access (path, amode);
}
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_SIG_UOF)
PACE_INLINE
unsigned int
pace_alarm (unsigned int seconds)
{
  return alarm (seconds);
}
#endif /* PACE_HAS_POSIX_SIG_UOF */

#if (PACE_HAS_POSIX_FS_UOF)
PACE_INLINE
int
pace_chdir (const char * path)
{
  return chdir (path);
}
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_FA_UOF)
PACE_INLINE
int
pace_chown (const char * path, uid_t owner, pace_gid_t group)
{
  return chown (path, owner, group);
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
  return dup (fildes);
}
#endif /* PACE_HAS_POSIX_FM_UOF */

#if (PACE_HAS_POSIX_FM_UOF)
PACE_INLINE
PACE_HANDLE
pace_dup2 (PACE_HANDLE fildes, PACE_HANDLE fildes2)
{
  return dup2 (fildes, fildes2);
}
#endif /* PACE_HAS_POSIX_FM_UOF */

#if (PACE_HAS_POSIX_MP_UOF)
PACE_INLINE
int
pace_execv (const char * path,
            char * const argv[])
{
  return execv (path, argv);
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
  return execve (path, argv, envp);
  /* if successful, this operation does NOT return */
}
#endif /* PACE_HAS_POSIX_MP_UOF */

#if (PACE_HAS_POSIX_MP_UOF)
PACE_INLINE
int
pace_execvp (const char * file,
             char * const argv[])
{
  return execvp (file, argv);
  /* if successful, this operation does NOT return */
}
#endif /* PACE_HAS_POSIX_MP_UOF */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_fdatasync (PACE_HANDLE fildes)
{
  return fdatasync (fildes);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_MP_UOF)
PACE_INLINE
pid_t
pace_fork ()
{
  return fork ();
}
#endif /* PACE_HAS_POSIX_MP_UOF */

#if (PACE_HAS_POSIX_FS_UOF)
PACE_INLINE
long
pace_fpathconf (PACE_HANDLE fildes, int name)
{
  return fpathconf (fildes, name);
}
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_fsync (PACE_HANDLE fildes)
{
  return fsync (fildes);
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
  return getegid ();
}
#endif /* PACE_HAS_POSIX_UGR_UOF */

#if (PACE_HAS_POSIX_UG_UOF)
PACE_INLINE
uid_t
pace_geteuid ()
{
  return geteuid ();
}
#endif /* PACE_HAS_POSIX_UGR_UOF */

#if (PACE_HAS_POSIX_UG_UOF)
PACE_INLINE
int
pace_getgroups (int gidsetsize, pace_gid_t grouplist[])
{
  return getgroups (gidsetsize, grouplist);
}
#endif /* PACE_HAS_POSIX_UG_UOF */

#if (PACE_HAS_POSIX_UG_UOF)
PACE_INLINE
uid_t
pace_getgid ()
{
  return getgid ();
}
#endif /* PACE_HAS_POSIX_UGR_UOF */

#if (PACE_HAS_POSIX_UG_UOF)
PACE_INLINE
char*
pace_getlogin ()
{
  return getlogin ();
}
#endif /* PACE_HAS_POSIX_UGR_UOF */

#if (PACE_HAS_POSIX_UGR_UOF)
PACE_INLINE
int
pace_getlogin_r (char * name, size_t namesize)
{
  /* this func will eventually get rewritten to *
   * autodetect _REENTRANT correctly            */
#if (PACE_HAS_REENTRANT) || (PACE_LYNXOS) || (PACE_SUNOS)    
  /* supported call */
  return getlogin_r (name, namesize);
#else
  /* unsupported call (emulated)                   *
   * careful, emulation is not 100% reentrant safe */
  char * non_reentrant_ptr_to_static_os_memory = getlogin ();
  if (0 == non_reentrant_ptr_to_static_os_memory)
    return errno;
  strncpy (name, non_reentrant_ptr_to_static_os_memory, namesize);
  return 1;
#endif /* PACE_HAS_REENTRANT */
}
#endif /* PACE_HAS_POSIX_UGR_UOF */

#if (PACE_HAS_POSIX_UG_UOF)
PACE_INLINE
pid_t
pace_getpgrp ()
{
  return getpgrp ();
}
#endif /* PACE_HAS_POSIX_UG_UOF */

#if (PACE_HAS_POSIX_MP_UOF)
PACE_INLINE
pid_t
pace_getpid ()
{
  return getpid ();
}
#endif /* PACE_HAS_POSIX_MP_UOF */

#if (PACE_HAS_POSIX_MP_UOF)
PACE_INLINE
pid_t
pace_getppid ()
{
  return getppid ();
}
#endif /* PACE_HAS_POSIX_MP_UOF */

#if (PACE_HAS_POSIX_UG_UOF)
PACE_INLINE
uid_t
pace_getuid ()
{
  return getuid ();
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
  return link (existing, new_link);
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
  return pathconf (path, name);
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
  return pipe (fildes);
}
#endif /* PACE_HAS_POSIX_P_UOF */

#if (PACE_HAS_POSIX_DI_UOF)
PACE_INLINE
ssize_t
pace_read (PACE_HANDLE fildes, void * buf, size_t nbyte)
{
  return read (fildes, buf, nbyte);
}
#endif /* PACE_HAS_POSIX_DI_UOF */

#if (PACE_HAS_POSIX_FS_UOF)
PACE_INLINE
int
pace_rmdir (const char * path)
{
  return rmdir (path);
}
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_UG_UOF)
PACE_INLINE
int
pace_setgid (pace_gid_t gid)
{
  return setgid (gid);
}
#endif /* PACE_HAS_POSIX_UG_UOF */

#if (PACE_HAS_POSIX_JC_UOF)
PACE_INLINE
int
pace_setpgid (pid_t pid, pid_t pgid)
{
  return setpgid (pid, pgid);
}
#endif /* PACE_HAS_POSIX_JC_UOF */

#if (PACE_HAS_POSIX_UG_UOF)
PACE_INLINE
pid_t
pace_setsid ()
{
  return setsid ();
}
#endif /* PACE_HAS_POSIX_UG_UOF */

#if (PACE_HAS_POSIX_UG_UOF)
PACE_INLINE
int
pace_setuid (uid_t uid)
{
  return setuid (uid);
}
#endif /* PACE_HAS_POSIX_UG_UOF */

#if (PACE_HAS_POSIX_MP_UOF)
PACE_INLINE
unsigned int
pace_sleep (unsigned int seconds)
{
  return sleep (seconds);
}
#endif /* PACE_HAS_POSIX_MP_UOF */

#if (PACE_HAS_POSIX_SP_UOF)
PACE_INLINE
long
pace_sysconf (int name)
{
  return sysconf (name);
}
#endif /* PACE_HAS_POSIX_SP_UOF */

#if (PACE_HAS_POSIX_JC_UOF)
PACE_INLINE
pid_t
pace_tcgetpgrp (PACE_HANDLE fildes)
{
  return tcgetpgrp (fildes);
}
#endif /* PACE_HAS_POSIX_JC_UOF */

#if (PACE_HAS_POSIX_JC_UOF)
PACE_INLINE
int
pace_tcsetpgrp (PACE_HANDLE fildes, pid_t pgrp_id)
{
  return tcsetpgrp (fildes, pgrp_id);
}
#endif /* PACE_HAS_POSIX_JC_UOF */

#if (PACE_HAS_POSIX_DS_UOF)
PACE_INLINE
char *
pace_ttyname (PACE_HANDLE fildes)
{
  return ttyname (fildes);
}
#endif /* PACE_HAS_POSIX_DS_UOF */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_ttyname_r (PACE_HANDLE fildes,
                char * name,
                size_t namesize)
{
#if (PACE_SUNOS) || (PACE_LYNXOS)
  if (ttyname_r (fildes, name, (int) namesize) == 0)
    return errno;
  return 0;
#else
  return ttyname_r (fildes, name, namesize);
#endif /* PACE_SUNOS */
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_FS_UOF)
PACE_INLINE
int
pace_unlink (const char * path)
{
  return unlink (path);
}
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_DI_UOF)
PACE_INLINE
ssize_t
pace_write (PACE_HANDLE fildes, const void * buf, size_t nbyte)
{
  return write (fildes, buf, nbyte);
}
#endif /* PACE_HAS_POSIX_DI_UOF */
