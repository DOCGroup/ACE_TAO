// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE bool
TAO_Transport::queue_is_empty_i (void) const
{
  return (this->head_ == 0);
}

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

ACE_INLINE TAO::Connection_Role
TAO_Transport::opened_as () const
{
  return this->opening_connection_role_;
}

ACE_INLINE void
TAO_Transport::opened_as (TAO::Connection_Role role)
{
  this->opening_connection_role_ = role;
}

ACE_INLINE TAO::Transport_Cache_Manager::HASH_MAP_ENTRY *
TAO_Transport::cache_map_entry (void)
{
  return this->cache_map_entry_;
}

ACE_INLINE void
TAO_Transport::cache_map_entry (
    TAO::Transport_Cache_Manager::HASH_MAP_ENTRY *entry)
{
  // Sync with TAO_Transport::purge_entry()
  ACE_GUARD (ACE_Lock, ace_mon, *this->handler_lock_);
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

ACE_INLINE bool
TAO_Transport::queue_is_empty (void)
{
  ACE_GUARD_RETURN (ACE_Lock,
                    ace_mon,
                    *this->handler_lock_,
                    false);
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

ACE_INLINE TAO_GIOP_Message_Base *
TAO_Transport::messaging_object (void)
{
  return this->messaging_object_;
}

//********************************************************************
// codeset related methods


ACE_INLINE TAO_Codeset_Translator_Base *
TAO_Transport::char_translator (void) const
{
  return this->char_translator_;
}

ACE_INLINE TAO_Codeset_Translator_Base *
TAO_Transport::wchar_translator (void) const
{
  return this->wchar_translator_;
}

ACE_INLINE void
TAO_Transport::char_translator (TAO_Codeset_Translator_Base *tf)
{
  this->char_translator_ = tf;
  this->tcs_set_ = 1;
}

ACE_INLINE void
TAO_Transport::wchar_translator (TAO_Codeset_Translator_Base *tf)
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
TAO_Transport::first_request_sent (bool flag)
{
  this->first_request_ = flag;
}

ACE_INLINE bool
TAO_Transport::first_request (void) const
{
  return this->first_request_;
}

ACE_INLINE bool
TAO_Transport::is_connected (void) const
{
  ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->handler_lock_, false);
  return this->is_connected_;
}

ACE_INLINE TAO_Connection_Handler *
TAO_Transport::connection_handler (void)
{
  return this->connection_handler_i();
}

ACE_INLINE size_t
TAO_Transport::recv_buffer_size (void) const
{
  return this->recv_buffer_size_;
}

ACE_INLINE size_t
TAO_Transport::sent_byte_count (void) const
{
  return this->sent_byte_count_;
}

#if TAO_HAS_TRANSPORT_CURRENT == 1

ACE_INLINE TAO::Transport::Stats*
TAO_Transport::stats (void) const
{
  return this->stats_;
}

ACE_INLINE
TAO::Transport::Stats::Stats ()
  : messages_rcvd_ (0)
  , messages_sent_ (0)
  , bytes_rcvd_()
  , bytes_sent_ ()
  , opened_since_ ()
{
}

ACE_INLINE void
TAO::Transport::Stats::messages_sent (size_t message_length)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->stat_mutex_);

  ++this->messages_sent_;
  this->bytes_sent_.sample (message_length);
}

ACE_INLINE CORBA::LongLong
TAO::Transport::Stats::messages_sent (void) const
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->stat_mutex_, 0);

  return this->messages_sent_;
}

ACE_INLINE CORBA::LongLong
TAO::Transport::Stats::bytes_sent (void) const
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->stat_mutex_, 0);

  return this->bytes_sent_.sum_;
}

ACE_INLINE void
TAO::Transport::Stats::messages_received (size_t message_length)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->stat_mutex_);

  ++this->messages_rcvd_;
  this->bytes_rcvd_.sample (message_length);
}

ACE_INLINE CORBA::LongLong
TAO::Transport::Stats::messages_received (void) const
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->stat_mutex_, 0);

  return this->messages_rcvd_;
}

ACE_INLINE CORBA::LongLong
TAO::Transport::Stats::bytes_received (void) const
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->stat_mutex_, 0);

  return this->bytes_rcvd_.sum_;
}

ACE_INLINE void
TAO::Transport::Stats::opened_since (const ACE_Time_Value& tv)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->stat_mutex_);

  this->opened_since_ = tv;
}

ACE_INLINE const ACE_Time_Value&
TAO::Transport::Stats::opened_since (void) const
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->stat_mutex_, ACE_Time_Value::zero);

  return this->opened_since_;
}

#endif /* TAO_HAS_TRANSPORT_CURRENT == 1 */

ACE_INLINE int
TAO_Transport::notify_reactor (void)
{
  if (!this->ws_->is_registered ())
    {
      return 0;
    }

  return this->notify_reactor_now ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
