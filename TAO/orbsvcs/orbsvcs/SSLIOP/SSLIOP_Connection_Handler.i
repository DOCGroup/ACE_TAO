// -*- C++ -*-
//
// $Id$


ACE_INLINE TAO_Transport *
TAO_SSLIOP_Connection_Handler::transport (void)
{
  return &(this->transport_);
}


ACE_INLINE
TAO_SSL_State_Guard::TAO_SSL_State_Guard (
  TAO_SSLIOP_Connection_Handler *handler,
  TAO_ORB_Core *orb_core,
  int &result)
  : handler_ (handler)
{
  // Set up the SSLIOP::Current object.
  result = this->handler_->setup_ssl_state (orb_core);
}

ACE_INLINE
TAO_SSL_State_Guard::~TAO_SSL_State_Guard (void)
{
  this->handler_->teardown_ssl_state ();
}
