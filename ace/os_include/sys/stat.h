// -*- C++ -*-

//=============================================================================
/**
 *  @file    stat.h
 *
 *  data returned by the stat() function 
 *
 *  $Id$
 *
 *  @author Don Hinton <dhinton@ieee.org>
 *  @author This code was originally in various places including ace/OS.h.
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_SYS_STAT_H
#define ACE_OS_INCLUDE_SYS_STAT_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/sys/types.h"

#if defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)
#  include <direct.h>
#endif /* ACE_WIN32 && !ACE_HAS_WINCE */

#if !defined (ACE_LACKS_SYS_STAT_H)
# include /**/ <sys/stat.h>
#endif /* !ACE_LACKS_SYS_STAT_H */

# if defined (ACE_LACKS_MODE_MASKS)
// MODE MASKS

// the following macros are for POSIX conformance.

#   if !defined (ACE_HAS_USER_MODE_MASKS)
#     define S_IRWXU 00700         /* read, write, execute: owner. */
#     define S_IRUSR 00400         /* read permission: owner. */
#     define S_IWUSR 00200         /* write permission: owner. */
#     define S_IXUSR 00100         /* execute permission: owner. */
#   endif /* ACE_HAS_USER_MODE_MASKS */
#   define S_IRWXG 00070           /* read, write, execute: group. */
#   define S_IRGRP 00040           /* read permission: group. */
#   define S_IWGRP 00020           /* write permission: group. */
#   define S_IXGRP 00010           /* execute permission: group. */
#   define S_IRWXO 00007           /* read, write, execute: other. */
#   define S_IROTH 00004           /* read permission: other. */
#   define S_IWOTH 00002           /* write permission: other. */
#   define S_IXOTH 00001           /* execute permission: other. */

// eVC doesn't define these bits, but MSVC does. The bit settings are copied
// from the MSVC defs.
#   if defined (ACE_HAS_WINCE)
#     define S_IFDIR 0040000
#     define S_IFREG 0100000
#   endif /* ACE_HAS_WINCE */
# endif /* ACE_LACKS_MODE_MASKS */

#if defined (ACE_HAS_WINCE)
struct stat
{
    /// always 0 on Windows platforms
    dev_t st_dev;

    /// always 0 on Windows platforms
    dev_t st_rdev;

    /// file attribute
    unsigned short st_mode;

    /// number of hard links
    short st_nlink;

    /// time of last access
    ACE_Time_Value st_atime;

    /// time of last data modification
    ACE_Time_Value st_mtime;

    /// time of creation
    ACE_Time_Value st_ctime;

    /// file size, in bytes
    off_t st_size;

    // Following members do not have direct conversion in Window platforms.
//    u_long st_blksize;        // optimal blocksize for I/O
//    u_long st_flags;          // user defined flags for file
};
#endif /* ACE_HAS_WINCE */


#include "ace/post.h"
#endif /* ACE_OS_INCLUDE_SYS_STAT_H */
