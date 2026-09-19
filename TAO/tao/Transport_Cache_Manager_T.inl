// -*- C++ -*-
//
TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  template <typename TT, typename TRDT, typename PSTRAT>
  ACE_INLINE int
  Transport_Cache_Manager_T<TT, TRDT, PSTRAT>::cache_transport (
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
        int_id.recycle_state (ENTRY_IDLE_AND_PURGABLE);
      else
        int_id.recycle_state (state);

      retval = this->bind_i (ext_id, int_id);
      if (retval == 0)
        {
          transport->touch_activity ();
        }
    }

    return retval;
  }

  template <typename TT, typename TRDT, typename PSTRAT>
  ACE_INLINE int
  Transport_Cache_Manager_T<TT, TRDT, PSTRAT>::purge_entry (HASH_MAP_ENTRY *&entry)
  {
    int retval = 0;

    if (entry)
    {
      HASH_MAP_ENTRY* cached_entry = nullptr;
      ACE_MT (ACE_GUARD_RETURN (ACE_Lock, guard, *this->cache_lock_, -1));
      if (entry != 0) // in case someone beat us to it (entry is reference to transport member)
      {
        // Store the entry in a temporary and zero out the reference.
        // If there is only one reference count for the transport, we will end up causing
        // it's destruction.  And the transport can not be holding a cache map entry if
        // that happens.
        cached_entry = entry;
        entry = nullptr;

        // now it's save to really purge the entry
        retval = this->purge_entry_i (cached_entry);
      }
    }

    return retval;
  }

  template <typename TT, typename TRDT, typename PSTRAT>
  ACE_INLINE int
  Transport_Cache_Manager_T<TT, TRDT, PSTRAT>::purge_entry_when_purgable (HASH_MAP_ENTRY *&entry)
  {
    // Even the first read must be locked: a referenced scan candidate may
    // have been removed from the cache since the snapshot was taken.
    ACE_MT (ACE_GUARD_RETURN (ACE_Lock, guard, *this->cache_lock_, -1));
    if (entry == nullptr || !this->is_entry_purgable_i (*entry))
      {
        return -1;
      }

    // Clear the transport's back pointer before releasing the cache reference.
    HASH_MAP_ENTRY *cached_entry = entry;
    entry = nullptr;
    return this->purge_entry_i (cached_entry);
  }

  template <typename TT, typename TRDT, typename PSTRAT>
  ACE_INLINE int
  Transport_Cache_Manager_T<TT, TRDT, PSTRAT>::purge_entry_if_idle_i (HASH_MAP_ENTRY *entry)
  {
    if (entry == nullptr || !this->is_entry_purgable_i (*entry)
        || !entry->int_id_.transport ()->idle_timeout_expired_i ()
        || !entry->int_id_.transport ()->is_idle ())
      {
        return -1;
      }

    entry->int_id_.transport ()->cache_map_entry_ = nullptr;
    return this->purge_entry_i (entry);
  }

  template <typename TT, typename TRDT, typename PSTRAT>
  ACE_INLINE void
  Transport_Cache_Manager_T<TT, TRDT, PSTRAT>::mark_connected (HASH_MAP_ENTRY *&entry, bool state)
  {
    ACE_MT (ACE_GUARD (ACE_Lock, guard, *this->cache_lock_));
    if (entry)
      {
        transport_type * const transport = entry->item ().transport ();
        if (transport != nullptr)
          {
            transport->touch_activity ();
            if (TAO_debug_level > 9 && state != entry->item ().is_connected ())
              {
                TAOLIB_DEBUG ((LM_DEBUG, ACE_TEXT ("TAO (%P|%t) - Transport_Cache_Manager_T")
                            ACE_TEXT ("::mark_connected, %s Transport[%d]\n"),
                            (state ? ACE_TEXT("true") : ACE_TEXT("false")),
                            transport->id ()));
              }
          }
        entry->item().is_connected (state);
      }
  }

  template <typename TT, typename TRDT, typename PSTRAT>
  ACE_INLINE int
  Transport_Cache_Manager_T<TT, TRDT, PSTRAT>::make_idle (HASH_MAP_ENTRY *&entry)
  {
    ACE_MT (ACE_GUARD_RETURN (ACE_Lock, guard, *this->cache_lock_, -1));
    if (!entry) // in case someone beat us to it (entry is reference to transport member)
      return -1;

    return this->make_idle_i (entry);
  }

  template <typename TT, typename TRDT, typename PSTRAT>
  ACE_INLINE typename Transport_Cache_Manager_T<TT, TRDT, PSTRAT>::Find_Result
  Transport_Cache_Manager_T<TT, TRDT, PSTRAT>::find (transport_descriptor_type *prop,
                                 transport_type *&transport,
                                 size_t &busy_count)
  {
    ACE_MT (ACE_GUARD_RETURN  (ACE_Lock,
                               guard,
                               *this->cache_lock_,
                               CACHE_FOUND_NONE));

    return this->find_i (prop, transport, busy_count);
  }

  template <typename TT, typename TRDT, typename PSTRAT>
  ACE_INLINE int
  Transport_Cache_Manager_T<TT, TRDT, PSTRAT>::
    close (Connection_Handler_Set &handlers)
  {
    // The cache lock pointer should only be zero if
    // Transport_Cache_Manager_T::open() was never called.  Note that
    // only one thread opens the Transport_Cache_Manager_T at any given
    // time, so it is safe to check for a non-zero lock pointer.
    if (this->cache_lock_ == 0)
      return -1;

    long timer_id = -1;
    int result = -1;
    {
      ACE_MT (ACE_GUARD_RETURN (ACE_Lock,
                                guard,
                                *this->cache_lock_,
                                -1));

      timer_id = this->stop_idle_scanner_i ();
      result = this->close_i (handlers);
    }

    if (timer_id != -1)
      {
        int const cancel_result = this->reactor_.cancel_timer (timer_id);
        if (cancel_result <= 0)
          {
            if (TAO_debug_level > 0)
              {
                TAOLIB_ERROR ((LM_ERROR,
                  ACE_TEXT ("TAO (%P|%t) - Transport_Cache_Manager_T::")
                  ACE_TEXT ("close, failed to cancel idle scanner timer %ld, ")
                  ACE_TEXT ("result %d\n"), timer_id, cancel_result));
              }
          }
        else if (TAO_debug_level > 6)
          {
            TAOLIB_DEBUG ((LM_DEBUG,
              ACE_TEXT ("TAO (%P|%t) - Transport_Cache_Manager_T::")
              ACE_TEXT ("close, canceled idle scanner timer %ld, result %d\n"),
              timer_id, cancel_result));
          }
      }

    return result;
  }

  template <typename TT, typename TRDT, typename PSTRAT>
  ACE_INLINE bool
  Transport_Cache_Manager_T<TT, TRDT, PSTRAT>::blockable_client_transports (
    Connection_Handler_Set &handlers)
  {
    ACE_MT (ACE_GUARD_RETURN (ACE_Lock,
                              guard,
                              *this->cache_lock_,
                              false));

    return this->blockable_client_transports_i (handlers);
  }

  template <typename TT, typename TRDT, typename PSTRAT>
  ACE_INLINE size_t
  Transport_Cache_Manager_T<TT, TRDT, PSTRAT>::current_size () const
  {
    return this->cache_map_.current_size ();
  }

  template <typename TT, typename TRDT, typename PSTRAT>
  ACE_INLINE size_t
  Transport_Cache_Manager_T<TT, TRDT, PSTRAT>::total_size () const
  {
    return this->cache_map_.total_size ();
  }

  template <typename TT, typename TRDT, typename PSTRAT>
  ACE_INLINE typename Transport_Cache_Manager_T<TT, TRDT, PSTRAT>::HASH_MAP &
  Transport_Cache_Manager_T<TT, TRDT, PSTRAT>::map ()
  {
    return this->cache_map_;
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
