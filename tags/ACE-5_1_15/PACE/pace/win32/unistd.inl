/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/unistd.inl
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

#include <stdarg.h>
#include <io.h>
#include <stdio.h>
#include <direct.h>
#include <process.h>

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
  return _access (path, amode);
}
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_SIG_UOF)
PACE_INLINE
unsigned int
pace_alarm (unsigned int seconds)
{
  PACE_UNUSED_ARG (seconds);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_SIG_UOF */

#if (PACE_HAS_POSIX_FS_UOF)
PACE_INLINE
int
pace_chdir (const char * path)
{
#if defined (__BORLANDC__)
  return chdir (path);
#else /* __BORLANDC__ */
  return _chdir (path);
#endif /* __BORLANDC__ */
}
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_FA_UOF)
PACE_INLINE
int
pace_chown (const char * path, pace_uid_t owner, pace_gid_t group)
{
  PACE_UNUSED_ARG (path);
  PACE_UNUSED_ARG (owner);
  PACE_UNUSED_ARG (group);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);  
}
#endif /* PACE_HAS_POSIX_FA_UOF */

#if (PACE_HAS_POSIX_DI_UOF)
PACE_INLINE
int
pace_close (PACE_HANDLE fildes)
{
  return pace_win32_close (fildes);
}
#endif /* PACE_HAS_POSIX_DI_UOF */

#if (PACE_HAS_POSIX_FM_UOF)
PACE_INLINE
PACE_HANDLE
pace_dup (PACE_HANDLE fildes)
{
  return pace_win32_dup (fildes);
}
#endif /* PACE_HAS_POSIX_FM_UOF */

#if (PACE_HAS_POSIX_FM_UOF)
PACE_INLINE
PACE_HANDLE
pace_dup2 (PACE_HANDLE fildes, PACE_HANDLE fildes2)
{
  PACE_HANDLE retval = (void*)0;
  PACE_UNUSED_ARG (fildes);
  PACE_UNUSED_ARG (fildes2);
  PACE_ERRNO_NO_SUPPORT_RETURN (retval);
}
#endif /* PACE_HAS_POSIX_FM_UOF */

#if (PACE_HAS_POSIX_MP_UOF)
PACE_INLINE
int
pace_execv (const char * path,
            char * const argv[])
{
#if defined (__BORLANDC__) || defined(__MINGW32__)
  return execv (path, argv);
#else /* __BORLANDC__ || __MINGW32__ */
  return _execv (path, (const char * const *) argv);
#endif /* __BORLANDC__ || __MINGW32__ */
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
#if defined (__BORLANDC__) || defined (__MINGW32__)
  return execve (path, argv, envp);
#else /* __BORLANDC__ || __MINGW32__ */
  return _execve (path, (const char *const *) argv,
                  (const char *const *) envp);
#endif /* __BORLANDC__ || __MINGW32__ */
  /* if successful, this operation does NOT return */
}
#endif /* PACE_HAS_POSIX_MP_UOF */

#if (PACE_HAS_POSIX_MP_UOF)
PACE_INLINE
int
pace_execvp (const char * file,
             char * const argv[])
{
#if defined (__BORLANDC__) || defined (__MINGW32__)
  return execvp (file, argv);
#else /* __BORLANDC__ || __MINGW32__ */
  return _execvp (file, (const char *const *) argv);
#endif /* __BORLANDC__ || __MINGW32__ */
  /* if successful, this operation does NOT return */
}
#endif /* PACE_HAS_POSIX_MP_UOF */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_fdatasync (PACE_HANDLE fildes)
{
  PACE_UNUSED_ARG (fildes);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_MP_UOF)
PACE_INLINE
pace_pid_t
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
  PACE_UNUSED_ARG (fildes);
  PACE_UNUSED_ARG (name);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_fsync (PACE_HANDLE fildes)
{
  PACE_UNUSED_ARG (fildes);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_ftruncate (PACE_HANDLE fildes, pace_off_t length)
{
  return pace_win32_ftruncate (fildes, length);
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
pace_uid_t
pace_getegid ()
{
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_UG_UOF */

#if (PACE_HAS_POSIX_UG_UOF)
PACE_INLINE
pace_uid_t
pace_geteuid ()
{
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_UG_UOF */

#if (PACE_HAS_POSIX_UG_UOF)
PACE_INLINE
int
pace_getgroups (int gidsetsize, pace_gid_t grouplist[])
{
  PACE_UNUSED_ARG (gidsetsize);
  PACE_UNUSED_ARG (grouplist);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_UG_UOF */

#if (PACE_HAS_POSIX_UG_UOF)
PACE_INLINE
pace_uid_t
pace_getgid ()
{
  pace_uid_t retval = -1;
  PACE_ERRNO_NO_SUPPORT_RETURN (retval);
}
#endif /* PACE_HAS_POSIX_UG_UOF */

#if (PACE_HAS_POSIX_UG_UOF)
PACE_INLINE
char *
pace_getlogin ()
{
  char * retval = (char*)0;
  PACE_ERRNO_NO_SUPPORT_RETURN (retval);
}
#endif /* PACE_HAS_POSIX_UG_UOF */

#if (PACE_HAS_POSIX_UGR_UOF)
PACE_INLINE
int
pace_getlogin_r (char * name, size_t namesize)
{
  PACE_UNUSED_ARG (name);
  PACE_UNUSED_ARG (namesize);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_UGR_UOF */

#if (PACE_HAS_POSIX_UG_UOF)
PACE_INLINE
pace_pid_t
pace_getpgrp ()
{
  pace_pid_t retval = -1;
  PACE_ERRNO_NO_SUPPORT_RETURN (retval);
}
#endif /* PACE_HAS_POSIX_UG_UOF */

#if (PACE_HAS_POSIX_MP_UOF)
PACE_INLINE
pace_pid_t
pace_getpid ()
{
  return GetCurrentProcessId ();
}
#endif /* PACE_HAS_POSIX_MP_UOF */

#if (PACE_HAS_POSIX_MP_UOF)
PACE_INLINE
pace_pid_t
pace_getppid ()
{
  pace_pid_t retval = -1;
  PACE_ERRNO_NO_SUPPORT_RETURN (retval);
}
#endif /* PACE_HAS_POSIX_MP_UOF */

#if (PACE_HAS_POSIX_UG_UOF)
PACE_INLINE
pace_uid_t
pace_getuid ()
{
  pace_uid_t retval = -1;
  PACE_ERRNO_NO_SUPPORT_RETURN (retval);
}
#endif /* PACE_HAS_POSIX_UG_UOF */

#if (PACE_HAS_POSIX_DS_UOF)
PACE_INLINE
int
pace_isatty (int fildes)
{
#if defined (__BORLANDC__) || defined (__MINGW32__)
  return isatty (fildes);
#else /* __BORLANDC__ || __MINGW32__ */
  return _isatty (fildes);
#endif /* __BORLANDC__ || __MINGW32__ */
}
#endif /* PACE_HAS_POSIX_DS_UOF */

#if (PACE_HAS_POSIX_FS_UOF)
PACE_INLINE
int
pace_link (const char * existing, const char * new_link)
{
  PACE_UNUSED_ARG (existing);
  PACE_UNUSED_ARG (new_link);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_FM_UOF)
PACE_INLINE
pace_off_t
pace_lseek (PACE_HANDLE fildes, pace_off_t offset, int whence)
{
  return pace_win32_lseek (fildes, offset, whence);
}
#endif /* PACE_HAS_POSIX_FM_UOF */

#if (PACE_HAS_POSIX_FS_UOF)
PACE_INLINE
long
pace_pathconf (const char * path, int name)
{
  PACE_UNUSED_ARG (path);
  PACE_UNUSED_ARG (name);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_SIG_UOF)
PACE_INLINE
int
pace_pause ()
{
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_SIG_UOF */

#if (PACE_HAS_POSIX_P_UOF)
PACE_INLINE
int
pace_pipe (PACE_HANDLE fildes[2])
{
  PACE_UNUSED_ARG (fildes);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_P_UOF */

#if (PACE_HAS_POSIX_DI_UOF)
PACE_INLINE
pace_ssize_t
pace_read (PACE_HANDLE fildes, void * buf, pace_size_t nbyte)
{
  return pace_win32_read (fildes, buf, nbyte);
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
  PACE_UNUSED_ARG (gid);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_UG_UOF */

#if (PACE_HAS_POSIX_JC_UOF)
PACE_INLINE
int
pace_setpgid (pace_pid_t pid, pace_pid_t pgid)
{
  PACE_UNUSED_ARG (pid);
  PACE_UNUSED_ARG (pgid);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_JC_UOF */

#if (PACE_HAS_POSIX_UG_UOF)
PACE_INLINE
pace_pid_t
pace_setsid ()
{
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);  
}
#endif /* PACE_HAS_POSIX_UG_UOF */

#if (PACE_HAS_POSIX_UG_UOF)
PACE_INLINE
int
pace_setuid (pace_uid_t uid)
{
  PACE_UNUSED_ARG (uid);
  PACE_ERRNO_NO_SUPPORT_RETURN (uid);
}
#endif /* PACE_HAS_POSIX_UG_UOF */

#if (PACE_HAS_POSIX_MP_UOF)
PACE_INLINE
unsigned int
pace_sleep (unsigned int seconds)
{
  Sleep (seconds * PACE_ONE_SECOND_IN_MSECS);
  return 0;
}
#endif /* PACE_HAS_POSIX_MP_UOF */

#if (PACE_HAS_POSIX_SP_UOF)
PACE_INLINE
long
pace_sysconf (int name)
{
  PACE_UNUSED_ARG (name);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_SP_UOF */

#if (PACE_HAS_POSIX_JC_UOF)
PACE_INLINE
pace_pid_t
pace_tcgetpgrp (PACE_HANDLE fildes)
{
  pace_pid_t retval = -1;
  PACE_UNUSED_ARG (fildes);
  PACE_ERRNO_NO_SUPPORT_RETURN (retval);
}
#endif /* PACE_HAS_POSIX_JC_UOF */

#if (PACE_HAS_POSIX_JC_UOF)
PACE_INLINE
int
pace_tcsetpgrp (PACE_HANDLE fildes, pace_pid_t pgrp_id)
{
  PACE_UNUSED_ARG (fildes);
  PACE_UNUSED_ARG (pgrp_id);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_JC_UOF */

#if (PACE_HAS_POSIX_DS_UOF)
PACE_INLINE
char *
pace_ttyname (PACE_HANDLE fildes)
{
  char * retval = (char*) 0;
  PACE_UNUSED_ARG (fildes);
  PACE_ERRNO_NO_SUPPORT_RETURN (retval);
}
#endif /* PACE_HAS_POSIX_DS_UOF */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_ttyname_r (PACE_HANDLE fildes,
                char * name,
                size_t namesize)
{
  PACE_UNUSED_ARG (fildes);
  PACE_UNUSED_ARG (name);
  PACE_UNUSED_ARG (namesize);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_FS_UOF)
PACE_INLINE
int
pace_unlink (const char * path)
{
  return _unlink (path);
}
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_DI_UOF)
PACE_INLINE
pace_ssize_t
pace_write (PACE_HANDLE fildes, const void * buf, size_t nbyte)
{
  return pace_win32_write (fildes, buf, nbyte);
}
#endif /* PACE_HAS_POSIX_DI_UOF */
