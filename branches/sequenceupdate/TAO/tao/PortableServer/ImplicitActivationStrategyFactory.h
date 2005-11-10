// -*- C++ -*-

//=============================================================================
/**
 *  @file ImplicitActivationStrategyFactory.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_PORTABLESERVER_IMPLICITACTIVATIONSTRATEGYFACTORY_H
#define TAO_PORTABLESERVER_IMPLICITACTIVATIONSTRATEGYFACTORY_H
#include /**/ "ace/pre.h"

#include "portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "StrategyFactory.h"
#include "ImplicitActivationPolicyC.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    class ImplicitActivationStrategy;

    class TAO_PortableServer_Export ImplicitActivationStrategyFactory
      : public StrategyFactory
    {
    public:
      /// Create a new servant retention strategy
      virtual ImplicitActivationStrategy* create (
        ::PortableServer::ImplicitActivationPolicyValue value) = 0;

      virtual void destroy (
        ImplicitActivationStrategy *strategy
        ACE_ENV_ARG_DECL) = 0;
    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_PORTABLESERVER_IMPLICITACTIVATIONSTRATEGYFACTORY_H */
