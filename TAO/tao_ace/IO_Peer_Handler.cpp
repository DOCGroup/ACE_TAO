#include "IO_Peer_Handler.h"


#if !defined (__ACE_INLINE__)
#include "IO_Peer_Handler.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao_ace, SOCK_Event_Reactive_Handler, "$Id$")

#define PR_ST_1 ACE_PEER_STREAM_1
#define PR_ST_2 ACE_PEER_STREAM_2

template <PR_ST_1>
TAO_ACE_IO_Peer_Handler<PR_ST_2>::TAO_ACE_IO_Peer_Handler (void)
{
  ACE_TRACE ("TAO_ACE_IO_Peer_Handler::TAO_ACE_IO_Peer_Handler");
}



template <PR_ST_1> ACE_HANDLE
TAO_ACE_IO_Peer_Handler<PR_ST_2>::get_handle (void) const
{
  ACE_TRACE ("TAO_ACE_SOCK_Peer_Handler::get_handle");
  return this->peer_.get_handle ();
}

template <PR_ST_1> void
TAO_ACE_IO_Peer_Handler<PR_ST_2>::set_handle (ACE_HANDLE h)
{
  ACE_TRACE ("TAO_ACE_SOCK_Peer_Handler::set_handle");
  this->peer_->set_handle (h);
}

template <PR_ST_1> ACE_PEER_STREAM &
TAO_ACE_IO_Peer_Handler<PR_ST_2>::peer (void) const
{
  ACE_TRACE ("TAO_ACE_SOCK_Peer_Handler::peer");
  return (ACE_PEER_STREAM &) this->peer_;
}


template <PR_ST_1> int
TAO_ACE_IO_Peer_Handler<PR_ST_2>::handle_input (ACE_HANDLE)
{
  ACE_TRACE ("TAO_ACE_SOCK_Peer_Handler::handle_input");

  return 0;
}

template <PR_ST_1> int
TAO_ACE_IO_Peer_Handler<PR_ST_2>::handle_output (ACE_HANDLE)
{
  ACE_TRACE ("TAO_ACE_SOCK_Peer_Handler::handle_output");

  return 0;
}

template <PR_ST_1> int
TAO_ACE_IO_Peer_Handler<PR_ST_2>::handle_exception (ACE_HANDLE)
{
  ACE_TRACE ("TAO_ACE_SOCK_Peer_Handler::handle_exception");

  return 0;
}
