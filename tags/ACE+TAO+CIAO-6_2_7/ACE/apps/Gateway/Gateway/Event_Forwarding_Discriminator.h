/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Event_Forwarding_Discriminator.h
 *
 *  $Id$
 *
 *  @author Doug Schmidt
 */
//=============================================================================


#ifndef _CONSUMER_MAP_H
#define _CONSUMER_MAP_H

#include "ace/Map_Manager.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Null_Mutex.h"
#include "Event.h"
#include "Consumer_Dispatch_Set.h"

/**
 * @class Event_Forwarding_Discriminator
 *
 * @brief Map events to the set of Consumer_Proxies that have subscribed
 * to receive the event.
 */
class Event_Forwarding_Discriminator
{
public:
  /// Associate Event with the Consumer_Dispatch_Set.
  int bind (Event_Key event, Consumer_Dispatch_Set *cds);

  /// Locate EXID and pass out parameter via INID.  If found,
  /// return 0, else -1.
  int unbind (Event_Key event);

  /// Break any association of EXID.
  int find (Event_Key event, Consumer_Dispatch_Set *&cds);

public:
  /// Map that associates <Event_Key>s (external ids) with
  /// <Consumer_Dispatch_Set> *'s <internal IDs>.
  ACE_Map_Manager<Event_Key, Consumer_Dispatch_Set *, ACE_Null_Mutex> map_;
};

/**
 * @class Event_Forwarding_Discriminator_Iterator
 *
 * @brief Define an iterator for the Consumer Map.
 */
class Event_Forwarding_Discriminator_Iterator
{
public:
  Event_Forwarding_Discriminator_Iterator (Event_Forwarding_Discriminator &mm);
  int next (Consumer_Dispatch_Set *&);
  int advance (void);

private:
  /// Map we are iterating over.
  ACE_Map_Iterator<Event_Key, Consumer_Dispatch_Set *, ACE_Null_Mutex> map_iter_;
};
#endif /* _CONSUMER_MAP_H */
