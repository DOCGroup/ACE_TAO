#include "RequestProcessingPolicyValue.h"
#include "PortableServerC.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID (PortableServer,
           POA_ThreadPolicy,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    RequestProcessingPolicyValue::~RequestProcessingPolicyValue (void)
    {
    }

    Default_Servant_Request_Processing_Policy::~Default_Servant_Request_Processing_Policy (void)
    {
    }

    ::PortableServer::RequestProcessingPolicyValue
    Default_Servant_Request_Processing_Policy::policy_type (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
          ACE_THROW_SPEC ((CORBA::SystemException))
    {
      return ::PortableServer::USE_DEFAULT_SERVANT;
    }

    ACE_FACTORY_DEFINE (TAO_PortableServer, Default_Servant_Request_Processing_Policy)

    ACE_STATIC_SVC_DEFINE (
        Default_Servant_Request_Processing_Policy,
        ACE_TEXT ("Default_Servant_Request_Processing_Policy"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (Default_Servant_Request_Processing_Policy),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

    Servant_Manager_Request_Processing_Policy::~Servant_Manager_Request_Processing_Policy (void)
    {
    }

    ::PortableServer::RequestProcessingPolicyValue
    Servant_Manager_Request_Processing_Policy::policy_type (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      return ::PortableServer::USE_SERVANT_MANAGER;
    }

    ACE_FACTORY_DEFINE (TAO_PortableServer, Servant_Manager_Request_Processing_Policy)

    ACE_STATIC_SVC_DEFINE (
        Servant_Manager_Request_Processing_Policy,
        ACE_TEXT ("Servant_Manager_Request_Processing_Policy"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (Servant_Manager_Request_Processing_Policy),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

    Active_Object_Map_Request_Processing_Policy::~Active_Object_Map_Request_Processing_Policy (void)
    {
    }

    ::PortableServer::RequestProcessingPolicyValue
    Active_Object_Map_Request_Processing_Policy::policy_type (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      return ::PortableServer::USE_ACTIVE_OBJECT_MAP_ONLY;
    }

    ACE_FACTORY_DEFINE (TAO_PortableServer, Active_Object_Map_Request_Processing_Policy)

    ACE_STATIC_SVC_DEFINE (
        Active_Object_Map_Request_Processing_Policy,
        ACE_TEXT ("Active_Object_Map_Request_Processing_Policy"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (Active_Object_Map_Request_Processing_Policy),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

    #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

    template class ACE_Dynamic_Service<RequestProcessingPolicyValue>;
    template class ACE_Dynamic_Service<Servant_Manager_Request_Processing_Policy>;
    template class ACE_Dynamic_Service<Default_Servant_Request_Processing_Policy>;
    template class ACE_Dynamic_Service<Active_Object_Map_Request_Processing_Policy>;

    #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

    #pragma instantiate ACE_Dynamic_Service<RequestProcessingPolicyValue>
    #pragma instantiate ACE_Dynamic_Service<Servant_Manager_Request_Processing_Policy>
    #pragma instantiate ACE_Dynamic_Service<Default_Servant_Request_Processing_Policy>
    #pragma instantiate ACE_Dynamic_Service<Active_Object_Map_Request_Processing_Policy>

    #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */
