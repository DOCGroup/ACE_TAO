// -*- C++ -*-
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

ACE_INLINE TAO_Transport_Cache_Manager::HASH_MAP_ENTRY *
TAO_Transport::cache_map_entry (void)
{
  return this->cache_map_entry_;
}


ACE_INLINE void
TAO_Transport::cache_map_entry (
    TAO_Transport_Cache_Manager::HASH_MAP_ENTRY *entry)
{
  this->cache_map_entry_ = entry;
}

ACE_INLINE int
TAO_Transport::flush_timer_pending (void) const
{
  return this->flush_timer_id_ != -1;
}

ACE_INLINE void
TAO_Transport::reset_flush_timer (void)
{
  this->flush_timer_id_ = -1;
  this->current_deadline_ = ACE_Time_Value::zero;
}

ACE_INLINE int
TAO_Transport::check_event_handler_i (const char *caller)
{
  // if there's no associated event handler, then we act like a null
  // transport
  if (this->event_handler_i () == 0)
    {
      this->report_invalid_event_handler (caller);
      errno = ENOENT;
      return -1;
    }
  return 0;
}

ACE_INLINE unsigned long
TAO_Transport::purging_order (void) const
{
  return this->purging_order_;
}

ACE_INLINE void
TAO_Transport::purging_order (unsigned long value)
{
  // This should only be called by the Transport Cache Manager when
  // it is holding it's lock.
  // The transport should still be here since the cache manager still
  // has a reference to it.
  this->purging_order_ = value;
}
