// -*- C++ -*-
//$Id$

ACE_INLINE
TAO_GIOP_Locate_Request_Header::TAO_GIOP_Locate_Request_Header (TAO_InputCDR &msg)
  : request_id_ (0),
    object_key_ (),
    profile_ (),
    addr_info_ (),
    incoming_ (&msg)
{
}

ACE_INLINE void
TAO_GIOP_Locate_Request_Header::request_id (CORBA::ULong id)
{
  this->request_id_ = id;
}

ACE_INLINE CORBA::ULong
TAO_GIOP_Locate_Request_Header::request_id (void)
{
  return this->request_id_;
}

ACE_INLINE const TAO_ObjectKey &
TAO_GIOP_Locate_Request_Header::object_key (void) const
{
  return object_key_;
}

ACE_INLINE TAO_ObjectKey &
TAO_GIOP_Locate_Request_Header::object_key (void)
{
  return this->object_key_;
}

ACE_INLINE IOP::TaggedProfile&
TAO_GIOP_Locate_Request_Header::tagged_profile (void)
{
  return this->profile_;
}


ACE_INLINE GIOP::IORAddressingInfo &
TAO_GIOP_Locate_Request_Header::addressing_info (void)
{
  return this->addr_info_;
}


ACE_INLINE TAO_InputCDR &
TAO_GIOP_Locate_Request_Header::incoming_stream (void)
{
  return *this->incoming_;
}

