/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for SunOS 5.6
// platforms using the SunC++ 4.x or g++ compilers.

#if !defined (ACE_CONFIG_H)

// ACE_CONFIG_H is defined by one of the following #included headers.

// #include the SunOS 5.5 config file, then add SunOS 5.6 updates below.

#if defined (__SUNPRO_CC)
# include "ace/config-sunos5.5-sunc++-4.x.h"
#elif defined (__GNUG__)
# include "ace/config-sunos5.5-g++.h"
#else  /* ! __SUNPRO_GNUG__ && ! __KCC */
# error unsupported compiler in ace/config-sunos5.6.h
#endif /* ! __GNUG__ && ! __KCC */

// SunOS 5.6 does support sched_get_priority_{min,max}
#undef ACE_THR_PRI_FIFO_DEF
#define ACE_THREAD_POSIX_SEM
#endif /* ACE_CONFIG_H */
