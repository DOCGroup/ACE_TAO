// -*- C++ -*-

//$Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO_GIOP_Message_State::TAO_GIOP_Message_State (void)
  : giop_version_ (TAO_DEF_GIOP_MAJOR,
                   TAO_DEF_GIOP_MINOR),
    byte_order_ (0),
    message_type_ (TAO_PLUGGABLE_MESSAGE_REQUEST),
    message_size_ (0),
    more_fragments_ (0)
{
}

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

ACE_INLINE TAO_Pluggable_Message_Type
TAO_GIOP_Message_State::message_type (void) const
{
  return this->message_type_;
}

ACE_INLINE CORBA::Octet
TAO_GIOP_Message_State::more_fragments (void) const
{
  return this->more_fragments_;
}

ACE_INLINE TAO_GIOP_Message_Version const &
TAO_GIOP_Message_State::giop_version (void) const
{
  return this->giop_version_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
