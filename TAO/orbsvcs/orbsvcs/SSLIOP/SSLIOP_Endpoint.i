// -*- C++ -*-
// $Id$


ACE_INLINE TAO_IIOP_Endpoint *
TAO_SSLIOP_Endpoint::iiop_endpoint (void) const
{
  return this->iiop_endpoint_;
}

ACE_INLINE CORBA::UShort
TAO_SSLIOP_Endpoint::ssl_port (void) const
{
  return this->ssl_component_.port;
}

ACE_INLINE TAO_SSLIOP_Client_Connection_Handler *&
TAO_SSLIOP_Endpoint::ssl_hint(void)
{
  return this->ssl_hint_;
}
