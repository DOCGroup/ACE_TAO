#include "LifespanPolicy.h"
#include "LifespanPolicyValue.h"
#include "ace/Dynamic_Service.h"
#include "ace/Service_Config.h"

#define TAO_PORTABLESERVER_SAFE_INCLUDE
#include "PortableServerC.h"
#undef TAO_PORTABLESERVER_SAFE_INCLUDE

ACE_RCSID (PortableServer,
           LifespanPolicy,
           "$Id$")

namespace TAO
{
  namespace Portable_Server
  {
    LifespanPolicy::LifespanPolicy () :
      value_ (0)
    {
    }

    void
    LifespanPolicy::init (
      const CORBA::Any &value ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::PolicyError))
    {
      ::PortableServer::LifespanPolicyValue thrvalue;
      if ((value >>= thrvalue) == 0)
        ACE_THROW (CORBA::PolicyError (CORBA::BAD_POLICY_VALUE));

      (void) this->init (thrvalue);
    }

    void
    LifespanPolicy::init (
      ::PortableServer::LifespanPolicyValue value)
    {
      switch (value)
        {
        case ::PortableServer::TRANSIENT :
          {
            this->value_ =
              ACE_Dynamic_Service<LifespanPolicyValue>::instance ("LifespanPolicyValueTransient");

            if (this->value_ == 0)
              {
                ACE_Service_Config::process_directive (
                  ACE_TEXT("dynamic LifespanPolicyValueTransient Service_Object *")
                  ACE_TEXT("TAO_PortableServer:_make_LifespanPolicyValueTransient()"));

                this->value_ =
                  ACE_Dynamic_Service<LifespanPolicyValue>::instance ("LifespanPolicyValueTransient");
              }
            break;
          }
        case ::PortableServer::PERSISTENT :
          {
            this->value_ =
              ACE_Dynamic_Service<LifespanPolicyValue>::instance ("LifespanPolicyValuePersistent");

            if (this->value_ == 0)
              {
                ACE_Service_Config::process_directive (
                  ACE_TEXT("dynamic LifespanPolicyValuePersistent Service_Object *")
                  ACE_TEXT("TAO_PortableServer:_make_LifespanPolicyValuePersistent()"));

                this->value_ =
                  ACE_Dynamic_Service<LifespanPolicyValue>::instance ("LifespanPolicyValuePersistent");
              }

            break;
          }
        }
    }

    CORBA::Policy_ptr
    LifespanPolicy::copy (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      LifespanPolicy *copy = 0;
      ACE_NEW_THROW_EX (copy,
                        LifespanPolicy,
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (CORBA::Policy::_nil ());

      (void) copy->init (this->value_->policy_type ());

      return copy;
    }

    void
    LifespanPolicy::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
    }

    ::PortableServer::LifespanPolicyValue
    LifespanPolicy::value (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      return value_->policy_type ();
    }

    CORBA::PolicyType
    LifespanPolicy::policy_type (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      return ::PortableServer::LIFESPAN_POLICY_ID;
    }

    TAO_Cached_Policy_Type
    LifespanPolicy::_tao_cached_type (void) const
    {
      return TAO_CACHED_POLICY_LIFESPAN;
    }

    TAO_Policy_Scope
    LifespanPolicy::_tao_scope (void) const
    {
      return TAO_POLICY_POA_SCOPE;
    }
  }
}

