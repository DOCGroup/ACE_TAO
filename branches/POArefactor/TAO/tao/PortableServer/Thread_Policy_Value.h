// -*- C++ -*-

//=============================================================================
/**
 *  @file Thread_Policy_Value.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_THREAD_POLICY_VALUE_H
#define TAO_THREAD_POLICY_VALUE_H
#include /**/ "ace/pre.h"

#include "portableserver_export.h"
#include "ThreadPolicyC.h"
#include "ace/Service_Object.h"
#include "ace/Service_Config.h"

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
  class TAO_PortableServer_Export Thread_Policy_Value
    : public virtual ACE_Service_Object
  {
    public:
      virtual ~Thread_Policy_Value (void);

      virtual PortableServer::ThreadPolicyValue policy_type (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException)) = 0;
  };

  class TAO_PortableServer_Export ORB_CTRL_Thread_Policy
    : public virtual Thread_Policy_Value
  {
    public:
      virtual ~ORB_CTRL_Thread_Policy (void);

      virtual PortableServer::ThreadPolicyValue policy_type (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));
  };

  class TAO_PortableServer_Export SINGLE_THREAD_Thread_Policy
    : public virtual Thread_Policy_Value
  {
    public:
      virtual ~SINGLE_THREAD_Thread_Policy  (void);

      virtual PortableServer::ThreadPolicyValue policy_type (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));
  };

  ACE_STATIC_SVC_DECLARE (ORB_CTRL_Thread_Policy)
  ACE_FACTORY_DECLARE (TAO_PortableServer, ORB_CTRL_Thread_Policy)

  ACE_STATIC_SVC_DECLARE (SINGLE_THREAD_Thread_Policy)
  ACE_FACTORY_DECLARE (TAO_PortableServer, SINGLE_THREAD_Thread_Policy)
}

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_THREAD_POLICY_VALUE_H */

