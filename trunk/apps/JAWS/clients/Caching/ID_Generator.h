/* -*- C++ -*- */

// $Id$

// ============================================================================
//
// = LIBRARY
//    none
//
// = FILENAME
//    ID_Generator.h
//
// = AUTHOR
//    Nanbor Wang
//
// ============================================================================

#if !defined (ACE_ID_GENERATOR_H)
#define ACE_ID_GENERATOR_h

#include "ace/Synch.h"

#define ACE_OFFER_ID_LENGTH 21

class ACE_Export ACE_ID_Generator
// Generate an offer ID according to current time and
// avoid duplicate ID.  Using a similar method like 
// beckry's algorithm.
{
public:
  static char *get_new_id (char *id);
  // allocate a new ID string and point <id> to it.
private:
  static void get_serial_id (time_t &t, size_t &s);
  // Atomically get info required to generate an offer ID.

  ACE_MT (static ACE_SYNCH_MUTEX *get_lock ();)
  // Get the lock instance.
  
  static time_t last_time_;
  // Record the time last offer ID generated.

  static size_t last_number_;
  // Record serial number of last offer ID with same 
  // generation time.

  ACE_MT (static ACE_SYNCH_MUTEX *lock_;)
  // mutex to access private member.
};

// #if defined (__ACE_INLINE__)
// #include "ID_Generator.i"
// #endif /* __ACE_INLINE__ */

#endif /* ACE_ID_GENERATOR_H */
