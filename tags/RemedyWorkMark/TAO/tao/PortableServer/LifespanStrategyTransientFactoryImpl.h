// -*- C++ -*-

//=============================================================================
/**
 *  @file LifespanStrategyTransientFactoryImpl.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_PORTABLESERVER_LIFEPSPANSTRATEGYTRANSIENTFACTORYIMPL_H
#define TAO_PORTABLESERVER_LIFEPSPANSTRATEGYTRANSIENTFACTORYIMPL_H

#include /**/ "ace/pre.h"

#include "tao/PortableServer/portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Config.h"
#include "tao/PortableServer/LifespanStrategyFactory.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    class TAO_PortableServer_Export LifespanStrategyTransientFactoryImpl
      : public LifespanStrategyFactory
    {
    public:
      /// Create a new strategy
      virtual LifespanStrategy* create (
        ::PortableServer::LifespanPolicyValue value);

      /// Cleanup the given strategy instance
      virtual void destroy (
        LifespanStrategy *strategy
       );
    };
  }
}

ACE_STATIC_SVC_DECLARE_EXPORT (TAO_PortableServer, LifespanStrategyTransientFactoryImpl)
ACE_FACTORY_DECLARE (TAO_PortableServer, LifespanStrategyTransientFactoryImpl)

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_PORTABLESERVER_LIFEPSPANSTRATEGYTRANSIENTFACTORYIMPL_H */
