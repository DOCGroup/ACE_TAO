// -*- C++ -*-
//
TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO_GIOP_Locate_Request_Header::TAO_GIOP_Locate_Request_Header (
  TAO_InputCDR &msg,
  TAO_ORB_Core *orb)
  : request_id_ (0),
    profile_ (orb),
    incoming_ (&msg)
{
}

ACE_INLINE void
TAO_GIOP_Locate_Request_Header::request_id (CORBA::ULong id)
{
  this->request_id_ = id;
}

ACE_INLINE CORBA::ULong
TAO_GIOP_Locate_Request_Header::request_id () const
{
  return this->request_id_;
}

ACE_INLINE const TAO::ObjectKey &
TAO_GIOP_Locate_Request_Header::object_key () const
{
  return this->profile_.object_key ();
}

ACE_INLINE TAO::ObjectKey &
TAO_GIOP_Locate_Request_Header::object_key ()
{
  return this->profile_.object_key ();
}

ACE_INLINE TAO_Tagged_Profile &
TAO_GIOP_Locate_Request_Header::profile ()
{
  return this->profile_;
}

ACE_INLINE TAO_InputCDR &
TAO_GIOP_Locate_Request_Header::incoming_stream ()
{
  return *this->incoming_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
