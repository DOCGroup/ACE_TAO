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
// Nanbor, please make sure you use the official ACE "= TITLE" syntax
// here.  Also, what is "beckry's algorithm?"  Do you mean "Berkeley's
// algorithm?"
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

  static ACE_SYNCH_MUTEX *get_lock (void);
  // Get the lock instance.
  
  static time_t last_time_;
  // Record the time last offer ID generated.

  static size_t last_number_;
  // Record serial number of last offer ID with same 
  // generation time.

  static ACE_SYNCH_MUTEX *lock_;
  // mutex to access private member.
};

#endif /* ACE_ID_GENERATOR_H */
