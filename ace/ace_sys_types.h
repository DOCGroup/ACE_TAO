/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    ace_sys_types.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt (schmidt@cs.wustl.edu)
 *  @author etc
 */
//=============================================================================

#ifndef ACE_ACE_SYS_TYPES_H
#define ACE_ACE_SYS_TYPES_H
#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

# if defined (ACE_HAS_WINCE)
#   include /**/ <types.h>

//typedef DWORD  nlink_t;

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

typedef unsigned int dev_t;

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

# else /* ! ACE_HAS_WINCE */
#   if defined (ACE_LACKS_SYS_TYPES_H)
#     if ! defined (ACE_PSOS)
  typedef unsigned char u_char;
  typedef unsigned short u_short;
  typedef unsigned int u_int;
  typedef unsigned long u_long;

  typedef unsigned char uchar_t;
  typedef unsigned short ushort_t;
  typedef unsigned int  uint_t;
  typedef unsigned long ulong_t;
#     endif /* ! defined (ACE_PSOS) */
#   else
#     include /**/ <sys/types.h>
#   endif  /* ACE_LACKS_SYS_TYPES_H */

#   if ! defined (ACE_PSOS)
#     include /**/ <sys/stat.h>
#   endif
# endif /* ACE_HAS_WINCE */

// this is normally defined in sys/select.h, but since it is included
// in sys/types, this will work for now.
# if defined (ACE_SELECT_USES_INT)
typedef int ACE_FD_SET_TYPE;
# else
typedef fd_set ACE_FD_SET_TYPE;
# endif /* ACE_SELECT_USES_INT */

// this doesn't belong here, but until I get an ace_aio.h file, here will be fine.
// Defining POSIX4 real-time signal range.
# if defined ACE_HAS_AIO_CALLS
#   define ACE_SIGRTMIN SIGRTMIN
#   define ACE_SIGRTMAX SIGRTMAX
# else /* !ACE_HAS_AIO_CALLS */
#   define ACE_SIGRTMIN 0
#   define ACE_SIGRTMAX 0
# endif /* ACE_HAS_AIO_CALLS */

# if defined (ACE_WIN32) && ! defined (ACE_HAS_WINCE) \
                         && ! defined (ACE_HAS_PHARLAP)
typedef TRANSMIT_FILE_BUFFERS ACE_TRANSMIT_FILE_BUFFERS;
typedef LPTRANSMIT_FILE_BUFFERS ACE_LPTRANSMIT_FILE_BUFFERS;
typedef PTRANSMIT_FILE_BUFFERS ACE_PTRANSMIT_FILE_BUFFERS;

#   define ACE_INFINITE INFINITE
#   define ACE_STATUS_TIMEOUT STATUS_TIMEOUT
#   define ACE_WAIT_FAILED WAIT_FAILED
#   define ACE_WAIT_TIMEOUT WAIT_TIMEOUT
# else /* ACE_WIN32 */
struct ACE_TRANSMIT_FILE_BUFFERS
{
  void *Head;
  size_t HeadLength;
  void *Tail;
  size_t TailLength;
};
typedef ACE_TRANSMIT_FILE_BUFFERS* ACE_PTRANSMIT_FILE_BUFFERS;
typedef ACE_TRANSMIT_FILE_BUFFERS* ACE_LPTRANSMIT_FILE_BUFFERS;

#   if !defined (ACE_INFINITE)
#     define ACE_INFINITE LONG_MAX
#   endif /* ACE_INFINITE */
#   define ACE_STATUS_TIMEOUT LONG_MAX
#   define ACE_WAIT_FAILED LONG_MAX
#   define ACE_WAIT_TIMEOUT LONG_MAX
# endif /* ACE_WIN32 */

// Default size of the ACE Reactor.
# if defined (FD_SETSIZE)
int const ACE_FD_SETSIZE = FD_SETSIZE;
# else
#   define ACE_FD_SETSIZE FD_SETSIZE
# endif /* ACE_FD_SETSIZE */


#endif /* ACE_ACE_SYS_TYPES_H */
