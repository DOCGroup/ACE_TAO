/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/posix/mman.inl
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_mlock (const void * addr, pace_size_t len)
{
  return mlock (((char*)addr), len);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_mlockall (int flags)
{
  return mlockall (flags);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
void *
pace_mmap (void * addr,
           pace_size_t len,
           int prot,
           int flags,
           PACE_HANDLE fildes,
           pace_off_t off)
{
  return mmap ((char *) addr, len, prot, flags, fildes, off);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_munlock (const void * addr, pace_size_t len)
{
  return munlock (((char*)addr), len);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_mprotect (void * addr,
               pace_size_t len,
               int prot)
{
  return mprotect ((char*)addr, len, prot);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_msync (void * addr,
            pace_size_t len,
            int flags)
{
  return msync ((char*)addr, len, flags);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_munlockall ()
{
  return munlockall ();
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_munmap (void * addr, pace_size_t len)
{
  return munmap ((char *) addr, len);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
PACE_HANDLE
pace_shm_open (const char * name,
               int oflag,
               pace_mode_t mode)
{
# if (PACE_POSIX_C_SOURCE > 2)
  return shm_open (name, oflag, mode);
# else /* ! PACE_POSIX_C_SOURCE */
  PACE_UNUSED_ARG (name);
  PACE_UNUSED_ARG (oflag);
  PACE_UNUSED_ARG (mode);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
# endif /* ! PACE_POSIX_C_SOURCE */
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_shm_unlink (const char * name)
{
# if (PACE_POSIX_C_SOURCE > 2)
  return shm_unlink (name);
# else /* ! PACE_POSIX_C_SOURCE */
  PACE_UNUSED_ARG (name);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
# endif /* ! PACE_POSIX_C_SOURCE */
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */
