#include "POA_ThreadPolicy.h"
#include "PortableServerC.h"
#include "Thread_Policy_Value.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID (PortableServer,
           POA_ThreadPolicy,
           "$Id$")

namespace TAO
{
  POA_ThreadPolicy::POA_ThreadPolicy () :
    value_ (0)
  {
  }

  void
  POA_ThreadPolicy::init (
    const CORBA::Any &value ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::PolicyError))
  {
    PortableServer::ThreadPolicyValue thrvalue;
    if ((value >>= thrvalue) == 0)
      ACE_THROW (CORBA::PolicyError (CORBA::BAD_POLICY_VALUE));

    (void) this->init (thrvalue);
  }

  void
  POA_ThreadPolicy::init (
    PortableServer::ThreadPolicyValue value)
  {
    switch (value)
      {
      case PortableServer::ORB_CTRL_MODEL :
        {
          this->value_ =
            ACE_Dynamic_Service<ORB_CTRL_Thread_Policy>::instance ("ORB_CTRL_Thread_Policy");
          break;
        }
      case PortableServer::SINGLE_THREAD_MODEL :
        {
          this->value_ =
            ACE_Dynamic_Service<SINGLE_THREAD_Thread_Policy>::instance ("SINGLE_THREAD_Thread_Policy");
          break;
        }
      }
  }

  CORBA::Policy_ptr
  POA_ThreadPolicy::copy (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    POA_ThreadPolicy *thread_policy_copy = 0;
    ACE_NEW_THROW_EX (thread_policy_copy,
                      POA_ThreadPolicy,
                      CORBA::NO_MEMORY ());
    ACE_CHECK_RETURN (CORBA::Policy::_nil ());

    (void) thread_policy_copy->init (this->value_->policy_type ());

    return thread_policy_copy;
  }

  void
  POA_ThreadPolicy::destroy (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
  }

  PortableServer::ThreadPolicyValue
  POA_ThreadPolicy::value (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return value_->policy_type (ACE_ENV_SINGLE_ARG_PARAMETER);
  }

  CORBA::PolicyType
  POA_ThreadPolicy::policy_type (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return PortableServer::THREAD_POLICY_ID;
  }
}
