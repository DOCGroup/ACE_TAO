// -*- C++ -*-

//=============================================================================
/**
 *  @file    RequestProcessingStrategy.cpp
 *
 *  $Id$
 *
 */
//=============================================================================

#include "RequestProcessingStrategy.h"

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
      TAO_Root_POA *poa,
      ::PortableServer::ServantRetentionPolicyValue sr_value
      ACE_ENV_ARG_DECL_NOT_USED)
    {
      poa_ = poa;
      sr_value_ = sr_value;
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

    ::PortableServer::ServantRetentionPolicyValue
    RequestProcessingStrategy::sr_type() const
    {
      return sr_value_;
    }
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

