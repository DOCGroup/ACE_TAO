// $Id$

#include "tao/PortableServer/POA_Cached_Policies.h"
#include "tao/PortableServer/POA_Policy_Set.h"
#include "tao/PortableServer/IdAssignmentPolicyC.h"
#include "tao/PortableServer/IdUniquenessPolicyC.h"
#include "tao/PortableServer/ImplicitActivationPolicyC.h"
#include "tao/PortableServer/LifespanPolicyC.h"
#include "tao/PortableServer/RequestProcessingPolicyC.h"
#include "tao/PortableServer/ServantRetentionPolicyC.h"
#include "tao/PortableServer/ThreadPolicyC.h"

#if !defined (__ACE_INLINE__)
# include "tao/PortableServer/POA_Cached_Policies.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(PortableServer,
          POA_Cached_Policies,
          "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    Cached_Policies::Cached_Policies () :
        thread_ (::PortableServer::ORB_CTRL_MODEL),
        lifespan_ (::PortableServer::TRANSIENT),
        id_uniqueness_ (::PortableServer::UNIQUE_ID),
        id_assignment_ (::PortableServer::SYSTEM_ID),
        implicit_activation_ (::PortableServer::NO_IMPLICIT_ACTIVATION),
        servant_retention_ (::PortableServer::RETAIN),
        request_processing_ (::PortableServer::USE_ACTIVE_OBJECT_MAP_ONLY),
        priority_model_ (Cached_Policies::NOT_SPECIFIED),
        server_priority_ (TAO_INVALID_PRIORITY)
    {
    }


    Cached_Policies::~Cached_Policies (void)
    {
    }

    void
    Cached_Policies::update (TAO_POA_Policy_Set &policy_set
                                     ACE_ENV_ARG_DECL)
    {
      for (CORBA::ULong i = 0; i < policy_set.num_policies (); i++)
        {
          CORBA::Policy_var policy = policy_set.get_policy_by_index (i);

          this->update_policy (policy.in ()
                               ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }
    }

    void
    Cached_Policies::update_policy (const CORBA::Policy_ptr policy
                                    ACE_ENV_ARG_DECL)
    {

    #if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
      ::PortableServer::ThreadPolicy_var thread
        = ::PortableServer::ThreadPolicy::_narrow (policy
                                                   ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      if (!CORBA::is_nil (thread.in ()))
        {
          this->thread_ = thread->value (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK;

          return;
        }
    #endif /* TAO_HAS_MINIMUM_POA == 0 */

    #if !defined (CORBA_E_MICRO)
      ::PortableServer::LifespanPolicy_var lifespan
        = ::PortableServer::LifespanPolicy::_narrow (policy
                                                     ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      if (!CORBA::is_nil (lifespan.in ()))
        {
          this->lifespan_ = lifespan->value (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK;

          return;
        }
    #endif

    #if !defined (CORBA_E_MICRO)
      ::PortableServer::IdUniquenessPolicy_var id_uniqueness
        = ::PortableServer::IdUniquenessPolicy::_narrow (policy
                                                         ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      if (!CORBA::is_nil (id_uniqueness.in ()))
        {
          this->id_uniqueness_ = id_uniqueness->value (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK;

          return;
        }
    #endif

    #if !defined (CORBA_E_MICRO)
      ::PortableServer::IdAssignmentPolicy_var id_assignment
        = ::PortableServer::IdAssignmentPolicy::_narrow (policy
                                                         ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      if (!CORBA::is_nil (id_assignment.in ()))
        {
          this->id_assignment_ = id_assignment->value (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK;

          return;
        }
    #endif

    #if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

      ::PortableServer::ImplicitActivationPolicy_var implicit_activation
        = ::PortableServer::ImplicitActivationPolicy::_narrow (policy
                                                               ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      if (!CORBA::is_nil (implicit_activation.in ()))
        {
          this->implicit_activation_ = implicit_activation->value (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK;

          return;
        }

      ::PortableServer::ServantRetentionPolicy_var servant_retention
        = ::PortableServer::ServantRetentionPolicy::_narrow (policy
                                                             ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      if (!CORBA::is_nil (servant_retention.in ()))
        {
          this->servant_retention_ = servant_retention->value (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK;

          return;
        }

      ::PortableServer::RequestProcessingPolicy_var request_processing
        = ::PortableServer::RequestProcessingPolicy::_narrow (policy
                                                              ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      if (!CORBA::is_nil (request_processing.in ()))
        {
          this->request_processing_ = request_processing->value (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK;

          return;
        }

    #endif /* TAO_HAS_MINIMUM_POA == 0 */

    #if defined (CORBA_E_MICRO)
      ACE_UNUSED_ARG (policy);
    #endif
    }
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
