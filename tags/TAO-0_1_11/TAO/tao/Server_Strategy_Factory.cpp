// $Id$

#include "tao/corba.h"

TAO_Server_Strategy_Factory::TAO_Server_Strategy_Factory (void)
{
}

TAO_Server_Strategy_Factory::~TAO_Server_Strategy_Factory(void)
{
}

int
TAO_Server_Strategy_Factory::open (void)
{
  return 0;
}

TAO_Server_Strategy_Factory::CREATION_STRATEGY *
TAO_Server_Strategy_Factory::creation_strategy (void)
{
  return 0;
}

TAO_Server_Strategy_Factory::ACCEPT_STRATEGY *
TAO_Server_Strategy_Factory::accept_strategy (void)
{
  return 0;
}

TAO_Server_Strategy_Factory::CONCURRENCY_STRATEGY *
TAO_Server_Strategy_Factory::concurrency_strategy (void)
{
  return 0;
}

TAO_Server_Strategy_Factory::SCHEDULING_STRATEGY *
TAO_Server_Strategy_Factory::scheduling_strategy (void)
{
  return 0;
}

TAO_Object_Table_Impl *
TAO_Server_Strategy_Factory::create_object_table (void)
{
  return 0;
}

ACE_Lock *
TAO_Server_Strategy_Factory::create_poa_lock (void)
{
  return 0;
}

ACE_Lock *
TAO_Server_Strategy_Factory::create_poa_mgr_lock (void)
{
  return 0;
}

ACE_Lock *
TAO_Server_Strategy_Factory::create_servant_lock (void)
{
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Creation_Strategy<TAO_Server_Connection_Handler>;
template class ACE_Acceptor<TAO_Server_Connection_Handler, ACE_SOCK_ACCEPTOR>;
template class ACE_Accept_Strategy<TAO_Server_Connection_Handler, ACE_SOCK_ACCEPTOR>;
template class ACE_Concurrency_Strategy<TAO_Server_Connection_Handler>;
template class ACE_Scheduling_Strategy<TAO_Server_Connection_Handler>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Creation_Strategy<TAO_Server_Connection_Handler>
#pragma instantiate ACE_Acceptor<TAO_Server_Connection_Handler, ACE_SOCK_ACCEPTOR>
#pragma instantiate ACE_Accept_Strategy<TAO_Server_Connection_Handler, ACE_SOCK_ACCEPTOR>
#pragma instantiate ACE_Concurrency_Strategy<TAO_Server_Connection_Handler>
#pragma instantiate ACE_Scheduling_Strategy<TAO_Server_Connection_Handler>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
