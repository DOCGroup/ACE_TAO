// $Id$

#include "ThreadPolicyFactoryImpl.h"
#include "ace/Dynamic_Service.h"
#include "ThreadPolicy.h"

ACE_RCSID (PortableServer,
           ThreadPolicyFactoryImpl,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    ThreadPolicyFactoryImpl::~ThreadPolicyFactoryImpl (void)
    {
    }

    ::PortableServer::ThreadPolicy_ptr
    ThreadPolicyFactoryImpl::create (
      ::PortableServer::ThreadPolicyValue value ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException))
    {
      ThreadPolicy* policy = 0;

      ACE_NEW_THROW_EX (policy,
                        ThreadPolicy,
                        CORBA::NO_MEMORY ());

      ACE_CHECK_RETURN (::PortableServer::ThreadPolicy::_nil ());

      (void) policy->init (value);

      return policy;
    }

    ::PortableServer::ThreadPolicy_ptr
    ThreadPolicyFactoryImpl::create (
      const CORBA::Any &value
      ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         CORBA::PolicyError))
    {
      ThreadPolicy* policy = 0;

      ACE_NEW_THROW_EX (policy,
                        ThreadPolicy,
                        CORBA::NO_MEMORY ());

      ACE_CHECK_RETURN (::PortableServer::LifespanPolicy::_nil ());

      (void) policy->init (value ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (::PortableServer::ThreadPolicy::_nil ());

      return policy;
    }

    ACE_STATIC_SVC_DEFINE (
        ThreadPolicyFactoryImpl,
        ACE_TEXT ("ThreadPolicyFactory"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (ThreadPolicyFactoryImpl),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

    ACE_FACTORY_DEFINE (TAO_PortableServer, ThreadPolicyFactoryImpl)

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

      template class ACE_Dynamic_Service<ThreadPolicyFactoryImpl>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Dynamic_Service<ThreadPolicyFactoryImpl>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */
