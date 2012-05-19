// -*- C++ -*-

//=============================================================================
/**
 *  @file ThreadStrategyFactory.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_PORTABLESERVER_THREADPOLICYSTRATEGYFACTORY_H
#define TAO_PORTABLESERVER_THREADPOLICYSTRATEGYFACTORY_H

#include /**/ "ace/pre.h"

#include "tao/PortableServer/StrategyFactory.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/ThreadPolicyC.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    class ThreadStrategy;

    class ThreadStrategyFactory
       : public StrategyFactory
    {
    public:
      /// Create a new servant retention strategy
      virtual ThreadStrategy *create (
        ::PortableServer::ThreadPolicyValue value) = 0;

      /// Cleanup the given strategy instance
      virtual void destroy (
        ThreadStrategy *strategy
        ) = 0;
    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_PORTABLESERVER_THREADPOLICYSTRATEGYFACTORY_H */
