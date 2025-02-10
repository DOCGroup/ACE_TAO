// -*- C++ -*-

//=============================================================================
/**
 *  @file IdUniquenessStrategyUnique.h
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_ID_UNIQUENESSSTRATEGYUNIQUE_H
#define TAO_ID_UNIQUENESSSTRATEGYUNIQUE_H
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
    class IdUniquenessStrategyUnique : public IdUniquenessStrategy
    {
    public:
      IdUniquenessStrategyUnique () = default;

      void strategy_init (TAO_Root_POA *poa) override;

      void strategy_cleanup () override;

      bool is_servant_activation_allowed (PortableServer::Servant servant, bool &wait_occurred_restart_call) override;

      bool allow_multiple_activations () const override;

    private:
      TAO_Root_POA* poa_ {};
    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_ID_UNIQUENESSSTRATEGYUNIQUE_H */
