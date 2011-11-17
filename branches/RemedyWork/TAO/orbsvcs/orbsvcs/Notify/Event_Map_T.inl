// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <class PROXY, class ACE_LOCK> ACE_INLINE  TAO_Notify_Event_Map_Entry_T<PROXY>*
TAO_Notify_Event_Map_T<PROXY, ACE_LOCK>::find (const TAO_Notify_EventType& event_type)
{
  TAO_Notify_Event_Map_Entry_T<PROXY>* entry = 0;

  ACE_READ_GUARD_RETURN (ACE_LOCK, ace_mon, this->lock_, 0);

  if (map_.find (event_type, entry) == 0)
    {
      entry->_incr_refcnt ();
      return entry;
    }
  else
    {
      // The event_type that we have doesn't hash directly to a particular
      // entry.  So, we need to iterate through the map and see if any of
      // the event type keys are equivalent to event_type.
      typedef ACE_Hash_Map_Iterator_Ex <TAO_Notify_EventType, ENTRY*,
                                        ACE_Hash<TAO_Notify_EventType>,
                                        ACE_Equal_To<TAO_Notify_EventType>,
                                        ACE_SYNCH_NULL_MUTEX>
              Iterator_Type;
      Iterator_Type end = this->map_.end ();
      for(Iterator_Type i = this->map_.begin (); i != end; ++i)
        {
          if ((*i).ext_id_ == event_type)
            {
              entry = (*i).int_id_;
              entry->_incr_refcnt ();
              return entry;
            }
        }
    }

  return 0;
}

template <class PROXY, class ACE_LOCK> ACE_INLINE void
TAO_Notify_Event_Map_T<PROXY, ACE_LOCK>::release (ENTRY* entry)
{
  ACE_WRITE_GUARD (ACE_LOCK, ace_mon, this->lock_);

  if (entry->_decr_refcnt () == 0)
    delete entry;
}

template <class PROXY, class ACE_LOCK>  ACE_INLINE  typename TAO_Notify_Event_Map_Entry_T<PROXY>::COLLECTION*
TAO_Notify_Event_Map_T<PROXY, ACE_LOCK>::broadcast_collection (void)
{
  return this->broadcast_entry_.collection ();
}

template <class PROXY, class ACE_LOCK>  ACE_INLINE  typename TAO_Notify_Event_Map_Entry_T<PROXY>::COLLECTION*
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

TAO_END_VERSIONED_NAMESPACE_DECL
