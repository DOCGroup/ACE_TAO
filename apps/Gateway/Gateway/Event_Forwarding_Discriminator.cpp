/* -*- C++ -*- */
// $Id$

#if !defined (_CONSUMER_MAP_C)
#define _CONSUMER_MAP_C

#include "Event_Forwarding_Discriminator.h"

// Bind the Event_Addr to the INT_ID.

int
Event_Forwarding_Discriminator::bind (Event_Addr event_addr, 
		    Dispatch_Set *Dispatch_Set)
{
  return this->map_.bind (event_addr, Dispatch_Set);
}

// Find the Dispatch_Set corresponding to the Event_Addr.

int
Event_Forwarding_Discriminator::find (Event_Addr event_addr, 
		    Dispatch_Set *&Dispatch_Set)
{
  return this->map_.find (event_addr, Dispatch_Set);
}

// Unbind (remove) the Event_Addr from the map.

int
Event_Forwarding_Discriminator::unbind (Event_Addr event_addr)
{
  return this->map_.unbind (event_addr);
}

Event_Forwarding_Discriminator_Iterator::Event_Forwarding_Discriminator_Iterator (Event_Forwarding_Discriminator &rt)
  : map_iter_ (rt.map_)
{
}

int
Event_Forwarding_Discriminator_Iterator::next (Dispatch_Set *&ss)
{
  // Loop in order to skip over inactive entries if necessary.

  for (ACE_Map_Entry<Event_Addr, Dispatch_Set *> *temp = 0;
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
Event_Forwarding_Discriminator_Iterator::advance (void)
{
  return this->map_iter_.advance ();
}
#endif /* _CONSUMER_MAP_C */
