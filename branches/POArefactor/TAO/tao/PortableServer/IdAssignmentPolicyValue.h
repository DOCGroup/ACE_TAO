// -*- C++ -*-

//=============================================================================
/**
 *  @file IdAssignmentPolicyValue.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_PORTABLESERVER_IDASSIGNMENTPOLICYVALUE_H
#define TAO_PORTABLESERVER_IDASSIGNMENTPOLICYVALUE_H
#include /**/ "ace/pre.h"

#include "portableserver_export.h"
#include "IdAssignmentPolicyC.h"
#include "ace/Service_Object.h"
#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace TAO
{
  namespace Portable_Server
  {
    // @@ Johnny, I would think pushing the concrete types to
    // different files  would be better. Any reason that you feel
    // against it?
    class TAO_PortableServer_Export IdAssignmentPolicyValue
      : public virtual ACE_Service_Object
    {
      public:
        virtual ~IdAssignmentPolicyValue (void);

        virtual ::PortableServer::IdAssignmentPolicyValue policy_type (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
          ACE_THROW_SPEC ((CORBA::SystemException)) = 0;
    };

    class TAO_PortableServer_Export User_IdAssignment_Policy
      : public virtual IdAssignmentPolicyValue
    {
      public:
        virtual ~User_IdAssignment_Policy (void);

        virtual ::PortableServer::IdAssignmentPolicyValue policy_type (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
          ACE_THROW_SPEC ((CORBA::SystemException));
    };

    class TAO_PortableServer_Export System_IdAssignment_Policy
      : public virtual IdAssignmentPolicyValue
    {
      public:
        virtual ~System_IdAssignment_Policy  (void);

        virtual ::PortableServer::IdAssignmentPolicyValue policy_type (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
          ACE_THROW_SPEC ((CORBA::SystemException));
    };

    ACE_STATIC_SVC_DECLARE (System_IdAssignment_Policy)
    ACE_FACTORY_DECLARE (TAO_PortableServer, System_IdAssignment_Policy)

    ACE_STATIC_SVC_DECLARE (User_IdAssignment_Policy)
    ACE_FACTORY_DECLARE (TAO_PortableServer, User_IdAssignment_Policy)
  } /* namespace Portable_Server */
} /* namespace TAO */

#include /**/ "ace/post.h"
#endif /* TAO_PORTABLESERVER_IDASSIGNMENTPOLICYVALUE_H */
