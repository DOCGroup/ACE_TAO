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
  return _chdir (path);
}
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_FA_UOF)
PACE_INLINE
int
pace_chown (const char * path, uid_t owner, pace_gid_t group)
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
  PACE_WIN32CALL_RETURN
    (PACE_ADAPT_RETVAL
     (CloseHandle (fildes), pace_result_), int, -1);
}
#endif /* PACE_HAS_POSIX_DI_UOF */

#if (PACE_HAS_POSIX_FM_UOF)
PACE_INLINE
PACE_HANDLE
pace_dup (PACE_HANDLE fildes)
{
  PACE_HANDLE new_fd;
  if (DuplicateHandle(GetCurrentProcess (),
                      fildes,
                      GetCurrentProcess(),
                      &new_fd,
                      0,
                      TRUE,
                      DUPLICATE_SAME_ACCESS))
    {
      return new_fd;
    }
  else
    {
      PACE_FAIL_RETURN (PACE_INVALID_HANDLE);
    }
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
  return _execv (path, (const char * const *) argv);
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
  return _execve (path, (const char *const *) argv,
                  (const char *const *) envp);
  /* if successful, this operation does NOT return */
}
#endif /* PACE_HAS_POSIX_MP_UOF */

#if (PACE_HAS_POSIX_MP_UOF)
PACE_INLINE
int
pace_execvp (const char * file,
             char * const argv[])
{
  return _execvp (file, (const char *const *) argv);
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
  if (SetFilePointer (fildes, length, NULL, FILE_BEGIN) != (unsigned) -1)
    {
      PACE_WIN32CALL_RETURN
        (PACE_ADAPT_RETVAL
         (SetEndOfFile (fildes), pace_result_), int, -1);
    }
  else
    {
      PACE_FAIL_RETURN (-1);
    }
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
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_UG_UOF */

#if (PACE_HAS_POSIX_UG_UOF)
PACE_INLINE
uid_t
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
uid_t
pace_getgid ()
{
  uid_t retval = -1;
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
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_UGR_UOF */

#if (PACE_HAS_POSIX_UG_UOF)
PACE_INLINE
pid_t
pace_getpgrp ()
{
  pid_t retval = -1;
  PACE_ERRNO_NO_SUPPORT_RETURN (retval);
}
#endif /* PACE_HAS_POSIX_UG_UOF */

#if (PACE_HAS_POSIX_MP_UOF)
PACE_INLINE
pid_t
pace_getpid ()
{
  return GetCurrentProcessId ();
}
#endif /* PACE_HAS_POSIX_MP_UOF */

#if (PACE_HAS_POSIX_MP_UOF)
PACE_INLINE
pid_t
pace_getppid ()
{
  pid_t retval = -1;
  PACE_ERRNO_NO_SUPPORT_RETURN (retval);
}
#endif /* PACE_HAS_POSIX_MP_UOF */

#if (PACE_HAS_POSIX_UG_UOF)
PACE_INLINE
uid_t
pace_getuid ()
{
  uid_t retval = -1;
  PACE_ERRNO_NO_SUPPORT_RETURN (retval);
}
#endif /* PACE_HAS_POSIX_UG_UOF */

#if (PACE_HAS_POSIX_DS_UOF)
PACE_INLINE
int
pace_isatty (int fildes)
{
  return _isatty (fildes);
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
# if SEEK_SET != FILE_BEGIN \
  || SEEK_CUR != FILE_CURRENT \
  || SEEK_END != FILE_END

  /* #error Windows NT is evil AND rude! */
  switch (whence) {

    case SEEK_SET: {
      whence = FILE_BEGIN;
      break;
    }
    case SEEK_CUR: {
      whence = FILE_CURRENT;
      break;
    }
    case SEEK_END: {
      whence = FILE_END;
      break;
    }
    default: {
      errno = EINVAL;
      return (off_t)-1; // rather safe than sorry
    }
  }
  PACE_OSCALL_RETURN (lseek (handle, offset, whence), off_t, -1);
# endif  /* SEEK_SET != FILE_BEGIN || SEEK_CUR != FILE_CURRENT || SEEK_END != FILE_END */
  DWORD result = SetFilePointer (fildes, offset, NULL, whence);
  if (result == PACE_SYSCALL_FAILED)
    {
      off_t retval = -1;
      PACE_FAIL_RETURN (retval);
    }
  else
    {
      return result;
    }
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
ssize_t
pace_read (PACE_HANDLE fildes, void * buf, size_t nbyte)
{
  PACE_UNUSED_ARG (fildes);
  PACE_UNUSED_ARG (buf);
  PACE_UNUSED_ARG (nbyte);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
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
pace_setpgid (pid_t pid, pid_t pgid)
{
  PACE_UNUSED_ARG (pid);
  PACE_UNUSED_ARG (pgid);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_JC_UOF */

#if (PACE_HAS_POSIX_UG_UOF)
PACE_INLINE
pid_t
pace_setsid ()
{
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);  
}
#endif /* PACE_HAS_POSIX_UG_UOF */

#if (PACE_HAS_POSIX_UG_UOF)
PACE_INLINE
int
pace_setuid (uid_t uid)
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
pid_t
pace_tcgetpgrp (PACE_HANDLE fildes)
{
  pid_t retval = -1;
  PACE_UNUSED_ARG (fildes);
  PACE_ERRNO_NO_SUPPORT_RETURN (retval);
}
#endif /* PACE_HAS_POSIX_JC_UOF */

#if (PACE_HAS_POSIX_JC_UOF)
PACE_INLINE
int
pace_tcsetpgrp (PACE_HANDLE fildes, pid_t pgrp_id)
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
  char * retval = (void*)0;
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
  return unlink (path);
}
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_DI_UOF)
PACE_INLINE
ssize_t
pace_write (PACE_HANDLE fildes, const void * buf, size_t nbyte)
{
  DWORD bytes_written; /* This is set to 0 byte WriteFile. */

  if (WriteFile (fildes, buf, nbyte, &bytes_written, 0))
    {
      return (ssize_t) bytes_written;
    }
  else
    {
      PACE_FAIL_RETURN (-1);
    }
}
#endif /* PACE_HAS_POSIX_DI_UOF */
