#include "PortableServer_PolicyFactory.h"
#include "POA_Policies.h"

ACE_RCSID (PortableServer,
           PortableServer_PolicyFactory,
           "$Id$")

CORBA::Policy_ptr
TAO_PortableServer_PolicyFactory::create_policy (
    CORBA::PolicyType type,
    const CORBA::Any &value
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CORBA::PolicyError))
{
#if (TAO_HAS_MINIMUM_POA == 0)

  if (type == PortableServer::THREAD_POLICY_ID)
    return TAO_Thread_Policy::create (value
                                      ACE_ENV_ARG_PARAMETER);

#endif /* TAO_HAS_MINIMUM_POA == 0 */

  if (type == PortableServer::LIFESPAN_POLICY_ID)
    return TAO_Lifespan_Policy::create (value
                                        ACE_ENV_ARG_PARAMETER);

  if (type == PortableServer::ID_UNIQUENESS_POLICY_ID)
    return TAO_Id_Uniqueness_Policy::create (value
                                             ACE_ENV_ARG_PARAMETER);

  if (type == PortableServer::ID_ASSIGNMENT_POLICY_ID)
    return TAO_Id_Assignment_Policy::create (value
                                             ACE_ENV_ARG_PARAMETER);

#if (TAO_HAS_MINIMUM_POA == 0)

  if (type == PortableServer::IMPLICIT_ACTIVATION_POLICY_ID)
    return TAO_Implicit_Activation_Policy::create (value
                                                   ACE_ENV_ARG_PARAMETER);

  if (type == PortableServer::SERVANT_RETENTION_POLICY_ID)
    return TAO_Servant_Retention_Policy::create (value
                                                 ACE_ENV_ARG_PARAMETER);

  if (type == PortableServer::REQUEST_PROCESSING_POLICY_ID)
      return TAO_Request_Processing_Policy::create (value
                                                    ACE_ENV_ARG_PARAMETER);

#endif /* TAO_HAS_MINIMUM_POA == 0 */

  ACE_THROW_RETURN (CORBA::PolicyError (CORBA::BAD_POLICY_TYPE),
                    CORBA::Policy::_nil ());
}
