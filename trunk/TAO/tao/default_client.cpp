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

#include "tao/default_client.h"
#include "tao/Orb_Core.h"

TAO_Default_Client_Strategy_Factory::TAO_Default_Client_Strategy_Factory (void)
{
}

TAO_Default_Client_Strategy_Factory::~TAO_Default_Client_Strategy_Factory (void)
{}

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

ACE_SVC_FACTORY_DEFINE (TAO_Default_Client_Strategy_Factory)
