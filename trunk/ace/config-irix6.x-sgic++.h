/* -*- C++ -*- */
// $Id$

// Use this file for IRIX 6.[234] if you have the pthreads patches
// installed.

#ifndef ACE_CONFIG_IRIX6X_H
#define ACE_CONFIG_IRIX6X_H

// Include basic (non-threaded) configuration
#include <ace/config-irix6.x-sgic++-nothreads.h>

// Add threading support

#define ACE_HAS_IRIX62_THREADS
#define ACE_HAS_PTHREAD_SIGMASK
#define ACE_HAS_UALARM

// Needed for the threading stuff?
#include /**/ <task.h>
#define PTHREAD_MIN_PRIORITY PX_PRIO_MIN
#define PTHREAD_MAX_PRIORITY PX_PRIO_MAX

// ACE supports threads.
#define ACE_HAS_THREADS

// Platform has no implementation of pthread_condattr_setpshared(),
// even though it supports pthreads! (like Irix 6.2)
#define ACE_LACKS_CONDATTR_PSHARED
#define ACE_LACKS_MUTEXATTR_PSHARED

// IRIX 6.2 supports a variant of POSIX Pthreads, supposedly POSIX 1c
#define ACE_HAS_PTHREADS
#define ACE_HAS_PTHREADS_STD

// Platforms has pthread_thr_sigsetmask
#define ACE_HAS_PTHREAD_SIGMASK

// Compiler/platform has thread-specific storage
#define ACE_HAS_THREAD_SPECIFIC_STORAGE

// The pthread_cond_timedwait call does not reset the timer.
#define ACE_LACKS_COND_TIMEDWAIT_RESET 1

// When threads are enabled READDIR_R is supported on IRIX.
#undef ACE_LACKS_READDIR_R

// Compile using multi-thread libraries
#if !defined (ACE_MT_SAFE)
  #define ACE_MT_SAFE 1
#endif /* ACE_MT_SAFE */

#endif /* ACE_CONFIG_IRIX6X_H */
