/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    apps
// 
// = FILENAME
//    Consumer_Map.h
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
#include "Consumer_Entry.h"

class Consumer_Map
{
  // = TITLE
  //    Define a generic consumer map based on the ACE Map_Manager.  
  //
  // = DESCRIPTION
  //    This class makes it easier to use the Map_Manager.
public:
  int bind (Event_Addr event, Consumer_Entry *Consumer_Entry);
  // Associate Event with the Consumer_Entry.

  int find (Event_Addr event, Consumer_Entry *&Consumer_Entry);
  // Break any association of EXID.

  int unbind (Event_Addr event);
  // Locate EXID and pass out parameter via INID.  If found,
  // return 0, else -1.

public:
  ACE_Map_Manager<Event_Addr, Consumer_Entry *, MAP_MUTEX> map_;
  // Map that associates Event Addrs (external ids) with Consumer_Entry *'s
  // <internal IDs>.
};

class Consumer_Map_Iterator
{
  // = TITLE
  //    Define an iterator for the Consumer Map.
public:
  Consumer_Map_Iterator (Consumer_Map &mm);
  int next (Consumer_Entry *&);
  int advance (void);

private:
  ACE_Map_Iterator<Event_Addr, Consumer_Entry *, MAP_MUTEX> map_iter_;
  // Map we are iterating over.
};
#endif /* _CONSUMER_MAP_H */
