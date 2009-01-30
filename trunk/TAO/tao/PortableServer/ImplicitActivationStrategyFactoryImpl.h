// -*- C++ -*-

//=============================================================================
/**
 *  @file ImplicitActivationStrategyFactoryImpl.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_PORTABLESERVER_IMPLICITACTIVATIONSTRATEGYFACTORYIMPL_H
#define TAO_PORTABLESERVER_IMPLICITACTIVATIONSTRATEGYFACTORYIMPL_H
#include /**/ "ace/pre.h"

#include "tao/PortableServer/portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Config.h"
#include "tao/PortableServer/ImplicitActivationStrategyFactory.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    class ImplicitActivationStrategyFactoryImpl
      : public ImplicitActivationStrategyFactory
    {
    public:
      /// Create a new servant retention strategy
      virtual ImplicitActivationStrategy* create (
        ::PortableServer::ImplicitActivationPolicyValue value);

      virtual void destroy (
        ImplicitActivationStrategy *strategy
       );
    };
  }
}

ACE_STATIC_SVC_DECLARE_EXPORT (TAO_PortableServer, ImplicitActivationStrategyFactoryImpl)
ACE_FACTORY_DECLARE (TAO_PortableServer, ImplicitActivationStrategyFactoryImpl)

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_PORTABLESERVER_IMPLICITACTIVATIONSTRATEGYFACTORYIMPL_H */
