//* -*- C++ -*- */
//$Id$

ACE_INLINE
TAO_Connection_Handler::TAO_Connection_Handler (void)
  : orb_core_ (0),
    tss_resources_ (0)
{
}

ACE_INLINE void
TAO_Connection_Handler::recycle_state (ACE_Recyclable_State new_state)
{
  this->recycle_state_ = new_state;
}

ACE_INLINE ACE_Recyclable_State
TAO_Connection_Handler::recycle_state (void)
{
  return this->recycle_state_;
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
