// $Id$

ACE_INLINE CORBA::ULong
TAO_Transport::tag (void) const
{
  return this->tag_;
}

ACE_INLINE TAO_ORB_Core *
TAO_Transport::orb_core (void) const
{
  return this->orb_core_;
}

ACE_INLINE TAO_Transport_Mux_Strategy *
TAO_Transport::tms (void) const
{
  return tms_;
}

// Return the Wait strategy used by the Transport.
ACE_INLINE TAO_Wait_Strategy *
TAO_Transport::wait_strategy (void) const
{
  return this->ws_;
}

ACE_INLINE int
TAO_Transport::bidirectional_flag (void) const
{
  return this->bidirectional_flag_;
}

ACE_INLINE void
TAO_Transport::bidirectional_flag (int flag)
{
  this->bidirectional_flag_ = flag;
}

/*ACE_INLINE TAO_Transport_Cache_Manager::HASH_MAP_ENTRY *
TAO_Transport::cache_map_entry (void)
{
  return this->cache_map_entry_;
}
*/

ACE_INLINE void
TAO_Transport::cache_map_entry (
    TAO_Transport_Cache_Manager::HASH_MAP_ENTRY *entry)
{
  this->cache_map_entry_ = entry;
}
