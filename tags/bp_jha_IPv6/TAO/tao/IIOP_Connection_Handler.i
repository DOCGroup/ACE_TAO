// -*- C++ -*-
// $Id$

ACE_INLINE TAO_Transport *
TAO_IIOP_Connection_Handler::transport (void)
{
  return &(this->transport_);
}
