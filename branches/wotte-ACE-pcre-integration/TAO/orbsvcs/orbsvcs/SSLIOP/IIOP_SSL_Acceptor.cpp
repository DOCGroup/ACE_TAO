// -*- C++ -*-
//
// $Id$

#include "IIOP_SSL_Acceptor.h"

#include "SSLIOP_Util.h"

#include "tao/ORB_Core.h"
#include "tao/debug.h"


ACE_RCSID (TAO_SSLIOP,
           IIOP_SSL_Acceptor,
           "$Id$")

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Acceptor<TAO_IIOP_SSL_Connection_Handler, ACE_SOCK_ACCEPTOR>;
template class ACE_Strategy_Acceptor<TAO_IIOP_SSL_Connection_Handler, ACE_SOCK_ACCEPTOR>;
template class ACE_Accept_Strategy<TAO_IIOP_SSL_Connection_Handler, ACE_SOCK_ACCEPTOR>;
template class ACE_Creation_Strategy<TAO_IIOP_SSL_Connection_Handler>;
template class ACE_Concurrency_Strategy<TAO_IIOP_SSL_Connection_Handler>;
template class ACE_Scheduling_Strategy<TAO_IIOP_SSL_Connection_Handler>;
template class TAO_Creation_Strategy<TAO_IIOP_SSL_Connection_Handler>;
template class TAO_Concurrency_Strategy<TAO_IIOP_SSL_Connection_Handler>;
template class TAO_Accept_Strategy<TAO_IIOP_SSL_Connection_Handler, ACE_SOCK_ACCEPTOR>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Acceptor<TAO_IIOP_SSL_Connection_Handler, ACE_SOCK_ACCEPTOR>
#pragma instantiate ACE_Strategy_Acceptor<TAO_IIOP_SSL_Connection_Handler, ACE_SOCK_ACCEPTOR>
#pragma instantiate ACE_Accept_Strategy<TAO_IIOP_SSL_Connection_Handler, ACE_SOCK_ACCEPTOR>
#pragma instantiate ACE_Creation_Strategy<TAO_IIOP_SSL_Connection_Handler>
#pragma instantiate ACE_Concurrency_Strategy<TAO_IIOP_SSL_Connection_Handler>
#pragma instantiate ACE_Scheduling_Strategy<TAO_IIOP_SSL_Connection_Handler>
#pragma instantiate TAO_Creation_Strategy<TAO_IIOP_SSL_Connection_Handler>
#pragma instantiate TAO_Concurrency_Strategy<TAO_IIOP_SSL_Connection_Handler>
#pragma instantiate TAO_Accept_Strategy<TAO_IIOP_SSL_Connection_Handler, ACE_SOCK_ACCEPTOR>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

TAO_IIOP_SSL_Acceptor::TAO_IIOP_SSL_Acceptor (CORBA::Boolean flag)
  : TAO_IIOP_Acceptor (flag),
    base_acceptor_ (),
    creation_strategy_ (0),
    concurrency_strategy_ (0),
    accept_strategy_ (0),
    handler_state_ ()
{
}

TAO_IIOP_SSL_Acceptor::~TAO_IIOP_SSL_Acceptor (void)
{
  // Make sure we are closed before we start destroying the
  // strategies.
  this->close ();

  delete this->creation_strategy_;
  delete this->concurrency_strategy_;
  delete this->accept_strategy_;
}

int
TAO_IIOP_SSL_Acceptor::close (void)
{
  return this->base_acceptor_.close ();
}

int
TAO_IIOP_SSL_Acceptor::open_i (const ACE_INET_Addr& addr,
                               ACE_Reactor *reactor)
{
  if (TAO_SSLIOP_Util::setup_handler_state (this->orb_core_,
                                            &(this->tcp_properties_),
                                            this->handler_state_) != 0)
      return -1;

  ACE_NEW_RETURN (this->creation_strategy_,
                  TAO_IIOP_SSL_CREATION_STRATEGY (this->orb_core_,
                                                  &(this->handler_state_),
                                                  this->lite_flag_),
                  -1);

  ACE_NEW_RETURN (this->concurrency_strategy_,
                  TAO_IIOP_SSL_CONCURRENCY_STRATEGY (this->orb_core_),
                  -1);

  ACE_NEW_RETURN (this->accept_strategy_,
                  TAO_IIOP_SSL_ACCEPT_STRATEGY (this->orb_core_),
                  -1);

  if (this->base_acceptor_.open (addr,
                                 reactor,
                                 this->creation_strategy_,
                                 this->accept_strategy_,
                                 this->concurrency_strategy_) == -1)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("\n\nTAO (%P|%t) IIOP_SSL_Acceptor::open_i ")
                    ACE_TEXT ("- %p\n\n"),
                    ACE_TEXT ("cannot open acceptor")));
      return -1;
    }

  ACE_INET_Addr address;

  // We do this make sure the port number the endpoint is listening on
  // gets set in the addr.
  if (this->base_acceptor_.acceptor ().get_local_addr (address) != 0)
    {
      // @@ Should this be a catastrophic error???
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("\n\nTAO (%P|%t) IIOP_SSL_Acceptor::open_i ")
                    ACE_TEXT ("- %p\n\n"),
                    ACE_TEXT ("cannot get local addr")));
      return -1;
    }

  // Set the port for each addr.  If there is more than one network
  // interface then the endpoint created on each interface will be on
  // the same port.  This is how a wildcard socket bind() is supposed
  // to work.
  u_short port = address.get_port_number ();
  for (size_t j = 0; j < this->endpoint_count_; ++j)
    this->addrs_[j].set_port_number (port, 1);

  if (TAO_debug_level > 5)
    {
      for (size_t i = 0; i < this->endpoint_count_; ++i)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("\nTAO (%P|%t) IIOP_SSL_Acceptor::open_i - ")
                      ACE_TEXT ("listening on: <%s:%u>\n"),
                      this->hosts_[i],
                      this->addrs_[i].get_port_number ()));
        }
    }

  return 0;
}
