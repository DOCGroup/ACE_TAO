// $Id$


ACE_INLINE int
TAO_Connection_Purging_Strategy::percent () const
{
  return this->percent_;
}


ACE_INLINE void
TAO_Connection_Purging_Strategy::percent (int pvalue)
{
  if (pvalue > 100)
    {
      pvalue = 100;
    }
  else if (pvalue < 0)
    {
      pvalue = 0;
    }
  this->percent_ = pvalue;
}


ACE_INLINE int
TAO_Connection_Purging_Strategy::cache_maximum () const
{
  return this->cache_maximum_;
}


ACE_INLINE int
TAO_Connection_Purging_Strategy::purge ()
{
  int need_to_purge = 0;
  DESCRIPTOR_SET sorted_set;

  // We must use a small scope, since we can't call close_entries()
  // if we are holding the lock.
  {
    ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, 0);
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
TAO_Connection_Purging_Strategy::add_to_cache (TAO_DESCRIPTOR_INTERFACE* prop,
                                               TAO_PURGING_CACHE_ITEM* item)
{
  if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("TAO (%P|%t) - ")
                            ACE_TEXT ("TAO_Connection_Purging_Strategy")
                            ACE_TEXT ("::add_to_cache (0x%x, 0x%x)\n"),
                            prop, item));
    }
  int status = this->cache_manager_.cache_transport (prop, item);

  if (status == 0)
    {
      // Update the item usage information
      ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, 0);
      this->update_item_i (prop, item);
    }

  return status;
}


ACE_INLINE int
TAO_Connection_Purging_Strategy::find_in_cache (
                                   TAO_DESCRIPTOR_INTERFACE* prop,
                                   TAO_PURGING_CACHE_ITEM*& item)
{
  int status = this->cache_manager_.find_transport (prop, item);

  if (status == 0)
    {
      // Update the item usage information
      ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, 0);
      this->update_item_i (prop, item);
    }

  return status;
}


ACE_INLINE int
TAO_Connection_Purging_Strategy::purge_from_cache (
                                   TAO_PURGING_MAP_ENTRY*& entry)
{
  if (TAO_debug_level > 0)
    {
      // @@ Chad: Please see how big the ACE_DEBUG expands too. Do you
      // think the compilers can inline such a big macros. I dont
      // believe so...
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("TAO (%P|%t) - ")
                            ACE_TEXT ("TAO_Connection_Purging_Strategy")
                            ACE_TEXT ("::purge_from_cache (0x%x)\n"),
                            entry));
    }

  // Usa a small scope.  No need to hold our lock while
  // the cache_manager purges it's entry.
  {
    ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, 0);
    this->remove_item_i (entry->ext_id_.property ());
  }

  return this->cache_manager_.purge_entry (entry);
}


ACE_INLINE int
TAO_Connection_Purging_Strategy::make_idle (TAO_PURGING_MAP_ENTRY*& entry)
{
  return this->cache_manager_.make_idle (entry);
}


ACE_INLINE void
TAO_Connection_Purging_Strategy::mark_invalid (TAO_PURGING_MAP_ENTRY*& entry)
{
  this->cache_manager_.mark_invalid (entry);
}


ACE_INLINE int
TAO_Connection_Purging_Strategy::open_cache (TAO_ORB_Core* orb_core,
                                             size_t size)
{
  return this->cache_manager_.open (orb_core, size);
}


ACE_INLINE int
TAO_Connection_Purging_Strategy::close_cache (
                                   ACE_Handle_Set &reactor_registered,
                                   TAO_EventHandlerSet &unregistered)
{
  {
    ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, 0);
    this->remove_all_i ();
  }
  return this->cache_manager_.close (reactor_registered, unregistered);
}


ACE_INLINE size_t
TAO_Connection_Purging_Strategy::total_size () const
{
  return this->cache_manager_.total_size ();
}


ACE_INLINE TAO_PURGING_CACHE_MANAGER&
TAO_Connection_Purging_Strategy::cache_manager ()
{
  return this->cache_manager_;
}


ACE_INLINE TAO_ULong_Connection_Purging_Strategy::HASH_MAP&
TAO_ULong_Connection_Purging_Strategy::tracking_map ()
{
  return this->tracking_map_;
}
