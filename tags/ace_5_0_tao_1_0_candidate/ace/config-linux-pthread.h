/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for Linux
// platforms using GNU C++ and the MIT threads package.

#ifndef ACE_CONFIG_H
#define ACE_CONFIG_H

#include "ace/config-linux-common.h"

#define ACE_HAS_THREADS
#define ACE_HAS_THREAD_SPECIFIC_STORAGE
#if !defined (ACE_MT_SAFE)
# define ACE_MT_SAFE 1
#endif
// And they're even POSIX pthreads (MIT implementation)
#define ACE_HAS_PTHREADS
#define ACE_HAS_PTHREADS_STD
#define ACE_LACKS_RWLOCK_T
#define ACE_HAS_SIGWAIT
// If ACE doesn't compile due to the lack of these methods, please
// send email to schmidt@cs.wustl.edu reporting this.
// #define ACE_LACKS_CONDATTR_PSHARED
// #define ACE_LACKS_MUTEXATTR_PSHARED

// To use pthreads on Linux you'll need to use the MIT version, for
// now...
#define _MIT_POSIX_THREADS 1
#include /**/ <pthread/mit/pthread.h>

#endif /* ACE_CONFIG_H */
