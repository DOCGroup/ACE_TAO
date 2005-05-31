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

namespace TAO
{
  namespace Portable_Server
  {
    RequestProcessingStrategy::RequestProcessingStrategy()
      : poa_ (0)
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

