// $Id$

#include "IdUniquenessPolicyFactory.h"
#include "ace/Dynamic_Service.h"
#include "IdUniquenessPolicy.h"

ACE_RCSID (PortableServer,
           IdUniquenessPolicyFactory,
           "$Id$")

namespace TAO
{
  namespace Portable_Server
  {
    IdUniquenessPolicyFactory::~IdUniquenessPolicyFactory (void)
    {
    }

    ::PortableServer::IdUniquenessPolicy_ptr
    IdUniquenessPolicyFactory::create (
      ::PortableServer::IdUniquenessPolicyValue value)
    {
      IdUniquenessPolicy* policy = 0;

      ACE_NEW_RETURN (policy,
                      IdUniquenessPolicy,
                      ::PortableServer::IdUniquenessPolicy::_nil ());

      (void) policy->init (value);

      return policy;
    }

    ::PortableServer::IdUniquenessPolicy_ptr
    IdUniquenessPolicyFactory::create (
      const CORBA::Any &value
      ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::PolicyError))
    {
      IdUniquenessPolicy* policy = 0;

      ACE_NEW_RETURN (policy,
                      IdUniquenessPolicy,
                      ::PortableServer::IdUniquenessPolicy::_nil ());

      (void) policy->init (value ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (::PortableServer::IdUniquenessPolicy::_nil ());

      return policy;
    }

    ACE_STATIC_SVC_DEFINE (
        IdUniquenessPolicyFactory,
        ACE_TEXT ("IdUniquenessPolicyFactory"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (IdUniquenessPolicyFactory),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

    ACE_FACTORY_DEFINE (TAO_PortableServer, IdUniquenessPolicyFactory)

    #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

    template class ACE_Dynamic_Service<IdUniquenessPolicyFactory>;

    #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

    #pragma instantiate ACE_Dynamic_Service<IdUniquenessPolicyFactory>

    #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}

