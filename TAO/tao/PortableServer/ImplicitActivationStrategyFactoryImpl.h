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

#include "portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Config.h"
#include "ImplicitActivationStrategyFactory.h"

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
        ACE_ENV_ARG_DECL);
    };

    ACE_STATIC_SVC_DECLARE_EXPORT (TAO_PortableServer, ImplicitActivationStrategyFactoryImpl)
    ACE_FACTORY_DECLARE (TAO_PortableServer, ImplicitActivationStrategyFactoryImpl)
  }
}

#include /**/ "ace/post.h"
#endif /* TAO_PORTABLESERVER_IMPLICITACTIVATIONSTRATEGYFACTORYIMPL_H */
