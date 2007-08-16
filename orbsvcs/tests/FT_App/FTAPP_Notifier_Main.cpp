/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    FTAPP_Notifier_Main.cpp
 *
 *  $Id$
 *
 *  This file is part of Fault Tolerant CORBA.
 *  This file provides the main routine for a stub implementation
 *  of the FaultNotifier interface.
 *
 *  @author Dale Wilson <wilson_d@ociweb.com>
 */
//=============================================================================

#include "StubFaultNotifier.h"
#include "tao/Utils/Server_Main.h"

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  TAO::Utils::Server_Main<StubFaultNotifier> server_main("FaultNotifier");
  return server_main.run(argc, argv);
}
