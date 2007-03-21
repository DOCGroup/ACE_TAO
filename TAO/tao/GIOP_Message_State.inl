// -*- C++ -*-

//$Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE CORBA::ULong
TAO_GIOP_Message_State::message_size (void) const
{
  CORBA::ULong len =
    this->message_size_ + TAO_GIOP_MESSAGE_HEADER_LEN;

  return len;
}

ACE_INLINE CORBA::ULong
TAO_GIOP_Message_State::payload_size (void) const
{
  return this->message_size_;
}

ACE_INLINE CORBA::Octet
TAO_GIOP_Message_State::byte_order (void) const
{
  return this->byte_order_;
}

ACE_INLINE void
TAO_GIOP_Message_State::reset (void)
{
  this->message_type_ = 0;
  this->message_size_ = 0;
  this->more_fragments_ = 0;
  this->missing_data_ = 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
