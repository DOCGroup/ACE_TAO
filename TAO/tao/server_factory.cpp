// ============================================================================
//
// = LIBRARY
//     TAO
// 
// = FILENAME
//     server_factory.cpp
//
// = AUTHOR
//     Chris Cleeland
//
// = VERSION
//     $Id$
// ============================================================================

#if 0
#if !defined (__ACE_INLINE__)
#  include "server_factory.i"
#endif /* __ACE_INLINE__ */
#endif /* 0 */

#include "tao/corba.h"

TAO_Server_Strategy_Factory::TAO_Server_Strategy_Factory (void)
{}

TAO_Server_Strategy_Factory::~TAO_Server_Strategy_Factory(void)
{}

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

TAO_Object_Table *
TAO_Server_Strategy_Factory::object_lookup_strategy (void)
{
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Creation_Strategy<TAO_OA_Connection_Handler>;
template class ACE_Accept_Strategy<TAO_OA_Connection_Handler, ACE_SOCK_ACCEPTOR>;
template class ACE_Concurrency_Strategy<TAO_OA_Connection_Handler>;
template class ACE_Scheduling_Strategy<TAO_OA_Connection_Handler>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Creation_Strategy<TAO_OA_Connection_Handler>
#pragma instantiate ACE_Accept_Strategy<TAO_OA_Connection_Handler, ACE_SOCK_ACCEPTOR>
#pragma instantiate ACE_Concurrency_Strategy<TAO_OA_Connection_Handler>
#pragma instantiate ACE_Scheduling_Strategy<TAO_OA_Connection_Handler>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
