// ============================================================================
//
// = LIBRARY
//    TAO
// 
// = FILENAME
//     default_client.cpp
//
// = AUTHOR
//     Chris Cleeland
//
// = VERSION
//     $Id$
// ============================================================================

#define ACE_BUILD_SVC_DLL
#if 0
#include "tao/default_client.h"
#endif /* 0 */

#include "tao/corba.h"

TAO_Default_Client_Strategy_Factory::TAO_Default_Client_Strategy_Factory (void)
{
  // When should I do this open ()?  It seems like this is way too
  // early, but doing it in the accessor for connector () seems like
  // it would be too late as well.
  // @@ Chris, a couple of thoughts:
  // 1. What is wrong with doing it here, in general?
  // 2. We should make sure not to use ACE_Reactor::instance() since
  //    it makes our ORB too tightly coupled to having just 1 reactor!
  //    I think it's clear now that we'll have one Reactor "per-ORB"
  //    and we may have multiple ORBs per process (e.g., consider the 
  //    "real-time rate-based ORB" we discussed the other day).
  connector_.open (ACE_Reactor::instance (),
		   &null_creation_strategy_,
		   &caching_connect_strategy_,
#if defined (TAO_HAS_CLIENT_CONCURRENCY)
		   concurrency_strategy_ ()
#else
		   0
#endif /* TAO_HAS_CLIENT_CONCURRENCY */
		   );
}

TAO_Default_Client_Strategy_Factory::~TAO_Default_Client_Strategy_Factory (void)
{}

int
TAO_Default_Client_Strategy_Factory::init (int argc, char *argv[])
{
  return this->parse_args (argc, argv);
}

TAO_Client_Strategy_Factory::CONNECTOR *
TAO_Default_Client_Strategy_Factory::connector (void)
{
  return &this->connector_;
}

int
TAO_Default_Client_Strategy_Factory::parse_args (int /* argc */, char **/*argv*/)
{
  // no args to parse at this time
  return 0;
}

#define TAO_HASH_ADDR ACE_Hash_Addr<ACE_INET_Addr, TAO_Client_Connection_Handler>
#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Cached_Connect_Strategy<TAO_Client_Connection_Handler, ACE_SOCK_CONNECTOR, ACE_SYNCH_RW_MUTEX>;
template class ACE_Creation_Strategy<TAO_Client_Connection_Handler>;
template class ACE_Connect_Strategy<TAO_Client_Connection_Handler, ACE_SOCK_CONNECTOR>;
//template class TAO_HASH_ADDR;
template class ACE_Hash_Map_Entry<TAO_HASH_ADDR, TAO_Client_Connection_Handler *>;
template class ACE_Hash_Map_Manager<TAO_HASH_ADDR, TAO_Client_Connection_Handler *, ACE_SYNCH_RW_MUTEX>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Cached_Connect_Strategy<TAO_Client_Connection_Handler, ACE_SOCK_CONNECTOR, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Creation_Strategy<TAO_Client_Connection_Handler>
#pragma instantiate ACE_Connect_Strategy<TAO_Client_Connection_Handler, ACE_SOCK_CONNECTOR>
//#pragma instantiate TAO_HASH_ADDR
#pragma instantiate ACE_Hash_Map_Entry<TAO_HASH_ADDR, TAO_Client_Connection_Handler *>
#pragma instantiate ACE_Hash_Map_Manager<TAO_HASH_ADDR, TAO_Client_Connection_Handler *, ACE_SYNCH_RW_MUTEX>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

ACE_SVC_FACTORY_DEFINE (TAO_Default_Client_Strategy_Factory)
