// -*- C++ -*-

//$Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO_GIOP_Message_State::TAO_GIOP_Message_State (void)
  : giop_version_ (TAO_DEF_GIOP_MAJOR,
                   TAO_DEF_GIOP_MINOR),
    byte_order_ (0),
    message_type_ (GIOP::Request),
    payload_size_ (0),
    more_fragments_ (false),
    compressed_ (false)
{
}

ACE_INLINE CORBA::ULong
TAO_GIOP_Message_State::message_size (void) const
{
  return this->payload_size_ + TAO_GIOP_MESSAGE_HEADER_LEN;
}

ACE_INLINE CORBA::ULong
TAO_GIOP_Message_State::payload_size (void) const
{
  return this->payload_size_;
}

ACE_INLINE CORBA::Octet
TAO_GIOP_Message_State::byte_order (void) const
{
  return this->byte_order_;
}

ACE_INLINE GIOP::MsgType
TAO_GIOP_Message_State::message_type (void) const
{
  return this->message_type_;
}

ACE_INLINE CORBA::Boolean
TAO_GIOP_Message_State::more_fragments (void) const
{
  return this->more_fragments_;
}

ACE_INLINE void
TAO_GIOP_Message_State::more_fragments (CORBA::Boolean fragment)
{
  this->more_fragments_ = fragment;
}

ACE_INLINE TAO_GIOP_Message_Version const &
TAO_GIOP_Message_State::giop_version (void) const
{
  return this->giop_version_;
}

#if defined (TAO_HAS_ZIOP) && TAO_HAS_ZIOP ==1
ACE_INLINE CORBA::Boolean
TAO_GIOP_Message_State::compressed (void) const
{
  return this->compressed_;
}
#endif

TAO_END_VERSIONED_NAMESPACE_DECL
