/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for SunOS 5.6
// platforms using the SunC++ 4.x or g++ compilers.

#ifndef ACE_CONFIG_H

// ACE_CONFIG_H is defined by one of the following #included headers.

// #include the SunOS 5.5 config file, then add SunOS 5.6 updates below.

#include "ace/config-sunos5.5.h"

#if (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 199506L) || \
    defined (__EXTENSIONS__)
# define ACE_HAS_2_PARAM_ASCTIME_R_AND_CTIME_R
# define ACE_HAS_SIGWAIT
// Hack 'cuz -DPOSIX_SOURCE=199506L and -DEXTENSIONS hides this.
# include <sys/types.h>
  extern "C" int madvise(caddr_t, size_t, int);
#endif /* _POSIX_C_SOURCE >= 199506L  ||  __EXTENSIONS__ */

// SunOS 5.6 has AIO calls.
#define ACE_HAS_AIO_CALLS

// To match the LynxOS's definition of the aiocb structure.
#define AIO_SYSRETURN aio_resultp.aio_return
#define AIO_SYSERRNO aio_resultp.aio_errno
#endif /* ACE_CONFIG_H */
