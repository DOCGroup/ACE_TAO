// -*- C++ -*-

//=============================================================================
/**
 *  @file ServantRetentionPolicyValueRetain.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_SERVANT_RETENTION_POLICY_VALUE_RETAIN_H
#define TAO_SERVANT_RETENTION_POLICY_VALUE_RETAIN_H
#include /**/ "ace/pre.h"

#include "portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ServantRetentionPolicyValue.h"
#include "ace/Service_Config.h"

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    class TAO_PortableServer_Export Retain_Servant_Retention_Policy
      : public virtual ServantRetentionPolicyValue
    {
      public:
        virtual ~Retain_Servant_Retention_Policy (void);

        virtual ::PortableServer::ServantRetentionPolicyValue policy_type (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
          ACE_THROW_SPEC ((CORBA::SystemException));
    };

    ACE_STATIC_SVC_DECLARE_EXPORT (TAO_PortableServer, Retain_Servant_Retention_Policy)
    ACE_FACTORY_DECLARE (TAO_PortableServer, Retain_Servant_Retention_Policy)
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

#include /**/ "ace/post.h"
#endif /* TAO_SERVANT_RETENTION_POLICY_VALUE_RETAIN_H */

