//$Id$

///////////////////////////////////////////////////////////////////
// Methods for TAO_GIOP_Message_Connector_11
//////////////////////////////////////////////////////////////////

ACE_INLINE CORBA::Octet
TAO_GIOP_Message_Connector_11:: major_version (void)
{
  // Any harm in hardcoding??
  return (CORBA::Octet) 1;
}

ACE_INLINE CORBA::Octet
TAO_GIOP_Message_Connector_11:: minor_version (void)
{
  // Any harm in hardcoding??
  return (CORBA::Octet) 1;
}

///////////////////////////////////////////////////////////////////
// Methods for TAO_GIOP_Message_Connector_10
//////////////////////////////////////////////////////////////////

ACE_INLINE CORBA::Octet
TAO_GIOP_Message_Connector_10:: minor_version (void)
{
  // Any harm in hardcoding??
  return (CORBA::Octet) 0;
}
