#include "ServantRetentionPolicy.h"
#include "ServantRetentionPolicyValue.h"
#include "ace/Dynamic_Service.h"

#define TAO_PORTABLESERVER_SAFE_INCLUDE
#include "PortableServerC.h"
#undef TAO_PORTABLESERVER_SAFE_INCLUDE

ACE_RCSID (PortableServer,
           ServantRetentionPolicy,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    ServantRetentionPolicy::ServantRetentionPolicy () :
      value_ (0)
    {
    }

    void
    ServantRetentionPolicy::init (
      const CORBA::Any &value ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::PolicyError))
    {
      ::PortableServer::ServantRetentionPolicyValue thrvalue;
      if ((value >>= thrvalue) == 0)
        ACE_THROW (CORBA::PolicyError (CORBA::BAD_POLICY_VALUE));

      (void) this->init (thrvalue);
    }

    void
    ServantRetentionPolicy::init (
      ::PortableServer::ServantRetentionPolicyValue value)
    {
      switch (value)
        {
        case ::PortableServer::RETAIN :
          {
            this->value_ =
              ACE_Dynamic_Service<Retain_Servant_Retention_Policy>::instance ("Retain_Servant_Retention_Policy");

            if (this->value_ == 0)
              {
                ACE_Service_Config::process_directive (
                  ::TAO::Portable_Server::ace_svc_desc_Retain_Servant_Retention_Policy);

                this->value_ =
                  ACE_Dynamic_Service<Retain_Servant_Retention_Policy>::instance ("Retain_Servant_Retention_Policy");
              }
            break;
          }
        case ::PortableServer::NON_RETAIN :
          {
            this->value_ =
              ACE_Dynamic_Service<Non_Retain_Servant_Retention_Policy>::instance ("Non_Retain_Servant_Retention_Policy");

            if (this->value_ == 0)
              {
                ACE_Service_Config::process_directive (
                  ::TAO::Portable_Server::ace_svc_desc_Non_Retain_Servant_Retention_Policy);

                this->value_ =
                  ACE_Dynamic_Service<Non_Retain_Servant_Retention_Policy>::instance ("Non_Retain_Servant_Retention_Policy");
              }

            break;
          }
        }
    }

    CORBA::Policy_ptr
    ServantRetentionPolicy::copy (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      ServantRetentionPolicy *copy = 0;
      ACE_NEW_THROW_EX (copy,
                        ServantRetentionPolicy,
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (CORBA::Policy::_nil ());

      (void) copy->init (this->value_->policy_type ());

      return copy;
    }

    void
    ServantRetentionPolicy::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
    }

    ::PortableServer::ServantRetentionPolicyValue
    ServantRetentionPolicy::value (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      return value_->policy_type (ACE_ENV_SINGLE_ARG_PARAMETER);
    }

    CORBA::PolicyType
    ServantRetentionPolicy::policy_type (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      return ::PortableServer::SERVANT_RETENTION_POLICY_ID;
    }

    TAO_Cached_Policy_Type
    ServantRetentionPolicy::_tao_cached_type (void) const
    {
      return TAO_CACHED_POLICY_SERVANT_RETENTION;
    }

    TAO_Policy_Scope
    ServantRetentionPolicy::_tao_scope (void) const
    {
      return TAO_POLICY_POA_SCOPE;
    }
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */
