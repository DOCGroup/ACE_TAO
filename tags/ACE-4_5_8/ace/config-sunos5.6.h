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

#if (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 199506L) || \
    defined (__EXTENSIONS__)
# undef  ACE_HAS_ONEARG_SIGWAIT
# define ACE_HAS_PTHREADS_1003_DOT_1C
# define ACE_HAS_2_PARAM_ASCTIME_R_AND_CTIME_R
// Hack 'cuz -DPOSIX_SOURCE=199506L and -DEXTENSIONS hides this.
# include <sys/types.h>
  extern "C" int madvise(caddr_t, size_t, int);
#endif /* _POSIX_C_SOURCE >= 199506L  ||  __EXTENSIONS__ */

#define ACE_THREAD_POSIX_SEM

// SunOS 5.6 does support sched_get_priority_{min,max}
#undef ACE_THR_PRI_FIFO_DEF

#endif /* ACE_CONFIG_H */
