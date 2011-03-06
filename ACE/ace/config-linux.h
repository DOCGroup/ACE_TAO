// -*- C++ -*-
//
// $Id$

// The following configuration file is designed to work for Linux
// platforms using GNU C++.

#ifndef ACE_CONFIG_LINUX_H
#define ACE_CONFIG_LINUX_H
#include /**/ "ace/pre.h"

#define ACE_PLATFORM_CONFIG config-linux.h

#include "ace/config-linux-common.h"

#define ACE_HAS_SVR4_DYNAMIC_LINKING
#define ACE_HAS_AUTOMATIC_INIT_FINI
#define ACE_HAS_DLSYM_SEGFAULT_ON_INVALID_HANDLE

#if !defined (ACE_MT_SAFE)
#define ACE_MT_SAFE 1                           // JCEJ 12/22/96        #1
#endif

#if ACE_MT_SAFE
// Yes, we do have threads.
#define ACE_HAS_THREADS
// And they're even POSIX pthreads (LinuxThreads implementation)
#define ACE_HAS_PTHREADS

#define ACE_HAS_CLOCK_GETTIME
#define ACE_HAS_CLOCK_SETTIME

#define ACE_HAS_RECURSIVE_MUTEXES

#define ACE_HAS_THREAD_SPECIFIC_STORAGE     // jcej 12/22/96         #2

#define ACE_HAS_RECURSIVE_THR_EXIT_SEMANTICS    // JCEJ 1/7-8/96

// Platform supports reentrant functions (i.e., all the POSIX *_r
// functions).
#define ACE_HAS_REENTRANT_FUNCTIONS

// uses ctime_r & asctime_r with only two parameters vs. three
#define ACE_HAS_2_PARAM_ASCTIME_R_AND_CTIME_R
#endif  /* ACE_MT_SAFE */

#include /**/ "ace/post.h"

#endif /* ACE_CONFIG_LINUX_H */
