/* -*- C++ -*- */
//$Id$

ACE_INLINE int
TAO_Transport_Cache_Manager::bind (TAO_Cache_ExtId &ext_id,
                                    TAO_Cache_IntId &int_id)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Lock,
                            guard,
                            *this->cache_lock_,
                            -1));

  return this->bind_i (ext_id,
                       int_id);
}


ACE_INLINE int
TAO_Transport_Cache_Manager::find (const TAO_Cache_ExtId &key,
                                    TAO_Cache_IntId &value)
{
  ACE_MT (ACE_GUARD_RETURN  (ACE_Lock,
                             guard,
                             *this->cache_lock_,
                             -1));

  return this->find_i (key,
                       value);
}


ACE_INLINE int
TAO_Transport_Cache_Manager::cache_transport (
                     TAO_Transport_Descriptor_Interface *prop,
                     TAO_Transport *transport)
{
  if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("TAO (%P|%t) - ")
                            ACE_TEXT ("TAO_Transport_Cache_Manager")
                            ACE_TEXT ("::cache_transport (0x%x, 0x%x)\n"),
                            prop, transport));
    }

  // Compose the ExternId & Intid
  TAO_Cache_ExtId ext_id (prop);
  TAO_Cache_IntId int_id (transport);

  return this->bind (ext_id,
                     int_id);

}

ACE_INLINE int
TAO_Transport_Cache_Manager::rebind (const TAO_Cache_ExtId &key,
                                      const TAO_Cache_IntId &value)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Lock,
                            guard,
                            *this->cache_lock_,
                            -1));

  return this->rebind_i (key,
                         value);
}

ACE_INLINE int
TAO_Transport_Cache_Manager::unbind (const TAO_Cache_ExtId &key)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Lock,
                            guard,
                            *this->cache_lock_,
                            -1));

  return this->unbind_i (key);
}

ACE_INLINE int
TAO_Transport_Cache_Manager::unbind (const TAO_Cache_ExtId &key,
                                      TAO_Cache_IntId &value)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Lock,
                            guard,
                            *this->cache_lock_,
                            -1));

  return this->unbind_i (key,
                         value);
}


ACE_INLINE int
TAO_Transport_Cache_Manager::purge (void)
{
  int need_to_purge = 0;
  DESCRIPTOR_SET sorted_set;

  // We must use a small scope, since we can't call close_entries()
  // if we are holding the lock.
  {
    ACE_MT (ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->cache_lock_, 0));
    need_to_purge = this->fill_set_i (sorted_set);
  }
   
  // Only call close_entries () if need_to_purge.  It takes control of
  // sorted_set and cleans up any allocated memory.  If !need_to_purge,
  // then there is nothing to de-allocate.
  if (need_to_purge)
    {
      this->close_entries (sorted_set);
    }

  return 0;
}

ACE_INLINE int
TAO_Transport_Cache_Manager::purge_entry (HASH_MAP_ENTRY *&entry)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Lock,
                            guard,
                            *this->cache_lock_,
                            -1));

  return this->purge_entry_i (entry);
}

ACE_INLINE void
TAO_Transport_Cache_Manager::mark_invalid (HASH_MAP_ENTRY *&entry)
{
  ACE_MT (ACE_GUARD (ACE_Lock,
                     guard,
                     *this->cache_lock_));

  this->mark_invalid_i (entry);
}



ACE_INLINE int
TAO_Transport_Cache_Manager::make_idle (HASH_MAP_ENTRY *&entry)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Lock,
                            guard,
                            *this->cache_lock_,
                            -1));
  return this->make_idle_i (entry);
}


ACE_INLINE int
TAO_Transport_Cache_Manager::close (ACE_Handle_Set &reactor_registered,
                                    TAO_EventHandlerSet &unregistered)
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

  return this->close_i (reactor_registered, unregistered);
}


ACE_INLINE size_t
TAO_Transport_Cache_Manager::current_size (void) const
{
  return this->cache_map_.current_size ();
}

ACE_INLINE size_t
TAO_Transport_Cache_Manager::total_size (void) const
{
  return this->cache_map_.total_size ();
}
