/* -*- C++ -*- */
// $Id$


#if !defined (_ROUTING_TABLE_C)
#define _ROUTING_TABLE_C


#include "Routing_Table.h"

/* Bind the EXT_ID to the INT_ID. */

template <class EXT_ID, class INT_ID, class LOCK> int 
Routing_Table<EXT_ID, INT_ID, LOCK>::bind (EXT_ID ext_id, INT_ID *int_id)
{
  return this->map_.bind (ext_id, int_id);
}

/* Find the INT_ID corresponding to the EXT_ID. */

template <class EXT_ID, class INT_ID, class LOCK> int 
Routing_Table<EXT_ID, INT_ID, LOCK>::find (EXT_ID ext_id, INT_ID *&int_id)
{
  return this->map_.find (ext_id, int_id);
}

/* Unbind (remove) the EXT_ID from the map. */

template <class EXT_ID, class INT_ID, class LOCK> int
Routing_Table<EXT_ID, INT_ID, LOCK>::unbind (EXT_ID ext_id)
{
  return this->map_.unbind (ext_id);
}

template <class EXT_ID, class INT_ID, class LOCK>
Routing_Iterator<EXT_ID, INT_ID, LOCK>::Routing_Iterator (Routing_Table<EXT_ID, 
							  INT_ID, LOCK> &rt,
							  int ignore_inactive)
  : map_iter_ (rt.map_), 
    ignore_inactive_ (ignore_inactive)
{
}

template <class EXT_ID, class INT_ID, class LOCK> int 
Routing_Iterator<EXT_ID, INT_ID, LOCK>::next (INT_ID *&ss)
{
  // Loop in order to skip over inactive entries if necessary.

  for (ACE_Map_Entry<EXT_ID, INT_ID *> *temp = 0;
       this->map_iter_.next (temp) != 0; 
       this->advance ())
    {
      // Skip over inactive entries if necessary.
      if (temp->int_id_->active () == 0 && this->ignore_inactive_)
	continue;

      // Otherwise, return the next item.
      ss = temp->int_id_;
      return 1;
    }
  return 0;
}

template <class EXT_ID, class INT_ID, class LOCK> int
Routing_Iterator<EXT_ID, INT_ID, LOCK>::advance (void)
{
  return this->map_iter_.advance ();
}
#endif /* _ROUTING_TABLE_C */
