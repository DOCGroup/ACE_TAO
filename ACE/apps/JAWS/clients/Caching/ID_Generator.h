/* -*- C++ -*- */


//=============================================================================
/**
 *  @file    ID_Generator.h
 *
 *  $Id$
 *
 *  @author Nanbor Wang
 */
//=============================================================================


#ifndef ACE_ID_GENERATOR_H
#define ACE_ID_GENERATOR_h

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#define ACE_OFFER_ID_LENGTH 21

/**
 * @class ACE_ID_Generator
 *
 * @brief An unique ID generator.
 *
 */
class ACE_ID_Generator

  //     Generate an offer ID according to current time and avoid
  //     duplicate ID.  It guarantees ID uniqueness within a process,
  //     i.e. no two threads may get the same ID string.  Using a
  //     similar method like the backery algorithm.
{
public:
  /// allocate a new ID string and point <id> to it.
  static char *get_new_id (char *id);

private:
  /// Atomically get info required to generate an offer ID.
  static void get_serial_id (time_t &t, size_t &s);

  /// Get the lock instance.
  static ACE_SYNCH_MUTEX *get_lock (void);

  /// Record the time last offer ID generated.
  static time_t last_time_;

  /// Record serial number of last offer ID with same
  /// generation time.
  static size_t last_number_;

  /// mutex to access private member.
  static ACE_SYNCH_MUTEX *lock_;
};

#endif /* ACE_ID_GENERATOR_H */
