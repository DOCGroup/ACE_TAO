// $Id$

#include "IdAssignmentPolicyFactoryImpl.h"
#include "ace/Dynamic_Service.h"
#include "IdAssignmentPolicy.h"

ACE_RCSID (PortableServer,
           IdAssignmentPolicyFactoryImpl,
           "$Id$")

namespace TAO
{
  namespace Portable_Server
  {
    IdAssignmentPolicyFactoryImpl::~IdAssignmentPolicyFactoryImpl (void)
    {
    }

    ::PortableServer::IdAssignmentPolicy_ptr
    IdAssignmentPolicyFactoryImpl::create (
      ::PortableServer::IdAssignmentPolicyValue value ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException))
    {
      IdAssignmentPolicy* policy = 0;

      ACE_NEW_THROW_EX (policy,
                        IdAssignmentPolicy,
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (::PortableServer::IdAssignmentPolicy::_nil ());

      (void) policy->init (value);

      return policy;
    }

    ::PortableServer::IdAssignmentPolicy_ptr
    IdAssignmentPolicyFactoryImpl::create (
      const CORBA::Any &value
      ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         CORBA::PolicyError))
    {
      IdAssignmentPolicy* policy = 0;

      ACE_NEW_THROW_EX (policy,
                        IdAssignmentPolicy,
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (::PortableServer::IdAssignmentPolicy::_nil ());

      (void) policy->init (value ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (::PortableServer::IdAssignmentPolicy::_nil ());

      return policy;
    }

ACE_STATIC_SVC_DEFINE (IdAssignmentPolicyFactoryImpl,
                       ACE_TEXT ("IdAssignmentPolicyFactory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (IdAssignmentPolicyFactoryImpl),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)

ACE_FACTORY_DEFINE (TAO_PortableServer, IdAssignmentPolicyFactoryImpl)

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

  template class ACE_Dynamic_Service<IdAssignmentPolicyFactoryImpl>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

    #pragma instantiate ACE_Dynamic_Service<IdAssignmentPolicyFactoryImpl>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}
