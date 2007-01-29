// -*- C++ -*-

//=============================================================================
/**
 *  @file IdUniquenessStrategy.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_ID_UNIQUENESS_STRATEGY_H
#define TAO_ID_UNIQUENESS_STRATEGY_H
#include /**/ "ace/pre.h"

#include "tao/PortableServer/portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/Policy_Strategy.h"
#include "tao/PortableServer/IdUniquenessPolicyC.h"
#include "tao/PortableServer/PS_ForwardC.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    class TAO_PortableServer_Export IdUniquenessStrategy
      : public Policy_Strategy
    {
    public:
      /*
       * Validate if the servant may be activated
       * @retval true This servant may be activated
       * @retval false This servant may not be activated
       */
      virtual bool is_servant_activation_allowed (
        PortableServer::Servant s,
        bool &w) = 0;

      virtual bool allow_multiple_activations (void) const = 0;

      virtual ::PortableServer::IdUniquenessPolicyValue type() const = 0;
    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_ID_UNIQUENESS_STRATEGY_H */
