/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    FaultNotifierMain.cpp
 *
 *  $Id$
 *
 *  This file is part of Fault Tolerant CORBA.
 *  This file provides the main routine for a process that
 *  implements the FaultNotifier interface and manages
 *  a set of FaultNotifiers.
 *
 *  @author Dale Wilson <wilson_d@ociweb.com>
 */
//=============================================================================

#include <tao/Utils/Server_Main.h>
#include "FT_Notifier_i.h"

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  TAO::Utils::Server_Main<TAO::FT_FaultNotifier_i> server_main("TAO_FaultNotifier");
  return server_main.run(argc, argv);
}

///////////////////////////////////
// Template instantiation for
// inept compilers.

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
  template TAO::Utils::Server_Main<FT_FaultNotifier_i>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
# pragma instantiate TAO::Utils::Server_Main<FT_FaultNotifier_i>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */



