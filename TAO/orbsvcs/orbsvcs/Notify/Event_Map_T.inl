// $Id$

template <class PROXY, class ACE_LOCK> ACE_INLINE  ACE_TYPENAME TAO_NS_Event_Map_Entry_T<PROXY>::COLLECTION*
TAO_NS_Event_Map_T<PROXY, ACE_LOCK>::find (const TAO_NS_EventType& event_type ACE_ENV_ARG_DECL_NOT_USED)
{
  TAO_NS_Event_Map_Entry_T<PROXY>* entry;

  ACE_READ_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, 0);

  if (map_.find (event_type, entry) == 0)
    return entry->collection ();
  else
    return 0;
}

template <class PROXY, class ACE_LOCK>  ACE_INLINE  ACE_TYPENAME TAO_NS_Event_Map_Entry_T<PROXY>::COLLECTION*
TAO_NS_Event_Map_T<PROXY, ACE_LOCK>::broadcast_collection (void)
{
  return this->broadcast_entry_.collection ();
}

template <class PROXY, class ACE_LOCK> ACE_INLINE void
TAO_NS_Event_Map_T<PROXY, ACE_LOCK>::attach_observer (TAO_NS_Event_Map_Observer* observer)
{
  this->observer_ = observer;
}

template <class PROXY, class ACE_LOCK> ACE_INLINE int
TAO_NS_Event_Map_T<PROXY, ACE_LOCK>::event_type_count (void)
{
  return this->event_type_count_;
}
