// -*- C++ -*-

//=============================================================================
/**
 *  @file ImplicitActivationPolicyValue.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_IMPLICIT_ACTIVATION_POLICY_VALUE_H
#define TAO_IMPLICIT_ACTIVATION_POLICY_VALUE_H
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
  namespace PortableServer
  {
    class TAO_PortableServer_Export ImplicitActivationPolicyValue
      : public virtual ACE_Service_Object
    {
      public:
        virtual ~ImplicitActivationPolicyValue (void);

        virtual ::PortableServer::ImplicitActivationPolicyValue policy_type (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
          ACE_THROW_SPEC ((CORBA::SystemException)) = 0;
    };

    class TAO_PortableServer_Export Implicit_Implicit_Activation_Policy
      : public virtual ImplicitActivationPolicyValue
    {
      public:
        virtual ~Implicit_Implicit_Activation_Policy (void);

        virtual ::PortableServer::ImplicitActivationPolicyValue policy_type (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
          ACE_THROW_SPEC ((CORBA::SystemException));
    };

    ACE_STATIC_SVC_DECLARE (Implicit_Implicit_Activation_Policy)
    ACE_FACTORY_DECLARE (TAO_PortableServer, Implicit_Implicit_Activation_Policy)

    class TAO_PortableServer_Export No_Implicit_Implicit_Activation_Policy
      : public virtual ImplicitActivationPolicyValue
    {
      public:
        virtual ~No_Implicit_Implicit_Activation_Policy  (void);

        virtual ::PortableServer::ImplicitActivationPolicyValue policy_type (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
          ACE_THROW_SPEC ((CORBA::SystemException));
    };

    ACE_STATIC_SVC_DECLARE (No_Implicit_Implicit_Activation_Policy)
    ACE_FACTORY_DECLARE (TAO_PortableServer, No_Implicit_Implicit_Activation_Policy)
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

#include /**/ "ace/post.h"
#endif /* TAO_IMPLICIT_ACTIVATION_POLICY_VALUE_H */

