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

    class TAO_PortableServer_Export RequestProcessingPolicyValueDefaultServant
      : public virtual RequestProcessingPolicyValue
    {
      public:
        virtual ~RequestProcessingPolicyValueDefaultServant (void);

        virtual ::PortableServer::RequestProcessingPolicyValue policy_type (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
          ACE_THROW_SPEC ((CORBA::SystemException));
    };

    ACE_STATIC_SVC_DECLARE (RequestProcessingPolicyValueDefaultServant)
    ACE_FACTORY_DECLARE (TAO_PortableServer, RequestProcessingPolicyValueDefaultServant)

    class TAO_PortableServer_Export RequestProcessingPolicyValueServantManager
      : public virtual RequestProcessingPolicyValue
    {
      public:
        virtual ~RequestProcessingPolicyValueServantManager  (void);

        virtual ::PortableServer::RequestProcessingPolicyValue policy_type (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
          ACE_THROW_SPEC ((CORBA::SystemException));
    };

    ACE_STATIC_SVC_DECLARE (RequestProcessingPolicyValueServantManager)
    ACE_FACTORY_DECLARE (TAO_PortableServer, RequestProcessingPolicyValueServantManager)

    class TAO_PortableServer_Export RequestProcessingPolicyValueAOMOnly
      : public virtual RequestProcessingPolicyValue
    {
      public:
        virtual ~RequestProcessingPolicyValueAOMOnly  (void);

        virtual ::PortableServer::RequestProcessingPolicyValue policy_type (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
          ACE_THROW_SPEC ((CORBA::SystemException));
    };

    ACE_STATIC_SVC_DECLARE (RequestProcessingPolicyValueAOMOnly)
    ACE_FACTORY_DECLARE (TAO_PortableServer, RequestProcessingPolicyValueAOMOnly)
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

#include /**/ "ace/post.h"
#endif /* TAO_REQUEST_PROCESSING_POLICY_VALUE_H */

