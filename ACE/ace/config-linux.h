// -*- C++ -*-
//
// $Id$

// The following configuration file is designed to work for Linux
// platforms using GNU C++.

#ifndef ACE_CONFIG_LINUX_H
#define ACE_CONFIG_LINUX_H
#include /**/ "ace/pre.h"

#if !defined (ACE_MT_SAFE)
#  define ACE_MT_SAFE 1
#endif

#define ACE_PLATFORM_CONFIG config-linux.h

#include "ace/config-linux-common.h"

#define ACE_HAS_SVR4_DYNAMIC_LINKING
#define ACE_HAS_AUTOMATIC_INIT_FINI
#define ACE_HAS_DLSYM_SEGFAULT_ON_INVALID_HANDLE
#define ACE_HAS_CLOCK_GETTIME
#define ACE_HAS_CLOCK_SETTIME
#define ACE_HAS_RECURSIVE_MUTEXES
#define ACE_HAS_THREAD_SPECIFIC_STORAGE
#define ACE_HAS_RECURSIVE_THR_EXIT_SEMANTICS
#define ACE_HAS_2_PARAM_ASCTIME_R_AND_CTIME_R

#if (__GLIBC__ == 2 && __GLIBC_MINOR__ > 3)
#  define ACE_HAS_REENTRANT_FUNCTIONS
#endif

#include /**/ "ace/post.h"

#endif /* ACE_CONFIG_LINUX_H */
