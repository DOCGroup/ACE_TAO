/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for SunOS 5.8
// (Solaris 8) platforms using the SunC++ 4.x, 5.x, 6.x, or g++ compilers.

#ifndef ACE_CONFIG_H

// ACE_CONFIG_H is defined by one of the following #included headers.

// #include the SunOS 5.7 config, then add any SunOS 5.8 updates below.

#include "ace/config-sunos5.7.h"

# if defined (_POSIX_PTHREAD_SEMANTICS)
#ifdef ACE_LACKS_RWLOCK_T
#undef ACE_LACKS_RWLOCK_T
#endif /* ACE_LACKS_RWLOCK_T */
#endif /* _POSIX_PTHREAD_SEMANTICS */

#endif /* ACE_CONFIG_H */
