// -*- C++ -*-

//=============================================================================
/**
 *  @file ServantRetentionStrategyNonRetainFactoryImpl.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_PORTABLESERVER_SERVANTRETENTIONSTRATEGYNONRETAIN_FACTORYIMPL_H
#define TAO_PORTABLESERVER_SERVANTRETENTIONSTRATEGYNONRETAIN_FACTORYIMPL_H
#include /**/ "ace/pre.h"

#include "tao/PortableServer/portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Config.h"
#include "tao/PortableServer/ServantRetentionStrategyFactory.h"

#if (TAO_HAS_MINIMUM_POA == 0)

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    class TAO_PortableServer_Export ServantRetentionStrategyNonRetainFactoryImpl
       : public ServantRetentionStrategyFactory
    {
    public:
      /// Create a new servant retention strategy
      virtual ServantRetentionStrategy* create (
        ::PortableServer::ServantRetentionPolicyValue value);

      virtual void destroy (
        ServantRetentionStrategy *strategy
        ACE_ENV_ARG_DECL);
    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DECLARE_EXPORT (TAO_PortableServer, ServantRetentionStrategyNonRetainFactoryImpl)
ACE_FACTORY_DECLARE (TAO_PortableServer, ServantRetentionStrategyNonRetainFactoryImpl)

#endif /* TAO_HAS_MINIMUM_POA == 0 */

#include /**/ "ace/post.h"
#endif /* TAO_PORTABLESERVER_SERVANTRETENTIONSTRATEGYNONRETAIN_FACTORYIMPL_H*/
