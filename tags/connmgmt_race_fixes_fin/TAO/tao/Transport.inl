// $Id$

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

ACE_INLINE CORBA::ULong
TAO_Transport::tag (void) const
{
  return this->tag_;
}

ACE_INLINE long
TAO_Transport::buffering_timer_id (void) const
{
  return this->buffering_timer_id_;
}

ACE_INLINE void
TAO_Transport::buffering_timer_id (long new_value)
{
  this->buffering_timer_id_ = new_value;
}

ACE_INLINE const ACE_Time_Value &
TAO_Transport::buffering_timeout_value (void) const
{
  return this->buffering_timeout_value_;
}

ACE_INLINE void
TAO_Transport::buffering_timeout_value (const ACE_Time_Value &new_value)
{
  this->buffering_timeout_value_ = new_value;
}

ACE_INLINE TAO_Transport_Buffering_Queue &
TAO_Transport::buffering_queue (void)
{
  if (this->buffering_queue_ == 0)
    {
      // Infinite high water mark: ACE_UINT32_MAX.
      this->buffering_queue_ =
        new TAO_Transport_Buffering_Queue (ACE_UINT32_MAX);
    }

  return *this->buffering_queue_;
}

ACE_INLINE void
TAO_Transport::dequeue_head (void)
{
  // Remove from the head of the queue.
  ACE_Message_Block *message_block = 0;
  int result = this->buffering_queue_->dequeue_head (message_block);

  // @@ What to do here on failures?
  ACE_ASSERT (result != -1);
  ACE_UNUSED_ARG (result);

  // Release the memory.
  message_block->release ();
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

ACE_INLINE void
TAO_Transport::dequeue_all (void)
{
  // Flush all queued messages.
  if (this->buffering_queue_)
    {
      while (!this->buffering_queue_->is_empty ())
        this->dequeue_head ();
    }
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
