#include "IdUniquenessPolicy.h"
#include "IdUniquenessPolicyValue.h"
#include "ace/Dynamic_Service.h"
#include "ace/Service_Config.h"

#define TAO_PORTABLESERVER_SAFE_INCLUDE
#include "PortableServerC.h"
#undef TAO_PORTABLESERVER_SAFE_INCLUDE

ACE_RCSID (PortableServer,
           IdUniquenessPolicy,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    IdUniquenessPolicy::IdUniquenessPolicy () :
      value_ (0)
    {
    }

    void
    IdUniquenessPolicy::init (
      const CORBA::Any &value ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::PolicyError))
    {
      ::PortableServer::IdUniquenessPolicyValue thrvalue;
      if ((value >>= thrvalue) == 0)
        ACE_THROW (CORBA::PolicyError (CORBA::BAD_POLICY_VALUE));

      (void) this->init (thrvalue);
    }

    void
    IdUniquenessPolicy::init (
      ::PortableServer::IdUniquenessPolicyValue value)
    {
      switch (value)
        {
        case ::PortableServer::UNIQUE_ID :
          {
            this->value_ =
              ACE_Dynamic_Service<IdUniquenessPolicyValue>::instance ("Unique_IdUniqueness_Policy");

            if (this->value_ == 0)
              {
                ACE_Service_Config::process_directive (
                  ACE_TEXT("dynamic Unique_IdUniqueness_Policy Service_Object *")
                  ACE_TEXT("TAO_PortableServer:_make_Unique_IdUniqueness_Policy()"));

                this->value_ =
                  ACE_Dynamic_Service<IdUniquenessPolicyValue>::instance ("Unique_IdUniqueness_Policy");
              }
            break;
          }
        case ::PortableServer::MULTIPLE_ID :
          {
            this->value_ =
              ACE_Dynamic_Service<IdUniquenessPolicyValue>::instance ("Multiple_IdUniqueness_Policy");

            if (this->value_ == 0)
              {
                ACE_Service_Config::process_directive (
                  ACE_TEXT("dynamic Multiple_IdUniqueness_Policy Service_Object *")
                  ACE_TEXT("TAO_PortableServer:_make_Multiple_IdUniqueness_Policy()"));

                this->value_ =
                  ACE_Dynamic_Service<IdUniquenessPolicyValue>::instance ("Multiple_IdUniqueness_Policy");
              }

            break;
          }
        }
    }

    CORBA::Policy_ptr
    IdUniquenessPolicy::copy (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      IdUniquenessPolicy *copy = 0;
      ACE_NEW_THROW_EX (copy,
                        IdUniquenessPolicy,
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (CORBA::Policy::_nil ());

      (void) copy->init (this->value_->policy_type ());

      return copy;
    }

    void
    IdUniquenessPolicy::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
    }

    ::PortableServer::IdUniquenessPolicyValue
    IdUniquenessPolicy::value (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      return value_->policy_type (ACE_ENV_SINGLE_ARG_PARAMETER);
    }

    CORBA::PolicyType
    IdUniquenessPolicy::policy_type (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      return ::PortableServer::ID_UNIQUENESS_POLICY_ID;
    }

    TAO_Cached_Policy_Type
    IdUniquenessPolicy::_tao_cached_type (void) const
    {
      return TAO_CACHED_POLICY_ID_UNIQUENESS;
    }

    TAO_Policy_Scope
    IdUniquenessPolicy::_tao_scope (void) const
    {
      return TAO_POLICY_POA_SCOPE;
    }
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */
