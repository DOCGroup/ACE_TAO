// $Id$

#include "Event_Channel.h"
#include "Proxy_Handler_Acceptor.h"

Proxy_Handler_Acceptor::Proxy_Handler_Acceptor (ACE_Event_Channel &ec)
  : event_channel_ (ec)
{
}

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class ACE_Acceptor<Proxy_Handler_Factory, ACE_SOCK_ACCEPTOR>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */
