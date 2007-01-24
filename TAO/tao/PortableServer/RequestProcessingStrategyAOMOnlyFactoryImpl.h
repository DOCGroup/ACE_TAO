// -*- C++ -*-

//=============================================================================
/**
 *  @file RequestProcessingStrategyAOMOnlyFactoryImpl.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_PORTABLESERVER_REQUESTPROCESSINGSTRATEGYAOMONLYFACTORYIMPL_H
#define TAO_PORTABLESERVER_REQUESTPROCESSINGSTRATEGYAOMONLYFACTORYIMPL_H
#include /**/ "ace/pre.h"

#include "tao/PortableServer/portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Config.h"
#include "tao/PortableServer/RequestProcessingStrategyFactory.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    class TAO_PortableServer_Export RequestProcessingStrategyAOMOnlyFactoryImpl
       : public RequestProcessingStrategyFactory
    {
    public:
      /// Create a new servant retention strategy
      virtual RequestProcessingStrategy* create (
        ::PortableServer::RequestProcessingPolicyValue value,
        ::PortableServer::ServantRetentionPolicyValue srvalue);

      virtual void destroy (
        RequestProcessingStrategy *strategy
       );
    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DECLARE_EXPORT (TAO_PortableServer, RequestProcessingStrategyAOMOnlyFactoryImpl)
ACE_FACTORY_DECLARE (TAO_PortableServer, RequestProcessingStrategyAOMOnlyFactoryImpl)


#include /**/ "ace/post.h"
#endif /* TAO_PORTABLESERVER_REQUESTPROCESSINGSTRATEGYAOMONLYFACTORYIMPL_H*/
