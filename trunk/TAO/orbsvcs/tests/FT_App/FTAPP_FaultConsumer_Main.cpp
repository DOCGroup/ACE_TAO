/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    FTAPP_FaultConsumer_Main.cpp
 *
 *  $Id$
 *
 *  This file is part of Fault Tolerant CORBA.
 *  This file provides the main routine for a stub FaultConsumer.
 *
 *  @author Steve Totten <totten_s@ociweb.com>
 */
//=============================================================================

#include "tao/Utils/Server_Main.h"
#include "ReplicationManagerFaultConsumerAdapter.h"

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  TAO::Utils::Server_Main<ReplicationManagerFaultConsumerAdapter>
    server_main("FaultConsumer");
  return server_main.run(argc, argv);
}

///////////////////////////////////
// Template instantiation for
// inept compilers.

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
  template class TAO::Utils::Server_Main<ReplicationManagerFaultConsumerAdapter>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
# pragma instantiate TAO::Utils::Server_Main<ReplicationManagerFaultConsumerAdapter>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
