// -*- C++ -*-

//=============================================================================
/**
 *  @file    Servant_Retention_Strategy.cpp
 *
 *  $Id$
 *
 */
//=============================================================================

#include "ServantRetentionStrategy.h"
#include "Request_Processing_Strategy.h"
#include "tao/PortableServer/Id_Uniqueness_Strategy.h"
#include "Lifespan_Strategy.h"
#include "tao/ORB_Core.h"
#include "Non_Servant_Upcall.h"
#include "Servant_Upcall.h"
#include "POA_Current_Impl.h"
#include "POA.h"
#include "tao/debug.h"
#include "tao/TSS_Resources.h"

// @todo Check the usage of non_retain and request_processing (default servant especially)

ACE_RCSID (PortableServer,
           ServantRetentionStrategy,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    Servant_Retention_Strategy::Servant_Retention_Strategy (void) :
      poa_ (0)
    {
    }

    Servant_Retention_Strategy::~Servant_Retention_Strategy (void)
    {
    }

    void
    Servant_Retention_Strategy::strategy_init (
      TAO_POA *poa)
    {
      poa_ = poa;
    }
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

