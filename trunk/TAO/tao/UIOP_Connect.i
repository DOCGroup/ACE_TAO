// -*- C++ -*-
// $Id$

#if !defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS)

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

ACE_INLINE int
TAO_UIOP_Client_Connection_Handler::handle_input (ACE_HANDLE)
{
  // Call the waiter to handle the input.
  return this->transport ()->wait_strategy ()->handle_input ();
}

ACE_INLINE int
TAO_UIOP_Client_Connection_Handler::close (u_long /* flags */)
{
  this->destroy ();

  return 0;
}

#endif /* ACE_LACKS_UNIX_DOMAIN_SOCKETS */
