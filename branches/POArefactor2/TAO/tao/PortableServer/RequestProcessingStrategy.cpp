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
    RequestProcessingStrategy::~RequestProcessingStrategy()
    {
    }
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

