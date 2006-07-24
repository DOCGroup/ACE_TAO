// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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

ACE_INLINE
int
TAO_AV_UDP_Flow_Handler::close (void)
{
  return  this->sock_dgram_.close ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
