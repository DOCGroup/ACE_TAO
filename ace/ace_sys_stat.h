/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    ace_sys_stat.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt (schmidt@cs.wustl.edu)
 *  @author etc
 */
//=============================================================================

#ifndef ACE_ACE_SYS_STAT_H
# define ACE_ACE_SYS_STAT_H
# include "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

# if !defined (ACE_LACKS_SYS_STAT_H) && !defined (ACE_LACKS_UNIX_SYS_HEADERS)
#   include /**/ <sys/stat.h>
# endif /* !ACE_LACKS_SYS_STAT_H && !ACE_LACKS_UNIX_SYS_HEADERS */

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

# endif /* ACE_LACKS_MODE_MASKS */

# if defined (ACE_WINCE)
// ACE_WINCE thing...
// CE's add-on for c-style fstat/stat functionalities.  This struct is
// by no mean complete compared to what you usually find in UNIX
// platforms.  Only members that have direct conversion using Win32's
// BY_HANDLE_FILE_INFORMATION are defined so that users can discover
// non-supported members at compile time.  Time values are of type
// ACE_Time_Value for easy comparison.

// Since CE does not have _stat by default as NT/2000 does, the 'stat'
// struct defined here will be used.  Also note that CE file system
// struct is only for the CE 3.0 or later.
// Refer to the WCHAR.H from Visual C++ and WIBASE.H from eVC 3.0.
struct stat
{
    dev_t st_dev;             // always 0 on Windows platforms
    dev_t st_rdev;            // always 0 on Windows platforms
    unsigned short st_mode;   // file attribute
    short st_nlink;           // number of hard links
    ACE_Time_Value st_atime; // time of last access
    ACE_Time_Value st_mtime; // time of last data modification
    ACE_Time_Value st_ctime;  // time of creation
    off_t st_size;           // file size, in bytes

    // Following members do not have direct conversion in Window platforms.
//    u_long st_blksize;        // optimal blocksize for I/O
//    u_long st_flags;          // user defined flags for file
};
# endif /* ACE_WINCE */

#endif /* ACE_ACE_SYS_STAT_H */
