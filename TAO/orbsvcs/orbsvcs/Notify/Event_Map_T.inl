// $Id$

#include "Event_Map_T.h"

template <class PROXY> ACE_INLINE TAO_NS_Event_Map_Entry_T<PROXY>::COLLECTION*
TAO_NS_Event_Map_Entry_T<PROXY>::collection (void)
{
  return collection_;
}

template <class PROXY> ACE_INLINE int
TAO_NS_Event_Map_Entry_T<PROXY>::count (void)
{
  return count_;
}

template <class PROXY, class ACE_LOCK> ACE_INLINE TAO_NS_Event_Map_T<PROXY, ACE_LOCK>::COLLECTION*
TAO_NS_Event_Map_T<PROXY, ACE_LOCK>::find (const TAO_NS_EventType& event_type ACE_ENV_ARG_DECL)
{
  TAO_NS_Event_Map_Entry_T<PROXY>* entry;

  ACE_READ_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, 0);

  if (map_.find (event_type, entry) == 0)
    return entry->collection ();
  else
    return 0;
}
