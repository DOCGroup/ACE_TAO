/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    FT_ReplicationManager_Main.cpp
 *
 *  $Id$
 *
 *  This file is part of Fault Tolerant CORBA.
 *  This file provides the main routine for a process that
 *  implements the FT_ReplicationManager interface.
 *
 *  @author Curt Hibbs <hibbs_c@ociweb.com>
 */
//=============================================================================

#include "FT_ReplicationManager.h"
#include <tao/Utils/Server_Main.h>

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  TAO::Utils::Server_Main<TAO::FT_ReplicationManager>
    server_main("ReplicationManager");
  return server_main.run(argc, argv);
}

///////////////////////////////////
// Template instantiation for
// inept compilers.

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
  template TAO::Utils::Server_Main<TAO::FT_ReplicationManager>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
# pragma instantiate TAO::Utils::Server_Main<TAO::FT_ReplicationManager>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

