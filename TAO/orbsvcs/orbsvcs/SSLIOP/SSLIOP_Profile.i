// -*- C++ -*-
// $Id$

ACE_INLINE CORBA::UShort
TAO_SSLIOP_Profile::ssl_port (void) const
{
  return this->ssl_port_;
}

ACE_INLINE TAO_SSLIOP_Client_Connection_Handler *&
TAO_SSLIOP_Profile::hint(void)
{
  return this->hint_;
}
