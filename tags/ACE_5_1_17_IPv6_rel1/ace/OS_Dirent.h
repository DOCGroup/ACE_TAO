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

#include "ace/config-all.h"
#include "ace/OS_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if !defined (ACE_WIN32) && !defined (ACE_PSOS) && !defined (ACE_HAS_WINCE)
# include /**/ <sys/types.h>
# if !defined (ACE_LACKS_UNISTD_H)
#   include /**/ <unistd.h>  // VxWorks needs this to compile
# endif /* !ACE_LACKS_UNISTD_H */
# include /**/ <dirent.h>
#endif /* !ACE_WIN32 && !ACE_PSOS */

// At least compile on some of the platforms without <ACE_DIR> info yet.
# if !defined (ACE_HAS_DIRENT)
typedef int ACE_DIR;
struct dirent {
};
# endif /* ACE_HAS_DIRENT */

#if defined (ACE_LACKS_STRUCT_DIR)
struct dirent {
  unsigned short d_ino;
  unsigned short d_off;
  unsigned short d_reclen;
  const ACE_TCHAR *d_name;
};

struct ACE_DIR {
  ACE_TCHAR *directory_name_;
  // The name of the directory we are looking into

  HANDLE current_handle_;
  // Remember the handle between calls.

  dirent dirent_;
  // The struct for the results

  ACE_TEXT_WIN32_FIND_DATA fdata_;
  // The struct for intermediate results.

  int started_reading_;
  // A flag to remember if we started reading already.
};
#elif defined (ACE_PSOS) && !defined (ACE_PSOS_DIAB_PPC)
// pHILE+ calls the DIR struct XDIR instead
# 
typedef XDIR ACE_DIR;
#else
typedef DIR ACE_DIR;
# endif /* ACE_LACKS_STRUCT_DIR */

#if defined rewinddir
# undef rewinddir
#endif /* rewinddir */

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
  static struct dirent *readdir (ACE_DIR *);
  static int readdir_r (ACE_DIR *dirp,
                        struct dirent *entry,
                        struct dirent **result);
  static long telldir (ACE_DIR *);
  static void seekdir (ACE_DIR *,
                       long loc);
  static void rewinddir (ACE_DIR *);

private:
  // Win32 emulation functions
  static ACE_DIR *opendir_emulation (const ACE_TCHAR *filename);
  static void closedir_emulation (ACE_DIR *);
  static struct dirent *readdir_emulation (ACE_DIR *);
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
