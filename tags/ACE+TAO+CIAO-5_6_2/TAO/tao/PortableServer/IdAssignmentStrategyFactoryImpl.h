// -*- C++ -*-

//=============================================================================
/**
 *  @file IdAssignmentStrategyFactoryImpl.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_PORTABLESERVER_IDASSIGNMENTSTRATEGYFACTORYIMPL_H
#define TAO_PORTABLESERVER_IDASSIGNMENTSTRATEGYFACTORYIMPL_H
#include /**/ "ace/pre.h"

#include "tao/PortableServer/portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/IdAssignmentStrategyFactory.h"
#include "ace/Service_Config.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    class TAO_PortableServer_Export IdAssignmentStrategyFactoryImpl
      : public IdAssignmentStrategyFactory
    {
    public:
      /// Create a new servant retention strategy
      virtual IdAssignmentStrategy* create (
        ::PortableServer::IdAssignmentPolicyValue value);

      virtual void destroy (IdAssignmentStrategy *strategy);
    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DECLARE_EXPORT (TAO_PortableServer, IdAssignmentStrategyFactoryImpl)
ACE_FACTORY_DECLARE (TAO_PortableServer, IdAssignmentStrategyFactoryImpl)

#include /**/ "ace/post.h"

#endif /* TAO_PORTABLESERVER_IDASSIGNMENTSTRATEGYFACTORYIMPL_H */
