// -*- C++ -*-

//=============================================================================
/**
 *  @file IdAssignment_Policy_Value.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_IDASSIGNMENT_POLICY_VALUE_H
#define TAO_IDASSIGNMENT_POLICY_VALUE_H
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
  class TAO_PortableServer_Export IdAssignment_Policy_Value
    : public virtual ACE_Service_Object
  {
    public:
      virtual ~IdAssignment_Policy_Value (void);

      virtual PortableServer::IdAssignmentPolicyValue policy_type (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException)) = 0;
  };

  class TAO_PortableServer_Export User_IdAssignment_Policy
    : public virtual IdAssignment_Policy_Value
  {
    public:
      virtual ~User_IdAssignment_Policy (void);

      virtual PortableServer::IdAssignmentPolicyValue policy_type (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));
  };

  class TAO_PortableServer_Export System_IdAssignment_Policy
    : public virtual IdAssignment_Policy_Value
  {
    public:
      virtual ~System_IdAssignment_Policy  (void);

      virtual PortableServer::IdAssignmentPolicyValue policy_type (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));
  };

  ACE_STATIC_SVC_DECLARE (System_IdAssignment_Policy)
  ACE_FACTORY_DECLARE (TAO_PortableServer, System_IdAssignment_Policy)

  ACE_STATIC_SVC_DECLARE (User_IdAssignment_Policy)
  ACE_FACTORY_DECLARE (TAO_PortableServer, User_IdAssignment_Policy)
}

#include /**/ "ace/post.h"
#endif /* TAO_IDASSIGNMENT_POLICY_VALUE_H */

