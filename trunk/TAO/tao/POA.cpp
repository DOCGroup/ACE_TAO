// @(#) $Id$

// auto_ptr class
#include "ace/Auto_Ptr.h"

#include "tao/POA.h"
#include "tao/ORB_Core.h"
#include "tao/Server_Request.h"
#include "tao/Server_Strategy_Factory.h"

// Forwarding Servant class
#include "tao/Forwarding_Servant.h"

#if !defined (__ACE_INLINE__)
# include "tao/POA.i"
#endif /* ! __ACE_INLINE__ */

// Timeprobes class
#include "tao/Timeprobe.h"

ACE_RCSID(tao, POA, "$Id$")

#if defined (ACE_ENABLE_TIMEPROBES)

static const char *TAO_POA_Timeprobe_Description[] =
{
  "POA::locate_poa_i - start",
  "POA::locate_poa_i - end",

  "POA::locate_poa_and_servant_i - start",
  "POA::locate_poa_and_servant_i - end",

  "POA::find_servant - start",
  "POA::find_servant - end",

  "POA::dispatch_servant - start",
  "POA::dispatch_servant - end",

  "POA::parse_key - start",
  "POA::parse_key - end",

  "Servant::_dispatch - start",
  "Servant::_dispatch - end",
};

enum
{
  // Timeprobe description table start key
  TAO_POA_LOCATE_POA_I_START = 200,
  TAO_POA_LOCATE_POA_I_END,

  TAO_POA_LOCATE_POA_AND_SERVANT_I_START,
  TAO_POA_LOCATE_POA_AND_SERVANT_I_END,

  TAO_POA_FIND_SERVANT_START,
  TAO_POA_FIND_SERVANT_END,

  TAO_POA_DISPATCH_SERVANT_START,
  TAO_POA_DISPATCH_SERVANT_END,

  TAO_POA_PARSE_KEY_START,
  TAO_POA_PARSE_KEY_END,

  TAO_SERVANT_DISPATCH_START,
  TAO_SERVANT_DISPATCH_END
};

// Setup Timeprobes
ACE_TIMEPROBE_EVENT_DESCRIPTIONS (TAO_POA_Timeprobe_Description,
                                  TAO_POA_LOCATE_POA_I_START);

#endif /* ACE_ENABLE_TIMEPROBES */

TAO_Thread_Policy::TAO_Thread_Policy (PortableServer::ThreadPolicyValue value,
                                      PortableServer::POA_ptr poa)
  : value_ (value),
    poa_ (PortableServer::POA::_duplicate (poa))
{
}

TAO_Thread_Policy::TAO_Thread_Policy (const TAO_Thread_Policy &rhs)
  : value_ (rhs.value_),
    poa_ (PortableServer::POA::_duplicate (rhs.poa_.in ()))
{
}

PortableServer::ThreadPolicyValue
TAO_Thread_Policy::value (CORBA::Environment &)
{
  return this->value_;
}

CORBA::Policy_ptr
TAO_Thread_Policy::copy (CORBA::Environment &ACE_TRY_ENV)
{
  auto_ptr<TAO_Thread_Policy> new_policy (new TAO_Thread_Policy (*this));

  CORBA::Policy_var result = new_policy->_this (ACE_TRY_ENV);

  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  // Make sure that the auto_ptr does not delete the
  // implementation object
  new_policy.release ();
  return result._retn ();
}

void
TAO_Thread_Policy::destroy (CORBA::Environment &ACE_TRY_ENV)
{
  // Remove self from POA
  //
  // Note that there is no real error checking here as we can't do
  // much about errors here anyway
  //
  ACE_TRY
    {
      PortableServer::POA_var poa = this->_default_POA (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::ObjectId_var id = poa->servant_to_id (this, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      poa->deactivate_object (id.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      delete this;
      return;
    }
  ACE_ENDTRY;

  // Commit suicide: must have been dynamically allocated
  delete this;
}

CORBA::PolicyType
TAO_Thread_Policy::policy_type (CORBA::Environment &)
{
  return 0;
}

/*
PortableServer::POA_ptr
TAO_Thread_Policy::_default_POA (CORBA::Environment &env)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}
*/

TAO_Lifespan_Policy::TAO_Lifespan_Policy (PortableServer::LifespanPolicyValue value,
                                          PortableServer::POA_ptr poa)
  : value_ (value),
    poa_ (PortableServer::POA::_duplicate (poa))
{
}

TAO_Lifespan_Policy::TAO_Lifespan_Policy (const TAO_Lifespan_Policy &rhs)
  : value_ (rhs.value_),
    poa_ (PortableServer::POA::_duplicate (rhs.poa_.in ()))
{
}

PortableServer::LifespanPolicyValue
TAO_Lifespan_Policy::value (CORBA::Environment &)
{
  return this->value_;
}

CORBA::Policy_ptr
TAO_Lifespan_Policy::copy (CORBA::Environment &ACE_TRY_ENV)
{
  auto_ptr<TAO_Lifespan_Policy> new_policy (new TAO_Lifespan_Policy (*this));

  CORBA::Policy_var result = new_policy->_this (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  // Make sure that the auto_ptr does not delete the
  // implementation object
  new_policy.release ();
  return result._retn ();
}

void
TAO_Lifespan_Policy::destroy (CORBA::Environment &ACE_TRY_ENV)
{
  // Remove self from POA
  //
  // Note that there is no real error checking here as we can't do
  // much about errors here anyway
  //
  ACE_TRY
    {
      PortableServer::POA_var poa = this->_default_POA (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::ObjectId_var id = poa->servant_to_id (this, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      poa->deactivate_object (id.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      delete this;
      return;
    }
  ACE_ENDTRY;

  // Commit suicide: must have been dynamically allocated
  delete this;
}

CORBA::PolicyType
TAO_Lifespan_Policy::policy_type (CORBA::Environment &)
{
  return 0;
}

/*
PortableServer::POA_ptr
TAO_Lifespan_Policy::_default_POA (CORBA::Environment &env)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}
*/

TAO_Id_Uniqueness_Policy::TAO_Id_Uniqueness_Policy (PortableServer::IdUniquenessPolicyValue value,
                                                    PortableServer::POA_ptr poa)
  : value_ (value),
    poa_ (PortableServer::POA::_duplicate (poa))
{
}

TAO_Id_Uniqueness_Policy::TAO_Id_Uniqueness_Policy (const TAO_Id_Uniqueness_Policy &rhs)
  : value_ (rhs.value_),
    poa_ (PortableServer::POA::_duplicate (rhs.poa_.in ()))
{
}

PortableServer::IdUniquenessPolicyValue
TAO_Id_Uniqueness_Policy::value (CORBA::Environment &)
{
  return this->value_;
}

CORBA::Policy_ptr
TAO_Id_Uniqueness_Policy::copy (CORBA::Environment &ACE_TRY_ENV)
{
  auto_ptr<TAO_Id_Uniqueness_Policy> new_policy (new TAO_Id_Uniqueness_Policy (*this));

  CORBA::Policy_var result = new_policy->_this (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  // Make sure that the auto_ptr does not delete the
  // implementation object
  new_policy.release ();
  return result._retn ();
}

void
TAO_Id_Uniqueness_Policy::destroy (CORBA::Environment &ACE_TRY_ENV)
{
  // Remove self from POA
  //
  // Note that there is no real error checking here as we can't do
  // much about errors here anyway
  //
  ACE_TRY
    {
      PortableServer::POA_var poa = this->_default_POA (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::ObjectId_var id = poa->servant_to_id (this,
                                                            ACE_TRY_ENV);
      ACE_TRY_CHECK;

      poa->deactivate_object (id.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      delete this;
      return;
    }
  ACE_ENDTRY;

  // Commit suicide: must have been dynamically allocated
  delete this;
}

CORBA::PolicyType
TAO_Id_Uniqueness_Policy::policy_type (CORBA::Environment &)
{
  return 0;
}

/*
PortableServer::POA_ptr
TAO_Id_Uniqueness_Policy::_default_POA (CORBA::Environment &env)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}
*/

TAO_Id_Assignment_Policy::TAO_Id_Assignment_Policy (PortableServer::IdAssignmentPolicyValue value,
                                                    PortableServer::POA_ptr poa)
  : value_ (value),
    poa_ (PortableServer::POA::_duplicate (poa))
{
}

TAO_Id_Assignment_Policy::TAO_Id_Assignment_Policy (const TAO_Id_Assignment_Policy &rhs)
  : value_ (rhs.value_),
    poa_ (PortableServer::POA::_duplicate (rhs.poa_.in ()))
{
}

PortableServer::IdAssignmentPolicyValue
TAO_Id_Assignment_Policy::value (CORBA::Environment &)
{
  return this->value_;
}

CORBA::Policy_ptr
TAO_Id_Assignment_Policy::copy (CORBA::Environment &ACE_TRY_ENV)
{
  auto_ptr<TAO_Id_Assignment_Policy> new_policy (new TAO_Id_Assignment_Policy (*this));

  CORBA::Policy_var result = new_policy->_this (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  // Make sure that the auto_ptr does not delete the
  // implementation object
  new_policy.release ();
  return result._retn ();
}

void
TAO_Id_Assignment_Policy::destroy (CORBA::Environment &ACE_TRY_ENV)
{
  // Remove self from POA
  //
  // Note that there is no real error checking here as we can't do
  // much about errors here anyway
  //
  ACE_TRY
    {
      PortableServer::POA_var poa = this->_default_POA (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::ObjectId_var id = poa->servant_to_id (this, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      poa->deactivate_object (id.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      delete this;
      return;
    }
  ACE_ENDTRY;

  // Commit suicide: must have been dynamically allocated
  delete this;
}

CORBA::PolicyType
TAO_Id_Assignment_Policy::policy_type (CORBA::Environment &)
{
  return 0;
}

/*
PortableServer::POA_ptr
TAO_Id_Assignment_Policy::_default_POA (CORBA::Environment &env)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}
*/

TAO_Implicit_Activation_Policy::TAO_Implicit_Activation_Policy (PortableServer::ImplicitActivationPolicyValue value,
                                                                PortableServer::POA_ptr poa)
  : value_ (value),
    poa_ (PortableServer::POA::_duplicate (poa))
{
}

TAO_Implicit_Activation_Policy::TAO_Implicit_Activation_Policy (const TAO_Implicit_Activation_Policy &rhs)
  : value_ (rhs.value_),
    poa_ (PortableServer::POA::_duplicate (rhs.poa_.in ()))
{
}

PortableServer::ImplicitActivationPolicyValue
TAO_Implicit_Activation_Policy::value (CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);
  return this->value_;
}

CORBA::Policy_ptr
TAO_Implicit_Activation_Policy::copy (CORBA::Environment &env)
{
  auto_ptr<TAO_Implicit_Activation_Policy> new_policy (new TAO_Implicit_Activation_Policy (*this));

  CORBA::Policy_var result = new_policy->_this (env);
  if (env.exception () != 0)
    return CORBA::Policy::_nil ();
  else
    {
      // Make sure that the auto_ptr does not delete the
      // implementation object
      new_policy.release ();
      return result._retn ();
    }
}

void
TAO_Implicit_Activation_Policy::destroy (CORBA::Environment &env)
{
  // Remove self from POA
  //
  // Note that there is no real error checking here as we can't do
  // much about errors here anyway
  //
  PortableServer::POA_var poa = this->_default_POA (env);
  if (env.exception () == 0)
    {
      PortableServer::ObjectId_var id = poa->servant_to_id (this, env);
      if (env.exception () == 0)
        poa->deactivate_object (id.in (), env);
    }

  // Commit suicide: must have been dynamically allocated
  delete this;
}

CORBA::PolicyType
TAO_Implicit_Activation_Policy::policy_type (CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);
  return 0;
}

/*
PortableServer::POA_ptr
TAO_Implicit_Activation_Policy::_default_POA (CORBA::Environment &env)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}
*/

TAO_Servant_Retention_Policy::TAO_Servant_Retention_Policy (PortableServer::ServantRetentionPolicyValue value,
                                                            PortableServer::POA_ptr poa)
  : value_ (value),
    poa_ (PortableServer::POA::_duplicate (poa))
{
}

TAO_Servant_Retention_Policy::TAO_Servant_Retention_Policy (const TAO_Servant_Retention_Policy &rhs)
  : value_ (rhs.value_),
    poa_ (PortableServer::POA::_duplicate (rhs.poa_.in ()))
{
}

PortableServer::ServantRetentionPolicyValue
TAO_Servant_Retention_Policy::value (CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);
  return this->value_;
}

CORBA::Policy_ptr
TAO_Servant_Retention_Policy::copy (CORBA::Environment &env)
{
  auto_ptr<TAO_Servant_Retention_Policy> new_policy (new TAO_Servant_Retention_Policy (*this));

  CORBA::Policy_var result = new_policy->_this (env);
  if (env.exception () != 0)
    return CORBA::Policy::_nil ();
  else
    {
      // Make sure that the auto_ptr does not delete the
      // implementation object
      new_policy.release ();
      return result._retn ();
    }
}

void
TAO_Servant_Retention_Policy::destroy (CORBA::Environment &env)
{
  // Remove self from POA
  //
  // Note that there is no real error checking here as we can't do
  // much about errors here anyway
  //
  PortableServer::POA_var poa = this->_default_POA (env);
  if (env.exception () == 0)
    {
      PortableServer::ObjectId_var id = poa->servant_to_id (this, env);
      if (env.exception () == 0)
        poa->deactivate_object (id.in (), env);
    }

  // Commit suicide: must have been dynamically allocated
  delete this;
}

CORBA::PolicyType
TAO_Servant_Retention_Policy::policy_type (CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);
  return 0;
}

/*
PortableServer::POA_ptr
TAO_Servant_Retention_Policy::_default_POA (CORBA::Environment &env)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}
*/

TAO_Request_Processing_Policy::TAO_Request_Processing_Policy (PortableServer::RequestProcessingPolicyValue value,
                                                              PortableServer::POA_ptr poa)
  : value_ (value),
    poa_ (PortableServer::POA::_duplicate (poa))
{
}

TAO_Request_Processing_Policy::TAO_Request_Processing_Policy (const TAO_Request_Processing_Policy &rhs)
  : value_ (rhs.value_),
    poa_ (PortableServer::POA::_duplicate (rhs.poa_.in ()))
{
}

PortableServer::RequestProcessingPolicyValue
TAO_Request_Processing_Policy::value (CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);
  return this->value_;
}

CORBA::Policy_ptr
TAO_Request_Processing_Policy::copy (CORBA::Environment &env)
{
  auto_ptr<TAO_Request_Processing_Policy> new_policy (new TAO_Request_Processing_Policy (*this));

  CORBA::Policy_var result = new_policy->_this (env);
  if (env.exception () != 0)
    return CORBA::Policy::_nil ();
  else
    {
      // Make sure that the auto_ptr does not delete the
      // implementation object
      new_policy.release ();
      return result._retn ();
    }
}

void
TAO_Request_Processing_Policy::destroy (CORBA::Environment &env)
{
  // Remove self from POA
  //
  // Note that there is no real error checking here as we can't do
  // much about errors here anyway
  //
  PortableServer::POA_var poa = this->_default_POA (env);
  if (env.exception () == 0)
    {
      PortableServer::ObjectId_var id = poa->servant_to_id (this, env);
      if (env.exception () == 0)
        poa->deactivate_object (id.in (), env);
    }

  // Commit suicide: must have been dynamically allocated
  delete this;
}

CORBA::PolicyType
TAO_Request_Processing_Policy::policy_type (CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);
  return 0;
}

/*
PortableServer::POA_ptr
TAO_Request_Processing_Policy::_default_POA (CORBA::Environment &env)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}
*/

// **************************************************
//
// TAO spcific POA locking policy (non-standard)
//
// **************************************************

TAO_Synchronization_Policy::TAO_Synchronization_Policy (PortableServer::SynchronizationPolicyValue value,
                                                        PortableServer::POA_ptr poa)
  : value_ (value),
    poa_ (PortableServer::POA::_duplicate (poa))
{
}

TAO_Synchronization_Policy::TAO_Synchronization_Policy (const TAO_Synchronization_Policy &rhs)
  : value_ (rhs.value_),
    poa_ (PortableServer::POA::_duplicate (rhs.poa_.in ()))
{
}

PortableServer::SynchronizationPolicyValue
TAO_Synchronization_Policy::value (CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);
  return this->value_;
}

CORBA::Policy_ptr
TAO_Synchronization_Policy::copy (CORBA::Environment &env)
{
  auto_ptr<TAO_Synchronization_Policy> new_policy (new TAO_Synchronization_Policy (*this));

  CORBA::Policy_var result = new_policy->_this (env);
  if (env.exception () != 0)
    return CORBA::Policy::_nil ();
  else
    {
      // Make sure that the auto_ptr does not delete the
      // implementation object
      new_policy.release ();
      return result._retn ();
    }
}

void
TAO_Synchronization_Policy::destroy (CORBA::Environment &env)
{
  // Remove self from POA
  //
  // Note that there is no real error checking here as we can't do
  // much about errors here anyway
  //
  PortableServer::POA_var poa = this->_default_POA (env);
  if (env.exception () == 0)
    {
      PortableServer::ObjectId_var id = poa->servant_to_id (this, env);
      if (env.exception () == 0)
        poa->deactivate_object (id.in (), env);
    }

  // Commit suicide: must have been dynamically allocated
  delete this;
}

CORBA::PolicyType
TAO_Synchronization_Policy::policy_type (CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);
  return 0;
}

/*
PortableServer::POA_ptr
TAO_Synchronization_Policy::_default_POA (CORBA::Environment &env)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}
*/

TAO_POA_Policies::TAO_POA_Policies (void)
  :  thread_ (PortableServer::ORB_CTRL_MODEL),
     lifespan_ (PortableServer::TRANSIENT),
     id_uniqueness_ (PortableServer::UNIQUE_ID),
     id_assignment_ (PortableServer::SYSTEM_ID),
     implicit_activation_ (PortableServer::NO_IMPLICIT_ACTIVATION),
     servant_retention_ (PortableServer::RETAIN),
     request_processing_ (PortableServer::USE_ACTIVE_OBJECT_MAP_ONLY),
     synchronization_ (PortableServer::DEFAULT_LOCK)
{
}

void
TAO_POA_Policies::parse_policies (const CORBA::PolicyList &policies,
                                  CORBA::Environment &env)
{
  for (CORBA::ULong i = 0;
       i < policies.length () && env.exception () == 0;
       i++)
    {
      this->parse_policy (policies[i], env);
    }

  if (this->validity_check () == -1)
    {
      CORBA::Exception *exception = new PortableServer::POA::InvalidPolicy;
      env.exception (exception);
      return;
    }
}

int
TAO_POA_Policies::validity_check (void)
{
  // The NON_RETAIN policy requires either the USE_DEFAULT_SERVANT or
  // USE_SERVANT_MANAGER policies.
  if (this->servant_retention_ == PortableServer::NON_RETAIN)
    if (this->request_processing_ != PortableServer::USE_SERVANT_MANAGER &&
        this->request_processing_ != PortableServer::USE_DEFAULT_SERVANT)
      return -1;

  // USE_ACTIVE_OBJECT_MAP_ONLY requires the RETAIN policy.
  if (this->request_processing_ == PortableServer::USE_ACTIVE_OBJECT_MAP_ONLY)
    if (this->servant_retention_ != PortableServer::RETAIN)
      return -1;

  // USE_DEFAULT_SERVANT requires the MULTIPLE_ID policy.
  if (this->request_processing_ == PortableServer::USE_DEFAULT_SERVANT)
    if (this->id_uniqueness_ != PortableServer::MULTIPLE_ID)
      return -1;

  // IMPLICIT_ACTIVATION requires the SYSTEM_ID and RETAIN policies.
  if (this->implicit_activation_ == PortableServer::IMPLICIT_ACTIVATION)
    if (this->servant_retention_ != PortableServer::RETAIN ||
        this->id_assignment_ != PortableServer::SYSTEM_ID)
      return -1;

  return 0;
}

void
TAO_POA_Policies::parse_policy (const CORBA::Policy_ptr policy,
                                CORBA::Environment &env)
{
  PortableServer::ThreadPolicy_var thread
    = PortableServer::ThreadPolicy::_narrow (policy, env);
  if (!CORBA::is_nil (thread.in ()))
    {
      this->thread_ = thread->value (env);
      return;
    }
  else
    {
      env.clear ();
    }

  PortableServer::LifespanPolicy_var lifespan
    = PortableServer::LifespanPolicy::_narrow (policy, env);
  if (!CORBA::is_nil (lifespan.in ()))
    {
      this->lifespan_ = lifespan->value (env);
      return;
    }
  else
    {
      env.clear ();
    }

  PortableServer::IdUniquenessPolicy_var id_uniqueness
    = PortableServer::IdUniquenessPolicy::_narrow (policy, env);
  if (!CORBA::is_nil (id_uniqueness.in ()))
    {
      this->id_uniqueness_ = id_uniqueness->value (env);
      return;
    }
  else
    {
      env.clear ();
    }

  PortableServer::IdAssignmentPolicy_var id_assignment
    = PortableServer::IdAssignmentPolicy::_narrow (policy, env);
  if (!CORBA::is_nil (id_assignment.in ()))
    {
      this->id_assignment_ = id_assignment->value (env);
      return;
    }
  else
    {
      env.clear ();
    }

  PortableServer::ImplicitActivationPolicy_var implicit_activation
    = PortableServer::ImplicitActivationPolicy::_narrow (policy, env);
  if (!CORBA::is_nil (implicit_activation.in ()))
    {
      this->implicit_activation_ = implicit_activation->value (env);
      return;
    }
  else
    {
      env.clear ();
    }

  PortableServer::ServantRetentionPolicy_var servant_retention
    = PortableServer::ServantRetentionPolicy::_narrow (policy, env);
  if (!CORBA::is_nil (servant_retention.in ()))
    {
      this->servant_retention_ = servant_retention->value (env);
      return;
    }
  else
    {
      env.clear ();
    }

  PortableServer::RequestProcessingPolicy_var request_processing
    = PortableServer::RequestProcessingPolicy::_narrow (policy, env);
  if (!CORBA::is_nil (request_processing.in ()))
    {
      this->request_processing_ = request_processing->value (env);
      return;
    }
  else
    {
      env.clear ();
    }

  PortableServer::SynchronizationPolicy_var synchronization
    = PortableServer::SynchronizationPolicy::_narrow (policy, env);
  if (!CORBA::is_nil (synchronization.in ()))
    {
      this->synchronization_ = synchronization->value (env);
      return;
    }
  else
    {
      env.clear ();
    }

  CORBA::Exception *exception = new PortableServer::POA::InvalidPolicy;
  env.exception (exception);
  return;
}

TAO_POA::TAO_POA (const TAO_POA::String &adapter_name,
                  TAO_POA_Manager &poa_manager,
                  const TAO_POA_Policies &policies,
                  TAO_POA *parent,
                  CORBA::Environment &env)
  : name_ (adapter_name),
    poa_manager_ (poa_manager),
    policies_ (policies),
    parent_ (parent),
    active_object_map_ (0),
    adapter_activator_ (),
    servant_activator_ (),
    servant_locator_ (),
    default_servant_ (0),
    children_ (),
    lock_ (0),
    closing_down_ (0),
    persistent_ (policies.lifespan () == PortableServer::PERSISTENT),
    system_id_ (policies.id_assignment () == PortableServer::SYSTEM_ID),
    creation_time_ (ACE_OS::gettimeofday ())
{
  // Create a lock for ourselves
  this->create_internal_lock ();

  // Set the complete name of this POA
  this->set_complete_name ();

  // Register self with manager
  this->poa_manager_.register_poa (this, env);

  // Create the active object map
  this->create_active_object_map ();
}

void
TAO_POA::create_active_object_map (void)
{
  int user_id_policy = !this->system_id ();
  int unique_id_policy = this->policies ().id_uniqueness () == PortableServer::UNIQUE_ID;

  // Create the active_object_map
  ACE_NEW (this->active_object_map_,
           TAO_Active_Object_Map (user_id_policy,
                                  unique_id_policy));
}

void
TAO_POA::create_internal_lock (void)
{
  switch (this->policies ().synchronization ())
    {
    case PortableServer::DEFAULT_LOCK:
      // Ask the server factory to create the lock.  svc.conf will be
      // consulted. If the user did not specify any preference in
      // svc.conf, THREAD_LOCK will be the default.
      this->lock_ = TAO_ORB_Core_instance ()->server_factory ()->create_poa_lock ();
      break;

    case PortableServer::THREAD_LOCK:
#if defined (ACE_HAS_THREADS)
      // Thread lock
      ACE_NEW (this->lock_, ACE_Lock_Adapter<ACE_Recursive_Thread_Mutex>);
      break;
#else
      /* FALL THROUGH */
#endif /* ACE_HAS_THREADS */

    case PortableServer::NULL_LOCK:
      // Null lock
      ACE_NEW (this->lock_, ACE_Lock_Adapter<ACE_Null_Mutex>);
      break;
    }
}

TAO_POA::~TAO_POA (void)
{
  // Delete the active object map
  delete this->active_object_map_;

  // Delete the dynamically allocated lock
  delete this->lock_;

  // Remove POA from the POAManager
  //
  // Note: Errors are ignored here since there is nothing we can do
  // about them
  //
  CORBA::Environment env;
  this->poa_manager_.remove_poa (this, env);
}

PortableServer::POA_ptr
TAO_POA::create_POA (const char *adapter_name,
                     PortableServer::POAManager_ptr poa_manager,
                     const CORBA::PolicyList &policies,
                     CORBA::Environment &env)
{
  // If any of the policy objects specified are not valid for the ORB
  // implementation, if conflicting policy objects are specified, or
  // if any of the specified policy objects require prior
  // administrative action that has not been performed, an
  // InvalidPolicy exception is raised containing the index in the
  // policies parameter value of the first offending policy object.
  TAO_POA_Policies tao_policies;
  tao_policies.parse_policies (policies, env);
  if (env.exception () != 0)
    return PortableServer::POA::_nil ();

  // If the poa_manager parameter is null, a new POAManager object is
  // created and associated with the new POA. Otherwise, the specified
  // POAManager object is associated with the new POA. The POAManager
  // object can be obtained using the attribute name the_POAManager.
  TAO_POA_Manager *poa_manager_impl = 0;
  if (CORBA::is_nil (poa_manager))
    {
      poa_manager_impl = this->poa_manager_.clone ();
    }
  else
    {
      PortableServer::Servant servant = poa_manager->_servant ();
      if (servant == 0)
        {
          CORBA::Exception *exception = new CORBA::OBJ_ADAPTER (CORBA::COMPLETED_NO);
          env.exception (exception);
          return PortableServer::POA::_nil ();
        }

      void *ptr = servant->_downcast (servant->_interface_repository_id ());
      POA_PortableServer::POAManager *mgr = (POA_PortableServer::POAManager *) ptr;
      poa_manager_impl = ACE_dynamic_cast (TAO_POA_Manager *, mgr);
    }

  TAO_POA *result = this->create_POA (adapter_name,
                                      *poa_manager_impl,
                                      tao_policies,
                                      env);
  if (env.exception () != 0)
    return PortableServer::POA::_nil ();

  return result->_this (env);
}

TAO_POA *
TAO_POA::create_POA_i (const TAO_POA::String &adapter_name,
                       TAO_POA_Manager &poa_manager,
                       const TAO_POA_Policies &policies,
                       CORBA::Environment &env)
{
  // If this is the leaf POA name
  if (this->leaf_poa_name (adapter_name, env))
    {
      // This operaton creates a new POA as a child of the target POA. The
      // specified name identifies the new POA with respect to other POAs
      // with the same parent POA. If the target POA already has a child
      // POA with the specified name, the AdapterAlreadyExists exception
      // is raised.
      int result = this->children_.find (adapter_name);

      // Child was found
      if (result != -1)
        {
          CORBA::Exception *exception = new PortableServer::POA::AdapterAlreadyExists;
          env.exception (exception);
          return 0;
        }

      //
      // Child was not found
      //

      // The specified policy objects are associated with the POA and used
      // to control its behavior. The policy objects are effectively
      // copied before this operation returns, so the application is free
      // to destroy them while the POA is in use. Policies are not
      // inherited from the parent POA.
      auto_ptr<TAO_POA> new_poa (this->clone (adapter_name,
                                              poa_manager,
                                              policies,
                                              this,
                                              env));

      if (env.exception () != 0)
        return 0;

      // Add to children map
      if (this->children_.bind (adapter_name, new_poa.get ()) != 0)
        {
          CORBA::Exception *exception = new CORBA::OBJ_ADAPTER (CORBA::COMPLETED_NO);
          env.exception (exception);
          return 0;
        }

      // Note: Creating a POA using a POA manager that is in the active
      // state can lead to race conditions if the POA supports preexisting
      // objects, because the new POA may receive a request before its
      // adapter activator, servant manager, or default servant have been
      // initialized. These problems do not occur if the POA is created by
      // an adapter activator registered with a parent of the new POA,
      // because requests are queued until the adapter activator
      // returns. To avoid these problems when a POA must be explicitly
      // initialized, the application can initialize the POA by invoking
      // find_POA with a TRUE activate parameter.

      // Everything is fine
      // Don't let the auto_ptr delete the implementation
      return new_poa.release ();
    }
  // Not the leaf POA name
  else
    {
      // Find the topmost name
      TAO_POA::String topmost_poa_name;
      TAO_POA::String tail_poa_name;
      this->parse_poa_name (adapter_name,
                            topmost_poa_name,
                            tail_poa_name,
                            env);

      if (env.exception () != 0)
        return 0;

      // If we are the topmost poa, let's create the tail
      if (topmost_poa_name == this->name_)
        {
          return this->create_POA_i (tail_poa_name,
                                     poa_manager,
                                     policies,
                                     env);
        }

      //
      // We are not the topmost POA
      //

      // Try to find the topmost child
      TAO_POA *child_poa = 0;
      int result = this->children_.find (topmost_poa_name, child_poa);

      // Child was not found or the topmost is us
      if (result != 0)
        {
          child_poa = this->create_POA_i (topmost_poa_name,
                                          poa_manager,
                                          policies,
                                          env);
          if (env.exception () != 0)
            return 0;
        }

      // At this point, the intermediate child POA was either found or
      // created
      return child_poa->create_POA (tail_poa_name,
                                    poa_manager,
                                    policies,
                                    env);
    }
}

TAO_POA *
TAO_POA::find_POA_i_optimized (const TAO_POA::String &adapter_name,
                               CORBA::Boolean activate_it,
                               CORBA::Environment &env)
{
  // If this is the leaf POA name
  if (this->leaf_poa_name (adapter_name, env))
    {
      // If the target POA is the parent of a child POA with the
      // specified name (relative to the target POA), that child POA
      // is returned.
      TAO_POA *child_poa = 0;
      int result = this->children_.find (adapter_name, child_poa);

      // Child was found
      if (result != -1)
        return child_poa;

      //
      // Child was not found
      //

      // If a child POA with the specified name does not exist and the
      // value of the activate_it parameter is TRUE, the target POA's
      // AdapterActivator, if one exists, is invoked, and, if it
      // successfully activates the child POA, that child POA is
      // returned.
      if (activate_it && !CORBA::is_nil (this->adapter_activator_.in ()))
        {
          PortableServer::POA_var self = this->_this (env);
          // Check for exceptions
          if (env.exception () != 0)
            return 0;

          ACE_CString null_terminated_adapter_name (adapter_name);

          CORBA::Boolean success =
            this->adapter_activator_->unknown_adapter (self.in (),
                                                       null_terminated_adapter_name.c_str (),
                                                       env);
          // Check for exceptions
          if (env.exception () != 0)
            return 0;

          // On success
          if (success)
            {
              // Search the children table again
              result = this->children_.find (adapter_name, child_poa);

              // Child was found
              if (result != -1)
                return child_poa;
            }
        }

      // Otherwise, the AdapterNonExistent exception is raised.
      CORBA::Exception *exception = new PortableServer::POA::AdapterNonExistent;
      env.exception (exception);
      return 0;
    }
  // Not the leaf POA name
  else
    {
      // Find the topmost name
      TAO_POA::String topmost_poa_name;
      TAO_POA::String tail_poa_name;
      this->parse_poa_name (adapter_name,
                            topmost_poa_name,
                            tail_poa_name,
                            env);

      if (env.exception () != 0)
        return 0;

      // If we are the topmost poa, let's create the tail
      if (topmost_poa_name == this->name_)
        {
          return this->find_POA_i (tail_poa_name,
                                   activate_it,
                                   env);
        }

      //
      // We are not the topmost POA
      //

      // Try to find the topmost child
      TAO_POA *child_poa;
      int result = this->children_.find (topmost_poa_name, child_poa);

      // Child was not found
      if (result != 0)
        {
          child_poa = this->find_POA_i_optimized (topmost_poa_name,
                                                  activate_it,
                                                  env);
          if (env.exception () != 0)
            return 0;
        }

      // At this point, the intermediate child POA was either found or
      // created
      return child_poa->find_POA (tail_poa_name,
                                  activate_it,
                                  env);
    }
}

void
TAO_POA::destroy_i (CORBA::Boolean etherealize_objects,
                    CORBA::Boolean wait_for_completion,
                    CORBA::Environment &ACE_TRY_ENV)
{
  this->closing_down_ = 1;

  // This operation destroys the POA and all descendant POAs. The POA
  // so destroyed (that is, the POA with its name) may be re-created
  // later in the same process. (This differs from the
  // POAManager::deactivate operation that does not allow a
  // re-creation of its associated POA in the same process.)

  // Remove POA from the parent
  if (this->parent_ != 0)
    this->parent_->delete_child (this->name_, ACE_TRY_ENV);
  ACE_CHECK;

  // Remove all children POAs
  for (CHILDREN::iterator iterator = this->children_.begin ();
       iterator != this->children_.end ();
       ++iterator)
    {
      TAO_POA *child_poa = (*iterator).int_id_;
      child_poa->destroy (etherealize_objects,
                          wait_for_completion,
                          ACE_TRY_ENV);
      ACE_CHECK;
    }

  // When a POA is destroyed, any requests that have started execution
  // continue to completion. Any requests that have not started
  // execution are processed as if they were newly arrived, that is,
  // the POA will attempt to cause recreation of the POA by invoking
  // one or more adapter activators as described in Section 3.3.3.

  // If the etherealize_objects parameter is TRUE, the POA has the
  // RETAIN policy, and a servant manager is registered with the POA,
  // the etherealize operation on the servant manager will be called
  // for each active object in the Active Object Map. The apparent
  // destruction of the POA occurs before any calls to etherealize are
  // made.  Thus, for example, an etherealize method that attempts to
  // invoke operations on the POA will receive the OBJECT_NOT_EXIST
  // exception.

  this->etherealize_servants (etherealize_objects,
                              wait_for_completion,
                              ACE_TRY_ENV);

  // If the wait_for_completion parameter is TRUE, the destroy
  // operation will return only after all requests in process have
  // completed and all invocations of etherealize have
  // completed. Otherwise, the destroy operation returns after
  // destroying the POAs.
}

void
TAO_POA::etherealize_servants (CORBA::Boolean etherealize_objects,
                               CORBA::Boolean wait_for_completion,
                               CORBA::Environment &ACE_TRY_ENV)
{
  // Remove the registered objects
  if (etherealize_objects)
    {
      if (this->policies ().servant_retention () == PortableServer::RETAIN &&
          this->policies ().request_processing () == PortableServer::USE_SERVANT_MANAGER &&
          !CORBA::is_nil (this->servant_activator_.in ()))
        {
          PortableServer::POA_var self = this->_this (ACE_TRY_ENV);
          ACE_CHECK;

          while (1)
            {
              TAO_Active_Object_Map::iterator iterator = this->active_object_map ().begin ();
              if (iterator == this->active_object_map ().end ())
                break;

              PortableServer::Servant servant = 0;
              PortableServer::ObjectId id ((*iterator).id_);

              // Remove from the active object map
              int result = this->active_object_map ().unbind (id, servant);
              if (result != 0)
                ACE_THROW (CORBA::OBJ_ADAPTER (CORBA::COMPLETED_NO));

              CORBA::Boolean remaining_activations = 0;

              // Check for remaining activations
              if (this->policies ().id_uniqueness () == PortableServer::MULTIPLE_ID &&
                  this->active_object_map ().find (servant) != -1)
                remaining_activations = 1;

              this->servant_activator_->etherealize (id,
                                                     self.in (),
                                                     servant,
                                                     1,
                                                     remaining_activations,
                                                     ACE_TRY_ENV);
              ACE_CHECK;
            }
        }
    }

  ACE_UNUSED_ARG (wait_for_completion);
}


void
TAO_POA::delete_child (const TAO_POA::String &child,
                       CORBA::Environment &env)
{
  // If we are not closing down, we must remove this child from our
  // collection.
  if (!this->closing_down_)
    {
      // Lock access to the POA for the duration of this transaction
      TAO_POA_WRITE_GUARD (ACE_Lock, monitor, this->lock (), env);

      this->delete_child_i (child,
                            env);
    }

  // If we are closing down, we are currently iterating over our
  // children and there is not need to remove this child from our
  // collection.
}

void
TAO_POA::delete_child_i (const TAO_POA::String &child,
                         CORBA::Environment &env)
{
  if (this->children_.unbind (child) != 0)
    {
      CORBA::Exception *exception = new CORBA::OBJ_ADAPTER (CORBA::COMPLETED_NO);
      env.exception (exception);
      return;
    }
}

PortableServer::ServantManager_ptr
TAO_POA::get_servant_manager_i (CORBA::Environment &env)
{
  // This operation requires the USE_SERVANT_MANAGER policy; if not
  // present, the WrongPolicy exception is raised.
  if (this->policies ().request_processing () != PortableServer::USE_SERVANT_MANAGER)
    {
      CORBA::Exception *exception = new PortableServer::POA::WrongPolicy;
      env.exception (exception);
      return PortableServer::ServantManager::_nil ();
    }

  // This operation returns the servant manager associated with the
  // POA.  If no servant manager has been associated with the POA, it
  // returns a null reference.
  if (this->policies ().servant_retention () == PortableServer::RETAIN)
    return PortableServer::ServantManager::_duplicate (this->servant_activator_.in ());
  else
    return PortableServer::ServantManager::_duplicate (this->servant_locator_.in ());
}

void
TAO_POA::set_servant_manager_i (PortableServer::ServantManager_ptr imgr,
                                CORBA::Environment &env)
{
  // This operation requires the USE_SERVANT_MANAGER policy; if not
  // present, the WrongPolicy exception is raised.
  if (this->policies ().request_processing () != PortableServer::USE_SERVANT_MANAGER)
    {
      CORBA::Exception *exception = new PortableServer::POA::WrongPolicy;
      env.exception (exception);
      return;
    }

  // This operation sets the default servant manager associated with
  // the POA.
  if (this->policies ().servant_retention () == PortableServer::RETAIN)
    {
      this->servant_activator_ = PortableServer::ServantActivator::_narrow (imgr, env);
      if (CORBA::is_nil (this->servant_activator_.in ()))
        {
          CORBA::Exception *exception = new PortableServer::POA::WrongPolicy;
          env.exception (exception);
          return;
        }
    }
  else
    {
      this->servant_locator_ = PortableServer::ServantLocator::_narrow (imgr, env);
      if (CORBA::is_nil (this->servant_locator_.in ()))
        {
          CORBA::Exception *exception = new PortableServer::POA::WrongPolicy;
          env.exception (exception);
          return;
        }
    }
}

PortableServer::Servant
TAO_POA::get_servant_i (CORBA::Environment &env)
{
  // This operation requires the USE_DEFAULT_SERVANT policy; if not
  // present, the WrongPolicy exception is raised.
  if (this->policies ().request_processing () != PortableServer::USE_DEFAULT_SERVANT)
    {
      CORBA::Exception *exception = new PortableServer::POA::WrongPolicy;
      env.exception (exception);
      return 0;
    }

  // This operation returns the default servant associated with the
  // POA.
  if (this->default_servant_ != 0)
    {
      return this->default_servant_;
    }
  else
    // If no servant has been associated with the POA, the NoServant
    // exception is raised.
    {
      CORBA::Exception *exception = new PortableServer::POA::NoServant;
      env.exception (exception);
      return 0;
    }
}

void
TAO_POA::set_servant_i (PortableServer::Servant servant,
                        CORBA::Environment &env)
{
  // This operation requires the USE_DEFAULT_SERVANT policy; if not
  // present, the WrongPolicy exception is raised.
  if (this->policies ().request_processing () != PortableServer::USE_DEFAULT_SERVANT)
    {
      CORBA::Exception *exception = new PortableServer::POA::WrongPolicy;
      env.exception (exception);
      return;
    }

  // This operation registers the specified servant with the POA as
  // the default servant. This servant will be used for all requests
  // for which no servant is found in the Active Object Map.
  this->default_servant_ = servant;
}

PortableServer::ObjectId *
TAO_POA::activate_object_i (PortableServer::Servant servant,
                            CORBA::Environment &env)
{
  // This operation requires the SYSTEM_ID and RETAIN policy; if not
  // present, the WrongPolicy exception is raised.
  if (!(this->policies ().id_assignment () == PortableServer::SYSTEM_ID &&
        this->policies ().servant_retention () == PortableServer::RETAIN))
    {
      CORBA::Exception *exception = new PortableServer::POA::WrongPolicy;
      env.exception (exception);
      return 0;
    }

  // If the POA has the UNIQUE_ID policy and the specified servant is
  // already in the Active Object Map, the ServantAlreadyActive
  // exception is raised.
  if (this->policies ().id_uniqueness () == PortableServer::UNIQUE_ID &&
      this->active_object_map ().find (servant) != -1)
    {
      CORBA::Exception *exception = new PortableServer::POA::ServantAlreadyActive;
      env.exception (exception);
      return 0;
    }

  // Otherwise, the activate_object operation generates an Object Id
  // and enters the Object Id and the specified servant in the Active
  // Object Map. The Object Id is returned.
  PortableServer::ObjectId_var new_id = this->create_object_id (servant, env);
  if (env.exception () != 0)
    return 0;

  // Add to the active object map
  if (this->active_object_map ().bind (new_id.in (), servant) == -1)
    {
      CORBA::Exception *exception = new CORBA::OBJ_ADAPTER (CORBA::COMPLETED_NO);
      env.exception (exception);
      return 0;
    }

  // Everything is finally ok
  return new_id._retn ();
}

void
TAO_POA::activate_object_with_id_i (const PortableServer::ObjectId &id,
                                    PortableServer::Servant servant,
                                    CORBA::Environment &env)
{
  // This operation requires the RETAIN policy; if not present, the
  // WrongPolicy exception is raised.
  if (this->policies ().servant_retention () != PortableServer::RETAIN)
    {
      CORBA::Exception *exception = new PortableServer::POA::WrongPolicy;
      env.exception (exception);
      return;
    }

  // If the POA has the SYSTEM_ID policy and it detects that the
  // Object Id value was not generated by the system or for this POA,
  // the activate_object_with_id operation may raise the BAD_PARAM
  // system exception.  An ORB is not required to detect all such
  // invalid Object Id values, but a portable application must not
  // invoke activate_object_with_id on a POA that has the SYSTEM_ID
  // policy with an Object Id value that was not previously generated
  // by the system for that POA, or, if the POA also has the
  // PERSISTENT policy, for a previous instantiation of the same POA.
  if (this->policies ().id_assignment () == PortableServer::SYSTEM_ID &&
      !this->is_poa_generated_id (id))
    {
      CORBA::Exception *exception = new CORBA::BAD_PARAM (CORBA::COMPLETED_NO);
      env.exception (exception);
      return;
    }

  // If the CORBA object denoted by the Object Id value is already
  // active in this POA (there is a servant bound to it in the Active
  // Object Map), the ObjectAlreadyActive exception is raised.
  if (this->active_object_map ().find (id) != -1)
    {
      CORBA::Exception *exception = new PortableServer::POA::ObjectAlreadyActive;
      env.exception (exception);
      return;
    }

  // If the POA has the UNIQUE_ID policy and the servant is already in
  // the Active Object Map, the ServantAlreadyActive exception is
  // raised.
  if (this->policies ().id_uniqueness () == PortableServer::UNIQUE_ID &&
      this->active_object_map ().find (servant) != -1)
    {
      CORBA::Exception *exception = new PortableServer::POA::ServantAlreadyActive;
      env.exception (exception);
      return;
    }

  // Otherwise, the activate_object_with_id operation enters an
  // association between the specified Object Id and the specified
  // servant in the Active Object Map.
  if (this->active_object_map ().bind (id, servant) == -1)
    {
      CORBA::Exception *exception = new CORBA::OBJ_ADAPTER (CORBA::COMPLETED_NO);
      env.exception (exception);
      return;
    }

  // Everything is finally ok
}

void
TAO_POA::deactivate_object_i (const PortableServer::ObjectId &oid,
                              CORBA::Environment &env)
{
  // This operation requires the RETAIN policy; if not present, the
  // WrongPolicy exception is raised.
  if (this->policies ().servant_retention () != PortableServer::RETAIN)
    {
      CORBA::Exception *exception = new PortableServer::POA::WrongPolicy;
      env.exception (exception);
      return;
    }

  // This operation causes the association of the Object Id specified
  // by the oid parameter and its servant to be removed from the
  // Active Object Map.
  PortableServer::Servant servant = 0;
  int result = this->active_object_map ().unbind (oid, servant);

  // If there is no active object associated with the specified Object
  // Id, the operation raises an ObjectNotActive exception.
  if (result != 0)
    {
      CORBA::Exception *exception = new PortableServer::POA::ObjectNotActive;
      env.exception (exception);
      return;
    }

  // If a servant manager is associated with the POA,
  // ServantLocator::etherealize will be invoked with the oid and the
  // servant. (The deactivate_object operation does not wait for the
  // etherealize operation to complete before deactivate_object
  // returns.)
  //
  // Note: If the servant associated with the oid is serving multiple
  // Object Ids, ServantLocator::etherealize may be invoked multiple
  // times with the same servant when the other objects are
  // deactivated. It is the responsibility of the object
  // implementation to refrain from destroying the servant while it is
  // active with any Id.
  if (!CORBA::is_nil (this->servant_activator_.in ()))
    {
      PortableServer::POA_var self = this->_this (env);
      if (env.exception () != 0)
        return;

      CORBA::Boolean remaining_activations = 0;

      // Check for remaining activations
      if (this->policies ().id_uniqueness () == PortableServer::MULTIPLE_ID &&
          this->active_object_map ().find (servant) != -1)
        remaining_activations = 1;

      this->servant_activator_->etherealize (oid,
                                             self.in (),
                                             servant,
                                             0,
                                             remaining_activations,
                                             env);
    }
}

CORBA::Object_ptr
TAO_POA::create_reference_i (const char *intf,
                             CORBA::Environment &env)
{
  // This operation requires the SYSTEM_ID policy; if not present, the
  // WrongPolicy exception is raised.
  if (this->policies ().id_assignment () != PortableServer::SYSTEM_ID)
    {
      CORBA::Exception *exception = new PortableServer::POA::WrongPolicy;
      env.exception (exception);
      return CORBA::Object::_nil ();
    }

  // This operation creates an object reference that encapsulates a
  // POA-generated Object Id value and the specified interface
  // repository id. This operation does not cause an activation to
  // take place. The resulting reference may be passed to clients, so
  // that subsequent requests on those references will cause the
  // appropriate servant manager to be invoked, if one is
  // available. The generated Object Id value may be obtained by
  // invoking POA::reference_to_id with the created reference.
  PortableServer::ObjectId_var new_id = this->create_object_id (0, env);
  if (env.exception () != 0)
    return CORBA::Object::_nil ();

  TAO_ObjectKey_var new_key = this->create_object_key (new_id.in ());

  // Ask the ORB to create you a reference
  return TAO_ORB_Core_instance ()->orb ()->key_to_object (new_key.in (), intf, env);
}

CORBA::Object_ptr
TAO_POA::create_reference_with_id (const PortableServer::ObjectId &oid,
                                   const char *intf,
                                   CORBA::Environment &env)
{
  // If the POA has the SYSTEM_ID policy and it detects that the
  // Object Id value was not generated by the system or for this POA,
  // the create_reference_with_id operation may raise the BAD_PARAM
  // system exception. An ORB is not required to detect all such
  // invalid Object Id values, but a portable application must not
  // invoke this operation on a POA that has the SYSTEM_ID policy with
  // an Object Id value that was not previously generated by the
  // system for that POA, or, if the POA also has the PERSISTENT
  // policy, for a previous instantiation of the same POA.
  if (this->policies ().id_assignment () == PortableServer::SYSTEM_ID &&
      !this->is_poa_generated_id (oid))
    {
      CORBA::Exception *exception = new CORBA::BAD_PARAM (CORBA::COMPLETED_NO);
      env.exception (exception);
      return CORBA::Object::_nil ();
    }

  // This operation creates an object reference that encapsulates the
  // specified Object Id and interface repository Id values. This
  // operation does not cause an activation to take place.  The
  // resulting reference may be passed to clients, so that subsequent
  // requests on those references will cause the object to be
  // activated if necessary, or the default servant used, depending on
  // the applicable policies.
  TAO_ObjectKey_var new_key = this->create_object_key (oid);

  // Ask the ORB to create you a reference
  return TAO_ORB_Core_instance ()->orb ()->key_to_object (new_key.in (), intf, env);
}

PortableServer::ObjectId *
TAO_POA::servant_to_id_i (PortableServer::Servant servant,
                          CORBA::Environment &env)
{
  // This operation requires the RETAIN and either the UNIQUE_ID or
  // IMPLICIT_ACTIVATION policies; if not present, the WrongPolicy
  // exception is raised.
  if (!(this->policies ().servant_retention () == PortableServer::RETAIN
        && (this->policies ().id_uniqueness () == PortableServer::UNIQUE_ID
            || this->policies ().implicit_activation () == PortableServer::IMPLICIT_ACTIVATION)))
    {
      CORBA::Exception *exception = new PortableServer::POA::WrongPolicy;
      env.exception (exception);
      return 0;
    }

  // This operation has three possible behaviors.

  // If the POA has the UNIQUE_ID policy and the specified servant is
  // active, the Object Id associated with that servant is returned.
  PortableServer::ObjectId id;
  if (this->policies ().id_uniqueness () == PortableServer::UNIQUE_ID &&
      this->active_object_map ().find (servant, id) != -1)
    {
      return new PortableServer::ObjectId (id);
    }

  // If the POA has the IMPLICIT_ACTIVATION policy and either the POA
  // has the MULTIPLE_ID policy or the specified servant is not
  // active, the servant is activated using a POA-generated Object Id
  // and the Interface Id associated with the servant, and that Object
  // Id is returned.
  if (this->policies ().implicit_activation () == PortableServer::IMPLICIT_ACTIVATION)
    {
      // If we reach here, then we either have the MULTIPLE_ID policy
      // or we have the UNIQUE_ID policy and we are not in the active
      // object map.
      PortableServer::ObjectId_var new_id = this->create_object_id (servant, env);
      if (env.exception () != 0)
        return 0;

      if (this->active_object_map ().bind (new_id.in (), servant) == -1)
        {
          CORBA::Exception *exception = new CORBA::OBJ_ADAPTER (CORBA::COMPLETED_NO);
          env.exception (exception);
          return 0;
        }

      // Everything is finally ok
      return new_id._retn ();
    }

  // Otherwise, the ServantNotActive exception is raised.
  CORBA::Exception *exception = new PortableServer::POA::ServantNotActive;
  env.exception (exception);
  return 0;
}

CORBA::Object_ptr
TAO_POA::servant_to_reference (PortableServer::Servant servant,
                               CORBA::Environment &env)
{
  // Note: The allocation of an Object Id value and installation in
  // the Active Object Map caused by implicit activation may actually
  // be deferred until an attempt is made to externalize the
  // reference. The real requirement here is that a reference is
  // produced that will behave appropriately (that is, yield a
  // consistent Object Id value when asked politely).
  PortableServer::ObjectId_var oid = this->servant_to_id (servant,
                                                          env);

  if (env.exception () != 0)
    return CORBA::Object::_nil ();
  else
    return this->create_reference_with_id (oid.in (),
                                           servant->_interface_repository_id (),
                                           env);
}

PortableServer::Servant
TAO_POA::reference_to_servant (CORBA::Object_ptr reference,
                               CORBA::Environment &env)
{
  // This operation requires the RETAIN policy or the
  // USE_DEFAULT_SERVANT policy. If neither policy is present, the
  // WrongPolicy exception is raised.
  if (!(this->policies ().servant_retention () == PortableServer::RETAIN
        || this->policies ().request_processing () == PortableServer::USE_DEFAULT_SERVANT))
    {
      CORBA::Exception *exception = new PortableServer::POA::WrongPolicy;
      env.exception (exception);
      return 0;
    }

  // If the POA has the RETAIN policy and the specified object is
  // present in the Active Object Map, this operation returns the
  // servant associated with that object in the Active Object Map.
  if (this->policies ().servant_retention () == PortableServer::RETAIN)
    {
      TAO_ObjectKey_var key = reference->_key (env);

      // If the object reference was not created by this POA, the
      // WrongAdapter exception is raised.
      PortableServer::ObjectId id;
      TAO_POA::String poa_name;
      CORBA::Boolean persistent = 0;
      CORBA::Boolean system_id = 0;
      TAO_Temporary_Creation_Time poa_creation_time;

      int result = this->parse_key (key.in (),
                                    poa_name,
                                    id,
                                    persistent,
                                    system_id,
                                    poa_creation_time);
      if (result != 0
          || poa_name != this->complete_name ()
          || persistent != this->persistent ()
          || system_id != this->system_id ()
          || !this->persistent ()
          && poa_creation_time != this->creation_time_)
        {
          CORBA::Exception *exception = new PortableServer::POA::WrongAdapter;
          env.exception (exception);
          return 0;
        }

      return this->id_to_servant (id, env);
    }

  // Otherwise, if the POA has the USE_DEFAULT_SERVANT policy and a
  // default servant has been registered with the POA, this operation
  // returns the default servant.
  if (this->policies ().request_processing () == PortableServer::USE_DEFAULT_SERVANT)
    {
      // Lock access to the POA for the duration of this transaction
      TAO_POA_READ_GUARD_RETURN (ACE_Lock, monitor, this->lock (), 0, env);

      if (this->default_servant_ != 0)
        {
          return this->default_servant_;
        }
      else
        // Otherwise, the ObjectNotActive exception is raised.
        {
          CORBA::Exception *exception = new PortableServer::POA::ObjectNotActive;
          env.exception (exception);
          return 0;
        }
    }

  // Not reached
  return 0;
}

PortableServer::ObjectId *
TAO_POA::reference_to_id (CORBA::Object_ptr reference,
                          CORBA::Environment &env)
{
  // The WrongPolicy exception is declared to allow future extensions.

  // This operation returns the Object Id value encapsulated by the
  // specified reference.

  // This operation is valid only if the reference was created by the
  // POA on which the operation is being performed.  If the object
  // reference was not created by this POA, the WrongAdapter exception
  // is raised.
  TAO_ObjectKey_var key = reference->_key (env);
  PortableServer::ObjectId id;
  TAO_POA::String poa_name;
  CORBA::Boolean persistent = 0;
  CORBA::Boolean system_id = 0;
  TAO_Temporary_Creation_Time poa_creation_time;

  int result = this->parse_key (key.in (),
                                poa_name,
                                id,
                                persistent,
                                system_id,
                                poa_creation_time);
  if (result != 0 ||
      poa_name != this->complete_name () ||
      persistent != this->persistent () ||
      system_id != this->system_id () ||
      !this->persistent () && poa_creation_time != this->creation_time_)
    {
      CORBA::Exception *exception = new PortableServer::POA::WrongAdapter;
      env.exception (exception);
      return 0;
    }

  // The object denoted by the reference does not have to be active
  // for this operation to succeed.
  return new PortableServer::ObjectId (id);
}

PortableServer::Servant
TAO_POA::id_to_servant_i (const PortableServer::ObjectId &oid,
                          CORBA::Environment &env)
{
  // This operation requires the RETAIN policy; if not present, the
  // WrongPolicy exception is raised.
  if (this->policies ().servant_retention () != PortableServer::RETAIN)
    {
      CORBA::Exception *exception = new PortableServer::POA::WrongPolicy;
      env.exception (exception);
      return 0;
    }

  // This operation returns the active servant associated with the
  // specified Object Id value.  If the Object Id value is not active
  // in the POA, an ObjectNotActive exception is raised.
  PortableServer::Servant servant = 0;
  if (this->active_object_map ().find (oid, servant) != -1)
    {
      return servant;
    }
  else
    // Otherwise, the ObjectNotActive exception is raised.
    {
      CORBA::Exception *exception = new PortableServer::POA::ObjectNotActive;
      env.exception (exception);
      return 0;
    }
}

CORBA::Object_ptr
TAO_POA::id_to_reference_i (const PortableServer::ObjectId &oid,
                            CORBA::Environment &env)
{
  // This operation requires the RETAIN policy; if not present, the
  // WrongPolicy exception is raised.
  if (this->policies ().servant_retention () != PortableServer::RETAIN)
    {
      CORBA::Exception *exception = new PortableServer::POA::WrongPolicy;
      env.exception (exception);
      return 0;
    }

  // If an object with the specified Object Id value is currently
  // active, a reference encapsulating the information used to
  // activate the object is returned.
  PortableServer::Servant servant = 0;
  if (this->active_object_map ().find (oid, servant) != -1)
    return this->create_reference_with_id (oid,
                                           servant->_interface_repository_id (),
                                           env);
  else
    // If the Object Id value is not active in the POA, an
    // ObjectNotActive exception is raised.
    {
      CORBA::Exception *exception = new PortableServer::POA::ObjectNotActive;
      env.exception (exception);
      return 0;
    }
}

void
TAO_POA::forward_object_i (const PortableServer::ObjectId &oid,
                           CORBA::Object_ptr forward_to,
                           CORBA::Environment &env)
{
  // First, deactivate the object
  this->deactivate_object_i (oid, env);

  // If failure
  if (env.exception () != 0)
    return;

  // If success, create a forwarding servant
  TAO_Forwarding_Servant *forwarding_servant
    = new TAO_Forwarding_Servant (forward_to,
                                  forward_to->_interface_repository_id ());

  // Register the forwarding servant with the same object Id
  this->activate_object_with_id_i (oid,
                                   forwarding_servant,
                                   env);
}

TAO_POA *
TAO_POA::locate_poa_i (const TAO_ObjectKey &key,
                       PortableServer::ObjectId &id,
                       CORBA::Environment &env)
{
  ACE_FUNCTION_TIMEPROBE (TAO_POA_LOCATE_POA_I_START);

  TAO_POA::String poa_name;
  CORBA::Boolean persistent = 0;
  CORBA::Boolean system_id = 0;
  TAO_Temporary_Creation_Time poa_creation_time;

  int result = this->parse_key (key,
                                poa_name,
                                id,
                                persistent,
                                system_id,
                                poa_creation_time);
  if (result != 0)
    {
      CORBA::Exception *exception = new CORBA::OBJ_ADAPTER (CORBA::COMPLETED_NO);
      env.exception (exception);
      return 0;
    }

  TAO_POA *poa = this->find_POA_i (poa_name,
                                   persistent,
                                   env);
  if (env.exception () != 0)
    return 0;

  // Make sure it is not an "old" objectId
  if (!poa->persistent () &&
      poa->creation_time () != poa_creation_time)
    {
      CORBA::Exception *exception = new CORBA::OBJECT_NOT_EXIST (CORBA::COMPLETED_NO);
      env.exception (exception);
      return 0;
    }

  return poa;
}

int
TAO_POA::locate_servant (const TAO_ObjectKey &key,
                         CORBA::Environment &env)
{
  // Lock access to the POA for the duration of this transaction
  TAO_POA_READ_GUARD_RETURN (ACE_Lock, monitor, this->lock (), -1, env);

  PortableServer::Servant servant = 0;
  TAO_POA::LOCATION_RESULT result = this->locate_servant_i (key, servant, env);

  switch (result)
    {
    case TAO_POA::FOUND:
      // Optimistic attitude
    case TAO_POA::DEFAULT_SERVANT:
    case TAO_POA::SERVANT_MANAGER:
      return 0;

    case TAO_POA::NOT_FOUND:
      return -1;
    }

  return -1;
}

PortableServer::Servant
TAO_POA::find_servant (const TAO_ObjectKey &key,
                       CORBA::Environment &env)
{
  // Lock access to the POA for the duration of this transaction
  TAO_POA_READ_GUARD_RETURN (ACE_Lock, monitor, this->lock (), 0, env);

  PortableServer::Servant servant = 0;
  TAO_POA::LOCATION_RESULT result = this->locate_servant_i (key, servant, env);

  switch (result)
    {
    case TAO_POA::FOUND:
      return servant;

    case TAO_POA::DEFAULT_SERVANT:
    case TAO_POA::SERVANT_MANAGER:
    case TAO_POA::NOT_FOUND:
      return 0;
    }

  return 0;
}

TAO_POA::LOCATION_RESULT
TAO_POA::locate_servant_i (const TAO_ObjectKey &key,
                           PortableServer::Servant &servant,
                           CORBA::Environment &env)
{
  PortableServer::ObjectId id;

  TAO_POA *poa = this->locate_poa_i (key,
                                     id,
                                     env);
  if (env.exception () != 0)
    return TAO_POA::NOT_FOUND;

  // If the POA has the RETAIN policy, the POA looks in the Active
  // Object Map to find if there is a servant associated with the
  // Object Id value from the request. If such a servant exists,
  // return TAO_POA::FOUND.
  if (poa->policies ().servant_retention () == PortableServer::RETAIN &&
      poa->active_object_map ().find (id, servant) != -1)
    // Success
    return TAO_POA::FOUND;

  // If the POA has the NON_RETAIN policy or has the RETAIN policy but
  // didn't find a servant in the Active Object Map, the POA takes the
  // following actions:

  // If the USE_ACTIVE_OBJECT_MAP_ONLY policy is in effect, the POA raises
  // the OBJECT_NOT_EXIST system exception.
  if (poa->policies ().request_processing () == PortableServer::USE_ACTIVE_OBJECT_MAP_ONLY)
    return TAO_POA::NOT_FOUND;

  // If the POA has the USE_DEFAULT_SERVANT policy, a default servant
  // has been associated with the POA, return
  // TAO_POA::DEFAULT_SERVANT. If no servant has been associated with
  // the POA, return TAO_POA::NOT_FOUND.
  if (poa->policies ().request_processing () == PortableServer::USE_DEFAULT_SERVANT)
    {
      if (poa->default_servant_ == 0)
        return TAO_POA::NOT_FOUND;
      else
        // Success
        return TAO_POA::DEFAULT_SERVANT;
    }

  // If the POA has the USE_SERVANT_MANAGER policy, a servant manager
  // has been associated with the POA, return
  // TAO_POA::SERVANT_MANAGER. If no servant manager has been
  // associated with the POA, return TAO_POA::NOT_FOUND.
  if (poa->policies ().request_processing () == PortableServer::USE_SERVANT_MANAGER)
    {
      if (CORBA::is_nil (poa->servant_activator_.in ()) &&
          CORBA::is_nil (poa->servant_locator_.in ()))
        return TAO_POA::NOT_FOUND;
      else
        // Success
        return TAO_POA::SERVANT_MANAGER;
    }

  // Failure
  return TAO_POA::NOT_FOUND;
}

PortableServer::Servant
TAO_POA::locate_poa_and_servant_i (const TAO_ObjectKey &key,
                                   const char *operation,
                                   PortableServer::ObjectId &id,
                                   TAO_POA *&poa_impl,
                                   TAO_ORB_Core *orb_core,
                                   CORBA::Environment &env)
{
  ACE_FUNCTION_TIMEPROBE (TAO_POA_LOCATE_POA_AND_SERVANT_I_START);

  poa_impl = this->locate_poa_i (key,
                                 id,
                                 env);
  if (env.exception () != 0)
    return 0;

  // If the POA has the RETAIN policy, the POA looks in the Active
  // Object Map to find if there is a servant associated with the
  // Object Id value from the request. If such a servant exists, the
  // POA invokes the appropriate method on the servant.
  if (poa_impl->policies ().servant_retention () == PortableServer::RETAIN)
    {
      PortableServer::Servant servant = 0;

      {
        ACE_FUNCTION_TIMEPROBE (TAO_POA_FIND_SERVANT_START);

        if (poa_impl->active_object_map ().find (id, servant) != -1)
          // Success
          return servant;
      }
    }

  // If the POA has the NON_RETAIN policy or has the RETAIN policy but
  // didn't find a servant in the Active Object Map, the POA takes the
  // following actions:

  // If the USE_ACTIVE_OBJECT_MAP_ONLY policy is in effect, the POA raises
  // the OBJECT_NOT_EXIST system exception.
  if (poa_impl->policies ().request_processing () == PortableServer::USE_ACTIVE_OBJECT_MAP_ONLY)
    {
      CORBA::Exception *exception = new CORBA::OBJECT_NOT_EXIST (CORBA::COMPLETED_NO);
      env.exception (exception);
      return 0;
    }

  // If the POA has the USE_DEFAULT_SERVANT policy, a default servant
  // has been associated with the POA so the POA will invoke the
  // appropriate method on that servant. If no servant has been
  // associated with the POA, the POA raises the OBJ_ADAPTER system
  // exception.
  if (poa_impl->policies ().request_processing () == PortableServer::USE_DEFAULT_SERVANT)
    {
      if (poa_impl->default_servant_ == 0)
        {
          CORBA::Exception *exception = new CORBA::OBJ_ADAPTER (CORBA::COMPLETED_NO);
          env.exception (exception);
          return 0;
        }
      else
        {
          // Success
          return poa_impl->default_servant_;
        }
    }

  // If the POA has the USE_SERVANT_MANAGER policy, a servant manager
  // has been associated with the POA so the POA will invoke incarnate
  // or preinvoke on it to find a servant that may handle the
  // request. (The choice of method depends on the NON_RETAIN or
  // RETAIN policy of the POA.) If no servant manager has been
  // associated with the POA, the POA raises the OBJ_ADAPTER system
  // exception.
  //
  // If a servant manager is located and invoked, but the servant
  // manager is not directly capable of incarnating the object, it
  // (the servant manager) may deal with the circumstance in a variety
  // of ways, all of which are the application's responsibility.  Any
  // system exception raised by the servant manager will be returned
  // to the client in the reply. In addition to standard CORBA
  // exceptions, a servant manager is capable of raising a
  // ForwardRequest exception. This exception includes an object
  // reference.
  //
  if (poa_impl->policies ().request_processing () == PortableServer::USE_SERVANT_MANAGER)
    {
      if (CORBA::is_nil (poa_impl->servant_activator_.in ()) &&
          CORBA::is_nil (poa_impl->servant_locator_.in ()))
        {
          CORBA::Exception *exception = new CORBA::OBJ_ADAPTER (CORBA::COMPLETED_NO);
          env.exception (exception);
          return 0;
        }

      PortableServer::POA_var poa = poa_impl->_this (env);
      if (env.exception () != 0)
        return 0;

      if (poa_impl->policies ().servant_retention () == PortableServer::RETAIN)
        {
          // @@
          // Invocations of incarnate on the servant manager are serialized.
          // Invocations of etherealize on the servant manager are serialized.
          // Invocations of incarnate and etherealize on the servant manager are mutually exclusive.
          PortableServer::Servant servant = poa_impl->servant_activator_->incarnate (id,
                                                                                     poa.in (),
                                                                                     env);
          if (env.exception () != 0 || servant == 0)
            return 0;

          // If the incarnate operation returns a servant that is
          // already active for a different Object Id and if the POA
          // also has the UNIQUE_ID policy, the incarnate has violated
          // the POA policy and is considered to be in error. The POA
          // will raise an OBJ_ADAPTER system exception for the
          // request.
          if (poa_impl->policies ().id_uniqueness () == PortableServer::UNIQUE_ID &&
              poa_impl->active_object_map ().find (servant) != -1)
            {
              CORBA::Exception *exception = new CORBA::OBJ_ADAPTER (CORBA::COMPLETED_NO);
              env.exception (exception);
              return 0;
            }

          // The POA enters the returned Servant value into the Active
          // Object Map so that subsequent requests with the same
          // ObjectId value will be delivered directly to that servant
          // without invoking the servant manager.
          if (poa_impl->active_object_map ().bind (id, servant) == -1)
            {
              CORBA::Exception *exception = new CORBA::OBJ_ADAPTER (CORBA::COMPLETED_NO);
              env.exception (exception);
              return 0;
            }

          // Success
          return servant;
        }
      else
        //
        // Don't retain servant
        //
        {
          // No serialization of invocations of preinvoke or
          // postinvoke may be assumed; there may be multiple
          // concurrent invocations of preinvoke for the same
          // ObjectId.
          //
          // The same thread will be used to preinvoke the object,
          // process the request, and postinvoke the object.
          //
          PortableServer::ServantLocator::Cookie cookie;
          PortableServer::Servant servant = poa_impl->servant_locator_->preinvoke (id,
                                                                                   poa.in (),
                                                                                   operation,
                                                                                   cookie,
                                                                                   env);
          if (env.exception () != 0 || servant == 0)
            return 0;

          // Remember the cookie
          TAO_POA_Current *poa_current = orb_core->poa_current ();
          poa_current->locator_cookie (cookie);

          // Success
          return servant;
        }
    }

  // Failure
  CORBA::Exception *exception = new CORBA::OBJ_ADAPTER (CORBA::COMPLETED_NO);
  env.exception (exception);
  return 0;
}

void
TAO_POA::dispatch_servant (const TAO_ObjectKey &key,
                           CORBA::ServerRequest &req,
                           void *context,
                           TAO_ORB_Core *orb_core,
                           CORBA::Environment &env)
{
  ACE_FUNCTION_TIMEPROBE (TAO_POA_DISPATCH_SERVANT_START);

  // Lock access to the POA for the duration of this transaction
  TAO_POA_READ_GUARD (ACE_Lock, monitor, this->lock (), env);

  this->dispatch_servant_i (key, req, context, orb_core, env);
}

void
TAO_POA::dispatch_servant_i (const TAO_ObjectKey &key,
                             CORBA::ServerRequest &req,
                             void *context,
                             TAO_ORB_Core *orb_core,
                             CORBA::Environment &env)
{
  PortableServer::ObjectId id;
  TAO_POA *poa = 0;
  const char *operation = req.operation ();

  // @@ Lots on non-exception safe code here!!

  // Setup for POA Current
  TAO_POA_Current current_context;

  // @@ This call changes the state, but if an exception is raised the
  // context will not be reset, you need a helper class that can do
  // that in the destructor, what about using the TAO_POA_Current
  // class itself?

  // Set the current context and remember the old one
  TAO_POA_Current *previous_context = orb_core->poa_current (&current_context);

  PortableServer::Servant servant = this->locate_poa_and_servant_i (key,
                                                                    operation,
                                                                    id,
                                                                    poa,
                                                                    orb_core,
                                                                    env);

  // @@ We should use the TRY macros here, notice that you detect the
  // exception but you do not attempt to fix the problems above
  if (env.exception () != 0 || servant == 0)
    return;

  // @@ Yet another place where you change the state without a class
  // whose destructor will return things to normal....

  // Setup for upcall
  poa->pre_invoke (key,
                   id,
                   servant,
                   &current_context,
                   env);

  {
    ACE_FUNCTION_TIMEPROBE (TAO_SERVANT_DISPATCH_START);

    // Upcall
    servant->_dispatch (req,
                        context,
                        env);
  }

  // @@ This shoul be done by a destructor....

  // Cleanup from upcall
  poa->post_invoke (servant,
                    operation,
                    &current_context,
                    env);

  // @@ this should also be done by a destructor....

  // Reset old context
  orb_core->poa_current (previous_context);

}

void
TAO_POA::pre_invoke (const TAO_ObjectKey &key,
                     const PortableServer::ObjectId &id,
                     PortableServer::Servant servant,
                     TAO_POA_Current *poa_current,
                     CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);

  poa_current->POA_impl (this);
  poa_current->object_key (key);
  poa_current->object_id (id);
  poa_current->servant (servant);
}

void
TAO_POA::post_invoke (PortableServer::Servant servant,
                      const char *operation,
                      TAO_POA_Current *poa_current,
                      CORBA::Environment &env)
{
  PortableServer::ServantLocator::Cookie cookie = poa_current->locator_cookie ();

  if (cookie != 0)
    {
      PortableServer::POA_var poa = poa_current->get_POA (env);

      this->servant_locator_->postinvoke (poa_current->object_id (),
                                          poa.in (),
                                          operation,
                                          cookie,
                                          servant,
                                          env);
    }

  // poa_current->clear ();

}

int
TAO_POA::parse_key (const TAO_ObjectKey &key,
                    TAO_POA::String &poa_name,
                    PortableServer::ObjectId &id,
                    CORBA::Boolean &persistent,
                    CORBA::Boolean &system_id,
                    TAO_Temporary_Creation_Time &poa_creation_time)
{
  ACE_FUNCTION_TIMEPROBE (TAO_POA_PARSE_KEY_START);

  int starting_at = 0;

  // Check the system id indicator
  char system_id_key_type = key[starting_at];
  if (system_id_key_type == this->system_id_key_char ())
    system_id = 1;
  else if (system_id_key_type == this->user_id_key_char ())
    system_id = 0;
  else
    // Incorrect key
    return -1;

  // Skip past the system id indicator
  starting_at += TAO_POA::system_id_key_type_length ();

  // Try to find the last separator
  int last_token_position = 0;
  if (system_id)
    // The minus one is because we want <last_token_position> to point
    // to the separator
    last_token_position = key.length () - this->system_id_size () - 1;
  else
    last_token_position = this->rfind (key, TAO_POA::name_separator ());

  // If not found, the name is not correct
  if (last_token_position == TAO_POA::String::npos)
    return -1;

#if !defined (POA_NO_TIMESTAMP)
  // Check the persistence indicator
  char persistent_key_type = key[starting_at];
  if (persistent_key_type == this->persistent_key_char ())
    persistent = 1;
  else if (persistent_key_type == this->transient_key_char ())
    persistent = 0;
  else
    // Incorrect key
    return -1;

  // Skip past the persistent indicator
  starting_at += TAO_POA::persistent_key_type_length ();

  // Take the creation time for the timestamp
  poa_creation_time.creation_time (&key[starting_at]);

  // Skip past the timestamp
  starting_at += TAO_Creation_Time::creation_time_length ();

#endif /* POA_NO_TIMESTAMP */

  // Take the substring from <starting_at> to last_token_position for
  // the POA name
  int how_many = last_token_position - starting_at;
  poa_name.set ((const char *) &key[starting_at],
                how_many,
                0);

  // Take the substring from (last_token_position + separator_length)
  // to length for the objectId
  starting_at = last_token_position + TAO_POA::name_separator_length ();
  how_many = key.length () - starting_at;

  // Reset the Id
  id.replace (how_many,
              how_many,
              (CORBA::Octet *) &key[starting_at],
              0);

  // Success
  return 0;
}

TAO_ObjectKey *
TAO_POA::create_object_key (const PortableServer::ObjectId &id)
{
  // Calculate the space required for the key
  int buffer_size =
    this->system_id_key_type_length () +
#if !defined (POA_NO_TIMESTAMP)
    this->persistent_key_type_length () +
    TAO_Creation_Time::creation_time_length () +
#endif /* POA_NO_TIMESTAMP */
    this->complete_name_.length () +
    TAO_POA::name_separator_length () +
    id.length ();

  // Create the buffer for the key
  CORBA::Octet *buffer = TAO_ObjectKey::allocbuf (buffer_size);

  // Keeps track of where the next infomation goes; start at 0 byte
  int starting_at = 0;

  // Copy the system id bit
  buffer[starting_at] = (CORBA::Octet) this->system_id_key_type ();
  starting_at += this->system_id_key_type_length ();

#if !defined (POA_NO_TIMESTAMP)
  // Copy the persistence bit
  buffer[starting_at] = (CORBA::Octet) this->persistent_key_type ();
  starting_at += this->persistent_key_type_length ();

  // Then copy the timestamp
  ACE_OS::memcpy (&buffer[starting_at],
                  this->creation_time_.creation_time (),
                  TAO_Creation_Time::creation_time_length ());
  starting_at += TAO_Creation_Time::creation_time_length ();
#endif /* POA_NO_TIMESTAMP */

  // Put the POA name into the buffer
  ACE_OS::memcpy (&buffer[starting_at],
                  this->complete_name_.c_str (),
                  this->complete_name_.length ());
  starting_at += this->complete_name_.length ();

  // Add the name separator
  buffer[starting_at] = (CORBA::Octet) TAO_POA::name_separator ();
  starting_at += TAO_POA::name_separator_length ();

  // Then copy the ID into the key
  ACE_OS::memcpy (&buffer[starting_at],
                  id.get_buffer (),
                  id.length ());

  // Create the key, giving the ownership of the buffer to the
  // sequence.
  return new TAO_ObjectKey (buffer_size,
                            buffer_size,
                            buffer,
                            1);
}

int
TAO_POA::is_poa_generated_id (const PortableServer::ObjectId &id)
{

#if defined (POA_NAME_IN_POA_GENERATED_ID)

  // Grab the buffer
  const char *id_buffer = (const char *) id.get_buffer ();

  // Check to see if the POA name is the first part of the id
  return
    this->name_.length () < id.length () &&
    ACE_OS::strncmp (id_buffer,
                     this->name_.c_str (),
                     this->name_.length ()) == 0;

#else /* POA_NAME_IN_POA_GENERATED_ID */

  ACE_UNUSED_ARG (id);
  return 1;

#endif /* POA_NAME_IN_POA_GENERATED_ID */
}

int
TAO_POA::is_poa_generated_key (const TAO_ObjectKey &key)
{
  // Grab the buffer
  const char *key_buffer = (const char *) key.get_buffer ();

  // Check to see if the complete POA name is the first part of the
  // key
  return
    this->complete_name_.length () < key.length () &&
    ACE_OS::strncmp (key_buffer,
                     this->complete_name_.c_str (),
                     this->complete_name_.length ()) == 0;
}

int
TAO_POA::leaf_poa_name (const TAO_POA::String &adapter_name,
                        CORBA::Environment &env)
{
  // This method does not throw any exceptions
  ACE_UNUSED_ARG (env);

  // Try to find the name separator
  if (adapter_name.find (TAO_POA::name_separator ()) == TAO_POA::String::npos)
    // If not found, the name was a leaf
    return 1;
  else
    // If found, the name was not a leaf
    return 0;
}

void
TAO_POA::parse_poa_name (const TAO_POA::String &adapter_name,
                         TAO_POA::String &topmost_poa_name,
                         TAO_POA::String &tail_poa_name,
                         CORBA::Environment &env)
{
  // Try to find the name separator
  int token_position = adapter_name.find (TAO_POA::name_separator ());

  // If not found, the name was a leaf, throw exception
  if (token_position == TAO_POA::String::npos)
    {
      CORBA::Exception *exception = new CORBA::OBJ_ADAPTER (CORBA::COMPLETED_NO);
      env.exception (exception);
      return;
    }
  else
    {
      // If found, take the substring from 0 to token_position
      int starting_at = 0;
      int how_many = token_position - starting_at;
      topmost_poa_name.set (&adapter_name[starting_at],
                            how_many,
                            0);

      // Take the substring from (token_position + separator_length)
      // to length
      starting_at = token_position + TAO_POA::name_separator_length ();
      how_many = adapter_name.length () - starting_at;
      tail_poa_name.set (&adapter_name[starting_at],
                         how_many,
                         0);
    }
}

void
TAO_POA::set_complete_name (void)
{
  if (this->parent_ != 0)
    {
      this->complete_name_ += this->parent_->complete_name ();
      this->complete_name_ += TAO_POA::name_separator ();
    }
  this->complete_name_ += this->name_;
}

PortableServer::ObjectId *
TAO_POA::string_to_ObjectId (const char *string)
{
  // Size of string
  //
  // We DO NOT include the zero terminator, as this is simply an
  // artifact of the way strings are stored in C.
  //
  CORBA::ULong buffer_size = ACE_OS::strlen (string);

  // Create the buffer for the Id
  CORBA::Octet *buffer = PortableServer::ObjectId::allocbuf (buffer_size);

  // Copy the contents
  ACE_OS::memcpy (buffer, string, buffer_size);

  // Create and return a new ID
  return new PortableServer::ObjectId (buffer_size,
                                       buffer_size,
                                       buffer,
                                       1);
}

PortableServer::ObjectId *
TAO_POA::string_to_ObjectId (const char *string,
                             int size)
{
  // Create the buffer for the Id
  CORBA::Octet *buffer = PortableServer::ObjectId::allocbuf (size);

  // Copy the contents
  ACE_OS::memcpy (buffer, string, size);

  // Create and return a new ID
  return new PortableServer::ObjectId (size,
                                       size,
                                       buffer,
                                       1);
}

PortableServer::ObjectId *
TAO_POA::wstring_to_ObjectId (const CORBA::WChar *string)
{
  // Size of Id
  //
  // We DO NOT include the zero terminator, as this is simply an
  // artifact of the way strings are stored in C.
  //
  CORBA::ULong string_length = ACE_OS::wslen (string);

  size_t buffer_size = string_length * sizeof (CORBA::WChar);

  // Create the buffer for the Id
  CORBA::Octet *buffer = PortableServer::ObjectId::allocbuf (buffer_size);

  // Copy contents
  ACE_OS::memcpy (buffer, string, buffer_size);

  // Create a new ID
  return new PortableServer::ObjectId (buffer_size,
                                       buffer_size,
                                       buffer,
                                       1);
}

CORBA::String
TAO_POA::ObjectId_to_string (const PortableServer::ObjectId &id)
{
  // Create space
  CORBA::String string = CORBA::string_alloc (id.length ());

  // Copy the data
  ACE_OS::memcpy (string, id.get_buffer (), id.length ());

  // Null terminate the string
  string[id.length ()] = '\0';

  // Return string
  return string;
}

CORBA::WChar *
TAO_POA::ObjectId_to_wstring (const PortableServer::ObjectId &id)
{
  // Create space
  CORBA::WString string = CORBA::wstring_alloc (id.length ());

  // Copy the data
  ACE_OS::memcpy (string, id.get_buffer (), id.length () * sizeof (CORBA::WChar));

  // Null terminate the string
  string[id.length ()] = '\0';

  // Return string
  return string;
}

PortableServer::ThreadPolicy_ptr
TAO_POA::create_thread_policy (PortableServer::ThreadPolicyValue value,
                               CORBA::Environment &env)
{
  PortableServer::POA_var self = this->_this (env);
  // Check for exceptions
  if (env.exception () != 0)
    return PortableServer::ThreadPolicy::_nil ();

  auto_ptr<TAO_Thread_Policy> new_policy (new TAO_Thread_Policy (value, self.in ()));
  PortableServer::ThreadPolicy_var result = new_policy->_this (env);
  if (env.exception () != 0)
    return PortableServer::ThreadPolicy::_nil ();
  else
    {
      // Make sure that the auto_ptr does not delete the
      // implementation
      new_policy.release ();
      return result._retn ();
    }
}

PortableServer::LifespanPolicy_ptr
TAO_POA::create_lifespan_policy (PortableServer::LifespanPolicyValue value,
                                 CORBA::Environment &env)
{
  PortableServer::POA_var self = this->_this (env);
  // Check for exceptions
  if (env.exception () != 0)
    return PortableServer::LifespanPolicy::_nil ();

  auto_ptr<TAO_Lifespan_Policy> new_policy (new TAO_Lifespan_Policy (value, self.in ()));
  PortableServer::LifespanPolicy_var result = new_policy->_this (env);
  if (env.exception () != 0)
    return PortableServer::LifespanPolicy::_nil ();
  else
    {
      // Make sure that the auto_ptr does not delete the
      // implementation
      new_policy.release ();
      return result._retn ();
    }
}

PortableServer::IdUniquenessPolicy_ptr
TAO_POA::create_id_uniqueness_policy (PortableServer::IdUniquenessPolicyValue value,
                                      CORBA::Environment &env)
{
  PortableServer::POA_var self = this->_this (env);
  // Check for exceptions
  if (env.exception () != 0)
    return PortableServer::IdUniquenessPolicy::_nil ();

  auto_ptr<TAO_Id_Uniqueness_Policy> new_policy (new TAO_Id_Uniqueness_Policy (value, self.in ()));
  PortableServer::IdUniquenessPolicy_var result = new_policy->_this (env);
  if (env.exception () != 0)
    return PortableServer::IdUniquenessPolicy::_nil ();
  else
    {
      // Make sure that the auto_ptr does not delete the
      // implementation
      new_policy.release ();
      return result._retn ();
    }
}

PortableServer::IdAssignmentPolicy_ptr
TAO_POA::create_id_assignment_policy (PortableServer::IdAssignmentPolicyValue value,
                                      CORBA::Environment &env)
{
  PortableServer::POA_var self = this->_this (env);
  // Check for exceptions
  if (env.exception () != 0)
    return PortableServer::IdAssignmentPolicy::_nil ();

  auto_ptr<TAO_Id_Assignment_Policy> new_policy (new TAO_Id_Assignment_Policy (value, self.in ()));
  PortableServer::IdAssignmentPolicy_var result = new_policy->_this (env);
  if (env.exception () != 0)
    return PortableServer::IdAssignmentPolicy::_nil ();
  else
    {
      // Make sure that the auto_ptr does not delete the
      // implementation
      new_policy.release ();
      return result._retn ();
    }
}

PortableServer::ImplicitActivationPolicy_ptr
TAO_POA::create_implicit_activation_policy (PortableServer::ImplicitActivationPolicyValue value,
                                            CORBA::Environment &env)
{
  PortableServer::POA_var self = this->_this (env);
  // Check for exceptions
  if (env.exception () != 0)
    return PortableServer::ImplicitActivationPolicy::_nil ();

  auto_ptr<TAO_Implicit_Activation_Policy> new_policy (new TAO_Implicit_Activation_Policy (value, self.in ()));
  PortableServer::ImplicitActivationPolicy_var result = new_policy->_this (env);
  if (env.exception () != 0)
    return PortableServer::ImplicitActivationPolicy::_nil ();
  else
    {
      // Make sure that the auto_ptr does not delete the
      // implementation
      new_policy.release ();
      return result._retn ();
    }
}

PortableServer::ServantRetentionPolicy_ptr
TAO_POA::create_servant_retention_policy (PortableServer::ServantRetentionPolicyValue value,
                                          CORBA::Environment &env)
{
  PortableServer::POA_var self = this->_this (env);
  // Check for exceptions
  if (env.exception () != 0)
    return PortableServer::ServantRetentionPolicy::_nil ();

  auto_ptr<TAO_Servant_Retention_Policy> new_policy (new TAO_Servant_Retention_Policy (value, self.in ()));
  PortableServer::ServantRetentionPolicy_var result = new_policy->_this (env);
  if (env.exception () != 0)
    return PortableServer::ServantRetentionPolicy::_nil ();
  else
    {
      // Make sure that the auto_ptr does not delete the
      // implementation
      new_policy.release ();
      return result._retn ();
    }
}

PortableServer::RequestProcessingPolicy_ptr
TAO_POA::create_request_processing_policy (PortableServer::RequestProcessingPolicyValue value,
                                           CORBA::Environment &env)
{
  PortableServer::POA_var self = this->_this (env);
  // Check for exceptions
  if (env.exception () != 0)
    return PortableServer::RequestProcessingPolicy::_nil ();

  auto_ptr<TAO_Request_Processing_Policy> new_policy (new TAO_Request_Processing_Policy (value, self.in ()));
  PortableServer::RequestProcessingPolicy_var result = new_policy->_this (env);
  if (env.exception () != 0)
    return PortableServer::RequestProcessingPolicy::_nil ();
  else
    {
      // Make sure that the auto_ptr does not delete the
      // implementation
      new_policy.release ();
      return result._retn ();
    }
}

// **************************************************
//
// TAO spcific POA locking policy (non-standard)
//
// **************************************************

PortableServer::SynchronizationPolicy_ptr
TAO_POA::create_synchronization_policy (PortableServer::SynchronizationPolicyValue value,
                                        CORBA::Environment &env)
{
  PortableServer::POA_var self = this->_this (env);
  // Check for exceptions
  if (env.exception () != 0)
    return PortableServer::SynchronizationPolicy::_nil ();

  auto_ptr<TAO_Synchronization_Policy> new_policy (new TAO_Synchronization_Policy (value, self.in ()));
  PortableServer::SynchronizationPolicy_var result = new_policy->_this (env);
  if (env.exception () != 0)
    return PortableServer::SynchronizationPolicy::_nil ();
  else
    {
      // Make sure that the auto_ptr does not delete the
      // implementation
      new_policy.release ();
      return result._retn ();
    }
}

void
TAO_POA::encode_sequence_to_string (CORBA::String &str,
                                    const TAO_Unbounded_Sequence<CORBA::Octet> &seq)
{
  // We must allocate a buffer which is (gag) 3 times the length
  // of the sequence, which is the length required in the worst-case
  // scenario of all non-printable characters.
  //
  // There are two strategies here...we could allocate all that space here,
  // fill it up, then copy-allocate new space of just the right length.
  // OR, we could just return this space.  The classic time-space tradeoff,
  // and for now we'll let time win out, which means that we only do the
  // allocation once.
  u_int len = 3 * seq.length() + 1 /* for zero termination */;
  str = CORBA::string_alloc (len);

  char *cp = str;

  for (u_int i = 0;
       cp < (cp+len) && i < seq.length();
       i++)
    {
      u_char byte = seq[i];
      if (isprint (byte) && byte != '\\')
        {
          *cp++ = (char) byte;
          continue;
        }

      *cp++ = '\\';
      *cp++ = ACE::nibble2hex ((byte >> 4) & 0x0f);
      *cp++ = ACE::nibble2hex (byte & 0x0f);
    }
  // Zero terminate
  *cp = '\0';
}

void
TAO_POA::decode_string_to_sequence (TAO_Unbounded_Sequence<CORBA::Octet> &seq,
                                    const char *str)
{
  if (str == 0)
    {
      seq.length (0);
      return;
    }

  u_int length = ACE_OS::strlen (str);
  const char *eos = str + length;
  const char *cp = str;

  // Set the length of the sequence to be as long as
  // we'll possibly need...we'll reset it to the actual
  // length later.
  seq.length (length);

  u_int i = 0;
  for (;
       cp < eos && i < seq.length ();
       i++)
    {
      if (*cp == '\\')
        {
          // This is an escaped non-printable,
          // so we decode the hex values into
          // the sequence's octet
          seq[i] = (u_char) (ACE::hex2byte (cp[1]) << 4);
          seq[i] |= (u_char) ACE::hex2byte (cp[2]);
          cp += 3;
        }
      else
        // Copy it in
        seq[i] = *cp++;
    }

  // Set the length appropriately
  seq.length (i);
}

CORBA::Boolean
TAO_Adapter_Activator::unknown_adapter (PortableServer::POA_ptr parent,
                                        const char *name,
                                        CORBA::Environment &env)
{
  // Default policies
  CORBA::PolicyList default_policies;

  // This assumes that the lock on the parent is recursive
  PortableServer::POA_var child = parent->create_POA (name,
                                                      PortableServer::POAManager::_nil (),
                                                      default_policies,
                                                      env);

  if (env.exception () != 0)
    return 0;
  else
    {
      PortableServer::AdapterActivator_var activator = this->_this (env);
      if (env.exception () != 0)
        {
          child->destroy (0,
                          0,
                          env);
          return 0;
        }

      child->the_activator (activator.in (), env);

      if (env.exception () != 0)
        {
          child->destroy (0,
                          0,
                          env);
          return 0;
        }

      // Finally everything is fine
      return 1;
    }
}

TAO_POA_Current::TAO_POA_Current (void)
  : poa_impl_ (0),
    object_id_ (0),
    object_key_ (0),
    cookie_ (0),
    servant_ (0)
{
}

TAO_POA_Current::TAO_POA_Current (TAO_POA *impl,
                                  const TAO_ObjectKey &key,
                                  const PortableServer::ObjectId &id,
                                  PortableServer::Servant servant)
  : poa_impl_ (impl),
    object_id_ (&id),
    object_key_ (&key),
    cookie_ (0),
    servant_ (servant)
{}

TAO_POA_Current::~TAO_POA_Current (void)
{
}

PortableServer::POA_ptr
TAO_POA_Current::get_POA (CORBA::Environment &env)
{
  if (!this->context_is_valid ())
    {
      CORBA::Exception *exception = new PortableServer::Current::NoContext;
      env.exception (exception);
      return PortableServer::POA::_nil ();
    }

  PortableServer::POA_var result = this->poa_impl_->_this (env);
  if (env.exception () != 0)
    return PortableServer::POA::_nil ();
  else
    return result._retn ();
}

PortableServer::ObjectId *
TAO_POA_Current::get_object_id (CORBA::Environment &env)
{
  if (!this->context_is_valid ())
    {
      CORBA::Exception *exception = new PortableServer::Current::NoContext;
      env.exception (exception);
      return 0;
    }

  // Create a new one and pass it back
  return new PortableServer::ObjectId (*this->object_id_);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Auto_Basic_Ptr<TAO_Id_Assignment_Policy>;
template class ACE_Auto_Basic_Ptr<TAO_Id_Uniqueness_Policy>;
template class ACE_Auto_Basic_Ptr<TAO_Implicit_Activation_Policy>;
template class ACE_Auto_Basic_Ptr<TAO_Lifespan_Policy>;
template class ACE_Auto_Basic_Ptr<TAO_Active_Object_Map_Iterator_Impl>;
template class ACE_Auto_Basic_Ptr<TAO_POA>;
template class ACE_Auto_Basic_Ptr<TAO_Request_Processing_Policy>;
template class ACE_Auto_Basic_Ptr<TAO_Synchronization_Policy>;
template class ACE_Auto_Basic_Ptr<TAO_Servant_Retention_Policy>;
template class ACE_Auto_Basic_Ptr<TAO_Thread_Policy>;
template class ACE_Lock_Adapter<ACE_Null_Mutex>;
#if defined (ACE_HAS_THREADS)
template class ACE_Lock_Adapter<ACE_Recursive_Thread_Mutex>;
template class ACE_Lock_Adapter<ACE_Thread_Mutex>;
#endif /* ACE_HAS_THREADS */
template class ACE_Map_Entry<TAO_Unbounded_Sequence<unsigned char>, TAO_ServantBase *>;
template class ACE_Hash_Map_Entry<ACE_CString, TAO_POA *>;
template class ACE_Hash<ACE_CString>;
template class ACE_Equal_To<ACE_CString>;
template class ACE_Hash_Map_Manager<ACE_CString, TAO_POA *, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<ACE_CString, TAO_POA *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<ACE_CString, TAO_POA *, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<ACE_CString, TAO_POA *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator<ACE_CString, TAO_POA *, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, TAO_POA *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, TAO_POA *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Write_Guard<ACE_Lock>;
template class ACE_Read_Guard<ACE_Lock>;
template class ACE_Unbounded_Set<TAO_POA *>;
template class ACE_Unbounded_Set_Iterator<TAO_POA *>;
template class TAO_Object_Manager<CORBA::Policy>;
template class TAO_Unbounded_Object_Sequence<CORBA::Policy>;
template class auto_ptr<TAO_Id_Assignment_Policy>;
template class auto_ptr<TAO_Id_Uniqueness_Policy>;
template class auto_ptr<TAO_Implicit_Activation_Policy>;
template class auto_ptr<TAO_Lifespan_Policy>;
template class auto_ptr<TAO_Active_Object_Map_Iterator_Impl>;
template class auto_ptr<TAO_POA>;
template class auto_ptr<TAO_Request_Processing_Policy>;
template class auto_ptr<TAO_Synchronization_Policy>;
template class auto_ptr<TAO_Servant_Retention_Policy>;
template class auto_ptr<TAO_Thread_Policy>;
template class ACE_Node<TAO_POA *>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Map_Entry<TAO_Unbounded_Sequence<unsigned char>, TAO_ServantBase *>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_Id_Assignment_Policy>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_Id_Uniqueness_Policy>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_Implicit_Activation_Policy>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_Lifespan_Policy>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_Active_Object_Map_Iterator_Impl>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_POA>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_Request_Processing_Policy>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_Synchronization_Policy>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_Servant_Retention_Policy>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_Thread_Policy>
#pragma instantiate ACE_Lock_Adapter<ACE_Null_Mutex>
#if defined (ACE_HAS_THREADS)
#pragma instantiate ACE_Lock_Adapter<ACE_Recursive_Thread_Mutex>
#pragma instantiate ACE_Lock_Adapter<ACE_Thread_Mutex>
#endif /* ACE_HAS_THREADS */
#pragma instantiate ACE_Hash_Map_Entry<ACE_CString, TAO_POA *>
#pragma instantiate ACE_Hash<ACE_CString>
#pragma instantiate ACE_Equal_To<ACE_CString>
#pragma instantiate ACE_Hash_Map_Manager<ACE_CString, TAO_POA *, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_CString, TAO_POA *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<ACE_CString, TAO_POA *, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_CString, TAO_POA *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<ACE_CString, TAO_POA *, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, TAO_POA *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, TAO_POA *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Write_Guard<ACE_Lock>
#pragma instantiate ACE_Read_Guard<ACE_Lock>
#pragma instantiate ACE_Unbounded_Set<TAO_POA *>
#pragma instantiate ACE_Unbounded_Set_Iterator<TAO_POA *>
#pragma instantiate TAO_Object_Manager<CORBA::Policy>
#pragma instantiate TAO_Unbounded_Object_Sequence<CORBA::Policy>
#pragma instantiate auto_ptr<TAO_Id_Assignment_Policy>
#pragma instantiate auto_ptr<TAO_Id_Uniqueness_Policy>
#pragma instantiate auto_ptr<TAO_Implicit_Activation_Policy>
#pragma instantiate auto_ptr<TAO_Lifespan_Policy>
#pragma instantiate auto_ptr<TAO_Active_Object_Map_Iterator_Impl>
#pragma instantiate auto_ptr<TAO_POA>
#pragma instantiate auto_ptr<TAO_Request_Processing_Policy>
#pragma instantiate auto_ptr<TAO_Synchronization_Policy>
#pragma instantiate auto_ptr<TAO_Servant_Retention_Policy>
#pragma instantiate auto_ptr<TAO_Thread_Policy>
#pragma instantiate ACE_Node<TAO_POA *>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
