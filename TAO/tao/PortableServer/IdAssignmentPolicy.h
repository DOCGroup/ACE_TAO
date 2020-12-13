// -*- C++ -*-

//=============================================================================
/**
 *  @file IdAssignmentPolicy.h
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_PORTABLESERVER_IDASSIGNMENTPOLICY_H
#define TAO_PORTABLESERVER_IDASSIGNMENTPOLICY_H
#include /**/ "ace/pre.h"

#include "portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "IdAssignmentPolicyC.h"
#include "tao/LocalObject.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

#if !defined (CORBA_E_MICRO)

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    class TAO_PortableServer_Export IdAssignmentPolicy
      : public virtual ::PortableServer::IdAssignmentPolicy,
        public virtual ::CORBA::LocalObject
    {
    public:
      IdAssignmentPolicy (::PortableServer::IdAssignmentPolicyValue value);

      CORBA::Policy_ptr copy ();

      void destroy ();

      ::PortableServer::IdAssignmentPolicyValue value ();

      CORBA::PolicyType policy_type ();

      /// Return the cached policy type for this policy.
      virtual TAO_Cached_Policy_Type _tao_cached_type () const;

      /// Returns the scope at which this policy can be applied. See orbconf.h.
      virtual TAO_Policy_Scope _tao_scope () const;

    private:
      ::PortableServer::IdAssignmentPolicyValue value_;
    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_PORTABLESERVER_IDASSIGNMENTPOLICY_H */
