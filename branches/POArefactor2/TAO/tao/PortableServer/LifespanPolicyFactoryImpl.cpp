// $Id$

#include "LifespanPolicyFactoryImpl.h"
#include "ace/Dynamic_Service.h"
#include "LifespanPolicy.h"

ACE_RCSID (PortableServer,
           LifespanPolicyFactoryImpl,
           "$Id$")

namespace TAO
{
  namespace Portable_Server
  {
    LifespanPolicyFactoryImpl::~LifespanPolicyFactoryImpl (void)
    {
    }

    ::PortableServer::LifespanPolicy_ptr
    LifespanPolicyFactoryImpl::create (
      ::PortableServer::LifespanPolicyValue value
      ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException))
    {
      LifespanPolicy* policy = 0;

      ACE_NEW_THROW_EX (policy,
                        LifespanPolicy,
                        CORBA::NO_MEMORY ());

      ACE_CHECK_RETURN (::PortableServer::LifespanPolicy::_nil ());

      (void) policy->init (value);

      return policy;
    }

    ::PortableServer::LifespanPolicy_ptr
    LifespanPolicyFactoryImpl::create (
      const CORBA::Any &value
      ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         CORBA::PolicyError))
    {
      LifespanPolicy* policy = 0;

      ACE_NEW_THROW_EX (policy,
                        LifespanPolicy,
                        CORBA::NO_MEMORY ());

      ACE_CHECK_RETURN (::PortableServer::LifespanPolicy::_nil ());

      (void) policy->init (value ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (::PortableServer::LifespanPolicy::_nil ());

      return policy;
    }

    ACE_STATIC_SVC_DEFINE (
        LifespanPolicyFactoryImpl,
        ACE_TEXT ("LifespanPolicyFactory"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (LifespanPolicyFactoryImpl),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

    ACE_FACTORY_DEFINE (TAO_PortableServer, LifespanPolicyFactoryImpl)

    #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

    template class ACE_Dynamic_Service<LifespanPolicyFactoryImpl>;

    #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

    #pragma instantiate ACE_Dynamic_Service<LifespanPolicyFactoryImpl>

    #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}

