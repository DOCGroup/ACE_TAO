// -*- C++ -*-

//=============================================================================
/**
 *  @file Servant_Retention_Policy_Value.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_SERVANT_RETENTION_POLICY_VALUE_H
#define TAO_SERVANT_RETENTION_POLICY_VALUE_H
#include /**/ "ace/pre.h"

#include "portableserver_export.h"
#include "ThreadPolicyC.h"
#include "ace/Service_Object.h"
#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  class TAO_PortableServer_Export Servant_Retention_Policy_Value
    : public virtual ACE_Service_Object
  {
    public:
      virtual ~Servant_Retention_Policy_Value (void);

      virtual PortableServer::ServantRetentionPolicyValue policy_type (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException)) = 0;
  };

  class TAO_PortableServer_Export Retain_Servant_Retention_Policy
    : public virtual Servant_Retention_Policy_Value
  {
    public:
      virtual ~Retain_Servant_Retention_Policy (void);

      virtual PortableServer::ServantRetentionPolicyValue policy_type (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));
  };

  ACE_STATIC_SVC_DECLARE (Retain_Servant_Retention_Policy)
  ACE_FACTORY_DECLARE (TAO_PortableServer, Retain_Servant_Retention_Policy)

  class TAO_PortableServer_Export Non_Retain_Servant_Retention_Policy
    : public virtual Servant_Retention_Policy_Value
  {
    public:
      virtual ~Non_Retain_Servant_Retention_Policy  (void);

      virtual PortableServer::ServantRetentionPolicyValue policy_type (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));
  };

  ACE_STATIC_SVC_DECLARE (Non_Retain_Servant_Retention_Policy)
  ACE_FACTORY_DECLARE (TAO_PortableServer, Non_Retain_Servant_Retention_Policy)
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

#include /**/ "ace/post.h"
#endif /* TAO_SERVANT_RETENTION_POLICY_VALUE_H */

