#include "POA_Id_UniquenessPolicy.h"
#include "PortableServerC.h"
#include "IdUniqueness_Policy_Value.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID (PortableServer,
           POA_Id_UniquenessPolicy,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  POA_Id_UniquenessPolicy::POA_Id_UniquenessPolicy () :
    value_ (0)
  {
  }

  void
  POA_Id_UniquenessPolicy::init (
    const CORBA::Any &value ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::PolicyError))
  {
    PortableServer::IdUniquenessPolicyValue thrvalue;
    if ((value >>= thrvalue) == 0)
      ACE_THROW (CORBA::PolicyError (CORBA::BAD_POLICY_VALUE));

    (void) this->init (thrvalue);
  }

  void
  POA_Id_UniquenessPolicy::init (
    PortableServer::IdUniquenessPolicyValue value)
  {
    switch (value)
      {
      case PortableServer::UNIQUE_ID :
        {
          this->value_ =
            ACE_Dynamic_Service<Unique_IdUniqueness_Policy>::instance ("Unique_IdUniqueness_Policy");

          if (this->value_ == 0)
            {
              ACE_Service_Config::process_directive (
                TAO::ace_svc_desc_Unique_IdUniqueness_Policy);

              this->value_ =
                ACE_Dynamic_Service<Unique_IdUniqueness_Policy>::instance ("Unique_IdUniqueness_Policy");
            }
          break;
        }
      case PortableServer::MULTIPLE_ID :
        {
          this->value_ =
            ACE_Dynamic_Service<Multiple_IdUniqueness_Policy>::instance ("Multiple_IdUniqueness_Policy");

          if (this->value_ == 0)
            {
              ACE_Service_Config::process_directive (
                TAO::ace_svc_desc_Multiple_IdUniqueness_Policy);

              this->value_ =
                ACE_Dynamic_Service<Multiple_IdUniqueness_Policy>::instance ("Multiple_IdUniqueness_Policy");
            }

          break;
        }
      }
  }

  CORBA::Policy_ptr
  POA_Id_UniquenessPolicy::copy (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    POA_Id_UniquenessPolicy *copy = 0;
    ACE_NEW_THROW_EX (copy,
                      POA_Id_UniquenessPolicy,
                      CORBA::NO_MEMORY ());
    ACE_CHECK_RETURN (CORBA::Policy::_nil ());

    (void) copy->init (this->value_->policy_type ());

    return copy;
  }

  void
  POA_Id_UniquenessPolicy::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
  }

  PortableServer::IdUniquenessPolicyValue
  POA_Id_UniquenessPolicy::value (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return value_->policy_type (ACE_ENV_SINGLE_ARG_PARAMETER);
  }

  CORBA::PolicyType
  POA_Id_UniquenessPolicy::policy_type (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return PortableServer::ID_UNIQUENESS_POLICY_ID;
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */
