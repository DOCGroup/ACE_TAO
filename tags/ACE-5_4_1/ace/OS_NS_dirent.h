// -*- C++ -*-

//=============================================================================
/**
 *  @file   OS_NS_dirent.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 *  @author Jesper S. M|ller<stophph@diku.dk>
 *  @author and a cast of thousands...
 *
 *  Originally in OS.h.
 */
//=============================================================================

#ifndef ACE_OS_NS_DIRENT_H
# define ACE_OS_NS_DIRENT_H

# include /**/ "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#  pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/os_dirent.h"
#include "ace/ACE_export.h"

#if defined (ACE_EXPORT_MACRO)
#  undef ACE_EXPORT_MACRO
#endif
#define ACE_EXPORT_MACRO ACE_Export

namespace ACE_OS {

  ACE_NAMESPACE_INLINE_FUNCTION
  void closedir (ACE_DIR *);

  ACE_NAMESPACE_INLINE_FUNCTION
  ACE_DIR *opendir (const ACE_TCHAR *filename);

  ACE_NAMESPACE_INLINE_FUNCTION
  dirent *readdir (ACE_DIR *);

  ACE_NAMESPACE_INLINE_FUNCTION
  int readdir_r (ACE_DIR *dirp,
                 struct dirent *entry,
                 struct dirent **result);

  ACE_NAMESPACE_INLINE_FUNCTION
  void rewinddir (ACE_DIR *);

  ACE_NAMESPACE_INLINE_FUNCTION
  int scandir (const ACE_TCHAR *dirname,
               struct dirent **namelist[],
               int (*selector) (const struct dirent *filename),
               int (*comparator) (const struct dirent **f1,
                                  const struct dirent **f2));

  ACE_NAMESPACE_INLINE_FUNCTION
  void seekdir (ACE_DIR *,
                long loc);

  ACE_NAMESPACE_INLINE_FUNCTION
  long telldir (ACE_DIR *);

  // Win32 emulation functions
  extern ACE_Export
  ACE_DIR *opendir_emulation (const ACE_TCHAR *filename);

  extern ACE_Export
  int scandir_emulation (const ACE_TCHAR *dirname,
                         dirent **namelist[],
                         int (*selector)(const dirent *entry),
                         int (*comparator)(const dirent **f1,
                                           const dirent**f2));

  extern ACE_Export
  void closedir_emulation (ACE_DIR *);

  extern ACE_Export
  dirent *readdir_emulation (ACE_DIR *);

} /* namespace ACE_OS */

# if defined (ACE_HAS_INLINED_OSCALLS)
#   if defined (ACE_INLINE)
#     undef ACE_INLINE
#   endif /* ACE_INLINE */
#   define ACE_INLINE inline
#   include "ace/OS_NS_dirent.inl"
# endif /* ACE_HAS_INLINED_OSCALLS */

# include /**/ "ace/post.h"
#endif /* ACE_OS_NS_DIRENT_H */
