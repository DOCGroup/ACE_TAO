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

PACE_INLINE
void
pace__exit (int status)
{
 _exit (status);
 return;
}

PACE_INLINE
int
pace_access (const char * path, int amode)
{
  return access (path, amode);
}

PACE_INLINE
unsigned int
pace_alarm (unsigned int seconds)
{
  return alarm (seconds);
}

PACE_INLINE
int
pace_chdir (const char * path)
{
  return chdir (path);
}

PACE_INLINE
int
pace_chown (const char * path, uid_t owner, pace_gid_t group)
{
  return chown (path, owner, group);
}

PACE_INLINE
int
pace_close (int fildes)
{
  return close (fildes);
}

PACE_INLINE
int
pace_dup (int fildes)
{
  return dup (fildes);
}

PACE_INLINE
int
pace_dup2 (int fildes, int fildes2)
{
  return dup2 (fildes, fildes2);
}

int
pace_execl (const char* path, const char* arg, ...)
{
  int result = 0;
  va_list ap;
  va_start (ap, arg);
  result = pace_execv (path, ap);
  va_end (ap);
  return result;
}

int
pace_execle (const char* path, const char* arg, ...)
{
  int result = 0;
  va_list ap;
  va_start (ap, arg);
  result = pace_execve (path, ap, 0);
  va_end (ap);
  return result;
}

int
pace_execlp (const char* file, const char* arg,  ...)
{
  int result = 0;
  va_list ap;
  va_start (ap, arg);
  result = pace_execvp (file, ap);
  va_end (ap);
  return result;
}

PACE_INLINE
int
pace_execv (const char * path,
            char * const argv[])
{
  return execv (path, argv);
  /* if successful, this operation does NOT return */
}

PACE_INLINE
int
pace_execve (const char * path,
             char * const argv[],
             char * const envp[])
{
  return execve (path, argv, envp);
  /* if successful, this operation does NOT return */
}

PACE_INLINE
int
pace_execvp (const char * file,
             char * const argv[])
{
  return execvp (file, argv);
  /* if successful, this operation does NOT return */
}

PACE_INLINE
int
pace_fdatasync (int fildes)
{
  return fdatasync (fildes);
}

PACE_INLINE
pid_t
pace_fork ()
{
  return fork ();
}

PACE_INLINE
long
pace_fpathconf (int fildes, int name)
{
  return fpathconf (fildes, name);
}

PACE_INLINE
int
pace_fsync (int fildes)
{
  return fildes;
}

PACE_INLINE
int
pace_ftruncate (int fildes, pace_off_t length)
{
  return ftruncate (fildes, length);
}

PACE_INLINE
char *
pace_getcwd (char * buf, size_t size)
{
  return getcwd (buf, size);
}

PACE_INLINE
uid_t
pace_getegid ()
{
  return getegid ();
}

PACE_INLINE
uid_t
pace_geteuid ()
{
  return geteuid ();
}

PACE_INLINE
int
pace_getgroups (int gidsetsize, pace_gid_t grouplist[])
{
  return getgroups (gidsetsize, grouplist);
}

PACE_INLINE
uid_t
pace_getgid ()
{
  return getgid ();
}

PACE_INLINE
char*
pace_getlogin ()
{
  return getlogin ();
}

PACE_INLINE
int
pace_getlogin_r (char * name, size_t namesize)
{
# if defined (PACE_HAS_POSIX_PTHREAD_SEMANTICS)
  return getlogin_r (name, namesize);
# else /* ! PACE_HAS_POSIX_PTHREAD_SEMANTICS */
  PACE_UNUSED_ARG (name);
  PACE_UNUSED_ARG (namesize);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
# endif /* ! PACE_HAS_POSIX_PTHREAD_SEMANTICS */
}

PACE_INLINE
pid_t
pace_getpgrp ()
{
  return getpgrp ();
}

PACE_INLINE
pid_t
pace_getpid ()
{
  return getpid ();
}

PACE_INLINE
pid_t
pace_getppid ()
{
  return getppid ();
}

PACE_INLINE
uid_t
pace_getuid ()
{
  return getuid ();
}

PACE_INLINE
int
pace_isatty (int fildes)
{
  return isatty (fildes);
}

PACE_INLINE
int
pace_link (const char * existing, const char * new_link)
{
  return link (existing, new_link);
}

PACE_INLINE
pace_off_t
pace_lseek (int fildes, pace_off_t offset, int whence)
{
  return lseek (fildes, offset, whence);
}

PACE_INLINE
int
pace_pause ()
{
  return pause ();
}

PACE_INLINE
long
pace_pathconf (const char * path, int name)
{
  return pathconf (path, name);
}

PACE_INLINE
int
pace_pipe (int fildes[2])
{
  return pipe (fildes);
}

PACE_INLINE
ssize_t
pace_read (int fildes, void * buf, size_t nbyte)
{
  return read (fildes, buf, nbyte);
}

PACE_INLINE
int
pace_rmdir (const char * path)
{
  return rmdir (path);
}

PACE_INLINE
int
pace_setgid (pace_gid_t gid)
{
  return setgid (gid);
}

PACE_INLINE
int
pace_setpgid (pid_t pid, pid_t pgid)
{
  return setpgid (pid, pgid);
}

PACE_INLINE
pid_t
pace_setsid ()
{
  return setsid ();
}

PACE_INLINE
int
pace_setuid (uid_t uid)
{
  return setuid (uid);
}

PACE_INLINE
unsigned int
pace_sleep (unsigned int seconds)
{
  return sleep (seconds);
}

PACE_INLINE
long
pace_sysconf (int name)
{
  return sysconf (name);
}

PACE_INLINE
char *
pace_ttyname (int fildes)
{
  return ttyname (fildes);
}

PACE_INLINE
int
pace_ttyname_r (int fildes,
                char * name,
                size_t namesize)
{
#if defined (PACE_HAS_POSIX_PTHREAD_SEMANTICS)
  return ttyname_r (fildes, name, namesize);
#else /* ! PACE_HAS_POSIX_PTHREAD_SEMANTICS */
  PACE_UNUSED_ARG (fildes);
  PACE_UNUSED_ARG (name);
  PACE_UNUSED_ARG (namesize);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
#endif /* ! PACE_HAS_POSIX_PTHREAD_SEMANTICS */
}

PACE_INLINE
int
pace_unlink (const char * path)
{
  return unlink (path);
}

PACE_INLINE
ssize_t
pace_write (int fildes, const void * buf, size_t nbyte)
{
  return write (fildes, buf, nbyte);
}
