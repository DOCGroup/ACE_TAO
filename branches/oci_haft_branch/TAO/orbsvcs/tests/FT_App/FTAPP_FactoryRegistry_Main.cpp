/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    FTAPP_FactoryRegistry_Main.cpp
 *
 *  $Id$
 *
 *  This file is part of Fault Tolerant CORBA.
 *  This file provides the main routine for a stub FactoryRegistry
 *
 *  @author Dale Wilson <wilson_d@ociweb.com>
 */
//=============================================================================

#include <tao/Utils/Server_Main.h>
#include "orbsvcs/PortableGroup/PG_FactoryRegistry.h"

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  TAO::Utils::Server_Main<TAO::PG_FactoryRegistry> server_main("FactoryRegistry");
  return server_main.run(argc, argv);
}

///////////////////////////////////
// Template instantiation for
// inept compilers.

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
  template TAO::Utils::Server_Main<FactoryRegistry_i>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
# pragma instantiate TAO::Utils::Server_Main<FactoryRegistry_i>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

