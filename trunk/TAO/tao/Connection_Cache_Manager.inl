/* -*- C++ -*- */
//$Id$

ACE_INLINE int
TAO_Connection_Cache_Manager::bind (TAO_Cache_ExtId &ext_id,
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
TAO_Connection_Cache_Manager::find (const TAO_Cache_ExtId &key,
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
TAO_Connection_Cache_Manager::
    cache_handler (TAO_Base_Connection_Property *prop,
                   TAO_Connection_Handler *handler)
{
  // Compose the ExternId & Intid
  TAO_Cache_ExtId ext_id (prop);
  TAO_Cache_IntId int_id (handler);

  return this->bind (ext_id,
                     int_id);

}

ACE_INLINE int
TAO_Connection_Cache_Manager::
    find_handler (TAO_Base_Connection_Property *prop,
                  TAO_Connection_Handler *&handler)
{
  // Compose the ExternId
  TAO_Cache_ExtId ext_id (prop);
  TAO_Cache_IntId int_id;

  int retval = this->find (ext_id,
                           int_id);
  if (retval == 0)
    {
      handler = int_id.handler ();
    }

  return retval;
}

ACE_INLINE int
TAO_Connection_Cache_Manager::rebind (const TAO_Cache_ExtId &key,
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
TAO_Connection_Cache_Manager::trybind (const TAO_Cache_ExtId &key,
                                       TAO_Cache_IntId &value)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Lock,
                            guard,
                            *this->cache_lock_,
                            -1));

  return this->trybind_i (key, value);
}

ACE_INLINE int
TAO_Connection_Cache_Manager::unbind (const TAO_Cache_ExtId &key)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Lock,
                            guard,
                            *this->cache_lock_,
                            -1));

  return this->unbind_i (key);
}

ACE_INLINE int
TAO_Connection_Cache_Manager::unbind (const TAO_Cache_ExtId &key,
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
TAO_Connection_Cache_Manager::purge (void)
{
  return 0;
}

ACE_INLINE int
TAO_Connection_Cache_Manager::purge_entry (HASH_MAP_ENTRY *&entry)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Lock,
                            guard,
                            *this->cache_lock_,
                            -1));

  return this->purge_entry_i (entry);
}


ACE_INLINE int
TAO_Connection_Cache_Manager::make_idle (HASH_MAP_ENTRY *&entry)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Lock,
                            guard,
                            *this->cache_lock_,
                            -1));
  return this->make_idle_i (entry);
}


ACE_INLINE int
TAO_Connection_Cache_Manager::close (ACE_Handle_Set &handle_Set)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Lock,
                            guard,
                            *this->cache_lock_,
                            -1));

  return this->close_i (handle_Set);
}


ACE_INLINE size_t
TAO_Connection_Cache_Manager::current_size (void) const
{
  return this->cache_map_.current_size ();
}

ACE_INLINE size_t
TAO_Connection_Cache_Manager::total_size (void) const
{
  return this->cache_map_.total_size ();
}
