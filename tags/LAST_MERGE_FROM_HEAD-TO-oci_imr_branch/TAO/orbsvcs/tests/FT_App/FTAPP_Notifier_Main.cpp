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

///////////////////////////////////
// Template instantiation for
// inept compilers.

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
  template class TAO::Utils::Server_Main<StubFaultNotifier>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
# pragma instantiate TAO::Utils::Server_Main<StubFaultNotifier>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

