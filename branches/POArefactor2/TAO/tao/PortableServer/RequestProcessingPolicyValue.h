// -*- C++ -*-

//=============================================================================
/**
 *  @file RequestProcessingPolicyValue.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_REQUEST_PROCESSING_POLICY_VALUE_H
#define TAO_REQUEST_PROCESSING_POLICY_VALUE_H
#include /**/ "ace/pre.h"

#include "portableserver_export.h"
#include "RequestProcessingPolicyC.h"
#include "ace/Service_Object.h"
#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    class TAO_PortableServer_Export RequestProcessingPolicyValue
      : public virtual ACE_Service_Object
    {
      public:
        virtual ~RequestProcessingPolicyValue (void);

        virtual ::PortableServer::RequestProcessingPolicyValue policy_type (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
          ACE_THROW_SPEC ((CORBA::SystemException)) = 0;
    };

    class TAO_PortableServer_Export Default_Servant_Request_Processing_Policy
      : public virtual RequestProcessingPolicyValue
    {
      public:
        virtual ~Default_Servant_Request_Processing_Policy (void);

        virtual ::PortableServer::RequestProcessingPolicyValue policy_type (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
          ACE_THROW_SPEC ((CORBA::SystemException));
    };

    ACE_STATIC_SVC_DECLARE (Default_Servant_Request_Processing_Policy)
    ACE_FACTORY_DECLARE (TAO_PortableServer, Default_Servant_Request_Processing_Policy)

    class TAO_PortableServer_Export Servant_Manager_Request_Processing_Policy
      : public virtual RequestProcessingPolicyValue
    {
      public:
        virtual ~Servant_Manager_Request_Processing_Policy  (void);

        virtual ::PortableServer::RequestProcessingPolicyValue policy_type (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
          ACE_THROW_SPEC ((CORBA::SystemException));
    };

    ACE_STATIC_SVC_DECLARE (Servant_Manager_Request_Processing_Policy)
    ACE_FACTORY_DECLARE (TAO_PortableServer, Servant_Manager_Request_Processing_Policy)

    class TAO_PortableServer_Export Active_Object_Map_Request_Processing_Policy
      : public virtual RequestProcessingPolicyValue
    {
      public:
        virtual ~Active_Object_Map_Request_Processing_Policy  (void);

        virtual ::PortableServer::RequestProcessingPolicyValue policy_type (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
          ACE_THROW_SPEC ((CORBA::SystemException));
    };

    ACE_STATIC_SVC_DECLARE (Active_Object_Map_Request_Processing_Policy)
    ACE_FACTORY_DECLARE (TAO_PortableServer, Active_Object_Map_Request_Processing_Policy)
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

#include /**/ "ace/post.h"
#endif /* TAO_REQUEST_PROCESSING_POLICY_VALUE_H */

