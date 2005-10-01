#include "IIOP_SSL_Acceptor.h"

#include "tao/ORB_Core.h"
#include "tao/debug.h"


ACE_RCSID (SSLIOP,
           IIOP_SSL_Acceptor,
           "$Id$")

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Acceptor<TAO::IIOP_SSL_Connection_Handler, ACE_SOCK_ACCEPTOR>;
template class ACE_Strategy_Acceptor<TAO::IIOP_SSL_Connection_Handler, ACE_SOCK_ACCEPTOR>;
template class ACE_Accept_Strategy<TAO::IIOP_SSL_Connection_Handler, ACE_SOCK_ACCEPTOR>;
template class ACE_Creation_Strategy<TAO::IIOP_SSL_Connection_Handler>;
template class ACE_Concurrency_Strategy<TAO::IIOP_SSL_Connection_Handler>;
template class ACE_Scheduling_Strategy<TAO::IIOP_SSL_Connection_Handler>;
template class TAO_Creation_Strategy<TAO::IIOP_SSL_Connection_Handler>;
template class TAO_Concurrency_Strategy<TAO::IIOP_SSL_Connection_Handler>;
template class TAO_Accept_Strategy<TAO::IIOP_SSL_Connection_Handler, ACE_SOCK_ACCEPTOR>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Acceptor<TAO::IIOP_SSL_Connection_Handler, ACE_SOCK_ACCEPTOR>
#pragma instantiate ACE_Strategy_Acceptor<TAO::IIOP_SSL_Connection_Handler, ACE_SOCK_ACCEPTOR>
#pragma instantiate ACE_Accept_Strategy<TAO::IIOP_SSL_Connection_Handler, ACE_SOCK_ACCEPTOR>
#pragma instantiate ACE_Creation_Strategy<TAO::IIOP_SSL_Connection_Handler>
#pragma instantiate ACE_Concurrency_Strategy<TAO::IIOP_SSL_Connection_Handler>
#pragma instantiate ACE_Scheduling_Strategy<TAO::IIOP_SSL_Connection_Handler>
#pragma instantiate TAO_Creation_Strategy<TAO::IIOP_SSL_Connection_Handler>
#pragma instantiate TAO_Concurrency_Strategy<TAO::IIOP_SSL_Connection_Handler>
#pragma instantiate TAO_Accept_Strategy<TAO::IIOP_SSL_Connection_Handler, ACE_SOCK_ACCEPTOR>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

TAO::IIOP_SSL_Acceptor::IIOP_SSL_Acceptor (CORBA::Boolean flag)
  : TAO_IIOP_Acceptor (flag),
    base_acceptor_ (),
    creation_strategy_ (0),
    concurrency_strategy_ (0),
    accept_strategy_ (0)
{
}

TAO::IIOP_SSL_Acceptor::~IIOP_SSL_Acceptor (void)
{
  // Make sure we are closed before we start destroying the
  // strategies.
  this->close ();

  delete this->creation_strategy_;
  delete this->concurrency_strategy_;
  delete this->accept_strategy_;
}

int
TAO::IIOP_SSL_Acceptor::close (void)
{
  return this->base_acceptor_.close ();
}
