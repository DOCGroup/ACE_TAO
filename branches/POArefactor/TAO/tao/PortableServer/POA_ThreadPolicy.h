// -*- C++ -*-

//=============================================================================
/**
 *  @file POA_ThreadPolicy.h
 *
 *  $Id$
 *
 *
 *
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
  class Thread_Policy_Value : public virtual ACE_Service_Object
  {
    PortableServer::ThreadPolicyValue policy_type (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));
  };


  class ORB_CTRL_MODEL_Policy : public Thread_Policy_Value
  {
    PortableServer::ThreadPolicyValue policy_type (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));
  };


  class TAO_PortableServer_Export POA_ThreadPolicy :
    public virtual PortableServer::ThreadPolicy,
    public virtual CORBA::LocalObject
    {
    public:
      POA_ThreadPolicy (PortableServer::ThreadPolicyValue v)
      {
        if (v == ORB_CTRL_MODEL)
          {
            this->value_ =
              ACE_Dynamic_Service<ORB_CTRL_MODEL_Policy>::instance ();
          }
          else if (// for other stuff)
          {
          }
      }

      CORBA::Policy_ptr copy (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));

      void destroy (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));

      CORBA::PolicyType policy_type (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));

     // todo
     private:
       Thread_Policy_Value *value_;
    };
}


// Define SVC configurator stuff for ORB_CTRL_MODEL_Policy so that we
//can only one instance of ORB_CTRL_MODEL_Policy. Theoretically it is a
//*const class* just shared among multiple instances of
//POA_ThreadID_Policy.

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_POA_THREADPOLICY_H */


