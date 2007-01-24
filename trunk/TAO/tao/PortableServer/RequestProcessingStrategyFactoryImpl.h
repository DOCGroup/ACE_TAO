// -*- C++ -*-

//=============================================================================
/**
 *  @file RequestProcessingStrategyFactoryImpl.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_PORTABLESERVER_REQUESTPROCESSINGSTRATEGYFACTORYIMPL_H
#define TAO_PORTABLESERVER_REQUESTPROCESSINGSTRATEGYFACTORYIMPL_H
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
    class TAO_PortableServer_Export RequestProcessingStrategyFactoryImpl
      : public RequestProcessingStrategyFactory
    {
    public:
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

ACE_STATIC_SVC_DECLARE_EXPORT (TAO_PortableServer, RequestProcessingStrategyFactoryImpl)
ACE_FACTORY_DECLARE (TAO_PortableServer, RequestProcessingStrategyFactoryImpl)


#include /**/ "ace/post.h"
#endif /* TAO_PORTABLESERVER_REQUESTPROCESSINGSTRATEGYFACTORYIMPL_H */
