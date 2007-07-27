// -*- C++ -*-

//=============================================================================
/**
 *  @file IdUniquenessStrategyMultiple.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_ID_UNIQUENESSSTRATEGY_MULITPLE_H
#define TAO_ID_UNIQUENESSSTRATEGY_MULITPLE_H
#include /**/ "ace/pre.h"

#include "tao/PortableServer/portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/IdUniquenessStrategy.h"
#include "ace/Service_Config.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    class TAO_PortableServer_Export IdUniquenessStrategyMultiple
      : public IdUniquenessStrategy
    {
    public:
      virtual void strategy_init (TAO_Root_POA *poa);

      virtual void strategy_cleanup (void);

      virtual bool is_servant_activation_allowed (
        PortableServer::Servant servant,
        bool &w);

      virtual bool allow_multiple_activations (void) const;

      virtual ::PortableServer::IdUniquenessPolicyValue type() const;
    };

  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DECLARE_EXPORT (TAO_PortableServer, IdUniquenessStrategyMultiple)
ACE_FACTORY_DECLARE (TAO_PortableServer, IdUniquenessStrategyMultiple)


#include /**/ "ace/post.h"
#endif /* TAO_ID_UNIQUENESSSTRATEGY_MULITPLE_H */
