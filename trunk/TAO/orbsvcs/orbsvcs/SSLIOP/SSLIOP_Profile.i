// -*- C++ -*-
// $Id$

ACE_INLINE CORBA::UShort
TAO_SSLIOP_Profile::ssl_port (void) const
{
  return this->ssl_component_.port;
}

ACE_INLINE TAO_SSLIOP_Client_Connection_Handler *&
TAO_SSLIOP_Profile::ssl_hint(void)
{
  return this->ssl_hint_;
}
