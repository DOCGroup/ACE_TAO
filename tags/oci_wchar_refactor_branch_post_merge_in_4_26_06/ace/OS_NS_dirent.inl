// -*- C++ -*-
//
// $Id$

#include "ace/OS_Memory.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE_OS
{

ACE_INLINE void
closedir (ACE_DIR *d)
{
#if defined (ACE_HAS_DIRENT)
# if defined (ACE_PSOS)

  u_long result = ::close_dir (&(d->xdir));
  delete d;
  if (result != 0)
    errno = result;

# else /* ! ACE_PSOS */

#   if defined (ACE_WIN32) && defined (ACE_LACKS_CLOSEDIR)
  ACE_OS::closedir_emulation (d);
  delete [] d->directory_name_;
  delete d;
#  elif defined (ACE_HAS_WCLOSEDIR) && defined (ACE_USES_WCHAR)
  ::wclosedir (d);
#   else /* ACE_WIN32 && ACE_LACKS_CLOSEDIR */
  ::closedir (d);
#   endif /* ACE_WIN32 && ACE_LACKS_CLOSEDIR */

# endif /* ACE_PSOS */
#else /* ACE_HAS_DIRENT */
  ACE_UNUSED_ARG (d);
#endif /* ACE_HAS_DIRENT */
}

ACE_INLINE ACE_DIR *
opendir (const ACE_TCHAR *filename)
{
#if defined (ACE_HAS_DIRENT)
#  if defined (ACE_PSOS)
  // The pointer to the <ACE_DIR> buffer *must* be passed to
  // <ACE_OS::closedir> to free it and avoid a memory leak.
  ACE_DIR *dir = 0;
  u_long result;
  ACE_NEW_RETURN (dir, ACE_DIR, 0);
  result = ::open_dir (const_cast<ACE_TCHAR *> (filename), &dir->xdir);
  if (result == 0)
    return dir;
  else
    {
      errno = result;
      return 0;
    }
#  else /* ! ACE_PSOS */
#    if defined (ACE_WIN32) && defined (ACE_LACKS_OPENDIR)
  return ::ACE_OS::opendir_emulation (filename);
#  elif defined (ACE_HAS_WOPENDIR) && defined (ACE_USES_WCHAR)
  return ::wopendir (filename);
#    elif defined (ACE_HAS_NONCONST_OPENDIR)
  return ::opendir (const_cast<char *> (filename));
#    else /* ! ACE_WIN32 && ACE_LACKS_OPENDIR */
  return ::opendir (ACE_TEXT_TO_CHAR_IN (filename));
#    endif /* ACE_WIN32 && ACE_LACKS_OPENDIR */
#  endif /* ACE_PSOS */
#else
  ACE_UNUSED_ARG (filename);
  ACE_NOTSUP_RETURN (0);
#endif /* ACE_HAS_DIRENT */
}

ACE_INLINE
struct ACE_DIRENT *
readdir (ACE_DIR *d)
{
#if defined (ACE_HAS_DIRENT)
#  if defined (ACE_PSOS)

  u_long result = ::read_dir (&d->xdir, &d->dirent);
  if (0 == result)
    return &d->dirent;
  else
    {
      errno = result;
      return 0;
    }

#  else /* ! ACE_PSOS */
#    if defined (ACE_WIN32) && defined (ACE_LACKS_READDIR)
  return ACE_OS::readdir_emulation (d);
#  elif defined (ACE_HAS_WREADDIR) && defined (ACE_USES_WCHAR)
  return ::wreaddir (d);
#    else /* ACE_WIN32 && ACE_LACKS_READDIR */
  return ::readdir (d);
#    endif /* ACE_WIN32 && ACE_LACKS_READDIR */
#  endif /* ACE_PSOS */
#else
  ACE_UNUSED_ARG (d);
  ACE_NOTSUP_RETURN (0);
#endif /* ACE_HAS_DIRENT */
}

ACE_INLINE int
readdir_r (ACE_DIR *dirp,
                   struct ACE_DIRENT *entry,
                   struct ACE_DIRENT **result)
{
#if !defined (ACE_HAS_REENTRANT_FUNCTIONS)
  ACE_UNUSED_ARG (entry);
  // <result> has better not be 0!
  *result = ACE_OS::readdir (dirp);
  if (*result)
    return 0; // Keep iterating
  else
    return 1; // Oops, some type of error!
#elif defined (ACE_HAS_DIRENT)  &&  !defined (ACE_LACKS_READDIR_R)
#  if (defined (sun) && (defined (_POSIX_PTHREAD_SEMANTICS) || \
                        (_FILE_OFFSET_BITS == 64) || \
                        (_POSIX_C_SOURCE - 0 >= 199506L))) || \
      (!defined (sun) && (defined (ACE_HAS_PTHREADS_STD) || \
                         defined (ACE_HAS_PTHREADS_DRAFT7) || \
                         defined (__USE_POSIX) || \
                         defined (__FreeBSD__) || \
                         defined (HPUX_11)))
#    if defined (__GNUG__) && defined (DIGITAL_UNIX)
  return readdir_r (dirp, entry, result);
#    else
  return ::readdir_r (dirp, entry, result);
#    endif /* defined (__GNUG__) && defined (DIGITAL_UNIX) */
#  else  /* ! POSIX.1c - this is draft 4 or draft 6 */
#    if defined (HPUX_10)   /* But HP 10.x doesn't follow the draft either */
    *result = entry;
    return ::readdir_r (dirp, entry);
#elif defined(__GNUC__) && defined (_AIX)
        return ::readdir_r (dirp, entry, result);
#    else
    // <result> had better not be 0!
    *result = ::readdir_r (dirp, entry);
    return 0;
#    endif /* HPUX_10 */
#  endif /* ! POSIX.1c */
#else  /* ! ACE_HAS_DIRENT  ||  ACE_LACKS_READDIR_R */
  ACE_UNUSED_ARG (dirp);
  ACE_UNUSED_ARG (entry);
  ACE_UNUSED_ARG (result);
  ACE_NOTSUP_RETURN (0);

#endif /* ACE_HAS_REENTRANT_FUNCTIONS */
}

ACE_INLINE void
rewinddir (ACE_DIR *d)
{
#if defined (ACE_HAS_DIRENT)
# if defined (ACE_LACKS_SEEKDIR)
#  if defined (ACE_LACKS_REWINDDIR)
  ACE_UNUSED_ARG (d);
#  elif defined (ACE_HAS_WREWINDDIR) && defined (ACE_USES_WCHAR)
   ::wrewinddir (d);
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

ACE_INLINE int
scandir (const ACE_TCHAR *dirname,
                 struct ACE_DIRENT **namelist[],
                 int (*selector)(const struct ACE_DIRENT *),
                 int (*comparator) (const struct ACE_DIRENT **f1,
                                    const struct ACE_DIRENT **f2))
{
#if defined (ACE_HAS_SCANDIR)
  return ::scandir (ACE_TEXT_TO_CHAR_IN (dirname),
                    namelist,
                    selector,
#  if defined (ACE_SCANDIR_CMP_USES_VOIDPTR)
                    reinterpret_cast<int(*)(const void*, const void*)> (comparator));
#  else
                    comparator);
#  endif /* ACE_SCANDIR_CMP_USES_VOIDPTR */
#else /* ! defined ( ACE_HAS_SCANDIR) */
  return ACE_OS::scandir_emulation (dirname, namelist, selector, comparator);
#endif /* ACE_HAS_SCANDIR */
}

ACE_INLINE void
seekdir (ACE_DIR *d, long loc)
{
#if defined (ACE_HAS_DIRENT)  &&  !defined (ACE_LACKS_SEEKDIR)
  ::seekdir (d, loc);
#else  /* ! ACE_HAS_DIRENT  ||  ACE_LACKS_SEEKDIR */
  ACE_UNUSED_ARG (d);
  ACE_UNUSED_ARG (loc);
#endif /* ! ACE_HAS_DIRENT  ||  ACE_LACKS_SEEKDIR */
}

ACE_INLINE long
telldir (ACE_DIR *d)
{
#if defined (ACE_HAS_DIRENT)  &&  !defined (ACE_LACKS_TELLDIR)
  return ::telldir (d);
#else  /* ! ACE_HAS_DIRENT  ||  ACE_LACKS_TELLDIR */
  ACE_UNUSED_ARG (d);
  ACE_NOTSUP_RETURN (-1);
#endif /* ! ACE_HAS_DIRENT  ||  ACE_LACKS_TELLDIR */
}

}

ACE_END_VERSIONED_NAMESPACE_DECL
