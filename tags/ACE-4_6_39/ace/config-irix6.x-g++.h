/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for the SGI
// Indigo2EX running Irix 6.2 platform using the GNU C++ Compiler

#ifndef ACE_CONFIG_H
#define ACE_CONFIG_H

#include "ace/config-g++-common.h"


// Platform supports the very odd IRIX 6.2 threads...
#define ACE_HAS_THREADS
#if !defined (ACE_MT_SAFE)
# define ACE_MT_SAFE 1
#endif /* !ACE_MT_SAFE */
#define ACE_HAS_IRIX62_THREADS

// IRIX 6.2 supports a variant of POSIX Pthreads, supposedly POSIX 1c
#define ACE_HAS_PTHREADS
#define ACE_HAS_PTHREADS_STD

#include "ace/config-irix6.x-common.h"

// Needed for the threading stuff?
#include /**/ <sched.h>
#include /**/ <task.h>

#define PTHREAD_MIN_PRIORITY PX_PRIO_MIN
#define PTHREAD_MAX_PRIORITY PX_PRIO_MAX

// Compiler/platform has thread-specific storage
#define ACE_HAS_THREAD_SPECIFIC_STORAGE

#define ACE_HAS_STRING_CLASS
#define IRIX6

// Denotes that GNU has cstring.h as standard
// which redefines memchr()
#define ACE_HAS_GNU_CSTRING_H

// Compiler/platform supports SVR4 signal typedef.
#define ACE_HAS_IRIX_53_SIGNALS

// Compiler/platform supports sys_siglist array.
//#define ACE_HAS_SYS_SIGLIST


#endif /* ACE_CONFIG_H */
