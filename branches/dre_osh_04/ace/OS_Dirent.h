// -*- C++ -*-

//=============================================================================
/**
 *  @file   OS_Dirent.h
 *
 *  $Id$
 *
 *  @author Doug Schmidt <schmidt@cs.wustl.edu>
 *  @author Jesper S. M|ller<stophph@diku.dk>
 *  @author and a cast of thousands...
 */
//=============================================================================

#ifndef ACE_OS_DIRENT_H
#define ACE_OS_DIRENT_H
#include /**/ "ace/pre.h"

#include "ace/OS_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/OS_Errno.h"
#include "ace/os_include/os_dirent.h"
#include "ace/os_include/sys/os_types.h"

/**
 * @namespace ACE_OS_Dirent
 *
 * @brief This class is a wrapper for the dirent.h operations
 *
 */
#define ACE_OS_Dirent ACE_OS
namespace ACE_OS {
  ACE_DIR *opendir (const ACE_TCHAR *filename);
  void closedir (ACE_DIR *);
  dirent *readdir (ACE_DIR *);
  int readdir_r (ACE_DIR *dirp,
                        struct dirent *entry,
                        struct dirent **result);
  long telldir (ACE_DIR *);
  void seekdir (ACE_DIR *,
                       long loc);
  void rewinddir (ACE_DIR *);

  int scandir (const ACE_TCHAR *dirname,
                      struct dirent **namelist[],
                      int (*selector) (const struct dirent *filename),
                      int (*comparator) (const struct dirent **f1,
                                         const struct dirent **f2));

  // Win32 emulation functions
  ACE_DIR *opendir_emulation (const ACE_TCHAR *filename);
  int scandir_emulation (const ACE_TCHAR *dirname,
                                dirent **namelist[],
                                int (*selector)(const dirent *entry),
                                int (*comparator)(const dirent **f1,
                                                  const dirent**f2));
  void closedir_emulation (ACE_DIR *);
  dirent *readdir_emulation (ACE_DIR *);
}; /* namespace ACE_OS */

# if defined (ACE_HAS_INLINED_OSCALLS)
#   if defined (ACE_INLINE)
#     undef ACE_INLINE
#   endif /* ACE_INLINE */
#   define ACE_INLINE inline
#   include "ace/OS_Dirent.inl"
# endif /* ACE_HAS_INLINED_OSCALLS */

#include /**/ "ace/post.h"
#endif /* ACE_OS_DIRENT_H */
