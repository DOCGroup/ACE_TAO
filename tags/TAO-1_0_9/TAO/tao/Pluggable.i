// -*- C++ -*-
// $Id$

// ****************************************************************

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

// ****************************************************************

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
