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

#include <tao/Utils/ServantMain.h>
#include "FactoryRegistry_i.h"

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  TAO::Utils::ServantMain<FactoryRegistry_i> servantMain("FactoryRegistry");
  return servantMain.Main(argc, argv);
}

///////////////////////////////////
// Template instantiation for
// inept compilers.

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
  template TAO::Utils::ServantMain<FactoryRegistry_i>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
# pragma instantiate TAO::Utils::ServantMain<FactoryRegistry_i>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

