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

ACE_INLINE int
TAO_IIOP_Client_Connection_Handler::handle_input (ACE_HANDLE)
{
  // Call the waiter to handle the input.
  return this->transport ()->wait_strategy ()->handle_input ();
}

ACE_INLINE int
TAO_IIOP_Client_Connection_Handler::close (u_long)
{
  this->destroy ();

  return 0;
}
