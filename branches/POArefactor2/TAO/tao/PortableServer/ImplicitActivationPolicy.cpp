#include "ImplicitActivationPolicy.h"
#include "ImplicitActivationPolicyValue.h"
#include "ace/Dynamic_Service.h"

#define TAO_PORTABLESERVER_SAFE_INCLUDE
#include "PortableServerC.h"
#undef TAO_PORTABLESERVER_SAFE_INCLUDE

ACE_RCSID (PortableServer,
           ImplicitActivationPolicy,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    ImplicitActivationPolicy::ImplicitActivationPolicy () :
      value_ (0)
    {
    }

    void
    ImplicitActivationPolicy::init (
      const CORBA::Any &value ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::PolicyError))
    {
      ::PortableServer::ImplicitActivationPolicyValue thrvalue;
      if ((value >>= thrvalue) == 0)
        ACE_THROW (CORBA::PolicyError (CORBA::BAD_POLICY_VALUE));

      (void) this->init (thrvalue);
    }

    void
    ImplicitActivationPolicy::init (
      ::PortableServer::ImplicitActivationPolicyValue value)
    {
      switch (value)
        {
        case ::PortableServer::IMPLICIT_ACTIVATION :
          {
            this->value_ =
              ACE_Dynamic_Service<Implicit_Implicit_Activation_Policy>::instance ("Implicit_Implicit_Activation_Policy");

            if (this->value_ == 0)
              {
                ACE_Service_Config::process_directive (
                  ::TAO::Portable_Server::ace_svc_desc_Implicit_Implicit_Activation_Policy);

                this->value_ =
                  ACE_Dynamic_Service<Implicit_Implicit_Activation_Policy>::instance ("Implicit_Implicit_Activation_Policy");
              }
            break;
          }
        case ::PortableServer::NO_IMPLICIT_ACTIVATION :
          {
            this->value_ =
              ACE_Dynamic_Service<No_Implicit_Implicit_Activation_Policy>::instance ("No_Implicit_Implicit_Activation_Policy");

            if (this->value_ == 0)
              {
                ACE_Service_Config::process_directive (
                  ::TAO::Portable_Server::ace_svc_desc_No_Implicit_Implicit_Activation_Policy);

                this->value_ =
                  ACE_Dynamic_Service<No_Implicit_Implicit_Activation_Policy>::instance ("No_Implicit_Implicit_Activation_Policy");
              }

            break;
          }
        }
    }

    CORBA::Policy_ptr
    ImplicitActivationPolicy::copy (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      ImplicitActivationPolicy *copy = 0;
      ACE_NEW_THROW_EX (copy,
                        ImplicitActivationPolicy,
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (CORBA::Policy::_nil ());

      (void) copy->init (this->value_->policy_type ());

      return copy;
    }

    void
    ImplicitActivationPolicy::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
    }

    ::PortableServer::ImplicitActivationPolicyValue
    ImplicitActivationPolicy::value (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      return value_->policy_type (ACE_ENV_SINGLE_ARG_PARAMETER);
    }

    CORBA::PolicyType
    ImplicitActivationPolicy::policy_type (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      return ::PortableServer::IMPLICIT_ACTIVATION_POLICY_ID;
    }

    TAO_Cached_Policy_Type
    ImplicitActivationPolicy::_tao_cached_type (void) const
    {
      return TAO_CACHED_POLICY_IMPLICIT_ACTIVATION;
    }

    TAO_Policy_Scope
    ImplicitActivationPolicy::_tao_scope (void) const
    {
      return TAO_POLICY_POA_SCOPE;
    }
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */
