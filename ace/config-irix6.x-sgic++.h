/* -*- C++ -*- */
// $Id$

// NOTE: THIS IS NOT A COMPLETE CONFIG FILE.

// Here we setup the common config for IRIX 6.2, 6.4 and hopefully 6.3
// but with pthreads enabled.

// This file is automatically included in several irix6.* config
// files, but is *not* included in the irix6.2-nothreads file.

#if !defined (ACE_CONFIG_IRIX6X_H)
#define ACE_CONFIG_IRIX6X_H

// Include basic (non-threaded) configuration
#include <ace/config-irix6.x-sgic++-nothreads.h>

// Add threading support

#define ACE_HAS_IRIX62_THREADS
#define ACE_HAS_PTHREAD_SIGMASK

// Needed for the threading stuff?
#include /**/ <sched.h>
#include /**/ <task.h>
#define PTHREAD_MIN_PRIORITY PX_PRIO_MIN
#define PTHREAD_MAX_PRIORITY PX_PRIO_MAX

// ACE supports threads.
#define ACE_HAS_THREADS

// Platform has no implementation of pthread_condattr_setpshared(),
// even though it supports pthreads! (like Irix 6.2)
#define ACE_LACKS_CONDATTR_PSHARED

// Platform lacks pthread_thr_sigsetmask (e.g., MVS, HP/UX, OSF/1 3.2,
// Irix 6.2)
#define ACE_LACKS_PTHREAD_THR_SIGSETMASK

// IRIX 6.2 supports a variant of POSIX Pthreads, supposedly POSIX 1c
#define ACE_HAS_PTHREADS

// Platform has pthread_equal().
#define ACE_HAS_PTHREAD_EQUAL

// Platform has pthread_t defined.
#define ACE_HAS_PTHREAD_T

// Compiler/platform has thread-specific storage
#define ACE_HAS_THREAD_SPECIFIC_STORAGE

// Compile using multi-thread libraries
#if !defined (ACE_MT_SAFE)
  #define ACE_MT_SAFE 1
#endif /* ACE_MT_SAFE */

#endif /* ACE_CONFIG_IRIX6X_H */
