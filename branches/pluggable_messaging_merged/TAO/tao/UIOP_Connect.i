// -*- C++ -*-
// $Id$

#if defined (TAO_HAS_UIOP)

ACE_INLINE TAO_Transport *
TAO_UIOP_Server_Connection_Handler::transport (void)
{
  return &(this->transport_);
}

ACE_INLINE TAO_Transport *
TAO_UIOP_Client_Connection_Handler::transport (void)
{
  return &(this->transport_);
}

#endif /* TAO_HAS_UIOP */
