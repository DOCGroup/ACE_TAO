// -*- C++ -*-

//$Id$

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
  this->request_id_ = 0;
  this->missing_data_ = 0;
}

ACE_INLINE const TAO_GIOP_Message_Version &
TAO_GIOP_Message_State::giop_version () const
{
  return this->giop_version_;
}

ACE_INLINE CORBA::Octet
TAO_GIOP_Message_State::more_fragments () const
{
  return this->more_fragments_;
}

ACE_INLINE CORBA::Octet
TAO_GIOP_Message_State::message_type () const
{
  return this->message_type_;
}

ACE_INLINE void
TAO_GIOP_Message_State::set_payload_size_from_buffer (const char *rd_ptr)
{
  // Move the read pointer
  rd_ptr += TAO_GIOP_MESSAGE_SIZE_OFFSET;

  this->message_size_ =  this->read_ulong (rd_ptr);
}
