// -*- C++ -*-

//=============================================================================
/**
 *  @file LifespanPolicyValue.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_LIFESPAN_POLICY_VALUE_H
#define TAO_LIFESPAN_POLICY_VALUE_H
#include /**/ "ace/pre.h"

#include "portableserver_export.h"
#include "LifespanPolicyC.h"
#include "ace/Service_Object.h"
#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace TAO
{
  namespace Portable_Server
  {
    class TAO_PortableServer_Export LifespanPolicyValue
      : public virtual ACE_Service_Object
    {
      public:
        virtual ~LifespanPolicyValue (void);

        virtual ::PortableServer::LifespanPolicyValue policy_type (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
          ACE_THROW_SPEC ((CORBA::SystemException)) = 0;
    };

    class TAO_PortableServer_Export Transient_Lifespan_Policy
      : public virtual LifespanPolicyValue
    {
      public:
        virtual ~Transient_Lifespan_Policy (void);

        virtual ::PortableServer::LifespanPolicyValue policy_type (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
          ACE_THROW_SPEC ((CORBA::SystemException));
    };

    class TAO_PortableServer_Export Persistent_Lifespan_Policy
      : public virtual LifespanPolicyValue
    {
      public:
        virtual ~Persistent_Lifespan_Policy (void);

        virtual ::PortableServer::LifespanPolicyValue policy_type (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
          ACE_THROW_SPEC ((CORBA::SystemException));
    };

    ACE_STATIC_SVC_DECLARE (Persistent_Lifespan_Policy)
    ACE_FACTORY_DECLARE (TAO_PortableServer, Persistent_Lifespan_Policy)

    ACE_STATIC_SVC_DECLARE (Transient_Lifespan_Policy)
    ACE_FACTORY_DECLARE (TAO_PortableServer, Transient_LifespanPolicy)
  }
}

#include /**/ "ace/post.h"
#endif /* TAO_LIFESPAN_POLICY_VALUE_H */

