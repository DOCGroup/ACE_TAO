// -*- C++ -*-

//=============================================================================
/**
 *  @file IdAssignmentPolicyFactoryImpl.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_PORTABLESERVER_IDASSIGNMENTPOLICYFACTORYIMPL_H
#define TAO_PORTABLESERVER_IDASSIGNMENTPOLICYFACTORYIMPL_H
#include /**/ "ace/pre.h"

#include "tao/PortableServer/portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Config.h"
#include "tao/PortableServer/IdAssignmentPolicyFactory.h"

namespace TAO
{
  namespace Portable_Server
  {
    class TAO_PortableServer_Export IdAssignmentPolicyFactoryImpl
      : public virtual IdAssignmentPolicyFactory
    {
    public:
      virtual ~IdAssignmentPolicyFactoryImpl (void);

      /// Create a new id uniqueness policy
      /**
       * @note If all the compilers supported covariant return types we could
       * change this to a CORBA::Policy_ptr create() call, which is defined
       * as pure virtual in the base. This is something for the future.
       */
      virtual ::PortableServer::IdAssignmentPolicy_ptr create (
        ::PortableServer::IdAssignmentPolicyValue value ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));

      /// Create a new id uniqueness policy
      virtual ::PortableServer::IdAssignmentPolicy_ptr create (
        const CORBA::Any &value ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         CORBA::PolicyError));
    };

    ACE_STATIC_SVC_DECLARE_EXPORT (TAO_PortableServer, IdAssignmentPolicyFactoryImpl)
    ACE_FACTORY_DECLARE (TAO_PortableServer, IdAssignmentPolicyFactoryImpl)
  }
}

#include /**/ "ace/post.h"
#endif /* TAO_PORTABLESERVER_IDASSIGNMENTPOLICYFACTORYIMPL_H */
