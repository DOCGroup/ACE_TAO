// -*- C++ -*-

//=============================================================================
/**
 *  @file IdAssignmentStrategyFactory.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_PORTABLESERVER_IDASSIGNMENTSTRATEGYFACTORY_H
#define TAO_PORTABLESERVER_IDASSIGNMENTSTRATEGYFACTORY_H
#include /**/ "ace/pre.h"

#include "portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "StrategyFactory.h"
#include "IdAssignmentPolicyC.h"

namespace TAO
{
  namespace Portable_Server
  {
    class IdAssignmentStrategy;

    class TAO_PortableServer_Export IdAssignmentStrategyFactory
      : public StrategyFactory
    {
    public:
      /// Create a new servant retention strategy
      virtual IdAssignmentStrategy* create (
        ::PortableServer::IdAssignmentPolicyValue value) = 0;

      virtual void destroy (
        IdAssignmentStrategy *strategy
        ACE_ENV_ARG_DECL) = 0;
    };
  }
}

#include /**/ "ace/post.h"
#endif /* TAO_PORTABLESERVER_IDASSIGNMENTSTRATEGYFACTORY_H */
