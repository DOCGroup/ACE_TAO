// -*- C++ -*-

//=============================================================================
/**
 *  @file IdAssignmentPolicyValueSystem.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_PORTABLESERVER_IDASSIGNMENTPOLICYVALUESYSTEM_H
#define TAO_PORTABLESERVER_IDASSIGNMENTPOLICYVALUESYSTEM_H
#include /**/ "ace/pre.h"

#include "portableserver_export.h"
#include "IdAssignmentPolicyValue.h"
#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace TAO
{
  namespace Portable_Server
  {
    class TAO_PortableServer_Export System_IdAssignment_Policy
      : public virtual IdAssignmentPolicyValue
    {
      public:
        virtual ~System_IdAssignment_Policy  (void);

        virtual ::PortableServer::IdAssignmentPolicyValue policy_type (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
          ACE_THROW_SPEC ((CORBA::SystemException));
    };

    ACE_STATIC_SVC_DECLARE_EXPORT (TAO_PortableServer, System_IdAssignment_Policy)
    ACE_FACTORY_DECLARE (TAO_PortableServer, System_IdAssignment_Policy)
  } /* namespace Portable_Server */
} /* namespace TAO */

#include /**/ "ace/post.h"
#endif /* TAO_PORTABLESERVER_IDASSIGNMENTPOLICYVALUESYSTEM_H */
