/* -*- C++ -*- */
// $Id$

// For IRIX 6.[234] with pthreads support.
// Note: Exceptions are supported in -n32 mode.
// You can select between -32 (aka -o32) and -n32 mode by selecting either
// platform_irix6.x-32_sgic++.GNU or platform_irix6.x-n32_sgic++.GNU

// For IRIX 6.2 there are several patches that should be applied to
// get reliable operation with multi-threading and exceptions.
// Specifically you should get a reasonable current IRIX, Compiler
// and POSIX patch-sets.

// For IRIX 6.[34] it's less critical, but it's still recommended
// that you apply the applicable patch-sets (IRIX and Compiler I believe).

// These patches are updated frequently, so you should ask your support
// contact or search SGI's web site (http://www.sgi.com) for the latest
// version.

// NOTE: We can't call this ACE_CONFIG_H, it's used by an included header file!
#if !defined (ACE_IRIX6_CONFIG_H)
#define ACE_IRIX6_CONFIG_H

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

#endif /* ACE_IRIX6_CONFIG_H */
