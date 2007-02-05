// -*- C++ -*-

//=============================================================================
/**
 *  @file ThreadStrategyFactoryImpl.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_PORTABLESERVER_THREADPOLICYSTRATEGYFACTORYIMPL_H
#define TAO_PORTABLESERVER_THREADPOLICYSTRATEGYFACTORYIMPL_H
#include /**/ "ace/pre.h"

#include "tao/PortableServer/portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Config.h"
#include "tao/PortableServer/ThreadStrategyFactory.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    class TAO_PortableServer_Export ThreadStrategyFactoryImpl
       : public ThreadStrategyFactory
    {
    public:
      /// Create a new servant retention strategy
      virtual ThreadStrategy* create (
        ::PortableServer::ThreadPolicyValue value);

      virtual void destroy (
        ThreadStrategy *strategy
       );
    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DECLARE_EXPORT (TAO_PortableServer, ThreadStrategyFactoryImpl)
ACE_FACTORY_DECLARE (TAO_PortableServer, ThreadStrategyFactoryImpl)

#include /**/ "ace/post.h"
#endif /* TAO_PORTABLESERVER_THREADPOLICYSTRATEGYFACTORYIMPL_H */
