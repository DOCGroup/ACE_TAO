#include "RequestProcessingPolicy.h"
#include "RequestProcessingPolicyValue.h"
#include "ace/Dynamic_Service.h"

#define TAO_PORTABLESERVER_SAFE_INCLUDE
#include "PortableServerC.h"
#undef TAO_PORTABLESERVER_SAFE_INCLUDE

ACE_RCSID (PortableServer,
           RequestProcessingPolicy,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    RequestProcessingPolicy::RequestProcessingPolicy () :
      value_ (0)
    {
    }

    void
    RequestProcessingPolicy::init (
      const CORBA::Any &value ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::PolicyError))
    {
      ::PortableServer::RequestProcessingPolicyValue thrvalue;
      if ((value >>= thrvalue) == 0)
        ACE_THROW (CORBA::PolicyError (CORBA::BAD_POLICY_VALUE));

      (void) this->init (thrvalue);
    }

    void
    RequestProcessingPolicy::init (
      ::PortableServer::RequestProcessingPolicyValue value)
    {
      switch (value)
        {
        case ::PortableServer::USE_ACTIVE_OBJECT_MAP_ONLY :
          {
            this->value_ =
              ACE_Dynamic_Service<RequestProcessingPolicyValue>::instance ("RequestProcessingPolicyValueAOMOnly");

            if (this->value_ == 0)
              {
                ACE_Service_Config::process_directive (
                  ACE_TEXT("dynamic RequestProcessingPolicyValueAOMOnly Service_Object *")
                  ACE_TEXT("TAO_PortableServer:_make_RequestProcessingPolicyValueAOMOnly()"));

                this->value_ =
                  ACE_Dynamic_Service<RequestProcessingPolicyValue>::instance ("RequestProcessingPolicyValueAOMOnly");
              }
            break;
          }
        case ::PortableServer::USE_DEFAULT_SERVANT :
          {
            this->value_ =
              ACE_Dynamic_Service<RequestProcessingPolicyValue>::instance ("RequestProcessingPolicyValueDefaultServant");

            if (this->value_ == 0)
              {
                ACE_Service_Config::process_directive (
                  ACE_TEXT("dynamic RequestProcessingPolicyValueDefaultServant Service_Object *")
                  ACE_TEXT("TAO_PortableServer:_make_RequestProcessingPolicyValueDefaultServant()"));

                this->value_ =
                  ACE_Dynamic_Service<RequestProcessingPolicyValue>::instance ("RequestProcessingPolicyValueDefaultServant");
              }
            break;
          }
        case ::PortableServer::USE_SERVANT_MANAGER :
          {
            this->value_ =
              ACE_Dynamic_Service<RequestProcessingPolicyValue>::instance ("RequestProcessingPolicyValueServantManager");

            if (this->value_ == 0)
              {
                ACE_Service_Config::process_directive (
                  ACE_TEXT("dynamic RequestProcessingPolicyValueServantManager Service_Object *")
                  ACE_TEXT("TAO_PortableServer:_make_RequestProcessingPolicyValueServantManager()"));

                this->value_ =
                  ACE_Dynamic_Service<RequestProcessingPolicyValue>::instance ("RequestProcessingPolicyValueServantManager");
              }
            break;
          }
        }
    }

    CORBA::Policy_ptr
    RequestProcessingPolicy::copy (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      RequestProcessingPolicy *copy = 0;
      ACE_NEW_THROW_EX (copy,
                        RequestProcessingPolicy,
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (CORBA::Policy::_nil ());

      (void) copy->init (this->value_->policy_type ());

      return copy;
    }

    void
    RequestProcessingPolicy::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
    }

    ::PortableServer::RequestProcessingPolicyValue
    RequestProcessingPolicy::value (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      return value_->policy_type (ACE_ENV_SINGLE_ARG_PARAMETER);
    }

    CORBA::PolicyType
    RequestProcessingPolicy::policy_type (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      return ::PortableServer::REQUEST_PROCESSING_POLICY_ID;
    }

    TAO_Cached_Policy_Type
    RequestProcessingPolicy::_tao_cached_type (void) const
    {
      return TAO_CACHED_POLICY_REQUEST_PROCESSING;
    }

    TAO_Policy_Scope
    RequestProcessingPolicy::_tao_scope (void) const
    {
      return TAO_POLICY_POA_SCOPE;
    }


  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */
