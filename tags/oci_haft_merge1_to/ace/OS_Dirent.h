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
#include "ace/pre.h"

#include "ace/OS_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/OS_Errno.h"
#include "ace/os_include/os_dirent.h"
#include "ace/os_include/sys/os_types.h"

/**
 * @class ACE_OS_Dirent
 *
 * @brief This class is a wrapper for the dirent.h operations
 *
 */
class ACE_OS_Export ACE_OS_Dirent
{
public:
  static ACE_DIR *opendir (const ACE_TCHAR *filename);
  static void closedir (ACE_DIR *);
  static dirent *readdir (ACE_DIR *);
  static int readdir_r (ACE_DIR *dirp,
                        struct dirent *entry,
                        struct dirent **result);
  static long telldir (ACE_DIR *);
  static void seekdir (ACE_DIR *,
                       long loc);
  static void rewinddir (ACE_DIR *);

  static int scandir (const ACE_TCHAR *dirname,
                      struct dirent **namelist[],
                      int (*selector) (const struct dirent *filename),
                      int (*comparator) (const struct dirent **f1,
                                         const struct dirent **f2));
private:
  // Win32 emulation functions
  static ACE_DIR *opendir_emulation (const ACE_TCHAR *filename);
  static int scandir_emulation (const ACE_TCHAR *dirname,
                                dirent **namelist[],
                                int (*selector)(const dirent *entry),
                                int (*comparator)(const dirent **f1,
                                                  const dirent**f2));
  static void closedir_emulation (ACE_DIR *);
  static dirent *readdir_emulation (ACE_DIR *);
};

# if defined (ACE_HAS_INLINED_OSCALLS)
#   if defined (ACE_INLINE)
#     undef ACE_INLINE
#   endif /* ACE_INLINE */
#   define ACE_INLINE inline
#   include "ace/OS_Dirent.inl"
# endif /* ACE_HAS_INLINED_OSCALLS */

#include "ace/post.h"
#endif /* ACE_OS_DIRENT_H */
