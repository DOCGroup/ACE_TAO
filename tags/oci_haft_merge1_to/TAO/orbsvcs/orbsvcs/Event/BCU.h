/* -*- C++ -*- */
//=============================================================================
/**
 *  @file BCU.h
 *
 *  $Id$
 *
 *  @author David Levine and Tim Harrison (harrison@cs.wustl.edu)
 *
 *
 *  Times how long it takes to generate each of the first N prime
 *  numbers.
 *
 */
//=============================================================================


#ifndef ACE_BCU_H
#define ACE_BCU_H
#include "ace/pre.h"

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "old_event_export.h"

// Check if a specific, hardcoded number is prime (via ACE::is_prime) <n>
// times.
TAO_RTOLDEvent_Export u_long ACE_BCU (u_long n);

// Check if <number> is prime (via ACE::is_prime ()) <n> times.
TAO_RTOLDEvent_Export u_long ACE_BCU (u_long number, u_long n);

#include "ace/post.h"
#endif /* ACE_BCU_H */
