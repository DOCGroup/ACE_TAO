// $Id$

#include "Event_Channel.h"
#include "Proxy_Handler_Acceptor.h"

Proxy_Handler_Acceptor::Proxy_Handler_Acceptor (ACE_Event_Channel &ec)
  : event_channel_ (ec)
{
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Acceptor<Proxy_Handler_Factory, ACE_SOCK_ACCEPTOR>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Acceptor<Proxy_Handler_Factory, ACE_SOCK_ACCEPTOR>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

