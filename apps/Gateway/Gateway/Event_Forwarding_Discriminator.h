/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    apps
// 
// = FILENAME
//    Event_Forwarding_Discriminator.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (_CONSUMER_MAP_H)
#define _CONSUMER_MAP_H

#include "ace/Map_Manager.h"
#include "Concurrency_Strategies.h"
#include "Event.h"
#include "Dispatch_Set.h"

class Event_Forwarding_Discriminator
{
  // = TITLE
  //    Define a generic consumer map based on the ACE Map_Manager.  
  //
  // = DESCRIPTION
  //    This class makes it easier to use the Map_Manager.
public:
  int bind (Event_Addr event, Dispatch_Set *Dispatch_Set);
  // Associate Event with the Dispatch_Set.

  int find (Event_Addr event, Dispatch_Set *&Dispatch_Set);
  // Break any association of EXID.

  int unbind (Event_Addr event);
  // Locate EXID and pass out parameter via INID.  If found,
  // return 0, else -1.

public:
  ACE_Map_Manager<Event_Addr, Dispatch_Set *, MAP_MUTEX> map_;
  // Map that associates Event Addrs (external ids) with Dispatch_Set *'s
  // <internal IDs>.
};

class Event_Forwarding_Discriminator_Iterator
{
  // = TITLE
  //    Define an iterator for the Consumer Map.
public:
  Event_Forwarding_Discriminator_Iterator (Event_Forwarding_Discriminator &mm);
  int next (Dispatch_Set *&);
  int advance (void);

private:
  ACE_Map_Iterator<Event_Addr, Dispatch_Set *, MAP_MUTEX> map_iter_;
  // Map we are iterating over.
};
#endif /* _CONSUMER_MAP_H */
