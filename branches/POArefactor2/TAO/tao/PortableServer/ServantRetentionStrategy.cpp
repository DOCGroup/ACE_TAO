// -*- C++ -*-

//=============================================================================
/**
 *  @file    ServantRetentionStrategy.cpp
 *
 *  $Id$
 *
 */
//=============================================================================

#include "ServantRetentionStrategy.h"

ACE_RCSID (PortableServer,
           ServantRetentionStrategy,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    ServantRetentionStrategy::ServantRetentionStrategy (void) :
      poa_ (0)
    {
    }

    ServantRetentionStrategy::~ServantRetentionStrategy (void)
    {
    }

    void
    ServantRetentionStrategy::strategy_init (
      TAO_POA *poa)
    {
      poa_ = poa;
    }
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

