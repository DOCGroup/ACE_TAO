// -*- C++ -*-

//=============================================================================
/**
 *  @file    RequestProcessingStrategy.cpp
 *
 *  $Id$
 *
 */
//=============================================================================

#include "tao/PortableServer/RequestProcessingStrategy.h"

ACE_RCSID (PortableServer,
           RequestProcessingStrategy,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    RequestProcessingStrategy::RequestProcessingStrategy()
      : poa_ (0)
    {
    }

    RequestProcessingStrategy::~RequestProcessingStrategy()
    {
    }

    void
    RequestProcessingStrategy::strategy_init(
      TAO_Root_POA *poa
      ACE_ENV_ARG_DECL_NOT_USED)
    {
      poa_ = poa;
    }

    void
    RequestProcessingStrategy::strategy_cleanup(ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    {
      poa_ = 0;
    }
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

