// -*- C++ -*-

//=============================================================================
/**
 *  @file IdUniquenessPolicy.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_POA_ID_UNIQUENESS_POLICY_H
#define TAO_POA_ID_UNIQUENESS_POLICY_H
#include /**/ "ace/pre.h"

#include "portableserver_export.h"
#include "IdUniquenessPolicyC.h"
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

namespace TAO
{
  namespace Portable_Server
  {
    class IdUniquenessPolicyValue;

    class TAO_PortableServer_Export IdUniquenessPolicy
      : public virtual ::PortableServer::IdUniquenessPolicy,
        public virtual TAO_Local_RefCounted_Object
    {
    public:
      IdUniquenessPolicy ();

      CORBA::Policy_ptr copy (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));

      void destroy (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));

      ::PortableServer::IdUniquenessPolicyValue value (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));

      CORBA::PolicyType policy_type (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));

      /// Return the cached policy type for this policy.
      virtual TAO_Cached_Policy_Type _tao_cached_type (void) const;

      /// Returns the scope at which this policy can be applied. See orbconf.h.
      virtual TAO_Policy_Scope _tao_scope (void) const;

      /// Initialise with the passed value
      void init (::PortableServer::IdUniquenessPolicyValue value);

      /// Initialise with the passed value
      void init (const CORBA::Any &value ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::PolicyError));

    private:
      ::PortableServer::IdUniquenessPolicyValue value_;
    };
  }
}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_POA_ID_UNIQUENESS_POLICY_H */
