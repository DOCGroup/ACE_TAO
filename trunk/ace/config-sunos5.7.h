/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for SunOS 5.7
// (Solaris 7) platforms using the SunC++ 4.x or g++ compilers.

#ifndef ACE_CONFIG_H

// ACE_CONFIG_H is defined by one of the following #included headers.

// #include the SunOS 5.6 config file, then add SunOS 5.7 updates below.

#include "ace/config-sunos5.6.h"

#if defined (__GNUG__)
# if __GNUC__ <= 2  &&  __GNUC_MINOR__ < 8
    // Assume that later g++ were built on SunOS 5.7, so they don't
    // have these problems . . .

    // Disable the following, because g++ 2.7.2.3 can't handle it.
    // Maybe later g++ versions can?
#   undef ACE_HAS_XPG4_MULTIBYTE_CHAR

    // The Solaris86 g++ 2.7.2.3 sys/types.h doesn't have these . . .
    typedef long          t_scalar_t;  /* historical versions */
    typedef unsigned long t_uscalar_t;
    typedef void          *timeout_id_t;
# endif /* __GNUC__ <= 2  &&  __GNUC_MINOR__ < 8 */
#endif /* __GNUG__ */

#if defined (__KCC)
typedef unsigned long long uint64_t;
#endif

// Solaris 5.7 supports SCHED_FIFO and SCHED_RR, as well as SCHED_OTHER.
#undef ACE_HAS_ONLY_SCHED_OTHER

#ifdef ACE_HAS_LIMITED_SELECT
#undef ACE_HAS_LIMITED_SELECT
#endif /* ACE_HAS_LIMITED_SELECT */


#endif /* ACE_CONFIG_H */
