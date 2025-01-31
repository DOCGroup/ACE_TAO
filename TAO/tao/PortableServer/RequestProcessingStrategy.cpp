// -*- C++ -*-

//=============================================================================
/**
 *  @file    RequestProcessingStrategy.cpp
 */
//=============================================================================

#include "tao/PortableServer/RequestProcessingStrategy.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    void
    RequestProcessingStrategy::strategy_init(TAO_Root_POA *poa)
    {
      poa_ = poa;
    }

    void
    RequestProcessingStrategy::strategy_cleanup()
    {
      poa_ = nullptr;
    }
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
