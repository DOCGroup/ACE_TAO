/* -*- C++ -*- */
// $Id$

#if !defined (_CONSUMER_MAP_C)
#define _CONSUMER_MAP_C

#include "Consumer_Map.h"

// Bind the Event_Addr to the INT_ID.

int
Consumer_Map::bind (Event_Addr event_addr, 
		    Consumer_Entry *Consumer_Entry)
{
  return this->map_.bind (event_addr, Consumer_Entry);
}

// Find the Consumer_Entry corresponding to the Event_Addr.

int
Consumer_Map::find (Event_Addr event_addr, 
		    Consumer_Entry *&Consumer_Entry)
{
  return this->map_.find (event_addr, Consumer_Entry);
}

// Unbind (remove) the Event_Addr from the map.

int
Consumer_Map::unbind (Event_Addr event_addr)
{
  return this->map_.unbind (event_addr);
}

Consumer_Map_Iterator::Consumer_Map_Iterator (Consumer_Map &rt)
  : map_iter_ (rt.map_)
{
}

int
Consumer_Map_Iterator::next (Consumer_Entry *&ss)
{
  // Loop in order to skip over inactive entries if necessary.

  for (ACE_Map_Entry<Event_Addr, Consumer_Entry *> *temp = 0;
       this->map_iter_.next (temp) != 0; 
       this->advance ())
    {
      // Otherwise, return the next item.
      ss = temp->int_id_;
      return 1;
    }
  return 0;
}

int
Consumer_Map_Iterator::advance (void)
{
  return this->map_iter_.advance ();
}
#endif /* _CONSUMER_MAP_C */
