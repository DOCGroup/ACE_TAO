#include "POA_Implicit_ActivationPolicy.h"
#include "PortableServerC.h"
#include "Implicit_Activation_Policy_Value.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID (PortableServer,
           POA_Implicit_ActivationPolicy,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  POA_Implicit_ActivationPolicy::POA_Implicit_ActivationPolicy () :
    value_ (0)
  {
  }

  void
  POA_Implicit_ActivationPolicy::init (
    const CORBA::Any &value ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::PolicyError))
  {
    PortableServer::ImplicitActivationPolicyValue thrvalue;
    if ((value >>= thrvalue) == 0)
      ACE_THROW (CORBA::PolicyError (CORBA::BAD_POLICY_VALUE));

    (void) this->init (thrvalue);
  }

  void
  POA_Implicit_ActivationPolicy::init (
    PortableServer::ImplicitActivationPolicyValue value)
  {
    switch (value)
      {
      case PortableServer::IMPLICIT_ACTIVATION :
        {
          this->value_ =
            ACE_Dynamic_Service<Implicit_Implicit_Activation_Policy>::instance ("Implicit_Implicit_Activation_Policy");

          if (this->value_ == 0)
            {
              ACE_Service_Config::process_directive (
                TAO::ace_svc_desc_Implicit_Implicit_Activation_Policy);

              this->value_ =
                ACE_Dynamic_Service<Implicit_Implicit_Activation_Policy>::instance ("Implicit_Implicit_Activation_Policy");
            }
          break;
        }
      case PortableServer::NO_IMPLICIT_ACTIVATION :
        {
          this->value_ =
            ACE_Dynamic_Service<No_Implicit_Implicit_Activation_Policy>::instance ("No_Implicit_Implicit_Activation_Policy");

          if (this->value_ == 0)
            {
              ACE_Service_Config::process_directive (
                TAO::ace_svc_desc_No_Implicit_Implicit_Activation_Policy);

              this->value_ =
                ACE_Dynamic_Service<No_Implicit_Implicit_Activation_Policy>::instance ("No_Implicit_Implicit_Activation_Policy");
            }

          break;
        }
      }
  }

  CORBA::Policy_ptr
  POA_Implicit_ActivationPolicy::copy (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    POA_Implicit_ActivationPolicy *copy = 0;
    ACE_NEW_THROW_EX (copy,
                      POA_Implicit_ActivationPolicy,
                      CORBA::NO_MEMORY ());
    ACE_CHECK_RETURN (CORBA::Policy::_nil ());

    (void) copy->init (this->value_->policy_type ());

    return copy;
  }

  void
  POA_Implicit_ActivationPolicy::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
  }

  PortableServer::ImplicitActivationPolicyValue
  POA_Implicit_ActivationPolicy::value (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return value_->policy_type (ACE_ENV_SINGLE_ARG_PARAMETER);
  }

  CORBA::PolicyType
  POA_Implicit_ActivationPolicy::policy_type (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return PortableServer::IMPLICIT_ACTIVATION_POLICY_ID;
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */
