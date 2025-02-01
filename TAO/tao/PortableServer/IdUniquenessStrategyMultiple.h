// -*- C++ -*-

//=============================================================================
/**
 *  @file IdUniquenessStrategyMultiple.h
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_ID_UNIQUENESSSTRATEGY_MULITPLE_H
#define TAO_ID_UNIQUENESSSTRATEGY_MULITPLE_H
#include /**/ "ace/pre.h"

#include "tao/PortableServer/IdUniquenessStrategy.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    class IdUniquenessStrategyMultiple : public IdUniquenessStrategy
    {
    public:
      void strategy_init (TAO_Root_POA *poa) override;

      void strategy_cleanup () override;

      bool is_servant_activation_allowed (PortableServer::Servant servant, bool &w) override;

      bool allow_multiple_activations () const override;
    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_ID_UNIQUENESSSTRATEGY_MULITPLE_H */
