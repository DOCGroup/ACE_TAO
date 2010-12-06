/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    FTAPP_Analyzer_Main.cpp
 *
 *  $Id$
 *
 *  This file is part of Fault Tolerant CORBA.
 *  This file provides the main routine for a stub FaultAnalyzer
 *
 *  @author Dale Wilson <wilson_d@ociweb.com>
 */
//=============================================================================

#include "tao/Utils/Server_Main.h"
#include "StubFaultAnalyzer.h"

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  TAO::Utils::Server_Main<StubFaultAnalyzer> server_run("FaultAnalyzer");
  return server_run.run(argc, argv);
}
