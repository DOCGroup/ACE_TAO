/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/mman.inl
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

PACE_INLINE
int
pace_mlock (const void * addr, size_t len)
{
  return mlock (((char*)addr), len);
}

PACE_INLINE
int
pace_mlockall (int flags)
{
  return mlockall (flags);
}

PACE_INLINE
void *
pace_mmap (void * addr,
           size_t len,
           int prot,
           int flags,
           int fildes,
           pace_off_t off)
{
  return mmap ((char *) addr, len, prot, flags, fildes, off);
}

PACE_INLINE
int
pace_munlock (const void * addr, size_t len)
{
  return munlock (((char*)addr), len);
}

PACE_INLINE
int
pace_mprotect (void * addr,
               size_t len,
               int prot)
{
  return mprotect (addr, len, prot);
}

PACE_INLINE
int
pace_msync (void * addr,
            size_t len,
            int flags)
{
  return msync (addr, len, flags);
}

PACE_INLINE
int
pace_munlockall ()
{
  return munlockall ();
}

PACE_INLINE
int
pace_munmap (void * addr, size_t len)
{
  return munmap ((char *) addr, len);
}

PACE_INLINE
int
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
