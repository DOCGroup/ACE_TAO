// -*- C++ -*-

//=============================================================================
/**
 *  @file POA_LifespanPolicy.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_POA_ID_ASSIGNMENT_POLICY_H
#define TAO_POA_ID_ASSIGNMENT_POLICY_H
#include /**/ "ace/pre.h"

#include "portableserver_export.h"
#include "LifespanPolicyC.h"
#include "tao/LocalObject.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    class LifespanPolicyValue;

    class TAO_PortableServer_Export POA_LifespanPolicy :
      public virtual ::PortableServer::LifespanPolicy,
      public virtual TAO_Local_RefCounted_Object
    {
    public:
      POA_LifespanPolicy ();

      CORBA::Policy_ptr copy (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));

      void destroy (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));

      ::PortableServer::LifespanPolicyValue value (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));

      CORBA::PolicyType policy_type (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));

      /// Initialise with the passed value
      void init (::PortableServer::LifespanPolicyValue value);

      /// Initialise with the passed value
      void init (const CORBA::Any &value ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::PolicyError));

    private:
      LifespanPolicyValue *value_;
    };
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_POA_ID_ASSIGNMENT_POLICY_H */
