/* -*- C++ -*- */
// $Id$

ACE_INLINE DIR *
ACE_OS_Dirent::opendir (const ACE_TCHAR *filename)
{
#if defined (ACE_HAS_DIRENT)
#  if defined (ACE_PSOS)

  // The pointer to the DIR buffer must be passed to ACE_OS_Dirent::closedir
  // in order to free it and avoid a memory leak.
  DIR *dir;
  u_long result;
  ACE_NEW_RETURN (dir, DIR, 0);
#    if defined (ACE_PSOS_DIAB_PPC)
  result = ::open_dir (ACE_const_cast (char *, filename), &(dir->xdir));
#    else
  result = ::open_dir (ACE_const_cast (char *, filename), dir);
#    endif /* defined ACE_PSOS_DIAB_PPC */
  if (result == 0)
    return dir;
  else
    {
      errno = result;
      return 0;
    }
#  else /* ! defined (ACE_PSOS) */
#    if defined (ACE_WIN32)
  return ::ACE_OS_Dirent::opendir_emulation (filename);
#    else /* ACE_WIN32 */
  // VxWorks' ::opendir () is declared with a non-const argument.
  return ::opendir (ACE_const_cast (char *, filename));
#    endif /* ACE_WIN32 */
#  endif /* ! defined (ACE_PSOS) */
#else
  ACE_UNUSED_ARG (filename);
  ACE_NOTSUP_RETURN (0);
#endif /* ACE_HAS_DIRENT */
}

ACE_INLINE void
ACE_OS_Dirent::closedir (DIR *d)
{
#if defined (ACE_HAS_DIRENT)
# if defined (ACE_PSOS)
  
  u_long result;

#   if defined (ACE_PSOS_DIAB_PPC)
  result = ::close_dir (&(d->xdir));
#   else
  result = ::close_dir (d);
#   endif /* defined ACE_PSOS_DIAB_PPC */

  delete d;
  if (result != 0)
    errno = result;

# else /* ! defined (ACE_PSOS) */

#   if defined (ACE_WIN32)
  ACE_OS_Dirent::closedir_emulation (d);
#   else /* ACE_WIN32 */
  ::closedir (d);
#   endif /* ACE_WIN32 */

# endif /* !ACE_PSOS */
#else /* ACE_HAS_DIRENT */
  ACE_UNUSED_ARG (d);
#endif /* ACE_HAS_DIRENT */
}

ACE_INLINE struct dirent *
ACE_OS_Dirent::readdir (DIR *d)
{
#if defined (ACE_HAS_DIRENT)
#  if defined (ACE_PSOS)
  // The returned pointer to the dirent struct must be deleted by the
  // caller to avoid a memory leak.
  struct dirent *dir_ent;
  u_long result;

  ACE_NEW_RETURN (dir_ent,
                  dirent,
                  0);
#if defined (ACE_PSOS_DIAB_PPC)
  result = ::read_dir (&(d->xdir), dir_ent);
#else
  result = ::read_dir (d, dir_ent);
#endif /* defined ACE_PSOS_DIAB_PPC) */

  if (0 == result)
    return dir_ent;
  else
    {
      errno = result;
      return 0;
    }

#  else /* ! defined (ACE_PSOS) */
#    if defined (ACE_WIN32)
  return ACE_OS_Dirent::readdir_emulation (d);
#    else /* defined (ACE_WIN32) */
  return ::readdir (d);
#    endif /* defined (ACE_WIN32) */
#  endif /* ! defined (ACE_PSOS) */
#else
  ACE_UNUSED_ARG (d);
  ACE_NOTSUP_RETURN (0);
#endif /* ACE_HAS_DIRENT */
}

ACE_INLINE int
ACE_OS_Dirent::readdir_r (DIR *dirp,
                   struct dirent *entry,
                   struct dirent **result)
{
# if !defined (ACE_HAS_REENTRANT_FUNCTIONS)
  ACE_UNUSED_ARG (entry);
  // <result> has better not be 0!
  *result = ACE_OS_Dirent::readdir (dirp);
  return 0;
# elif defined (ACE_HAS_DIRENT)  &&  !defined (ACE_LACKS_READDIR_R)
# if (defined (sun) && (defined (_POSIX_PTHREAD_SEMANTICS) || \
                        (_FILE_OFFSET_BITS == 64))) || \
      (!defined (sun) && (defined (ACE_HAS_PTHREADS_STD) || \
                         defined (ACE_HAS_PTHREADS_DRAFT7) || \
                         defined (__USE_POSIX)))
# if defined (__GNUG__) && defined (DIGITAL_UNIX)
  return readdir_r (dirp, entry, result);
# else
  return ::readdir_r (dirp, entry, result);
# endif /* defined (__GNUG__) && defined (DIGITAL_UNIX) */
    return ::readdir_r (dirp, entry, result);
# else  /* ! POSIX.1c - this is draft 4 or draft 6 */
#   if defined (HPUX_10)   /* But HP 10.x doesn't follow the draft either */
    *result = entry;
    return ::readdir_r (dirp, entry);
#   else
    // <result> had better not be 0!
    *result = ::readdir_r (dirp, entry);
    return 0;
#   endif /* HPUX_10 */
# endif /* ! POSIX.1c */
#else  /* ! ACE_HAS_DIRENT  ||  ACE_LACKS_READDIR_R */
  ACE_UNUSED_ARG (dirp);
  ACE_UNUSED_ARG (entry);
  ACE_UNUSED_ARG (result);
  ACE_NOTSUP_RETURN (0);

#endif /* !ACE_HAS_REENTRANT_FUNCTIONS */
}

ACE_INLINE long
ACE_OS_Dirent::telldir (DIR *d)
{
#if defined (ACE_HAS_DIRENT)  &&  !defined (ACE_LACKS_TELLDIR)
  return ::telldir (d);
#else  /* ! ACE_HAS_DIRENT  ||  ACE_LACKS_TELLDIR */
  ACE_UNUSED_ARG (d);
  ACE_NOTSUP_RETURN (-1);
#endif /* ! ACE_HAS_DIRENT  ||  ACE_LACKS_TELLDIR */
}

ACE_INLINE void
ACE_OS_Dirent::seekdir (DIR *d, long loc)
{
#if defined (ACE_HAS_DIRENT)  &&  !defined (ACE_LACKS_SEEKDIR)
  ::seekdir (d, loc);
#else  /* ! ACE_HAS_DIRENT  ||  ACE_LACKS_SEEKDIR */
  ACE_UNUSED_ARG (d);
  ACE_UNUSED_ARG (loc);
#endif /* ! ACE_HAS_DIRENT  ||  ACE_LACKS_SEEKDIR */
}

ACE_INLINE void
ACE_OS_Dirent::rewinddir (DIR *d)
{
#if defined (ACE_HAS_DIRENT)
# if defined (ACE_LACKS_SEEKDIR)
#  if defined (ACE_LACKS_REWINDDIR)
  ACE_UNUSED_ARG (d);
#  else /* ! defined (ACE_LACKS_REWINDDIR) */
   ::rewinddir (d);
#  endif /* ! defined (ACE_LACKS_REWINDDIR) */
# else  /* ! ACE_LACKS_SEEKDIR */
    // We need to implement <rewinddir> using <seekdir> since it's often
    // defined as a macro...
   ::seekdir (d, long (0));
# endif /* ! ACE_LACKS_SEEKDIR */
#else
  ACE_UNUSED_ARG (d);
#endif /* ACE_HAS_DIRENT */
}
