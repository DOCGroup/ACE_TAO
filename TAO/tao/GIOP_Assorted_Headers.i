//$Id$

ACE_INLINE 
TAO_GIOP_Locate_Request_Header::TAO_GIOP_Locate_Request_Header (TAO_InputCDR &msg)
  : request_id_ (0),
    addressing_disposition_ (0),
    target_address_ (),
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

ACE_INLINE void
TAO_GIOP_Locate_Request_Header::addressing_disposition (CORBA::Short
                                                      add)
{
  this->addressing_disposition_ = add;
}

ACE_INLINE CORBA::Short
TAO_GIOP_Locate_Request_Header::addressing_disposition (void)
{
  return this->addressing_disposition_;
}

ACE_INLINE const GIOP::TargetAddress &
TAO_GIOP_Locate_Request_Header::target_address (void) const
{
  return this->target_address_;
}

ACE_INLINE GIOP::TargetAddress &
TAO_GIOP_Locate_Request_Header::target_address (void)
{
  return this->target_address_;
}

ACE_INLINE TAO_InputCDR &
TAO_GIOP_Locate_Request_Header::incoming_stream (void)
{
  return *this->incoming_;
}

/*TAO_ObjectKey &
TAO_GIOP_Locate_Request_Header::object_key_ref (void)
{
  // Not a good idea, need to check here..
  return this->target_address_.object_key ();
  // For the rest do the checeks here
}
*/
