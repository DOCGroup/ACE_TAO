// $Id$
//
// Config file for AIX 5.1

#include "ace/config-aix-4.x.h"

// AIX 5.1 has AIO, but it doesn't have the same API as other POSIX
// systems, and the semantics of operations are a bit different. Will take
// some real work to get this going.
//#define ACE_HAS_AIO_CALLS

// I think this is correct, but needs to be verified...   -Steve Huston
#define ACE_HAS_SIGTIMEDWAIT
