// -*- C++ -*-
//
// $Id$


ACE_INLINE TAO_IIOP_Endpoint *
TAO_SSLIOP_Endpoint::iiop_endpoint (void) const
{
  return this->iiop_endpoint_;
}

ACE_INLINE const SSLIOP::SSL &
TAO_SSLIOP_Endpoint::ssl_component (void) const
{
  return this->ssl_component_;
}

/*ACE_INLINE TAO_SSLIOP_Client_Connection_Handler *&
TAO_SSLIOP_Endpoint::ssl_hint(void)
{
  return this->ssl_hint_;
}
*/
