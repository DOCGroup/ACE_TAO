// $Id$

template <class PROXY, class ACE_LOCK> ACE_INLINE  TAO_Notify_Event_Map_Entry_T<PROXY>*
TAO_Notify_Event_Map_T<PROXY, ACE_LOCK>::find (const TAO_Notify_EventType& event_type ACE_ENV_ARG_DECL_NOT_USED)
{
  TAO_Notify_Event_Map_Entry_T<PROXY>* entry;

  ACE_READ_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, 0);

  if (map_.find (event_type, entry) == 0)
    {
      entry->_incr_refcnt ();
      return entry;
    }
  else
    return 0;
}

template <class PROXY, class ACE_LOCK> ACE_INLINE void
TAO_Notify_Event_Map_T<PROXY, ACE_LOCK>::release (ENTRY* entry)
{
  ACE_WRITE_GUARD (ACE_LOCK, ace_mon, this->lock_);

  if (entry->_decr_refcnt () == 0)
    delete entry;
}

template <class PROXY, class ACE_LOCK>  ACE_INLINE  ACE_TYPENAME TAO_Notify_Event_Map_Entry_T<PROXY>::COLLECTION*
TAO_Notify_Event_Map_T<PROXY, ACE_LOCK>::broadcast_collection (void)
{
  return this->broadcast_entry_.collection ();
}

template <class PROXY, class ACE_LOCK>  ACE_INLINE  ACE_TYPENAME TAO_Notify_Event_Map_Entry_T<PROXY>::COLLECTION*
TAO_Notify_Event_Map_T<PROXY, ACE_LOCK>::updates_collection (void)
{
  return this->updates_entry_.collection ();
}

template <class PROXY, class ACE_LOCK> ACE_INLINE int
TAO_Notify_Event_Map_T<PROXY, ACE_LOCK>::proxy_count (void)
{
  return this->proxy_count_;
}

template <class PROXY, class ACE_LOCK> ACE_INLINE const TAO_Notify_EventTypeSeq&
TAO_Notify_Event_Map_T<PROXY, ACE_LOCK>::event_types (void)
{
  return this->event_types_;
}
