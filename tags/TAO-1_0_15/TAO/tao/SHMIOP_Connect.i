// -*- C++ -*-
// $Id$

ACE_INLINE TAO_Transport *
TAO_SHMIOP_Server_Connection_Handler::transport (void)
{
  return &(this->transport_);
}

ACE_INLINE TAO_Transport *
TAO_SHMIOP_Client_Connection_Handler::transport (void)
{
  return &(this->transport_);
}
