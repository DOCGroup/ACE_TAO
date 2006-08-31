// -*- C++ -*-

//=============================================================================
/**
 * @file    EventGuard.h
 *
 * $Id$
 *
 * @author James H. Hill <hillj@isis.vanderbilt.edu>
 */
//=============================================================================

#ifndef _CUTS_EVENT_GUARD_H_
#define _CUTS_EVENT_GUARD_H_

#include "cuts/config.h"
#include "cuts/CUTS_export.h"
#include "ace/RW_Thread_Mutex.h"
#include "ace/Thread_Mutex.h"
#include <map>

// forward declarations
class CUTS_Bounded_Counter;

#define CUTS_REGISTER_GUARD_EVENT(counter, guard, event, count) \
  counter = guard.register_event (CUTS_Event_Info <::OBV_##event>::event_id_, count)

//=============================================================================
/**
 * @class CUTS_Event_Guard
 */
//=============================================================================

class CUTS_Export CUTS_Event_Guard
{
public:
  /// Constructor.
  CUTS_Event_Guard (void);

  /// Destructor.
  virtual ~CUTS_Event_Guard (void);

  /**
   * Add a new event counter for the event count. If there
   * is already an event for the specified ID, then the
   * counter will not be added and will return false.
   */
  CUTS_Bounded_Counter * register_event (long id, size_t count);

  /// Remove an existing counter from the guard.
  void unregister_event (long id);

  /**
   * Determine if an event guard is enabled.
   *
   * @retval    true    The guard is enabled.
   * @retval    false   The guard is not enabled.
   */
  bool enabled (void);

  /// Reset the event guard.
  void reset (void);

  /**
   * Handle the event of the specified ID. This will cause the event counter
   * to increment the number of events received.
   */
  void handle_event (long id);

private:
  /// Type definition for the set of counters for the guard.
  typedef std::map <long, CUTS_Bounded_Counter *> CUTS_Counter_Map;

  /// Counters for the guard.
  CUTS_Counter_Map counter_;

  /// Mutex used in double-lock checking pattern.
  ACE_RW_Thread_Mutex mutex_;
};

#if defined (__CUTS_INLINE__)
# include "cuts/Event_Guard.inl"
#endif

#endif  // !defined _CUTS_EVENT_GUARD_H_
