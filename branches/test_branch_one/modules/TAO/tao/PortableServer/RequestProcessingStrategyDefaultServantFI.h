// -*- C++ -*-

//=============================================================================
/**
 *  @file RequestProcessingStrategyDefaultServantFI.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_PORTABLESERVER_REQUESTPROCESSINGSTRATEGYDEFAULTSERVANTFACTORYIMPL_H
#define TAO_PORTABLESERVER_REQUESTPROCESSINGSTRATEGYDEFAULTSERVANTFACTORYIMPL_H
#include /**/ "ace/pre.h"

#include "tao/PortableServer/portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Config.h"
#include "tao/PortableServer/RequestProcessingStrategyFactory.h"

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    class TAO_PortableServer_Export RequestProcessingStrategyDefaultServantFactoryImpl
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

ACE_STATIC_SVC_DECLARE_EXPORT (TAO_PortableServer, RequestProcessingStrategyDefaultServantFactoryImpl)
ACE_FACTORY_DECLARE (TAO_PortableServer, RequestProcessingStrategyDefaultServantFactoryImpl)

#endif /* TAO_HAS_MINIMUM_POA == 0 */

#include /**/ "ace/post.h"
#endif /* TAO_PORTABLESERVER_REQUESTPROCESSINGSTRATEGYDEFAULTSERVANTFACTORYIMPL_H*/
