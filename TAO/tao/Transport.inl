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

ACE_INLINE size_t
TAO_Transport::id (void) const
{
  return this->id_;
}

ACE_INLINE void
TAO_Transport::id (size_t id)
{
  this->id_ = id;
}

ACE_INLINE int
TAO_Transport::queue_is_empty (void)
{
  ACE_GUARD_RETURN (ACE_Lock,
                    ace_mon,
                    *this->handler_lock_,
                    -1);
  return this->queue_is_empty_i ();
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

//********************************************************************
// codeset related methods


ACE_INLINE TAO_Codeset_Translator_Factory *
TAO_Transport::char_translator (void) const
{
  return this->char_translator_;
}

ACE_INLINE TAO_Codeset_Translator_Factory *
TAO_Transport::wchar_translator (void) const
{
  return this->wchar_translator_;
}

ACE_INLINE void
TAO_Transport::char_translator (TAO_Codeset_Translator_Factory *tf)
{
  this->char_translator_ = tf;
  this->tcs_set_ = 1;
}

ACE_INLINE void
TAO_Transport::wchar_translator (TAO_Codeset_Translator_Factory *tf)
{
  this->wchar_translator_ = tf;
  this->tcs_set_ = 1;
}

/// CodeSet negotiation
ACE_INLINE CORBA::Boolean
TAO_Transport::is_tcs_set(void) const
{
  return tcs_set_;
}

ACE_INLINE void
TAO_Transport::first_request_sent (void)
{
  this->first_request_ = 0;
}



/*****************************************************/
ACE_INLINE
TAO_Transport_Refcount_Guard::TAO_Transport_Refcount_Guard (TAO_Transport *t)
  :tr_ (TAO_Transport::_duplicate (t))
{
}

ACE_INLINE
TAO_Transport_Refcount_Guard::~TAO_Transport_Refcount_Guard (void)
{
  TAO_Transport::release (this->tr_);
}
