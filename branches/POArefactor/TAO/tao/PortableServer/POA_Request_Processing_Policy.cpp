#include "POA_Request_Processing_Policy.h"
#include "PortableServerC.h"
#include "Request_Processing_Policy_Value.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID (PortableServer,
           POA_Request_Processing_Policy,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  POA_Request_Processing_Policy::POA_Request_Processing_Policy () :
    value_ (0)
  {
  }

  void
  POA_Request_Processing_Policy::init (
    const CORBA::Any &value ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::PolicyError))
  {
    PortableServer::RequestProcessingPolicyValue thrvalue;
    if ((value >>= thrvalue) == 0)
      ACE_THROW (CORBA::PolicyError (CORBA::BAD_POLICY_VALUE));

    (void) this->init (thrvalue);
  }

  void
  POA_Request_Processing_Policy::init (
    PortableServer::RequestProcessingPolicyValue value)
  {
    switch (value)
      {
      case PortableServer::USE_ACTIVE_OBJECT_MAP_ONLY :
        {
          this->value_ =
            ACE_Dynamic_Service<Active_Object_Map_Request_Processing_Policy>::instance ("Active_Object_Map_Request_Processing_Policy");

          if (this->value_ == 0)
            {
              ACE_Service_Config::process_directive (
                TAO::ace_svc_desc_Active_Object_Map_Request_Processing_Policy);

              this->value_ =
                ACE_Dynamic_Service<Active_Object_Map_Request_Processing_Policy>::instance ("Active_Object_Map_Request_Processing_Policy");
            }
          break;
        }
      case PortableServer::USE_DEFAULT_SERVANT :
        {
          this->value_ =
            ACE_Dynamic_Service<Default_Servant_Request_Processing_Policy>::instance ("Default_Servant_Request_Processing_Policy");

          if (this->value_ == 0)
            {
              ACE_Service_Config::process_directive (
                TAO::ace_svc_desc_Default_Servant_Request_Processing_Policy);

              this->value_ =
                ACE_Dynamic_Service<Default_Servant_Request_Processing_Policy>::instance ("Default_Servant_Request_Processing_Policy");
            }

          break;
        }
      case PortableServer::USE_SERVANT_MANAGER :
        {
          this->value_ =
            ACE_Dynamic_Service<Servant_Manager_Request_Processing_Policy>::instance ("Servant_Manager_Request_Processing_Policy");

          if (this->value_ == 0)
            {
              ACE_Service_Config::process_directive (
                TAO::ace_svc_desc_Servant_Manager_Request_Processing_Policy);

              this->value_ =
                ACE_Dynamic_Service<Servant_Manager_Request_Processing_Policy>::instance ("Servant_Manager_Request_Processing_Policy");
            }

          break;
        }
      }
  }

  CORBA::Policy_ptr
  POA_Request_Processing_Policy::copy (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    POA_Request_Processing_Policy *copy = 0;
    ACE_NEW_THROW_EX (copy,
                      POA_Request_Processing_Policy,
                      CORBA::NO_MEMORY ());
    ACE_CHECK_RETURN (CORBA::Policy::_nil ());

    (void) copy->init (this->value_->policy_type ());

    return copy;
  }

  void
  POA_Request_Processing_Policy::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
  }

  PortableServer::RequestProcessingPolicyValue
  POA_Request_Processing_Policy::value (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return value_->policy_type (ACE_ENV_SINGLE_ARG_PARAMETER);
  }

  CORBA::PolicyType
  POA_Request_Processing_Policy::policy_type (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return PortableServer::REQUEST_PROCESSING_POLICY_ID;
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */
