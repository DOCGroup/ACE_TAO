// @(#) $Id$

// CORBA
#include "tao/corba.h"

// POA
#include "tao/poa.h"

// Exception macros
#include "tao/poa_macros.h"

// auto_ptr class
#include "ace/Auto_Ptr.h"

// This is the maximum space require to convert the ulong into a string
const int TAO_POA::max_space_required_for_ulong = 24;

// This is the maximum space require to convert the ulong into a string
const int TAO_Creation_Time::max_space_required_for_two_ulong_to_hex = 8 * 2;

#if defined (ACE_HAS_WCHAR_TYPEDEFS_CHAR)
extern "C"
{
  u_int wslen (const CORBA::WChar *);
  CORBA::WChar *wscpy (CORBA::WChar *, const CORBA::WChar *);
}
#endif /* ACE_HAS_WCHAR_TYPEDEFS_CHAR */

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
TAO_Thread_Policy::value (CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);
  return this->value_;
}

PortableServer::Policy_ptr
TAO_Thread_Policy::copy (CORBA::Environment &env)
{
  auto_ptr<TAO_Thread_Policy> new_policy (new TAO_Thread_Policy (*this));

  PortableServer::Policy_var result = new_policy->_this (env);

  if (env.exception () != 0)
    return PortableServer::Policy::_nil ();
  else
    {
      // Make sure that the auto_ptr does not delete the
      // implementation object
      new_policy.release ();
      return result._retn ();
    }
}

void
TAO_Thread_Policy::destroy (CORBA::Environment &env)
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

PortableServer::POA_ptr
TAO_Thread_Policy::_default_POA (CORBA::Environment &env)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

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
TAO_Lifespan_Policy::value (CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);
  return this->value_;
}

PortableServer::Policy_ptr
TAO_Lifespan_Policy::copy (CORBA::Environment &env)
{
  auto_ptr<TAO_Lifespan_Policy> new_policy (new TAO_Lifespan_Policy (*this));

  PortableServer::Policy_var result = new_policy->_this (env);
  if (env.exception () != 0)
    return PortableServer::Policy::_nil ();
  else
    {
      // Make sure that the auto_ptr does not delete the
      // implementation object
      new_policy.release ();
      return result._retn ();
    }
}

void
TAO_Lifespan_Policy::destroy (CORBA::Environment &env)
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

PortableServer::POA_ptr
TAO_Lifespan_Policy::_default_POA (CORBA::Environment &env)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

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
TAO_Id_Uniqueness_Policy::value (CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);
  return this->value_;
}

PortableServer::Policy_ptr
TAO_Id_Uniqueness_Policy::copy (CORBA::Environment &env)
{
  auto_ptr<TAO_Id_Uniqueness_Policy> new_policy (new TAO_Id_Uniqueness_Policy (*this));

  PortableServer::Policy_var result = new_policy->_this (env);
  if (env.exception () != 0)
    return PortableServer::Policy::_nil ();
  else
    {
      // Make sure that the auto_ptr does not delete the
      // implementation object
      new_policy.release ();
      return result._retn ();
    }
}

void
TAO_Id_Uniqueness_Policy::destroy (CORBA::Environment &env)
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

PortableServer::POA_ptr
TAO_Id_Uniqueness_Policy::_default_POA (CORBA::Environment &env)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

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
TAO_Id_Assignment_Policy::value (CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);
  return this->value_;
}

PortableServer::Policy_ptr
TAO_Id_Assignment_Policy::copy (CORBA::Environment &env)
{
  auto_ptr<TAO_Id_Assignment_Policy> new_policy (new TAO_Id_Assignment_Policy (*this));

  PortableServer::Policy_var result = new_policy->_this (env);
  if (env.exception () != 0)
    return PortableServer::Policy::_nil ();
  else
    {
      // Make sure that the auto_ptr does not delete the
      // implementation object
      new_policy.release ();
      return result._retn ();
    }
}

void
TAO_Id_Assignment_Policy::destroy (CORBA::Environment &env)
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

PortableServer::POA_ptr
TAO_Id_Assignment_Policy::_default_POA (CORBA::Environment &env)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

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

PortableServer::Policy_ptr
TAO_Implicit_Activation_Policy::copy (CORBA::Environment &env)
{
  auto_ptr<TAO_Implicit_Activation_Policy> new_policy (new TAO_Implicit_Activation_Policy (*this));

  PortableServer::Policy_var result = new_policy->_this (env);
  if (env.exception () != 0)
    return PortableServer::Policy::_nil ();
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

PortableServer::POA_ptr
TAO_Implicit_Activation_Policy::_default_POA (CORBA::Environment &env)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

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

PortableServer::Policy_ptr
TAO_Servant_Retention_Policy::copy (CORBA::Environment &env)
{
  auto_ptr<TAO_Servant_Retention_Policy> new_policy (new TAO_Servant_Retention_Policy (*this));

  PortableServer::Policy_var result = new_policy->_this (env);
  if (env.exception () != 0)
    return PortableServer::Policy::_nil ();
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

PortableServer::POA_ptr
TAO_Servant_Retention_Policy::_default_POA (CORBA::Environment &env)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

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

PortableServer::Policy_ptr
TAO_Request_Processing_Policy::copy (CORBA::Environment &env)
{
  auto_ptr<TAO_Request_Processing_Policy> new_policy (new TAO_Request_Processing_Policy (*this));

  PortableServer::Policy_var result = new_policy->_this (env);
  if (env.exception () != 0)
    return PortableServer::Policy::_nil ();
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

PortableServer::POA_ptr
TAO_Request_Processing_Policy::_default_POA (CORBA::Environment &env)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

TAO_POA_Policies::TAO_POA_Policies (void)
  :  thread_ (PortableServer::ORB_CTRL_MODEL),
     lifespan_ (PortableServer::TRANSIENT),
     id_uniqueness_ (PortableServer::UNIQUE_ID),
     id_assignment_ (PortableServer::SYSTEM_ID),
     implicit_activation_ (PortableServer::NO_IMPLICIT_ACTIVATION),
     servant_retention_ (PortableServer::RETAIN),
     request_processing_ (PortableServer::USE_ACTIVE_OBJECT_MAP_ONLY)
{
}

void
TAO_POA_Policies::parse_policies (const PortableServer::PolicyList &policies,
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
TAO_POA_Policies::parse_policy (const PortableServer::Policy_ptr policy,
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

  CORBA::Exception *exception = new PortableServer::POA::InvalidPolicy;
  env.exception (exception);
  return;
}

PortableServer::ThreadPolicyValue
TAO_POA_Policies::thread (void) const
{
  return this->thread_;
}

void
TAO_POA_Policies::thread (PortableServer::ThreadPolicyValue value)
{
  this->thread_ = value;
}

PortableServer::LifespanPolicyValue
TAO_POA_Policies::lifespan (void) const
{
  return this->lifespan_;
}

void
TAO_POA_Policies::lifespan (PortableServer::LifespanPolicyValue value)
{
  this->lifespan_ = value;
}

PortableServer::IdUniquenessPolicyValue
TAO_POA_Policies::id_uniqueness (void) const
{
  return this->id_uniqueness_;
}

void
TAO_POA_Policies::id_uniqueness (PortableServer::IdUniquenessPolicyValue value)
{
  this->id_uniqueness_ = value;
}

PortableServer::IdAssignmentPolicyValue
TAO_POA_Policies::id_assignment (void) const
{
  return this->id_assignment_;
}

void
TAO_POA_Policies::id_assignment (PortableServer::IdAssignmentPolicyValue value)
{
  this->id_assignment_ = value;
}

PortableServer::ImplicitActivationPolicyValue
TAO_POA_Policies::implicit_activation (void) const
{
  return this->implicit_activation_;
}

void
TAO_POA_Policies::implicit_activation (PortableServer::ImplicitActivationPolicyValue value)
{
  this->implicit_activation_ = value;
}

PortableServer::ServantRetentionPolicyValue
TAO_POA_Policies::servant_retention (void) const
{
  return this->servant_retention_;
}

void
TAO_POA_Policies::servant_retention (PortableServer::ServantRetentionPolicyValue value)
{
  this->servant_retention_ = value;
}

PortableServer::RequestProcessingPolicyValue
TAO_POA_Policies::request_processing (void) const
{
  return this->request_processing_;
}

void
TAO_POA_Policies::request_processing (PortableServer::RequestProcessingPolicyValue value)
{
  this->request_processing_ = value;
}

TAO_Creation_Time::TAO_Creation_Time (const ACE_Time_Value &creation_time)
{
  // Convert seconds and micro seconds into string
  ACE_OS::sprintf (this->time_stamp_,
                   "%08.8x%08.8x",
                   creation_time.sec (),
                   creation_time.usec ());
}

TAO_Creation_Time::TAO_Creation_Time (void)
{
  ACE_OS::memset (this->time_stamp_, 
                  0, 
                  TAO_Creation_Time::creation_time_length ());
}

void
TAO_Creation_Time::creation_time (const void *creation_time)
{
  ACE_OS::memcpy (this->time_stamp_, 
                  creation_time, 
                  TAO_Creation_Time::creation_time_length ());
}
  
const void *
TAO_Creation_Time::creation_time (void) const
{
  return &this->time_stamp_[0];
}
  
int
TAO_Creation_Time::creation_time_length (void) 
{
  return TAO_Creation_Time::max_space_required_for_two_ulong_to_hex;
}
  
int 
TAO_Creation_Time::operator== (const TAO_Creation_Time &rhs) const
{
  return ACE_OS::memcmp (this->time_stamp_, 
                         rhs.time_stamp_, 
                         TAO_Creation_Time::creation_time_length ()) == 0;
}

int 
TAO_Creation_Time::operator!= (const TAO_Creation_Time &rhs) const
{
  return ACE_OS::memcmp (this->time_stamp_, 
                         rhs.time_stamp_, 
                         TAO_Creation_Time::creation_time_length ()) != 0;
}

int 
TAO_Creation_Time::operator== (const TAO_Temporary_Creation_Time &rhs) const
{
  return rhs == *this;
}

int 
TAO_Creation_Time::operator!= (const TAO_Temporary_Creation_Time &rhs) const
{
  return rhs != *this;
}

TAO_Temporary_Creation_Time::TAO_Temporary_Creation_Time (void)
  : time_stamp_ (0)
{
}

void 
TAO_Temporary_Creation_Time::creation_time (const void *creation_time)
{
  this->time_stamp_ = (void *) creation_time;
}
  
int 
TAO_Temporary_Creation_Time::operator== (const TAO_Creation_Time &rhs) const
{
  return ACE_OS::memcmp (this->time_stamp_, 
                         rhs.creation_time (), 
                         TAO_Creation_Time::creation_time_length ()) == 0;
}

int 
TAO_Temporary_Creation_Time::operator!= (const TAO_Creation_Time &rhs) const
{
  return ACE_OS::memcmp (this->time_stamp_, 
                         rhs.creation_time (), 
                         TAO_Creation_Time::creation_time_length ()) != 0;
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
    active_object_map_ (new TAO_Object_Table),
    delete_active_object_map_ (1),
    adapter_activator_ (),
    servant_activator_ (),
    servant_locator_ (),
    default_servant_ (0),
    children_ (),
    lock_ (),
    closing_down_ (0),
    counter_ (0),
    creation_time_ (ACE_OS::gettimeofday ())
{
  this->set_complete_name ();

  // Register self with manager
  this->poa_manager_.register_poa (this, env);
}

TAO_POA::TAO_POA (const TAO_POA::String &adapter_name,
                  TAO_POA_Manager &poa_manager,
                  const TAO_POA_Policies &policies,
                  TAO_POA *parent,
                  TAO_Object_Table &active_object_map,
                  CORBA::Environment &env)
  : name_ (adapter_name),
    poa_manager_ (poa_manager),
    policies_ (policies),
    parent_ (parent),
    active_object_map_ (&active_object_map),
    delete_active_object_map_ (0),
    adapter_activator_ (),
    servant_activator_ (),
    servant_locator_ (),
    default_servant_ (0),
    children_ (),
    lock_ (),
    closing_down_ (0),
    counter_ (0),
    creation_time_ (ACE_OS::gettimeofday ())
{
  this->set_complete_name ();

  // Register self with manager
  this->poa_manager_.register_poa (this, env);
}

TAO_POA *
TAO_POA::clone (const TAO_POA::String &adapter_name,
                TAO_POA_Manager &poa_manager,
                const TAO_POA_Policies &policies,
                TAO_POA *parent,
                CORBA::Environment &env)
{
  return new TAO_POA (adapter_name,
                      poa_manager,
                      policies,
                      parent,
                      env);
}

TAO_POA *
TAO_POA::clone (const TAO_POA::String &adapter_name,
                TAO_POA_Manager &poa_manager,
                const TAO_POA_Policies &policies,
                TAO_POA *parent,
                TAO_Object_Table &active_object_map,
                CORBA::Environment &env)
{
  return new TAO_POA (adapter_name,
                      poa_manager,
                      policies,
                      parent,
                      active_object_map,
                      env);
}

TAO_POA::~TAO_POA (void)
{
  if (this->delete_active_object_map_)
    delete active_object_map_;

  // Remove POA from the POAManager
  //
  // Note: Errors are ignored here since there is nothing we can do
  // about them
  //
  CORBA::Environment env;
  this->poa_manager_.remove_poa (this, env);
}

ACE_Lock &
TAO_POA::lock (void)
{
  return this->lock_;
}

PortableServer::POA_ptr
TAO_POA::create_POA (const char *adapter_name,
                     PortableServer::POAManager_ptr poa_manager,
                     const PortableServer::PolicyList &policies,
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
TAO_POA::create_POA (const TAO_POA::String &adapter_name,
                     TAO_POA_Manager &poa_manager,
                     const TAO_POA_Policies &policies,
                     CORBA::Environment &env)
{
  // Lock access to the POA for the duration of this transaction
  TAO_POA_WRITE_GUARD_RETURN (ACE_Lock, monitor, this->lock (), 0, env);

  return this->create_POA_i (adapter_name,
                             poa_manager,
                             policies,
                             env);
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

PortableServer::POA_ptr
TAO_POA::find_POA (const char *adapter_name,
                   CORBA::Boolean activate_it,
                   CORBA::Environment &env)
{
  TAO_POA::String name (adapter_name);

  TAO_POA *result = this->find_POA (name,
                                    activate_it,
                                    env);

  if (env.exception () != 0)
    return PortableServer::POA::_nil ();

  return result->_this (env);
}

TAO_POA *
TAO_POA::find_POA (const TAO_POA::String &adapter_name,
                   CORBA::Boolean activate_it,
                   CORBA::Environment &env)
{
  // Check if we are the POA the user is looking for
  if (adapter_name == this->name_)
    return this;

  // Lock access to the POA for the duration of this transaction
  TAO_POA_WRITE_GUARD_RETURN (ACE_Lock, monitor, this->lock (), 0, env);

  return this->find_POA_i_optimized (adapter_name,
                                     activate_it,
                                     env);

}

TAO_POA *
TAO_POA::find_POA_i (const TAO_POA::String &adapter_name,
                     CORBA::Boolean activate_it,
                     CORBA::Environment &env)
{
  // Check if we are the POA the user is looking for
  if (adapter_name == this->name_)
    return this;

  return this->find_POA_i_optimized (adapter_name,
                                     activate_it,
                                     env);
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

          CORBA::Boolean success =
            this->adapter_activator_->unknown_adapter (self.in (),
                                                       adapter_name.c_str (),
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
TAO_POA::destroy (CORBA::Boolean etherealize_objects,
                  CORBA::Boolean wait_for_completion,
                  CORBA::Environment &env)
{
  // Lock access to the POA for the duration of this transaction
  TAO_POA_WRITE_GUARD (ACE_Lock, monitor, this->lock (), env);

  this->destroy_i (etherealize_objects,
                   wait_for_completion,
                   env);
}

void
TAO_POA::destroy_i (CORBA::Boolean etherealize_objects,
                    CORBA::Boolean wait_for_completion,
                    CORBA::Environment &env)
{
  this->closing_down_ = 1;

  // This operation destroys the POA and all descendant POAs. The POA
  // so destroyed (that is, the POA with its name) may be re-created
  // later in the same process. (This differs from the
  // POAManager::deactivate operation that does not allow a
  // re-creation of its associated POA in the same process.)

  // Remove POA from the parent
  if (this->parent_ != 0)
    this->parent_->delete_child (this->name_, env);

  if (env.exception () != 0)
    return;

  // Remove all children POAs
  for (CHILDREN::iterator iterator = this->children_.begin ();
       iterator != this->children_.end () && env.exception () == 0;
       iterator++)
    {
      TAO_POA *child_poa = (*iterator).int_id_;
      child_poa->destroy (etherealize_objects,
                          wait_for_completion,
                          env);
    }

  if (env.exception () != 0)
    return;

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

  // Remove the registered objects
  if (etherealize_objects)
    {
      if (this->policies ().servant_retention () == PortableServer::RETAIN &&
          this->policies ().request_processing () == PortableServer::USE_SERVANT_MANAGER &&
          !CORBA::is_nil (this->servant_activator_.in ()))
        {
          PortableServer::POA_var self = this->_this (env);
          if (env.exception () != 0)
            return;

          while (1)
            {
              TAO_Object_Table::iterator iterator = this->active_object_map ().begin ();
              if (iterator == this->active_object_map ().end () || env.exception () != 0)
                break;

              PortableServer::Servant servant = 0;
              PortableServer::ObjectId id ((*iterator).ext_id_);

              int result = this->active_object_map ().unbind (id, servant);
              if (result != 0)
                {
                  CORBA::Exception *exception = new CORBA::OBJ_ADAPTER (CORBA::COMPLETED_NO);
                  env.exception (exception);
                  return;
                }

              CORBA::Boolean remaining_activations = CORBA::B_FALSE;

              if (this->policies ().id_uniqueness () == PortableServer::MULTIPLE_ID &&
                  this->active_object_map ().find (servant) != -1)
                remaining_activations = CORBA::B_TRUE;

              this->servant_activator_->etherealize (id,
                                                     self.in (),
                                                     servant,
                                                     CORBA::B_TRUE,
                                                     remaining_activations,
                                                     env);
            }
        }
    }

  // If the wait_for_completion parameter is TRUE, the destroy
  // operation will return only after all requests in process have
  // completed and all invocations of etherealize have
  // completed. Otherwise, the destroy operation returns after
  // destroying the POAs.

  ACE_UNUSED_ARG (wait_for_completion);

  // Commit suicide
  delete this;
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

TAO_POA_Policies &
TAO_POA::policies (void)
{
  return this->policies_;
}

TAO_Object_Table &
TAO_POA::active_object_map (void)
{
  return *this->active_object_map_;
}

PortableServer::ServantManager_ptr
TAO_POA::get_servant_manager (CORBA::Environment &env)
{
  // Lock access to the POA for the duration of this transaction
  TAO_POA_READ_GUARD_RETURN (ACE_Lock, monitor, this->lock (), PortableServer::ServantManager::_nil (), env);

  return this->get_servant_manager_i (env);
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
TAO_POA::set_servant_manager (PortableServer::ServantManager_ptr imgr,
                              CORBA::Environment &env)
{
  // Lock access to the POA for the duration of this transaction
  TAO_POA_WRITE_GUARD (ACE_Lock, monitor, this->lock (), env);

  this->set_servant_manager_i (imgr,
                               env);
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
TAO_POA::get_servant (CORBA::Environment &env)
{
  // Lock access to the POA for the duration of this transaction
  TAO_POA_READ_GUARD_RETURN (ACE_Lock, monitor, this->lock (), 0, env);

  return this->get_servant_i (env);
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
TAO_POA::set_servant (PortableServer::Servant servant,
                      CORBA::Environment &env)
{
  // Lock access to the POA for the duration of this transaction
  TAO_POA_WRITE_GUARD (ACE_Lock, monitor, this->lock (), env);

  this->set_servant_i (servant,
                       env);
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
TAO_POA::activate_object (PortableServer::Servant servant,
                          CORBA::Environment &env)
{
  // Lock access to the POA for the duration of this transaction
  TAO_POA_WRITE_GUARD_RETURN (ACE_Lock, monitor, this->lock (), 0, env);

  return this->activate_object_i (servant,
                                  env);
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
  PortableServer::ObjectId_var new_id = this->create_object_id ();

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
TAO_POA::activate_object_with_id (const PortableServer::ObjectId &id,
                                  PortableServer::Servant servant,
                                  CORBA::Environment &env)
{
  // Lock access to the POA for the duration of this transaction
  TAO_POA_WRITE_GUARD (ACE_Lock, monitor, this->lock (), env);

  this->activate_object_with_id_i (id,
                                   servant,
                                   env);
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
TAO_POA::deactivate_object (const PortableServer::ObjectId &oid,
                            CORBA::Environment &env)
{
  // Lock access to the POA for the duration of this transaction
  TAO_POA_WRITE_GUARD (ACE_Lock, monitor, this->lock (), env);

  this->deactivate_object_i (oid,
                             env);
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

      CORBA::Boolean remaining_activations = CORBA::B_FALSE;

      if (this->policies ().id_uniqueness () == PortableServer::MULTIPLE_ID &&
          this->active_object_map ().find (servant) != -1)
        remaining_activations = CORBA::B_TRUE;

      this->servant_activator_->etherealize (oid,
                                             self.in (),
                                             servant,
                                             CORBA::B_FALSE,
                                             remaining_activations,
                                             env);
    }
}

CORBA::Object_ptr
TAO_POA::create_reference (const char *intf,
                           CORBA::Environment &env)
{
  // Lock access to the POA for the duration of this transaction
  TAO_POA_WRITE_GUARD_RETURN (ACE_Lock, monitor, this->lock (), CORBA::Object::_nil (), env);

  return this->create_reference_i (intf,
                                   env);
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
  PortableServer::ObjectId_var new_id = this->create_object_id ();
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
TAO_POA::servant_to_id (PortableServer::Servant servant,
                        CORBA::Environment &env)
{
  // If we had upgradeable locks, this would initially be a read lock
  //
  // Lock access to the POA for the duration of this transaction
  TAO_POA_WRITE_GUARD_RETURN (ACE_Lock, monitor, this->lock (), 0, env);

  return this->servant_to_id_i (servant,
                                env);
}

PortableServer::ObjectId *
TAO_POA::servant_to_id_i (PortableServer::Servant servant,
                          CORBA::Environment &env)
{
  // This operation requires the RETAIN and either the UNIQUE_ID or
  // IMPLICIT_ACTIVATION policies; if not present, the WrongPolicy
  // exception is raised.
  if (!(this->policies ().servant_retention () == PortableServer::RETAIN &&
        (this->policies ().id_uniqueness () == PortableServer::UNIQUE_ID ||
         this->policies ().implicit_activation () == PortableServer::IMPLICIT_ACTIVATION)))
    {
      CORBA::Exception *exception = new PortableServer::POA::WrongPolicy;
      env.exception (exception);
      return 0;
    }

  // This operation has three possible behaviors.

  // If the POA has the UNIQUE_ID policy and the specified servant is
  // active, the Object Id associated with that servant is returned.
  PortableServer::ObjectId_var id;
  PortableServer::ObjectId_out id_out (id);
  if (this->policies ().id_uniqueness () == PortableServer::UNIQUE_ID &&
      this->active_object_map ().find (servant, id_out) != -1)
    {
      return id._retn ();
    }

  // If the POA has the IMPLICIT_ACTIVATION policy and either the POA
  // has the MULTIPLE_ID policy or the specified servant is not
  // active, the servant is activated using a POA-generated Object Id
  // and the Interface Id associated with the servant, and that Object
  // Id is returned.
  if (this->policies ().implicit_activation () == PortableServer::IMPLICIT_ACTIVATION)
    {
      if (this->policies ().id_uniqueness () == PortableServer::MULTIPLE_ID ||
          this->active_object_map ().find (servant) != 0)
        {
          PortableServer::ObjectId_var new_id = this->create_object_id ();

          if (this->active_object_map ().bind (new_id.in (), servant) == -1)
            {
              CORBA::Exception *exception = new CORBA::OBJ_ADAPTER (CORBA::COMPLETED_NO);
              env.exception (exception);
              return 0;
            }

          // Everything is finally ok
          return new_id._retn ();
        }
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
  if (!(this->policies ().servant_retention () == PortableServer::RETAIN ||
        this->policies ().request_processing () == PortableServer::USE_DEFAULT_SERVANT))
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
      TAO_ObjectKey_var key = reference->key (env);

      // If the object reference was not created by this POA, the
      // WrongAdapter exception is raised.
      PortableServer::ObjectId_var id;
      PortableServer::ObjectId_out id_out (id);
      TAO_POA::String poa_name;
      CORBA::Boolean persistent = CORBA::B_FALSE;
      TAO_Temporary_Creation_Time poa_creation_time;

      int result = this->parse_key_temporary_id (key.in (),
                                                 poa_name,
                                                 id_out,
                                                 persistent,
                                                 poa_creation_time);
      if (result != 0 ||
          poa_name != this->complete_name () ||
          persistent != this->persistent () ||
          !this->persistent () && poa_creation_time != this->creation_time_)
        {
          CORBA::Exception *exception = new PortableServer::POA::WrongAdapter;
          env.exception (exception);
          return 0;
        }

      return this->id_to_servant (id.in (), env);
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
  TAO_ObjectKey_var key = reference->key (env);
  PortableServer::ObjectId_var id;
  PortableServer::ObjectId_out id_out (id);
  TAO_POA::String poa_name;
  CORBA::Boolean persistent = CORBA::B_FALSE;
  TAO_Temporary_Creation_Time poa_creation_time;

  int result = this->parse_key_permanent_id (key.in (),
                                             poa_name,
                                             id_out,
                                             persistent,
                                             poa_creation_time);
  if (result != 0 ||
      poa_name != this->complete_name () ||
      persistent != this->persistent () ||
      !this->persistent () && poa_creation_time != this->creation_time_)
    {
      CORBA::Exception *exception = new PortableServer::POA::WrongAdapter;
      env.exception (exception);
      return 0;
    }

  // The object denoted by the reference does not have to be active
  // for this operation to succeed.
  return id._retn ();
}

PortableServer::Servant
TAO_POA::id_to_servant (const PortableServer::ObjectId &oid,
                        CORBA::Environment &env)
{
  // Lock access to the POA for the duration of this transaction
  TAO_POA_READ_GUARD_RETURN (ACE_Lock, monitor, this->lock (), 0, env);

  return this->id_to_servant_i (oid,
                                env);
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
TAO_POA::id_to_reference (const PortableServer::ObjectId &oid,
                          CORBA::Environment &env)
{
  // Lock access to the POA for the duration of this transaction
  TAO_POA_READ_GUARD_RETURN (ACE_Lock, monitor, this->lock (), 0, env);

  return this->id_to_reference_i (oid, env);
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

PortableServer::POA_ptr
TAO_POA::the_parent (CORBA::Environment &env)
{
  if (this->parent_ != 0)
    return this->parent_->_this (env);
  else
    return PortableServer::POA::_nil ();
}

PortableServer::POAManager_ptr
TAO_POA::the_POAManager (CORBA::Environment &env)
{
  return this->poa_manager_._this (env);
}

PortableServer::AdapterActivator_ptr
TAO_POA::the_activator (CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);

  return PortableServer::AdapterActivator::_duplicate (this->adapter_activator_.in ());
}

void
TAO_POA::the_activator (PortableServer::AdapterActivator_ptr adapter_activator,
                        CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);

  this->adapter_activator_ = PortableServer::AdapterActivator::_duplicate (adapter_activator);
}

TAO_POA *
TAO_POA::locate_poa_i (const TAO_ObjectKey &key,
                       PortableServer::ObjectId_out id,
                       CORBA::Environment &env)
{
  TAO_POA::String poa_name;
  CORBA::Boolean persistent = CORBA::B_FALSE;
  TAO_Temporary_Creation_Time poa_creation_time;

  int result = this->parse_key_temporary_id (key,
                                             poa_name,
                                             id,
                                             persistent,
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
  // Lock access to the POAManager for the duration of this transaction
  TAO_POA_READ_GUARD_RETURN (ACE_Lock, monitor, this->lock (), -1, env);

  return this->locate_servant_i (key, env);
}

int
TAO_POA::locate_servant_i (const TAO_ObjectKey &key,
                           CORBA::Environment &env)
{
  PortableServer::ObjectId_var id;
  PortableServer::ObjectId_out id_out (id);

  TAO_POA *poa = this->locate_poa_i (key,
                                     id_out,
                                     env);
  if (env.exception () != 0)
    return -1;

  // If the POA has the RETAIN policy, the POA looks in the Active
  // Object Map to find if there is a servant associated with the
  // Object Id value from the request. If such a servant exists,
  // return 0.
  if (poa->policies ().servant_retention () == PortableServer::RETAIN)
    {
      PortableServer::Servant servant = 0;
      if (poa->active_object_map ().find (id.in (), servant) != -1)
        // Success
        return 0;
    }

  // If the POA has the NON_RETAIN policy or has the RETAIN policy but
  // didn't find a servant in the Active Object Map, the POA takes the
  // following actions:

  // If the USE_ACTIVE_OBJECT_MAP_ONLY policy is in effect, the POA raises
  // the OBJECT_NOT_EXIST system exception.
  if (poa->policies ().request_processing () == PortableServer::USE_ACTIVE_OBJECT_MAP_ONLY)
    return -1;

  // If the POA has the USE_DEFAULT_SERVANT policy, a default servant
  // has been associated with the POA, return 0. If no servant has
  // been associated with the POA, return -1.
  if (poa->policies ().request_processing () == PortableServer::USE_DEFAULT_SERVANT)
    {
      if (poa->default_servant_ == 0)
        return -1;
      else
        // Success
        return 0;
    }

  // If the POA has the USE_SERVANT_MANAGER policy, a servant manager
  // has been associated with the POA, return 0. If no servant manager
  // has been associated with the POA, return -1.
  if (poa->policies ().request_processing () == PortableServer::USE_SERVANT_MANAGER)
    {
      if (CORBA::is_nil (poa->servant_activator_.in ()) &&
          CORBA::is_nil (poa->servant_locator_.in ()))
        return -1;
      else
        // Success
        return 0;
    }

  // Failure
  return -1;
}

PortableServer::Servant
TAO_POA::locate_poa_and_servant_i (const TAO_ObjectKey &key,
                                   const char *operation,
                                   PortableServer::ObjectId_out id,
                                   TAO_POA *&poa_impl,
                                   CORBA::Environment &env)
{
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
      if (poa_impl->active_object_map ().find (*id.ptr (), servant) != -1)
        // Success
        return servant;
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
          PortableServer::Servant servant = poa_impl->servant_activator_->incarnate (*id.ptr (),
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
          if (poa_impl->active_object_map ().bind (*id.ptr (), servant) == -1)
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
          PortableServer::Servant servant = poa_impl->servant_locator_->preinvoke (*id.ptr (),
                                                                                   poa.in (),
                                                                                   operation,
                                                                                   cookie,
                                                                                   env);
          if (env.exception () != 0 || servant == 0)
            return 0;

          // Remember the cookie
          TAO_ORB_Core *orb_core = TAO_ORB_Core_instance ();
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
                           CORBA::Environment &env)
{
  // Lock access to the POAManager for the duration of this transaction
  TAO_POA_READ_GUARD (ACE_Lock, monitor, this->lock (), env);

  this->dispatch_servant_i (key, req, context, env);
}

void
TAO_POA::dispatch_servant_i (const TAO_ObjectKey &key,
                             CORBA::ServerRequest &req,
                             void *context,
                             CORBA::Environment &env)
{
  PortableServer::ObjectId_var id;
  PortableServer::ObjectId_out id_out (id);
  TAO_POA *poa = 0;
  // const char *operation = req.operation ();
  const char *operation = req.op_name ();

  PortableServer::Servant servant = this->locate_poa_and_servant_i (key,
                                                                    operation,
                                                                    id_out,
                                                                    poa,
                                                                    env);
  if (env.exception () != 0 || servant == 0)
    return;

  // Setup for upcall
  TAO_POA_Current upcall_context (poa, key, id.in (), servant);
  TAO_POA_Current *previous_context;
  poa->pre_invoke (upcall_context,
                   previous_context,
                   env);

  servant->_dispatch (req,
                      context,
                      env);

  // Cleanup from upcall
  poa->post_invoke (servant,
                    operation,
                    previous_context,
                    env);
}

void
TAO_POA::pre_invoke (TAO_POA_Current &upcall_context,
                     TAO_POA_Current *&previous_context,
                     CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);

  TAO_ORB_Core *orb_core = TAO_ORB_Core_instance ();
  previous_context = orb_core->poa_current (&upcall_context);
}

void
TAO_POA::post_invoke (PortableServer::Servant servant,
                      const char *operation,
                      TAO_POA_Current *previous_context,
                      CORBA::Environment &env)
{
  TAO_ORB_Core *orb_core = TAO_ORB_Core_instance ();
  TAO_POA_Current *poa_current = orb_core->poa_current (previous_context);

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

const TAO_Creation_Time &
TAO_POA::creation_time (void)
{
  return this->creation_time_;
}

int
TAO_POA::parse_key_permanent_id (const TAO_ObjectKey &key,
                                 TAO_POA::String &poa_name,
                                 PortableServer::ObjectId_out id,
                                 CORBA::Boolean &persistent,
                                 TAO_Temporary_Creation_Time &poa_creation_time)
{
  return this->parse_key (key,
                          poa_name,
                          id,
                          persistent,
                          poa_creation_time,
                          0);
}

int
TAO_POA::parse_key_temporary_id (const TAO_ObjectKey &key,
                                 TAO_POA::String &poa_name,
                                 PortableServer::ObjectId_out id,
                                 CORBA::Boolean &persistent,
                                 TAO_Temporary_Creation_Time &poa_creation_time)
{
  return this->parse_key (key,
                          poa_name,
                          id,
                          persistent,
                          poa_creation_time,
                          1);
}

int 
TAO_POA::rfind (const TAO_ObjectKey &key,
                char c, 
                int pos) const
{
  if (pos == ACE_CString::npos)
    pos = key.length ();
  
  for (int i = pos - 1; i >= 0; i--)
    if (key[i] == c)
      return i;

  return TAO_POA::String::npos;  
}


int
TAO_POA::parse_key (const TAO_ObjectKey &key,
                    TAO_POA::String &poa_name,
                    PortableServer::ObjectId_out id,
                    CORBA::Boolean &persistent,
                    TAO_Temporary_Creation_Time &poa_creation_time,
                    int temporary_id)
{
  // Try to find the last separator
  int last_token_position = this->rfind (key, TAO_POA::name_separator ());

  // If not found, the name is not correct
  if (last_token_position == TAO_POA::String::npos)
    return -1;

  // Check the first byte (persistence indicator)
  char object_key_type = key[0];
  if (object_key_type == this->persistent_key_type ())
    persistent = 1;
  else if (object_key_type == this->transient_key_type ())
    persistent = 0;
  else
    // Incorrect key
    return -1;

  // Starting at object_key_type_length, take the next
  // creation_time_length byte for the timestamp
  int starting_at = TAO_POA::object_key_type_length ();
  poa_creation_time.creation_time (&key[starting_at]);

  // Take the substring from creation_time_length +
  // object_key_type_length to last_token_position for the POA name
  starting_at = TAO_POA::object_key_type_length () + TAO_Creation_Time::creation_time_length ();
  int how_many = last_token_position - starting_at;
  poa_name.set ((const char *) &key[starting_at],
                how_many);

  // Take the substring from (last_token_position + separator_length)
  // to length for the objectId
  starting_at = last_token_position + TAO_POA::name_separator_length ();
  how_many = key.length () - starting_at;
  
  // Only for the life time of the upcall
  if (temporary_id)
    id = new PortableServer::ObjectId (how_many,
                                       how_many,
                                       (CORBA::Octet *) &key[starting_at],
                                       CORBA::B_FALSE);
  else
    // You can use the id as long as you like
    id = TAO_POA::string_to_ObjectId ((const char *) &key[starting_at],
                                      how_many);

  // Success
  return 0;
}

CORBA::Boolean
TAO_POA::persistent (void)
{
  return this->policies ().lifespan () == PortableServer::PERSISTENT;
}

PortableServer::ObjectId *
TAO_POA::create_object_id (void)
{
  // Note:  This method assumes two things:
  // 1. Locks are held when this method is called
  // 2. Size of octet == size of string element

  // Buffer for counter
  char counter[TAO_POA::max_space_required_for_ulong];

  // Convert counter into string
  ACE_OS::sprintf (counter,
                   "%ld",
                   this->counter_);

#if defined (POA_NAME_IN_POA_GENERATED_ID)

  // Calculate the required buffer size.
  // Note: 1 is for the null terminator
  TAO_POA::String id =
    this->name_ +
    TAO_POA::id_separator () +
    counter;

  char *result = id.c_str ();

#else /* POA_NAME_IN_POA_GENERATED_ID */
  
  char *result = counter;
  
#endif /* POA_NAME_IN_POA_GENERATED_ID */

  // Increment counter
  this->counter_++;

  // Create the sequence
  return TAO_POA::string_to_ObjectId (result);
}

TAO_ObjectKey *
TAO_POA::create_object_key (const PortableServer::ObjectId &id)
{
  // Calculate the space required for the key
  int buffer_size =
    this->object_key_type_length () +
    TAO_Creation_Time::creation_time_length () +
    this->complete_name_.length () +
    TAO_POA::name_separator_length () +
    id.length ();
  
  // Create the buffer for the key
  CORBA::Octet *buffer = TAO_ObjectKey::allocbuf (buffer_size);

  // Keeps track of where the next infomation goes; start at 0 byte
  int starting_at = 0;

  // Copy the persistence bit
  buffer[starting_at] = (CORBA::Octet) this->object_key_type ();

  // Then copy the timestamp
  starting_at += this->object_key_type_length ();  
  ACE_OS::memcpy (&buffer[starting_at], 
                  this->creation_time_.creation_time (),
                  TAO_Creation_Time::creation_time_length ());

  // Put the POA name into the buffer
  starting_at += TAO_Creation_Time::creation_time_length ();
  ACE_OS::memcpy (&buffer[starting_at],
                  this->complete_name_.c_str (),
                  this->complete_name_.length ());
  
  // Add the name separator
  starting_at += this->complete_name_.length ();
  buffer[starting_at] = (CORBA::Octet) TAO_POA::name_separator ();

  // Then copy the ID into the key
  starting_at += TAO_POA::name_separator_length ();
  ACE_OS::memcpy (&buffer[starting_at], 
                  id.buffer (), 
                  id.length ());

  // Create the key, giving the ownership of the buffer to the
  // sequence.
  return new TAO_ObjectKey (buffer_size,
                            buffer_size,
                            buffer,
                            CORBA::B_TRUE);
}

int
TAO_POA::is_poa_generated_id (const PortableServer::ObjectId &id)
{

#if defined (POA_NAME_IN_POA_GENERATED_ID)

  // Grab the buffer
  const char *id_buffer = (const char *) id.buffer ();

  // Check to see if the POA name is the first part of the id
  return 
    this->name_.length () < id.length () &&
    ACE_OS::strncmp (id_buffer,
                     this->name_.c_str (),
                     this->name_.length ()) == 0;

#else /* POA_NAME_IN_POA_GENERATED_ID */
  
  return 1;

#endif /* POA_NAME_IN_POA_GENERATED_ID */
}

int
TAO_POA::is_poa_generated_key (const TAO_ObjectKey &key)
{
  // Grab the buffer
  const char *key_buffer = (const char *) key.buffer ();

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
      topmost_poa_name = adapter_name.substr (starting_at,
                                              how_many);

      // Take the substring from (token_position + separator_length)
      // to length
      starting_at = token_position + TAO_POA::name_separator_length ();
      how_many = adapter_name.length () - starting_at;
      tail_poa_name = adapter_name.substr (starting_at,
                                           how_many);
    }
}

CORBA::String
TAO_POA::the_name (CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);

  return CORBA::string_dup (this->name_.c_str ());
}

TAO_POA::String
TAO_POA::complete_name (void)
{
  return this->complete_name_;
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
                                       CORBA::B_TRUE);
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
                                       CORBA::B_TRUE);
}

PortableServer::ObjectId *
TAO_POA::wstring_to_ObjectId (const CORBA::WChar *string)
{
  // Size of Id
  //
  // We DO NOT include the zero terminator, as this is simply an
  // artifact of the way strings are stored in C.
  //
#if defined (ACE_HAS_WCHAR_TYPEDEFS_CHAR)
  CORBA::ULong string_length = wslen (string);
#else  /* ! ACE_HAS_WCHAR_TYPEDEFS_CHAR */
  CORBA::ULong string_length = ACE_OS::strlen (string);
#endif /* ! ACE_HAS_WCHAR_TYPEDEFS_CHAR */

  size_t buffer_size = string_length * sizeof (CORBA::WChar);

  // Create the buffer for the Id
  CORBA::Octet *buffer = PortableServer::ObjectId::allocbuf (buffer_size);

  // Copy contents
  ACE_OS::memcpy (buffer, string, buffer_size);

  // Create a new ID
  return new PortableServer::ObjectId (buffer_size,
                                       buffer_size,
                                       buffer,
                                       CORBA::B_TRUE);
}

CORBA::String
TAO_POA::ObjectId_to_string (const PortableServer::ObjectId &id)
{
  // Create space
  CORBA::String string = CORBA::string_alloc (id.length ());
  
  // Copy the data
  ACE_OS::memcpy (string, id.buffer (), id.length ());

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
  ACE_OS::memcpy (string, id.buffer (), id.length () * sizeof (CORBA::WChar));

  // Null terminate the string
  string[id.length ()] = '\0';  

  // Return string
  return string;
}

char
TAO_POA::name_separator (void)
{
  return '/';
}

char
TAO_POA::id_separator (void)
{
  return ':';
}

CORBA::ULong
TAO_POA::name_separator_length (void)
{
  return sizeof (char);
}

CORBA::ULong
TAO_POA::id_separator_length (void)
{
  return sizeof (char);
}

char
TAO_POA::object_key_type (void)
{
  if (this->persistent ())
    return TAO_POA::persistent_key_type ();
  else
    return TAO_POA::transient_key_type ();
}

char
TAO_POA::persistent_key_type (void)
{
  return 'P';
}

char
TAO_POA::transient_key_type (void)
{
  return 'T';
}

CORBA::ULong
TAO_POA::object_key_type_length (void)
{
  return sizeof (char);
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
      if (isascii (byte) && isprint (byte) && byte != '\\')
        {
          *cp++ = (char) byte;
          continue;
        }

      *cp++ = '\\';
      *cp++ = ACE::nibble2hex (byte & 0x0f);
      *cp++ = ACE::nibble2hex ((byte >> 4) & 0x0f);
    }
  // Zero terminate
  *cp = '\0';
}

void
TAO_POA::decode_string_to_sequence (TAO_Unbounded_Sequence<CORBA::Octet> &seq,
                                    CORBA::String str)
{
  if (str == 0)
    {
      seq.length (0);
      return;
    }

  u_int length = ACE_OS::strlen (str);
  char *eos = str + length;
  char *cp = str;

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
        {
          // Copy it in
          seq[i] = *cp++;
        }
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
  PortableServer::PolicyList default_policies;

  // This assumes that the lock on the parent is recursive
  PortableServer::POA_var child = parent->create_POA (name,
                                                      PortableServer::POAManager::_nil (),
                                                      default_policies,
                                                      env);

  if (env.exception () != 0)
    return CORBA::B_FALSE;
  else
    {
      PortableServer::AdapterActivator_var activator = this->_this (env);
      if (env.exception () != 0)
        {
          child->destroy (CORBA::B_FALSE,
                          CORBA::B_FALSE,
                          env);
          return CORBA::B_FALSE;
        }

      child->the_activator (activator.in (), env);

      if (env.exception () != 0)
        {
          child->destroy (CORBA::B_FALSE,
                          CORBA::B_FALSE,
                          env);
          return CORBA::B_FALSE;
        }

      // Finally everything is fine
      return CORBA::B_TRUE;
    }
}

TAO_POA_Manager::TAO_POA_Manager (void)
  : state_ (HOLDING),
    closing_down_ (0),
    lock_ (),
    poa_collection_ ()
{
}

TAO_POA_Manager::~TAO_POA_Manager (void)
{
}

void
TAO_POA_Manager::activate (CORBA::Environment &env)
{
  // Lock access to the POAManager for the duration of this transaction
  TAO_POA_WRITE_GUARD (ACE_Lock, monitor, this->lock (), env);

  // This operation changes the state of the POA manager to active. If
  // issued while the POA manager is in the inactive state, the
  // AdapterInactive exception is raised.  Entering the active state
  // enables the associated POAs to process requests.

  if (this->state_ == INACTIVE)
    {
      CORBA::Exception *exception = new PortableServer::POAManager::AdapterInactive;
      env.exception (exception);
      return;
    }
  else
    this->state_ = ACTIVE;
}

void
TAO_POA_Manager::hold_requests (CORBA::Boolean wait_for_completion,
                                CORBA::Environment &env)
{
  // Lock access to the POAManager for the duration of this transaction
  TAO_POA_WRITE_GUARD (ACE_Lock, monitor, this->lock (), env);

  // This operation changes the state of the POA manager to
  // holding. If issued while the POA manager is in the inactive
  // state, the AdapterInactive exception is raised.  Entering the
  // holding state causes the associated POAs to queue incoming
  // requests.  Any requests that have been queued but have not
  // started executing will continue to be queued while in the holding
  // state.

  if (this->state_ == INACTIVE)
    {
      CORBA::Exception *exception = new PortableServer::POAManager::AdapterInactive;
      env.exception (exception);
      return;
    }
  else
    this->state_ = HOLDING;

  // If the wait_for_completion parameter is FALSE, this operation
  // returns immediately after changing the state. If the parameter is
  // TRUE, this operation does not return until either there are no
  // actively executing requests in any of the POAs associated with
  // this POA manager (that is, all requests that were started prior
  // to the state change have completed) or the state of the POA
  // manager is changed to a state other than holding.

  ACE_UNUSED_ARG (wait_for_completion);
}

void
TAO_POA_Manager::discard_requests (CORBA::Boolean wait_for_completion,
                                   CORBA::Environment &env)
{
  // Lock access to the POAManager for the duration of this transaction
  TAO_POA_WRITE_GUARD (ACE_Lock, monitor, this->lock (), env);

  // This operation changes the state of the POA manager to
  // discarding. If issued while the POA manager is in the inactive
  // state, the AdapterInactive exception is raised.  Entering the
  // discarding state causes the associated POAs to discard incoming
  // requests.  In addition, any requests that have been queued but
  // have not started executing are discarded. When a request is
  // discarded, a TRANSIENT system exception is returned to the
  // client.

  if (this->state_ == INACTIVE)
    {
      CORBA::Exception *exception = new PortableServer::POAManager::AdapterInactive;
      env.exception (exception);
      return;
    }
  else
    this->state_ = DISCARDING;

  // If the wait_for_completion parameter is FALSE, this operation
  // returns immediately after changing the state. If the parameter is
  // TRUE, this operation does not return until either there are no
  // actively executing requests in any of the POAs associated with
  // this POA manager (that is, all requests that were started prior
  // to the state change have completed) or the state of the POA
  // manager is changed to a state other than discarding.

  ACE_UNUSED_ARG (wait_for_completion);
}

void
TAO_POA_Manager::deactivate (CORBA::Boolean etherealize_objects,
                             CORBA::Boolean wait_for_completion,
                             CORBA::Environment &env)
{
  // Lock access to the POAManager for the duration of this transaction
  TAO_POA_WRITE_GUARD (ACE_Lock, monitor, this->lock (), env);

  this->closing_down_ = 1;

  // This operation changes the state of the POA manager to
  // inactive. If issued while the POA manager is in the inactive
  // state, the AdapterInactive exception is raised.  Entering the
  // inactive state causes the associated POAs to reject requests that
  // have not begun to be executed as well as any new requests.

  if (this->state_ == INACTIVE)
    {
      CORBA::Exception *exception = new PortableServer::POAManager::AdapterInactive;
      env.exception (exception);
      return;
    }
  else
    this->state_ = INACTIVE;

  // After changing the state, if the etherealize_objects parameter is:
  //
  // a) TRUE - the POA manager will cause all associated POAs that
  // have the RETAIN and USE_SERVANT_MANAGER policies to perform the
  // etherealize operation on the associated servant manager for all
  // active objects.
  //
  // b) FALSE - the etherealize operation is not called. The purpose
  // is to provide developers with a means to shut down POAs in a
  // crisis (for example, unrecoverable error) situation.

  for (POA_COLLECTION::iterator iterator = this->poa_collection_.begin ();
       iterator != this->poa_collection_.end () && env.exception () == 0;
       iterator++)
    {
      TAO_POA *poa = *iterator;
      poa->destroy (etherealize_objects,
                    wait_for_completion,
                    env);
    }

  // If the wait_for_completion parameter is FALSE, this operation
  // will return immediately after changing the state. If the
  // parameter is TRUE, this operation does not return until there are
  // no actively executing requests in any of the POAs associated with
  // this POA manager (that is, all requests that were started prior
  // to the state change have completed) and, in the case of a TRUE
  // etherealize_objects, all invocations of etherealize have
  // completed for POAs having the RETAIN and USE_SERVANT_MANAGER
  // policies.

  // If the ORB::shutdown operation is called, it makes a call on
  // deactivate with a TRUE etherealize_objects parameter for each POA
  // manager known in the process; the wait_for_completion parameter
  // to deactivate will be the same as the similarly named parameter
  // of ORB::shutdown.
}

TAO_POA_Manager::Processing_State
TAO_POA_Manager::state (CORBA::Environment &env)
{
  // Lock access to the POAManager for the duration of this transaction
  TAO_POA_READ_GUARD_RETURN (ACE_Lock, monitor, this->lock (), UNKNOWN, env);

  return this->state_;
}

void
TAO_POA_Manager::remove_poa (TAO_POA *poa,
                             CORBA::Environment &env)
{
  // If we are not closing down, we must remove this poa from our
  // collection.
  if (!this->closing_down_)
    {
      // Lock access to the POAManager for the duration of this transaction
      TAO_POA_WRITE_GUARD (ACE_Lock, monitor, this->lock (), env);

      this->remove_poa_i (poa,
                          env);
    }

  // If we are closing down, we are currently iterating over our poa
  // collection and there is not need to remove this poa from our
  // collection.
}

void
TAO_POA_Manager::remove_poa_i (TAO_POA *poa,
                               CORBA::Environment &env)
{
  int result = this->poa_collection_.remove (poa);

  if (result != 0)
    {
      CORBA::Exception *exception = new CORBA::OBJ_ADAPTER (CORBA::COMPLETED_NO);
      env.exception (exception);
      return;
    }
}

void
TAO_POA_Manager::register_poa (TAO_POA *poa,
                               CORBA::Environment &env)
{
  // Lock access to the POAManager for the duration of this transaction
  TAO_POA_WRITE_GUARD (ACE_Lock, monitor, this->lock (), env);

  this->register_poa_i (poa,
                        env);
}

void
TAO_POA_Manager::register_poa_i (TAO_POA *poa,
                                 CORBA::Environment &env)
{
  int result = this->poa_collection_.insert (poa);

  if (result != 0)
    {
      CORBA::Exception *exception = new CORBA::OBJ_ADAPTER (CORBA::COMPLETED_NO);
      env.exception (exception);
      return;
    }
}

ACE_Lock &
TAO_POA_Manager::lock (void)
{
  return this->lock_;
}

TAO_POA_Manager *
TAO_POA_Manager::clone (void)
{
  return new TAO_POA_Manager;
}

TAO_Strategy_POA::TAO_Strategy_POA (const TAO_POA::String &adapter_name,
                                    TAO_POA_Manager &poa_manager,
                                    const TAO_POA_Policies &policies,
                                    TAO_POA *parent,
                                    CORBA::Environment &env)
  : TAO_POA (adapter_name,
             poa_manager,
             policies,
             parent,
             env),
    lock_ (0)
{
  this->lock_ = TAO_ORB_Core_instance ()->server_factory ()->create_poa_lock ();
}

TAO_Strategy_POA::TAO_Strategy_POA (const TAO_POA::String &adapter_name,
                                    TAO_POA_Manager &poa_manager,
                                    const TAO_POA_Policies &policies,
                                    TAO_POA *parent,
                                    TAO_Object_Table &active_object_map,
                                    CORBA::Environment &env)
  : TAO_POA (adapter_name,
             poa_manager,
             policies,
             parent,
             active_object_map,
             env),
    lock_ (0)
{
  this->lock_ = TAO_ORB_Core_instance ()->server_factory ()->create_poa_lock ();
}

TAO_Strategy_POA::~TAO_Strategy_POA (void)
{
  delete this->lock_;
}

TAO_POA *
TAO_Strategy_POA::clone (const TAO_POA::String &adapter_name,
                         TAO_POA_Manager &poa_manager,
                         const TAO_POA_Policies &policies,
                         TAO_POA *parent,
                         CORBA::Environment &env)
{
  return new SELF (adapter_name,
                   poa_manager,
                   policies,
                   parent,
                   env);
}

TAO_POA *
TAO_Strategy_POA::clone (const TAO_POA::String &adapter_name,
                         TAO_POA_Manager &poa_manager,
                         const TAO_POA_Policies &policies,
                         TAO_POA *parent,
                         TAO_Object_Table &active_object_map,
                         CORBA::Environment &env)
{
  return new SELF (adapter_name,
                   poa_manager,
                   policies,
                   parent,
                   active_object_map,
                   env);
}

ACE_Lock &
TAO_Strategy_POA::lock (void)
{
  return *this->lock_;
}

TAO_Strategy_POA_Manager::TAO_Strategy_POA_Manager (void)
  : TAO_POA_Manager (),
    lock_ (0)
{
  this->lock_ = TAO_ORB_Core_instance ()->server_factory ()->create_poa_mgr_lock ();
}

TAO_Strategy_POA_Manager::~TAO_Strategy_POA_Manager (void)
{
  delete this->lock_;
}

TAO_POA_Manager *
TAO_Strategy_POA_Manager::clone (void)
{
  return new SELF;
}

ACE_Lock &
TAO_Strategy_POA_Manager::lock (void)
{
  return *this->lock_;
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

void
TAO_POA_Current::clear (void)
{
  this->poa_impl_ = 0;
  this->object_id_ = 0;
  this->servant_ = 0;
  this->object_key_ = 0;
  this->cookie_ = 0;
}

int
TAO_POA_Current::context_is_valid (void)
{
  return
    this->poa_impl_ != 0 &&
    this->object_id_ != 0 &&
    this->servant_ != 0 &&
    this->object_key_ != 0;
}

void
TAO_POA_Current::POA_impl (TAO_POA *impl)
{
  this->poa_impl_ = impl;
}

TAO_POA *
TAO_POA_Current::POA_impl (void) const
{
  return this->poa_impl_;
}

void
TAO_POA_Current::object_id (const PortableServer::ObjectId &id)
{
  this->object_id_ = &id;
}

const PortableServer::ObjectId &
TAO_POA_Current::object_id (void) const
{
  return *this->object_id_;
}

void
TAO_POA_Current::object_key (const TAO_ObjectKey &key)
{
  this->object_key_ = &key;
}

const TAO_ObjectKey &
TAO_POA_Current::object_key (void) const
{
  return *this->object_key_;
}

void
TAO_POA_Current::servant (PortableServer::Servant servant)
{
  this->servant_ = servant;
}

PortableServer::Servant
TAO_POA_Current::servant (void) const
{
  return this->servant_;
}

int
TAO_POA_Current::in_upcall (void) const
{
  return (this->servant_ != 0);
}

PortableServer::ServantLocator::Cookie
TAO_POA_Current::locator_cookie (void) const
{
  return this->cookie_;
}

void
TAO_POA_Current::locator_cookie (PortableServer::ServantLocator::Cookie cookie)
{
  this->cookie_ = cookie;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Auto_Basic_Ptr<TAO_Id_Assignment_Policy>;
template class ACE_Auto_Basic_Ptr<TAO_Id_Uniqueness_Policy>;
template class ACE_Auto_Basic_Ptr<TAO_Implicit_Activation_Policy>;
template class ACE_Auto_Basic_Ptr<TAO_Lifespan_Policy>;
template class ACE_Auto_Basic_Ptr<TAO_Object_Table_Iterator_Impl>;
template class ACE_Auto_Basic_Ptr<TAO_POA>;
template class ACE_Auto_Basic_Ptr<TAO_Request_Processing_Policy>;
template class ACE_Auto_Basic_Ptr<TAO_Servant_Retention_Policy>;
template class ACE_Auto_Basic_Ptr<TAO_Thread_Policy>;
template class ACE_Lock_Adapter<ACE_Null_Mutex>;
#if defined (ACE_HAS_THREADS)
template class ACE_Lock_Adapter<ACE_Recursive_Thread_Mutex>;
template class ACE_Lock_Adapter<ACE_Thread_Mutex>;
#endif /* ACE_HAS_THREADS */
template class ACE_Map_Entry<TAO_Unbounded_Sequence<unsigned char>, TAO_ServantBase *>;
template class ACE_Map_Iterator<ACE_CString, TAO_POA *, ACE_Null_Mutex>;
template class ACE_Map_Iterator_Base<ACE_CString, TAO_POA *, ACE_Null_Mutex>;
template class ACE_Map_Manager<ACE_CString, TAO_POA *, ACE_Null_Mutex>;
template class ACE_Read_Guard<ACE_Lock>;
template class ACE_Unbounded_Set<TAO_POA *>;
template class ACE_Unbounded_Set_Iterator<TAO_POA *>;
template class ACE_Write_Guard<ACE_Lock>;
template class TAO_Object_Manager<PortableServer::Policy>;
template class TAO_Unbounded_Object_Sequence<PortableServer::Policy>;
template class auto_ptr<TAO_Id_Assignment_Policy>;
template class auto_ptr<TAO_Id_Uniqueness_Policy>;
template class auto_ptr<TAO_Implicit_Activation_Policy>;
template class auto_ptr<TAO_Lifespan_Policy>;
template class auto_ptr<TAO_Object_Table_Iterator_Impl>;
template class auto_ptr<TAO_POA>;
template class auto_ptr<TAO_Request_Processing_Policy>;
template class auto_ptr<TAO_Servant_Retention_Policy>;
template class auto_ptr<TAO_Thread_Policy>;
template class ACE_Map_Entry<ACE_CString, TAO_POA *>;
template class ACE_Map_Reverse_Iterator<ACE_CString, TAO_POA *, ACE_Null_Mutex>;
template class ACE_Node<TAO_POA *>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_Id_Assignment_Policy>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_Id_Uniqueness_Policy>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_Implicit_Activation_Policy>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_Lifespan_Policy>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_Object_Table_Iterator_Impl>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_POA>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_Request_Processing_Policy>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_Servant_Retention_Policy>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_Thread_Policy>
#pragma instantiate ACE_Lock_Adapter<ACE_Null_Mutex>
#if defined (ACE_HAS_THREADS)
#pragma instantiate ACE_Lock_Adapter<ACE_Recursive_Thread_Mutex>
#pragma instantiate ACE_Lock_Adapter<ACE_Thread_Mutex>
#endif /* ACE_HAS_THREADS */
#pragma instantiate ACE_Map_Entry<TAO_Unbounded_Sequence<unsigned char>, TAO_ServantBase *>
#pragma instantiate ACE_Map_Iterator<ACE_CString, TAO_POA *, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Iterator_Base<ACE_CString, TAO_POA *, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Manager<ACE_CString, TAO_POA *, ACE_Null_Mutex>
#pragma instantiate ACE_Read_Guard<ACE_Lock>
#pragma instantiate ACE_Unbounded_Set<TAO_POA *>
#pragma instantiate ACE_Unbounded_Set_Iterator<TAO_POA *>
#pragma instantiate ACE_Write_Guard<ACE_Lock>
#pragma instantiate TAO_Object_Manager<PortableServer::Policy>
#pragma instantiate TAO_Unbounded_Object_Sequence<PortableServer::Policy>
#pragma instantiate auto_ptr<TAO_Id_Assignment_Policy>
#pragma instantiate auto_ptr<TAO_Id_Uniqueness_Policy>
#pragma instantiate auto_ptr<TAO_Implicit_Activation_Policy>
#pragma instantiate auto_ptr<TAO_Lifespan_Policy>
#pragma instantiate auto_ptr<TAO_Object_Table_Iterator_Impl>
#pragma instantiate auto_ptr<TAO_POA>
#pragma instantiate auto_ptr<TAO_Request_Processing_Policy>
#pragma instantiate auto_ptr<TAO_Servant_Retention_Policy>
#pragma instantiate auto_ptr<TAO_Thread_Policy>
#pragma instantiate ACE_Map_Entry<ACE_CString, TAO_POA *>
#pragma instantiate ACE_Map_Reverse_Iterator<ACE_CString, TAO_POA *, ACE_Null_Mutex>
#pragma instantiate ACE_Node<TAO_POA *>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
