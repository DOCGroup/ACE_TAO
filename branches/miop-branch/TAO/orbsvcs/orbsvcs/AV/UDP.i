/* -*- C++ -*- */

// $Id$

//----------------------------------------------------------------------
// TAO_AV_UDP_Flow_Handler
//----------------------------------------------------------------------
ACE_INLINE
const ACE_SOCK_Dgram *
TAO_AV_UDP_Flow_Handler::get_socket (void) const
{
  return &this->sock_dgram_;
}

ACE_INLINE
int
TAO_AV_UDP_Flow_Handler::open (ACE_Addr &address)
{
  return this->sock_dgram_.open (address);
}
