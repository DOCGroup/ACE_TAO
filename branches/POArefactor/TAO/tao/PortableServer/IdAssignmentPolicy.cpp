#include "IdAssignmentPolicy.h"
#include "PortableServerC.h"
#include "IdAssignmentPolicyValue.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID (PortableServer,
           IdAssignmentPolicy,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace PortableServer
  {
    IdAssignmentPolicy::IdAssignmentPolicy () :
      value_ (0)
    {
    }

    void
    IdAssignmentPolicy::init (
      const CORBA::Any &value ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::PolicyError))
    {
      ::PortableServer::IdAssignmentPolicyValue thrvalue;
      if ((value >>= thrvalue) == 0)
        ACE_THROW (CORBA::PolicyError (CORBA::BAD_POLICY_VALUE));

      (void) this->init (thrvalue);
    }

    void
    IdAssignmentPolicy::init (
      ::PortableServer::IdAssignmentPolicyValue value)
    {
      switch (value)
        {
        case ::PortableServer::USER_ID :
          {
            this->value_ =
              ACE_Dynamic_Service<User_IdAssignment_Policy>::instance ("User_IdAssignment_Policy");

            if (this->value_ == 0)
              {
                ACE_Service_Config::process_directive (
                  ::TAO::PortableServer::ace_svc_desc_User_IdAssignment_Policy);

                this->value_ =
                  ACE_Dynamic_Service<User_IdAssignment_Policy>::instance ("User_IdAssignment_Policy");
              }
            break;
          }
        case ::PortableServer::SYSTEM_ID :
          {
            this->value_ =
              ACE_Dynamic_Service<System_IdAssignment_Policy>::instance ("System_IdAssignment_Policy");

            if (this->value_ == 0)
              {
                ACE_Service_Config::process_directive (
                  ::TAO::PortableServer::ace_svc_desc_System_IdAssignment_Policy);

                this->value_ =
                  ACE_Dynamic_Service<System_IdAssignment_Policy>::instance ("System_IdAssignment_Policy");
              }

            break;
          }
        }
    }

    CORBA::Policy_ptr
    IdAssignmentPolicy::copy (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      IdAssignmentPolicy *copy = 0;
      ACE_NEW_THROW_EX (copy,
                        IdAssignmentPolicy,
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (CORBA::Policy::_nil ());

      (void) copy->init (this->value_->policy_type ());

      return copy;
    }

    void
    IdAssignmentPolicy::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
    }

    ::PortableServer::IdAssignmentPolicyValue
    IdAssignmentPolicy::value (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      return value_->policy_type (ACE_ENV_SINGLE_ARG_PARAMETER);
    }

    CORBA::PolicyType
    IdAssignmentPolicy::policy_type (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      return ::PortableServer::ID_ASSIGNMENT_POLICY_ID;
    }
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */
