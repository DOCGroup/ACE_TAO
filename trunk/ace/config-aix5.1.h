// $Id$
//
// Config file for AIX 5.1

// This define is needed for building with Visual Age C++ 5 in incremental
// mode. In the batch mode build, platform_aix_ibm.GNU sets it. The incremental
// mode compiler won't be supported after ACE 5.3, so this will also go away
// in that timeframe, so don't worry about future AIX versions.
#if !defined (ACE_AIX_VERS)
#  define ACE_AIX_VERS 501
#endif

#include "ace/config-aix-4.x.h"

// AIX 5.1 has AIO, but it doesn't have the same API as other POSIX
// systems, and the semantics of operations are a bit different. Will take
// some real work to get this going.
//#define ACE_HAS_AIO_CALLS

// I think this is correct, but needs to be verified...   -Steve Huston
#define ACE_HAS_SIGTIMEDWAIT
