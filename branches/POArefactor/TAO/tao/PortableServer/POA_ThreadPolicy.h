// -*- C++ -*-

//=============================================================================
/**
 *  @file POA_ThreadPolicy.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_POA_THREADPOLICY_H
#define TAO_POA_THREADPOLICY_H
#include /**/ "ace/pre.h"

#include "portableserver_export.h"
#include "ThreadPolicyC.h"
#include "tao/LocalObject.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

namespace TAO
{
  class Thread_Policy_Value;

  class TAO_PortableServer_Export POA_ThreadPolicy :
    public virtual PortableServer::ThreadPolicy,
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    POA_ThreadPolicy ();

    CORBA::Policy_ptr copy (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    void destroy (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    PortableServer::ThreadPolicyValue value (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    CORBA::PolicyType policy_type (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    /// Initialise with the passed value
    void init (PortableServer::ThreadPolicyValue value);

    /// Initialise with the passed value
    void init (const CORBA::Any &value ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::PolicyError));

  private:
    Thread_Policy_Value *value_;
  };
}

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_POA_THREADPOLICY_H */
