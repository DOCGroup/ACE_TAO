/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//    ace ORB
//
// = FILENAME
//    Benchmark Computation Units
//
// = AUTHOR
//    David Levine and Tim Harrison (harrison@cs.wustl.edu)
//
// = DESCRIPTION
//
//    Times how long it takes to generate each of the first N prime
//    numbers.
// ============================================================================

#ifndef ACE_BCU_H
#define ACE_BCU_H

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/orbsvcs_export.h"


TAO_ORBSVCS_Export u_long ACE_BCU (u_long n);
// Check if a specific, hardcoded number is prime (via ACE::is_prime) <n>
// times.


TAO_ORBSVCS_Export u_long ACE_BCU (u_long number, u_long n);
// Check if <number> is prime (via ACE::is_prime ()) <n> times.

#endif /* ACE_BCU_H */
