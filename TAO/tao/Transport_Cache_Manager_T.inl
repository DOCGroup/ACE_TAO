
// -*- C++ -*-
//
//$Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  template <typename TT, typename TDT, typename PS>
  ACE_INLINE int
  Transport_Cache_Manager_T<TT, TDT, PS>::cache_transport (
    transport_descriptor_type *prop,
    transport_type *transport,
    Cache_Entries_State state)
  {
    // Compose the ExternId & Intid
    Cache_ExtId ext_id (prop);
    int retval = 0;
    {
      ACE_MT (ACE_GUARD_RETURN (ACE_Lock,
                                guard,
                                *this->cache_lock_,
                                -1));
      Cache_IntId int_id (transport);

      // If it has already connected, go directly to the IDLE_BNP state
      if (int_id.is_connected () && state == ENTRY_CONNECTING)
        int_id.recycle_state (ENTRY_IDLE_BUT_NOT_PURGABLE);
      else
        int_id.recycle_state (state);

      retval = this->bind_i (ext_id, int_id);
    }

    return retval;
  }

  template <typename TT, typename TDT, typename PS>
  ACE_INLINE int
  Transport_Cache_Manager_T<TT, TDT, PS>::cache_idle_transport (
    transport_descriptor_type *prop,
    transport_type *transport)
  {
    return cache_transport(prop, transport, ENTRY_IDLE_AND_PURGABLE);
  }

  template <typename TT, typename TDT, typename PS>
  ACE_INLINE int
  Transport_Cache_Manager_T<TT, TDT, PS>::purge_entry (HASH_MAP_ENTRY *&entry)
  {
    ACE_MT (ACE_GUARD_RETURN (ACE_Lock, guard, *this->cache_lock_, -1));
    return this->purge_entry_i (entry);
  }

  template <typename TT, typename TDT, typename PS>
  ACE_INLINE void
  Transport_Cache_Manager_T<TT, TDT, PS>::mark_invalid (HASH_MAP_ENTRY *entry)
  {
    ACE_MT (ACE_GUARD (ACE_Lock, guard, *this->cache_lock_));
    this->mark_invalid_i (entry);
  }

  template <typename TT, typename TDT, typename PS>
  ACE_INLINE void
  Transport_Cache_Manager_T<TT, TDT, PS>::mark_connected (HASH_MAP_ENTRY *entry, bool state)
  {
    ACE_MT (ACE_GUARD (ACE_Lock, guard, *this->cache_lock_));
    if (entry == 0)
      return;

    if (TAO_debug_level > 9 && state != entry->item ().is_connected ())
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("TAO (%P|%t) - Transport_Cache_Manager_T")
                  ACE_TEXT ("::mark_connected %C Transport=%d\n"),
                  (state ? "true" : "false"), entry->item ().transport ()
                  ));
    entry->item().is_connected (state);
  }

  template <typename TT, typename TDT, typename PS>
  ACE_INLINE int
  Transport_Cache_Manager_T<TT, TDT, PS>::make_idle (HASH_MAP_ENTRY *entry)
  {
    ACE_MT (ACE_GUARD_RETURN (ACE_Lock, guard, *this->cache_lock_, -1));
    return this->make_idle_i (entry);
  }

  template <typename TT, typename TDT, typename PS>
  ACE_INLINE typename Transport_Cache_Manager_T<TT, TDT, PS>::Find_Result
  Transport_Cache_Manager_T<TT, TDT, PS>::find (transport_descriptor_type *prop,
                                 transport_type *&transport,
                                 size_t &busy_count)
  {
    ACE_MT (ACE_GUARD_RETURN  (ACE_Lock,
                               guard,
                               *this->cache_lock_,
                               CACHE_FOUND_NONE));

    return this->find_i (prop, transport, busy_count);
  }

  template <typename TT, typename TDT, typename PS>
  ACE_INLINE int
  Transport_Cache_Manager_T<TT, TDT, PS>::
    close (Connection_Handler_Set &handlers)
  {
    // The cache lock pointer should only be zero if
    // Transport_Cache_Manager_T::open() was never called.  Note that
    // only one thread opens the Transport_Cache_Manager_T at any given
    // time, so it is safe to check for a non-zero lock pointer.
    if (this->cache_lock_ == 0)
      return -1;

    ACE_MT (ACE_GUARD_RETURN (ACE_Lock,
                              guard,
                              *this->cache_lock_,
                              -1));

    return this->close_i (handlers);
  }

  template <typename TT, typename TDT, typename PS>
  ACE_INLINE bool
  Transport_Cache_Manager_T<TT, TDT, PS>::blockable_client_transports (
    Connection_Handler_Set &handlers)
  {
    ACE_MT (ACE_GUARD_RETURN (ACE_Lock,
                              guard,
                              *this->cache_lock_,
                              false));

    return this->blockable_client_transports_i (handlers);
  }

  template <typename TT, typename TDT, typename PS>
  ACE_INLINE size_t
  Transport_Cache_Manager_T<TT, TDT, PS>::current_size (void) const
  {
    return this->cache_map_.current_size ();
  }

  template <typename TT, typename TDT, typename PS>
  ACE_INLINE size_t
  Transport_Cache_Manager_T<TT, TDT, PS>::total_size (void) const
  {
    return this->cache_map_.total_size ();
  }

  template <typename TT, typename TDT, typename PS>
  ACE_INLINE typename Transport_Cache_Manager_T<TT, TDT, PS>::HASH_MAP &
  Transport_Cache_Manager_T<TT, TDT, PS>::map (void)
  {
    return this->cache_map_;
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
