#include "IdUniquenessPolicyValue.h"
#include "PortableServerC.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID (PortableServer,
           IdUniquenessPolicyValue,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    IdUniquenessPolicyValue::~IdUniquenessPolicyValue (void)
    {
    }

    Unique_IdUniqueness_Policy::~Unique_IdUniqueness_Policy (void)
    {
    }

    Multiple_IdUniqueness_Policy::~Multiple_IdUniqueness_Policy (void)
    {
    }

    ::PortableServer::IdUniquenessPolicyValue
    Multiple_IdUniqueness_Policy::policy_type (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      return ::PortableServer::MULTIPLE_ID;
    }

    ::PortableServer::IdUniquenessPolicyValue
    Unique_IdUniqueness_Policy::policy_type (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
          ACE_THROW_SPEC ((CORBA::SystemException))
    {
      return ::PortableServer::UNIQUE_ID;
    }

    ACE_FACTORY_DEFINE (TAO_PortableServer, Unique_IdUniqueness_Policy)

    ACE_STATIC_SVC_DEFINE (
        Unique_IdUniqueness_Policy,
        ACE_TEXT ("Unique_IdUniqueness_Policy"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (Unique_IdUniqueness_Policy),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

    ACE_FACTORY_DEFINE (TAO_PortableServer, Multiple_IdUniqueness_Policy)

    ACE_STATIC_SVC_DEFINE (
        Multiple_IdUniqueness_Policy,
        ACE_TEXT ("Multiple_IdUniqueness_Policy"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (Multiple_IdUniqueness_Policy),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

    #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

    template class ACE_Dynamic_Service<IdUniquenessPolicyValue>;
    template class ACE_Dynamic_Service<Multiple_IdUniqueness_Policy>;
    template class ACE_Dynamic_Service<Unique_IdUniqueness_Policy>;

    #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

    #pragma instantiate ACE_Dynamic_Service<IdUniquenessPolicyValue>
    #pragma instantiate ACE_Dynamic_Service<Multiple_IdUniqueness_Policy>
    #pragma instantiate ACE_Dynamic_Service<Unique_IdUniqueness_Policy>

    #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */
