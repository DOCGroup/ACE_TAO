// -*- C++ -*-

//=============================================================================
/**
 *  @file    RequestProcessingStrategy.cpp
 *
 *  $Id$
 */
//=============================================================================

#include "tao/PortableServer/RequestProcessingStrategy.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    RequestProcessingStrategy::RequestProcessingStrategy (void)
      : poa_ (0)
    {
    }

    void
    RequestProcessingStrategy::strategy_init(
      TAO_Root_POA *poa,
      ::PortableServer::ServantRetentionPolicyValue sr_value)
    {
      poa_ = poa;
      sr_value_ = sr_value;
    }

    void
    RequestProcessingStrategy::strategy_init(TAO_Root_POA *poa)
    {
      poa_ = poa;
    }

    void
    RequestProcessingStrategy::strategy_cleanup(void)
    {
      poa_ = 0;
    }

    ::PortableServer::ServantRetentionPolicyValue
    RequestProcessingStrategy::sr_type() const
    {
      return sr_value_;
    }
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
