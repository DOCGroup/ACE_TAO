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
  class TAO_PortableServer_Export Thread_Policy_Value : public virtual ACE_Service_Object
  {
    public:
      virtual PortableServer::ThreadPolicyValue policy_type (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException)) = 0;
  };


  class TAO_PortableServer_Export ORB_CTRL_Thread_Policy : public Thread_Policy_Value
  {
    public:
      virtual PortableServer::ThreadPolicyValue policy_type (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));
  };

  class TAO_PortableServer_Export SINGLE_THREAD_Thread_Policy : public Thread_Policy_Value
  {
    public:
      virtual PortableServer::ThreadPolicyValue policy_type (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));
  };

  class TAO_PortableServer_Export POA_ThreadPolicy :
    public virtual PortableServer::ThreadPolicy,
    public virtual CORBA::LocalObject
  {
    public:
      POA_ThreadPolicy (PortableServer::ThreadPolicyValue value);

      CORBA::Policy_ptr copy (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));

      void destroy (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));

      PortableServer::ThreadPolicyValue value (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));

     private:
       Thread_Policy_Value *value_;
  };

  ACE_STATIC_SVC_DECLARE (ORB_CTRL_Thread_Policy)
  ACE_FACTORY_DECLARE (TAO_PortableServer, ORB_CTRL_Thread_Policy)
}

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_POA_THREADPOLICY_H */

