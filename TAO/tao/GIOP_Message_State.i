// -*- C++ -*-

//$Id$

//
// Inlined methods for TAO_GIOP_Message_State
//
ACE_INLINE int
TAO_GIOP_Message_State::message_fragmented (void)
{
  if (this->more_fragments_)
    return 1;

  return 0;
}

ACE_INLINE void
TAO_GIOP_Message_State::reset (int /*reset_contents*/)
{
  this->message_size_ = 0;
  this->more_fragments_ = 0;
}

ACE_INLINE CORBA::Boolean
TAO_GIOP_Message_State::header_received (void) const
{
  return this->message_size_ != 0;
}
