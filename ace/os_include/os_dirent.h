// -*- C++ -*-

//=============================================================================
/**
 *  @file    os_dirent.h
 *
 *  format of directory entries
 *
 *  $Id$
 *
 *  @author Don Hinton <dhinton@dresystems.com>
 *  @author This code was originally in various places including ace/OS.h.
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_OS_DIRENT_H
#define ACE_OS_INCLUDE_OS_DIRENT_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/sys/os_types.h"
#include "ace/os_include/os_limits.h"

#if defined (VXWORKS)
#  include "ace/os_include/unistd.h"  // VxWorks needs this to compile
#endif /* VXWORKS */

#if !defined (ACE_LACKS_DIRENT_H)
# include /**/ <dirent.h>
#endif /* !ACE_LACKS_DIRENT_H */

// Place all additions (especially function declarations) within extern "C" {}
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#if !defined (MAXNAMLEN)
#  define MAXNAMLEN NAME_MAX
#endif /* !MAXNAMLEN */

// At least compile on some of the platforms without <ACE_DIR> info yet.
#if !defined (ACE_HAS_DIRENT)
typedef int ACE_DIR;
struct dirent {
};
#endif /* ACE_HAS_DIRENT */

#if defined (ACE_LACKS_STRUCT_DIR)
struct dirent {
  unsigned short d_ino;
  unsigned short d_off;
  unsigned short d_reclen;
  // This must be a ACE_TCHAR * and not a one element
  // ACE_TCHAR array.  It causes problems on wide
  // character builds with Visual C++ 6.0.
  ACE_TCHAR *d_name;
};

struct ACE_DIR {
  /// The name of the directory we are looking into
  ACE_TCHAR *directory_name_;

  /// Remember the handle between calls.
  HANDLE current_handle_;

  /// The struct for the results
  dirent *dirent_;

  /// The struct for intermediate results.
  ACE_TEXT_WIN32_FIND_DATA fdata_;

  /// A flag to remember if we started reading already.
  int started_reading_;
};
#elif defined (ACE_PSOS) && !defined (ACE_PSOS_DIAB_PPC)
// Create our own definition of the DIR struct, like what
// is available in the newer DIAB PPC header files
struct ACE_DIR
{
  /// The directory handle
  XDIR            xdir;

  /// The directory entry
  struct dirent   dirent;
};
#else
typedef DIR ACE_DIR;
# endif /* ACE_LACKS_STRUCT_DIR */

#if defined rewinddir
# undef rewinddir
#endif /* rewinddir */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#include "ace/post.h"
#endif /* ACE_OS_INCLUDE_OS_DIRENT_H */
