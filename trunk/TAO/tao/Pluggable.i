// -*- C++ -*-
// $Id$


// ****************************************************************
ACE_INLINE ssize_t
TAO_Transport::read (char *buf,
                     size_t len,
                     const ACE_Time_Value * /*max_wait_time*/)
{
  return ACE::recv (this->handle (),
                    (void *) buf,
                    len);
}

// @@ One of the orb_core () methods should be done away. Will visit
// @@ this later -- Bala

ACE_INLINE TAO_ORB_Core *
TAO_Transport::orb_core (void) const
{
  return this->orb_core_;
}

ACE_INLINE TAO_ORB_Core *
TAO_Transport::orb_core (void)
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

// ****************************************************************
ACE_INLINE TAO_ORB_Core *
TAO_Connector::orb_core (void)
{
  return this->orb_core_;
}

ACE_INLINE void
TAO_Connector::orb_core (TAO_ORB_Core *orb_core)
{
  this->orb_core_ = orb_core;
}


ACE_INLINE CORBA::ULong
TAO_Connector::tag (void) const
{
  return this->tag_;
}

// ****************************************************************

ACE_INLINE CORBA::ULong
TAO_Acceptor::tag (void) const
{
  return this->tag_;
}

ACE_INLINE CORBA::Short
TAO_Acceptor::priority (void) const
{
  return this->priority_;
}
