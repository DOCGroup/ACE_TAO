//$Id$

ACE_INLINE
TAO_GIOP_Message_Acceptors::TAO_GIOP_Message_Acceptors (void)
{
  //no-op
}

ACE_INLINE CORBA::Octet
TAO_GIOP_Message_Acceptors::major_version (void)
{
  return this->accept_state_->major_version ();
}

ACE_INLINE CORBA::Octet
TAO_GIOP_Message_Acceptors::minor_version (void)
{
  return this->accept_state_->minor_version ();
}
