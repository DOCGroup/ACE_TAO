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

#include "tao/Utils/Server_Main.h"
#include "orbsvcs/PortableGroup/PG_FactoryRegistry.h"

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  TAO::Utils::Server_Main<TAO::PG_FactoryRegistry> server_main("FactoryRegistry");
  return server_main.run(argc, argv);
}
