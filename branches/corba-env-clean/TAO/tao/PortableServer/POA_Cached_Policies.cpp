// @(#) $Id$

#include "POA_Cached_Policies.h"
#include "POA_Policy_Set.h"

#if !defined (__ACE_INLINE__)
# include "POA_Cached_Policies.i"
#endif /* ! __ACE_INLINE__ */


ACE_RCSID(tao, POA, "$Id$")


TAO_POA_Cached_Policies::TAO_POA_Cached_Policies ()
  : thread_ (PortableServer::ORB_CTRL_MODEL),
    lifespan_ (PortableServer::TRANSIENT),
    id_uniqueness_ (PortableServer::UNIQUE_ID),
    id_assignment_ (PortableServer::SYSTEM_ID),
    implicit_activation_ (PortableServer::NO_IMPLICIT_ACTIVATION),
    servant_retention_ (PortableServer::RETAIN),
    request_processing_ (PortableServer::USE_ACTIVE_OBJECT_MAP_ONLY),
    priority_model_ (TAO_POA_Cached_Policies::NOT_SPECIFIED),
    server_priority_ (TAO_INVALID_PRIORITY)
{
}


TAO_POA_Cached_Policies::~TAO_POA_Cached_Policies (void)
{
}

void
TAO_POA_Cached_Policies::update (TAO_POA_Policy_Set &policy_set
                                 TAO_ENV_ARG_DECL)
{
  for (CORBA::ULong i = 0; i < policy_set.num_policies (); i++)
    {
      CORBA::Policy_var policy = policy_set.get_policy_by_index (i);

      this->update_policy (policy.in ()
                           TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

void
TAO_POA_Cached_Policies::update_policy (const CORBA::Policy_ptr policy
                                        TAO_ENV_ARG_DECL)
{

#if (TAO_HAS_MINIMUM_POA == 0)

  PortableServer::ThreadPolicy_var thread
    = PortableServer::ThreadPolicy::_narrow (policy
                                             TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (!CORBA::is_nil (thread.in ()))
    {
      this->thread_ = thread->value (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      return;
    }

#endif /* TAO_HAS_MINIMUM_POA == 0 */

  PortableServer::LifespanPolicy_var lifespan
    = PortableServer::LifespanPolicy::_narrow (policy
                                               TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (!CORBA::is_nil (lifespan.in ()))
    {
      this->lifespan_ = lifespan->value (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      return;
    }

  PortableServer::IdUniquenessPolicy_var id_uniqueness
    = PortableServer::IdUniquenessPolicy::_narrow (policy
                                                   TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (!CORBA::is_nil (id_uniqueness.in ()))
    {
      this->id_uniqueness_ = id_uniqueness->value (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      return;
    }

  PortableServer::IdAssignmentPolicy_var id_assignment
    = PortableServer::IdAssignmentPolicy::_narrow (policy
                                                   TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (!CORBA::is_nil (id_assignment.in ()))
    {
      this->id_assignment_ = id_assignment->value (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      return;
    }

#if (TAO_HAS_MINIMUM_POA == 0)

  PortableServer::ImplicitActivationPolicy_var implicit_activation
    = PortableServer::ImplicitActivationPolicy::_narrow (policy
                                                         TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (!CORBA::is_nil (implicit_activation.in ()))
    {
      this->implicit_activation_ = implicit_activation->value (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      return;
    }

  PortableServer::ServantRetentionPolicy_var servant_retention
    = PortableServer::ServantRetentionPolicy::_narrow (policy
                                                       TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (!CORBA::is_nil (servant_retention.in ()))
    {
      this->servant_retention_ = servant_retention->value (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      return;
    }

  PortableServer::RequestProcessingPolicy_var request_processing
    = PortableServer::RequestProcessingPolicy::_narrow (policy
                                                        TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (!CORBA::is_nil (request_processing.in ()))
    {
      this->request_processing_ = request_processing->value (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      return;
    }

#endif /* TAO_HAS_MINIMUM_POA == 0 */
}
