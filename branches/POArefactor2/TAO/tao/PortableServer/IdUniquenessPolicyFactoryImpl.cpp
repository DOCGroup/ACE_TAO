// $Id$

#include "IdUniquenessPolicyFactoryImpl.h"
#include "ace/Dynamic_Service.h"
#include "IdUniquenessPolicy.h"

ACE_RCSID (PortableServer,
           IdUniquenessPolicyFactoryImpl,
           "$Id$")

namespace TAO
{
  namespace Portable_Server
  {
    IdUniquenessPolicyFactoryImpl::~IdUniquenessPolicyFactoryImpl (void)
    {
    }

    ::PortableServer::IdUniquenessPolicy_ptr
    IdUniquenessPolicyFactoryImpl::create (
      ::PortableServer::IdUniquenessPolicyValue value
      ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException))
    {
      IdUniquenessPolicy* policy = 0;

      ACE_NEW_THROW_EX (policy,
                        IdUniquenessPolicy,
                        CORBA::NO_MEMORY ());

      ACE_CHECK_RETURN (::PortableServer::IdUniquenessPolicy::_nil ());

      (void) policy->init (value);

      return policy;
    }

    ::PortableServer::IdUniquenessPolicy_ptr
    IdUniquenessPolicyFactoryImpl::create (
      const CORBA::Any &value
      ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         CORBA::PolicyError))
    {
      IdUniquenessPolicy* policy = 0;

      ACE_NEW_THROW_EX (policy,
                        IdUniquenessPolicy,
                        CORBA::NO_MEMORY ());

      ACE_CHECK_RETURN (::PortableServer::IdUniquenessPolicy::_nil ());

      // @todo, possible memory leak

      (void) policy->init (value ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (::PortableServer::IdUniquenessPolicy::_nil ());

      return policy;
    }

    ACE_STATIC_SVC_DEFINE (
        IdUniquenessPolicyFactoryImpl,
        ACE_TEXT ("IdUniquenessPolicyFactory"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (IdUniquenessPolicyFactoryImpl),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

    ACE_FACTORY_DEFINE (TAO_PortableServer, IdUniquenessPolicyFactoryImpl)

    #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

    template class ACE_Dynamic_Service<IdUniquenessPolicyFactoryImpl>;

    #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

    #pragma instantiate ACE_Dynamic_Service<IdUniquenessPolicyFactoryImpl>

    #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}

