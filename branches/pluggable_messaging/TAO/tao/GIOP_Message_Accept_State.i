//$Id$

/////////////////////////////////////////////////////
//  TAO_GIOP_Message_Accept_State_11
////////////////////////////////////////////////////

ACE_INLINE CORBA::Octet
TAO_GIOP_Message_Accept_State_11::major_version (void)
{
  return (CORBA::Octet) 1;
}

ACE_INLINE CORBA::Octet
TAO_GIOP_Message_Accept_State_11::minor_version (void)
{
  return (CORBA::Octet) 1;
}

/////////////////////////////////////////////////////
//  TAO_GIOP_Message_Accept_State_10
////////////////////////////////////////////////////
ACE_INLINE CORBA::Octet
TAO_GIOP_Message_Accept_State_10::minor_version (void)
{
  return (CORBA::Octet) 0;
}
