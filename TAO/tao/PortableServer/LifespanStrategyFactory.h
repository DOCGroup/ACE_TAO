// -*- C++ -*-

//=============================================================================
/**
 *  @file LifespanStrategyFactory.h
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_PORTABLESERVER_LIFEPSPANSTRATEGYFACTORY_H
#define TAO_PORTABLESERVER_LIFEPSPANSTRATEGYFACTORY_H
#include /**/ "ace/pre.h"

#include "tao/PortableServer/portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/StrategyFactory.h"
#include "tao/PortableServer/LifespanPolicyC.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    class LifespanStrategy;

    class TAO_PortableServer_Export LifespanStrategyFactory
      : public StrategyFactory
    {
    public:
      /// Create a new servant retention strategy
      virtual LifespanStrategy* create (::PortableServer::LifespanPolicyValue value) = 0;

      /// Cleanup the given strategy instance
      virtual void destroy (LifespanStrategy *strategy) = 0;
    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_PORTABLESERVER_LIFEPSPANSTRATEGYFACTORY_H */
