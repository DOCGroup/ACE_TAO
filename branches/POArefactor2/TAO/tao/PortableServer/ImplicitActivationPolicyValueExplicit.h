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

#ifndef TAO_IMPLICIT_ACTIVATION_POLICY_VALUE_EXPLICIT_H
#define TAO_IMPLICIT_ACTIVATION_POLICY_VALUE_EXPLICIT_H
#include /**/ "ace/pre.h"

#include "portableserver_export.h"
#include "ImplicitActivationPolicyValue.h"
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
    class TAO_PortableServer_Export No_Implicit_Implicit_Activation_Policy
      : public virtual ImplicitActivationPolicyValue
    {
      public:
        virtual ~No_Implicit_Implicit_Activation_Policy  (void);

        virtual ::PortableServer::ImplicitActivationPolicyValue policy_type (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
          ACE_THROW_SPEC ((CORBA::SystemException));
    };

    ACE_STATIC_SVC_DECLARE_EXPORT (TAO_PortableServer, No_Implicit_Implicit_Activation_Policy)
    ACE_FACTORY_DECLARE (TAO_PortableServer, No_Implicit_Implicit_Activation_Policy)
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

#include /**/ "ace/post.h"
#endif /* TAO_IMPLICIT_ACTIVATION_POLICY_VALUE_EXPLICIT_H */

