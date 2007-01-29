/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    FT_Replica.cpp
 *
 *  $Id$
 *
 *  This file is part of Fault Tolerant CORBA.
 *  Implement the FT_TEST::Replica IDL interface.
 *
 *  @author Dale Wilson <wilson_d@ociweb.com>
 */
//=============================================================================

#include "tao/Utils/Server_Main.h"
#include "FT_ReplicaFactory_i.h"

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  TAO::Utils::Server_Main<FT_ReplicaFactory_i> server_main("TestReplicaFactory");
  return server_main.run(argc, argv);
}
