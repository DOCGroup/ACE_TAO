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

#include <tao/Utils/ServantMain.h>
#include "FT_Notifier_i.h"

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  TAO::Utils::ServantMain<FT_FaultNotifier_i> servantMain("FaultNotifier");
  return servantMain.Main(argc, argv);
}

///////////////////////////////////
// Template instantiation for
// inept compilers.

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
  template TAO::Utils::ServantMain<FT_FaultNotifier_i>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
# pragma instantiate TAO::Utils::ServantMain<FT_FaultNotifier_i>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */



