// -*- C++ -*-

//=============================================================================
/**
 *  @file IdAssignmentPolicyFactory.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_LOADABLE_ID_ASSIGNMENT_POLICY_H
#define TAO_LOADABLE_ID_ASSIGNMENT_POLICY_H
#include /**/ "ace/pre.h"

#include "tao/PortableServer/portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/PolicyFactory.h"
#include "tao/PortableServer/PortableServerC.h"
#include "ace/Service_Config.h"
#include "tao/PolicyC.h"

namespace CORBA
{
  class PolicyError;
}

namespace TAO
{
  namespace Portable_Server
  {
    class TAO_PortableServer_Export IdAssignmentPolicyFactory :
       public virtual PolicyFactory
    {
    public:
      virtual ~IdAssignmentPolicyFactory (void);

      /// Create a new id uniqueness policy
      /**
       * @note If all the compilers supported covariant return types we could
       * change this to a CORBA::Policy_ptr create() call, which is defined
       * as pure virtual in the base. This is something for the future.
       */
      ::PortableServer::IdAssignmentPolicy_ptr create (
        ::PortableServer::IdAssignmentPolicyValue value);

      /// Create a new id uniqueness policy
      ::PortableServer::IdAssignmentPolicy_ptr create (
        const CORBA::Any &value ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::PolicyError));
    };

    ACE_STATIC_SVC_DECLARE_EXPORT (TAO_PortableServer, IdAssignmentPolicyFactory)
    ACE_FACTORY_DECLARE (TAO_PortableServer, IdAssignmentPolicyFactory)
  }
}

#include /**/ "ace/post.h"
#endif /* TAO_LOADABLE_ID_ASSIGNMENT_POLICY_H */
