// -*- C++ -*-
// $Id$

ACE_INLINE TAO_Transport *
TAO_IIOP_Server_Connection_Handler::transport (void)
{
  return &(this->transport_);
}

ACE_INLINE TAO_Transport *
TAO_IIOP_Client_Connection_Handler::transport (void)
{
  return &(this->transport_);
}
