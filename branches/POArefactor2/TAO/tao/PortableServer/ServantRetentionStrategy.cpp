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
    ServantRetentionStrategy::~ServantRetentionStrategy (void)
    {
    }
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

