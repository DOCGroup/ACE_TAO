// $Id$
//
// Config file for AIX 5.1 and 5.2.

// This define is needed for building with Visual Age C++ 5 in incremental
// mode. In the batch mode build, platform_aix_ibm.GNU sets it. The incremental
// mode compiler won't be supported after ACE 5.3, so this will also go away
// in that timeframe, so don't worry about future AIX versions.
#if !defined (ACE_AIX_VERS)
#  define ACE_AIX_VERS 501
#endif

// AIX 5.1 has AIO, but it doesn't have the same API as other POSIX
// systems, and the semantics of operations are a bit different. Will take
// some real work to get this going.
// AIX 5.2, however, has the POSIX API implemented. However, the libc functions
// to implement it aren't exported by default. You need to use smit to enable
// them. So, leave AIO disabled unless the user explicitly enables it.
// config-aix-4.x.h will set ACE_HAS_AIO_CALLS if config-posix.h senses the
// feature-test macros, so set up ACE_HAS_AIO_CALLS as 0 if the user didn't
// set it. Then check for 0 after including config-aix-4.x.h and remove it
// if so.
#if !defined (ACE_HAS_AIO_CALLS)
#  define _ACE_DISABLE_AIO_CALLS_
#endif
#include "ace/config-aix-4.x.h"
#if defined (_ACE_DISABLE_AIO_CALLS_)
#  if defined (ACE_HAS_AIO_CALLS)
#    undef ACE_HAS_AIO_CALLS
#  endif
#  undef _ACE_DISABLE_AIO_CALLS_
#endif

// I think this is correct, but needs to be verified...   -Steve Huston
#define ACE_HAS_SIGTIMEDWAIT

// AIX 5.1 has netinet/tcp.h
#undef ACE_LACKS_NETINET_TCP_H
