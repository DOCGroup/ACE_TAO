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
#include "default_client.h"

TAO_Default_Client_Strategy_Factory::TAO_Default_Client_Strategy_Factory (void)
{
  // When should I do this open ()?  It seems like this is way too
  // early, but doing it in the accessor for connector () seems like
  // it would be too late as well.
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

TAO_Client_Strategy_Factory::CONNECTOR*
TAO_Default_Client_Strategy_Factory::connector (void)
{
  return &connector_;
}

int
TAO_Default_Client_Strategy_Factory::parse_args (int argc, char *argv[])
{
  // no args to parse at this time
  return 0;
}

#if ! defined(__ACE_INLINE__)
#  include "default_client.i"
#endif

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Cached_Connect_Strategy<TAO_Client_Connection_Handler, ACE_SOCK_CONNECTOR, ACE_SYNCH_RW_MUTEX>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Cached_Connect_Strategy<TAO_Client_Connection_Handler, ACE_SOCK_CONNECTOR, ACE_SYNCH_RW_MUTEX>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

ACE_SVC_FACTORY_DEFINE (TAO_Default_Client_Strategy_Factory)
