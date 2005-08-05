// -*- C++ -*-

//=============================================================================
/**
 *  @file ServantRetentionStrategyFactory.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_PORTABLESERVER_SERVANTRETENTIONSTRATEGYFACTORY_H
#define TAO_PORTABLESERVER_SERVANTRETENTIONSTRATEGYFACTORY_H
#include /**/ "ace/pre.h"

#include "portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/StrategyFactory.h"
#include "tao/PortableServer/ServantRetentionPolicyC.h"

namespace TAO
{
  namespace Portable_Server
  {
    class ServantRetentionStrategy;

    class TAO_PortableServer_Export ServantRetentionStrategyFactory
       : public StrategyFactory
    {
    public:
      /// Create a new servant retention strategy
      virtual ServantRetentionStrategy* create (
        ::PortableServer::ServantRetentionPolicyValue value) = 0;

      virtual void destroy (
        ServantRetentionStrategy *strategy
        ACE_ENV_ARG_DECL) = 0;
    };
  }
}

#include /**/ "ace/post.h"
#endif /* TAO_PORTABLESERVER_SERVANTRETENTIONSTRATEGYFACTORY_H */
