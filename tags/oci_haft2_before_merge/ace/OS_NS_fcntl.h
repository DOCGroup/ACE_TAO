// -*- C++ -*-

//=============================================================================
/**
 *  @file   OS_NS_fcntl.h
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

#ifndef ACE_OS_NS_FCNTL_H
# define ACE_OS_NS_FCNTL_H

# include /**/ "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#  pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/os_fcntl.h"
#include "ace/Global_Macros.h"  // for LPSECURITY_ATTRIBUTES :-(
#include "ace/ACE_export.h"

#if defined (ACE_EXPORT_MACRO)
#  undef ACE_EXPORT_MACRO
#endif
#define ACE_EXPORT_MACRO ACE_Export

namespace ACE_OS {

  ACE_NAMESPACE_INLINE_FUNCTION
  int fcntl (ACE_HANDLE handle,
             int cmd,
             long arg = 0);

  // It used to be that the <perms> argument default was 0 on all
  // platforms. Further, the ACE_OS::open implementations ignored <perms>
  // for Win32 and always supplied read|write|delete. To preserve
  // backward compatibility and allow users to pass in values
  // that are used as desired, the defaults are now what the default
  // action used to be on Win32. The implementation now obeys what is passed.
#if defined (ACE_WIN32)
#  if defined (ACE_HAS_WINNT4) && (ACE_HAS_WINNT4 == 1)
#    define ACE_DEFAULT_OPEN_PERMS (FILE_SHARE_READ | FILE_SHARE_WRITE | \
                                    FILE_SHARE_DELETE)
#  else
#    define ACE_DEFAULT_OPEN_PERMS (FILE_SHARE_READ | FILE_SHARE_WRITE)
#  endif /* ACE_HAS_WINCE */
#else
#  define ACE_DEFAULT_OPEN_PERMS 0
#endif  /* ACE_WIN32 */

  /// The O_APPEND flag is only partly supported on Win32. If you specify
  /// O_APPEND, then the file pointer will be positioned at the end of
  /// the file initially during open, but it is not re-positioned at
  /// the end prior to each write, as specified by POSIX.  This
  /// is generally good enough for typical situations, but it is ``not
  /// quite right'' in its semantics.
  extern ACE_Export 
  ACE_HANDLE open (const char *filename,
                   int mode,
                   int perms = ACE_DEFAULT_OPEN_PERMS,
                   LPSECURITY_ATTRIBUTES sa = 0);
#if defined (ACE_HAS_WCHAR)
  extern ACE_Export 
  ACE_HANDLE open (const wchar_t *filename,
                   int mode,
                   int perms = ACE_DEFAULT_OPEN_PERMS,
                   LPSECURITY_ATTRIBUTES sa = 0);
#endif /* ACE_HAS_WCHAR */

} /* namespace ACE_OS */

# if defined (ACE_HAS_INLINED_OSCALLS)
#   if defined (ACE_INLINE)
#     undef ACE_INLINE
#   endif /* ACE_INLINE */
#   define ACE_INLINE inline
#   include "ace/OS_NS_fcntl.inl"
# endif /* ACE_HAS_INLINED_OSCALLS */

# include /**/ "ace/post.h"
#endif /* ACE_OS_NS_FCNTL_H */
