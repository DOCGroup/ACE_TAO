//* -*- C++ -*- */
//$Id$

ACE_INLINE
TAO_Connection_Handler::TAO_Connection_Handler (void)
  : orb_core_ (0),
    tss_resources_ (0),
    ref_count_ (1),
    cache_map_entry_ (0),
    is_registered_ (0)
{
}

ACE_INLINE TAO_Connection_Cache_Manager::HASH_MAP_ENTRY *
TAO_Connection_Handler::cache_map_entry (void)
{
  return this->cache_map_entry_;
}

ACE_INLINE void
TAO_Connection_Handler::cache_map_entry (
    TAO_Connection_Cache_Manager::HASH_MAP_ENTRY *entry)
{
  this->cache_map_entry_ = entry;
}

ACE_INLINE void
TAO_Connection_Handler::incr_ref_count (void)
{
  this->ref_count_ ++;
}

ACE_INLINE void
TAO_Connection_Handler::decr_ref_count (void)
{
  this->ref_count_ --;

  if (this->ref_count_ == 0)
    {
      // This check is required. If the Cache map has been closed or
      // vanishes, the entry that we have is invalid. So, do this
      // check before we access the internals of the
      // <cache_map_entry_>
      if (this->cache_map_entry_)
        {
          // Now mark the Ext_Id for us as closed
          this->cache_map_entry_->int_id_.recycle_state
            (ACE_RECYCLABLE_CLOSED);
        }

      // Now the handler is gone
      delete this;
    }
}


ACE_INLINE CORBA::Boolean
TAO_Connection_Handler::is_registered (void)
{
  return this->is_registered_;
}

ACE_INLINE void
TAO_Connection_Handler::is_registered (CORBA::Boolean flag)
{
  this->is_registered_ = flag;
}

ACE_INLINE TAO_ORB_Core *
TAO_Connection_Handler::orb_core (void)
{
  return this->orb_core_;
}

ACE_INLINE TAO_ORB_Core_TSS_Resources *
TAO_Connection_Handler::tss_resources (void)
{
  return this->tss_resources_;
}
