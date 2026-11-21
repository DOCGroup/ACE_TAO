/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    ID_Generator.h
 *
 *  @author Nanbor Wang
 */
//=============================================================================

#ifndef ACE_ID_GENERATOR_H
#define ACE_ID_GENERATOR_H

#include "ace/config-all.h"
#include "ace/Synch_Traits.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#define ACE_OFFER_ID_LENGTH 21

/**
 * @class ACE_ID_Generator
 *
 * @brief Generates unique offer IDs.
 *
 * IDs are based on the current time and are unique within a process, including
 * when generated concurrently by multiple threads.
 */
class ACE_ID_Generator
{
public:
  /// allocate a new ID string and point <id> to it.
  static char *get_new_id (char *id);

private:
  /// Atomically get info required to generate an offer ID.
  static void get_serial_id (time_t &t, size_t &s);

  /// Get the lock instance.
  static ACE_SYNCH_MUTEX &get_lock ();

  /// Record the time last offer ID generated.
  static time_t last_time_;

  /// Record serial number of last offer ID with same
  /// generation time.
  static size_t last_number_;
};

#endif /* ACE_ID_GENERATOR_H */
