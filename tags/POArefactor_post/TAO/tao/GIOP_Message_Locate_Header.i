// -*- C++ -*-
//$Id$

ACE_INLINE
TAO_GIOP_Locate_Request_Header::TAO_GIOP_Locate_Request_Header (TAO_InputCDR &msg,
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
TAO_GIOP_Locate_Request_Header::request_id (void)
{
  return this->request_id_;
}

ACE_INLINE const TAO::ObjectKey &
TAO_GIOP_Locate_Request_Header::object_key (void) const
{
  return this->profile_.object_key ();
}

ACE_INLINE TAO::ObjectKey &
TAO_GIOP_Locate_Request_Header::object_key (void)
{
  return this->profile_.object_key ();
}

ACE_INLINE TAO_Tagged_Profile &
TAO_GIOP_Locate_Request_Header::profile (void)
{
  return this->profile_;
}

ACE_INLINE TAO_InputCDR &
TAO_GIOP_Locate_Request_Header::incoming_stream (void)
{
  return *this->incoming_;
}

