// -*- C++ -*-

//=============================================================================
/**
 *  @file LifespanStrategyFactory.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_PORTABLESERVER_LIFEPSPANSTRATEGYFACTORY_H
#define TAO_PORTABLESERVER_LIFEPSPANSTRATEGYFACTORY_H
#include /**/ "ace/pre.h"

#include "portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "StrategyFactory.h"
#include "LifespanPolicyC.h"

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
      virtual LifespanStrategy* create (
        ::PortableServer::LifespanPolicyValue value) = 0;

      /// Cleanup the given strategy instance
      virtual void destroy (
        LifespanStrategy *strategy
        ACE_ENV_ARG_DECL) = 0;
    };
  }
}

#include /**/ "ace/post.h"
#endif /* TAO_PORTABLESERVER_LIFEPSPANSTRATEGYFACTORY_H */
