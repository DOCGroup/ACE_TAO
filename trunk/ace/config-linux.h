/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for Linux
// platforms using GNU C++.

#ifndef ACE_CONFIG_H
#define ACE_CONFIG_H
#include "ace/pre.h"

#include "ace/config-linux-common.h"

#define ACE_HAS_SVR4_DYNAMIC_LINKING
#define ACE_HAS_AUTOMATIC_INIT_FINI

#if !defined (ACE_MT_SAFE)
#define ACE_MT_SAFE 1                           // JCEJ 12/22/96        #1
#endif

#if ACE_MT_SAFE
// Yes, we do have threads.
#define ACE_HAS_THREADS
// And they're even POSIX pthreads (LinuxThreads implementation)
#define ACE_HAS_PTHREADS

// ... and the final standard even!
#define ACE_HAS_PTHREADS_STD
#define ACE_HAS_THREAD_SPECIFIC_STORAGE     // jcej 12/22/96         #2

#define ACE_LACKS_THREAD_STACK_ADDR             // JCEJ 12/17/96
#define ACE_LACKS_THREAD_STACK_SIZE             // JCEJ 12/17/96

#define ACE_LACKS_RWLOCK_T                      // JCEJ 12/23/96        #1
#define ACE_HAS_RECURSIVE_THR_EXIT_SEMANTICS    // JCEJ 1/7-8/96

#if defined(__GLIBC__)
// Platform supports reentrant functions (i.e., all the POSIX *_r
// functions).
#define ACE_HAS_REENTRANT_FUNCTIONS
// getprotobyname_r have a different signature!
#define ACE_LACKS_NETDB_REENTRANT_FUNCTIONS
// uses ctime_r & asctime_r with only two parameters vs. three
#define ACE_HAS_2_PARAM_ASCTIME_R_AND_CTIME_R
#endif
#include /**/ <pthread.h>
#endif /*ACE_MT_SAFE*/


#include "ace/post.h"
#endif /* ACE_CONFIG_H */
