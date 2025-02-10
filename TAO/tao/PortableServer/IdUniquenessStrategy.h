// -*- C++ -*-

//=============================================================================
/**
 *  @file IdUniquenessStrategy.h
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_ID_UNIQUENESS_STRATEGY_H
#define TAO_ID_UNIQUENESS_STRATEGY_H
#include /**/ "ace/pre.h"

#include "tao/PortableServer/IdUniquenessPolicyC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/PS_ForwardC.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    class IdUniquenessStrategy
    {
    public:
      IdUniquenessStrategy ();
      virtual ~IdUniquenessStrategy () = default;

      virtual void strategy_init (TAO_Root_POA *poa) = 0;

      virtual void strategy_cleanup () = 0;

      /*
       * Validate if the servant may be activated
       * @retval true This servant may be activated
       * @retval false This servant may not be activated
       */
      virtual bool is_servant_activation_allowed (PortableServer::Servant s, bool &w) = 0;

      virtual bool allow_multiple_activations () const = 0;
    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_ID_UNIQUENESS_STRATEGY_H */
