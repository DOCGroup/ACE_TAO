// $Id$

#include "tao/default_client.h"
#include "tao/ORB_Core.h"

#if !defined (__ACE_INLINE__)
# include "tao/default_client.i"
#endif /* ! __ACE_INLINE__ */

TAO_Default_Client_Strategy_Factory::TAO_Default_Client_Strategy_Factory (void)
{
}

TAO_Default_Client_Strategy_Factory::~TAO_Default_Client_Strategy_Factory (void)
{
}

int
TAO_Default_Client_Strategy_Factory::init (int argc, char *argv[])
{
  return this->parse_args (argc, argv);
}

int
TAO_Default_Client_Strategy_Factory::parse_args (int /* argc */, char ** /* argv */)
{
  // no args to parse at this time
  return 0;
}

ACE_FACTORY_DEFINE (TAO, TAO_Default_Client_Strategy_Factory)
