// -*- C++ -*-

//=============================================================================
/**
 *  @file   OS_NS_sys_stat.h
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

#ifndef ACE_OS_NS_SYS_STAT_H
# define ACE_OS_NS_SYS_STAT_H

# include /**/ "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#  pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/sys/os_types.h"
#include "ace/Default_Constants.h" // for ACE_DEFAULT_DIR_PERMS

# if defined (ACE_WIN32) && !defined (ACE_HAS_WINCE) && !defined (__BORLANDC__)
      typedef struct _stat ACE_stat;
# else
      typedef struct stat ACE_stat;
# endif /* ACE_WIN32 */

namespace ACE_OS {

  ACE_HANDLE creat (const ACE_TCHAR *filename,
                    mode_t mode);

  //@{ @name A set of wrappers for low-level file operations.

  // non-standard
  long filesize (ACE_HANDLE handle);

  long filesize (const ACE_TCHAR *handle);


  //@}

  int fstat (ACE_HANDLE,
             ACE_stat *);

  int lstat (const char *,
             ACE_stat *);

  int mkdir (const ACE_TCHAR *path,
             mode_t mode = ACE_DEFAULT_DIR_PERMS);

  int mkfifo (const ACE_TCHAR *file,
              mode_t mode = ACE_DEFAULT_FILE_PERMS);

  int stat (const ACE_TCHAR *file, ACE_stat *);

  mode_t umask (mode_t cmask);

} /* namespace ACE_OS */

# if defined (ACE_HAS_INLINED_OSCALLS)
#   if defined (ACE_INLINE)
#     undef ACE_INLINE
#   endif /* ACE_INLINE */
#   define ACE_INLINE inline
#   include "ace/OS_NS_sys_stat.inl"
# endif /* ACE_HAS_INLINED_OSCALLS */

# include /**/ "ace/post.h"
#endif /* ACE_OS_NS_SYS_STAT_H */
