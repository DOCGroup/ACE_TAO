// -*- C++ -*-

// $Id$

#include "LoadBalancer_i.h"
#include "ReplicaProxy.h"

ACE_RCSID(orbsvcs, LoadBalancer, "$Id$")

#if !defined (__ACE_INLINE__)
#include "LoadBalancer_i.i"
#endif /* __ACE_INLINE__ */

TAO_LB_LoadBalancer::TAO_LB_LoadBalancer (
     const char * interface_id,
     TAO_LB_LoadBalancing_Strategy *strategy,
     PortableServer::POA_ptr root_poa)
  : locator_ (this),
    strategy_ (strategy),
    poa_ ()
{
  (void) this->init (interface_id, root_poa);
}

TAO_LB_LoadBalancer::~TAO_LB_LoadBalancer (void)
{
  // Nothing else
}

LoadBalancing::ReplicaProxy_ptr
TAO_LB_LoadBalancer::connect (LoadBalancing::ReplicaControl_ptr control,
                              CORBA::Object_ptr replica
                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((LoadBalancing::ReplicaProxy::NilControl,
                   LoadBalancing::ReplicaProxy::NilReplica,
                   CORBA::SystemException))
{
  TAO_LB_ReplicaProxy *proxy = 0;
  ACE_NEW_THROW_EX (proxy,
                    TAO_LB_ReplicaProxy,
                    CORBA::NO_MEMORY (
                      CORBA_SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (LoadBalancing::ReplicaProxy::_nil ());

  PortableServer::ServantBase_var proxy_servant = proxy;

  proxy->connect (this, control, replica ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (LoadBalancing::ReplicaProxy::_nil ());

  if (this->strategy_->insert (proxy) == -1)
    {
      ACE_THROW_RETURN (CORBA::INTERNAL (),
                        LoadBalancing::ReplicaProxy::_nil ());
    }

  return proxy->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::Object_ptr
TAO_LB_LoadBalancer::group_identity (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::Object::_duplicate (this->group_identity_.in ());
}

void
TAO_LB_LoadBalancer::load_changed (TAO_LB_ReplicaProxy *proxy
                                   ACE_ENV_ARG_DECL)
{
  this->strategy_->load_changed (proxy ACE_ENV_ARG_PARAMETER);
}

int
TAO_LB_LoadBalancer::init (const char * repository_id,
                           PortableServer::POA_ptr root_poa)
{
  ACE_TRY_NEW_ENV
    {
      // Create the appropriate RequestProcessingPolicy
      // (USE_SERVANT_MANAGER) and ServantRetentionPolicy (NON_RETAIN)
      // for a ServantLocator.
      PortableServer::RequestProcessingPolicy_var request =
        root_poa->create_request_processing_policy (
          PortableServer::USE_SERVANT_MANAGER
          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::ServantRetentionPolicy_var retention =
        root_poa->create_servant_retention_policy (
          PortableServer::NON_RETAIN
          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Create the PolicyList.
      CORBA::PolicyList policy_list;
      policy_list.length (2);
      policy_list[0] =
        PortableServer::RequestProcessingPolicy::_duplicate (
          request.in ());
      policy_list[1] =
        PortableServer::ServantRetentionPolicy::_duplicate (
           retention. in ());

      // Create the child POA with the ServantManager (ReplicaLocator)
      // above policies.
      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      this->poa_ = root_poa->create_POA ("TAO_LB_ReplicaLocator_POA",
                                         poa_manager.in (),
                                         policy_list
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Activate the child POA.
      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      request->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      retention->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Now set the ReplicaLocator as the child POA's Servant
      // Manager.
      this->poa_->set_servant_manager (&this->locator_
                                       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // @@ What ObjectId should be used?
      PortableServer::ObjectId_var oid =
        PortableServer::string_to_ObjectId ("TAO_LB_ObjectGroup");

      this->group_identity_ =
        this->poa_->create_reference_with_id (oid.in (),
                                              repository_id
                                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // @@ Should we do anything here?

      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "(%P|%t) Load Balancer initialization:");

      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
