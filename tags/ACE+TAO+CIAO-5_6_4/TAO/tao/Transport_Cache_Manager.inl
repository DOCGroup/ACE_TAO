// -*- C++ -*-
//
//$Id$


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  ACE_INLINE int
  Transport_Cache_Manager::bind (Cache_ExtId &ext_id,
                                 Cache_IntId &int_id)
  {
    ACE_MT (ACE_GUARD_RETURN (ACE_Lock,
                              guard,
                              *this->cache_lock_,
                              -1));

    return this->bind_i (ext_id,
                         int_id);
  }

  ACE_INLINE int
  Transport_Cache_Manager::cache_transport (
    TAO_Transport_Descriptor_Interface *prop,
    TAO_Transport *transport)
  {
    // Compose the ExternId & Intid
    Cache_ExtId ext_id (prop);
    Cache_IntId int_id (transport);

    int retval = 0;
    {
      ACE_MT (ACE_GUARD_RETURN (ACE_Lock,
                                guard,
                                *this->cache_lock_,
                                -1));

      // Do as the semantics of this method dictates
      int_id.recycle_state (ENTRY_BUSY);

      retval = this->bind_i (ext_id,
                             int_id);
    }

    return retval;
  }

  ACE_INLINE int
  Transport_Cache_Manager::cache_idle_transport (
    TAO_Transport_Descriptor_Interface *prop,
    TAO_Transport *transport)
  {
    // Compose the ExternId & Intid
    Cache_ExtId ext_id (prop);
    Cache_IntId int_id (transport);

    int retval = 0;
    {
      ACE_MT (ACE_GUARD_RETURN (ACE_Lock,
                                guard,
                                *this->cache_lock_,
                                -1));

      // Do as the semantics of this method dictates
      int_id.recycle_state (ENTRY_IDLE_AND_PURGABLE);
      retval = this->bind_i (ext_id,
                             int_id);
    }

    return retval;
  }

  ACE_INLINE int
  Transport_Cache_Manager::purge_entry (HASH_MAP_ENTRY *&entry)
  {
    // Double checked locking
    if(entry == 0)
      return 0;

    ACE_MT (ACE_GUARD_RETURN (ACE_Lock, guard, *this->cache_lock_, -1));

    return this->purge_entry_i (entry);
  }

  ACE_INLINE void
  Transport_Cache_Manager::mark_invalid (HASH_MAP_ENTRY *&entry)
  {
    if(entry == 0)
      return;

    // Double checked locking
    ACE_MT (ACE_GUARD (ACE_Lock, guard, *this->cache_lock_));

    this->mark_invalid_i (entry);
  }

  ACE_INLINE int
  Transport_Cache_Manager::make_idle (HASH_MAP_ENTRY *&entry)
  {
    if(entry == 0)
      return -1;

    ACE_MT (ACE_GUARD_RETURN (ACE_Lock, guard, *this->cache_lock_, -1));
    return this->make_idle_i (entry);
  }

  ACE_INLINE int
  Transport_Cache_Manager::close (Connection_Handler_Set &handlers)
  {
    // The cache lock pointer should only be zero if
    // Transport_Cache_Manager::open() was never called.  Note that
    // only one thread opens the Transport_Cache_Manager at any given
    // time, so it is safe to check for a non-zero lock pointer.
    if (this->cache_lock_ == 0)
      return -1;

    ACE_MT (ACE_GUARD_RETURN (ACE_Lock,
                              guard,
                              *this->cache_lock_,
                              -1));

    return this->close_i (handlers);
  }

  ACE_INLINE bool
  Transport_Cache_Manager::blockable_client_transports (
    Connection_Handler_Set &handlers)
  {
    ACE_MT (ACE_GUARD_RETURN (ACE_Lock,
                              guard,
                              *this->cache_lock_,
                              false));

    return this->blockable_client_transports_i (handlers);
  }

  ACE_INLINE size_t
  Transport_Cache_Manager::current_size (void) const
  {
    return this->cache_map_.current_size ();
  }

  ACE_INLINE size_t
  Transport_Cache_Manager::total_size (void) const
  {
    return this->cache_map_.total_size ();
  }

  ACE_INLINE Transport_Cache_Manager::HASH_MAP &
  Transport_Cache_Manager::map (void)
  {
    return this->cache_map_;
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
