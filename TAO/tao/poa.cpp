// @(#) $Id$

// CORBA
#include "tao/corba.h" 

// POA 
#include "tao/poa.h"

// Exception macros
#include "tao/poa_macros.h"

// auto_ptr class
#include "ace/Auto_Ptr.h"

template <class STUB, class COLOCATED_SKELETON, class IMPLEMENTATION>
IMPLEMENTATION *
stub_to_impl (STUB stub)
{
  COLOCATED_SKELETON *colocated = ACE_dynamic_cast (COLOCATED_SKELETON *, stub);

  return ACE_dynamic_cast (IMPLEMENTATION *, colocated->_get_servant ());
}

TAO_Thread_Policy::TAO_Thread_Policy (PortableServer::ThreadPolicyValue value)
  : value_ (value)
{
}

TAO_Thread_Policy::TAO_Thread_Policy (const TAO_Thread_Policy &rhs)
  : value_ (rhs.value_)
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
  // Commit suicide:  must have been dynamically allocated
  delete this;
}

TAO_Lifespan_Policy::TAO_Lifespan_Policy (PortableServer::LifespanPolicyValue value)
  : value_ (value)
{
}

TAO_Lifespan_Policy::TAO_Lifespan_Policy (const TAO_Lifespan_Policy &rhs)
  : value_ (rhs.value_)
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
  // Commit suicide: must have been dynamically allocated
  delete this;
}

TAO_Id_Uniqueness_Policy::TAO_Id_Uniqueness_Policy (PortableServer::IdUniquenessPolicyValue value)
  : value_ (value)
{
}

TAO_Id_Uniqueness_Policy::TAO_Id_Uniqueness_Policy (const TAO_Id_Uniqueness_Policy &rhs)
  : value_ (rhs.value_)
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
  // Commit suicide: must have been dynamically allocated
  delete this;
}

TAO_Id_Assignment_Policy::TAO_Id_Assignment_Policy (PortableServer::IdAssignmentPolicyValue value)
  : value_ (value)
{
}

TAO_Id_Assignment_Policy::TAO_Id_Assignment_Policy (const TAO_Id_Assignment_Policy &rhs)
  : value_ (rhs.value_)
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
  // Commit suicide: must have been dynamically allocated
  delete this;
}

TAO_Implicit_Activation_Policy::TAO_Implicit_Activation_Policy (PortableServer::ImplicitActivationPolicyValue value)
  : value_ (value)
{
}

TAO_Implicit_Activation_Policy::TAO_Implicit_Activation_Policy (const TAO_Implicit_Activation_Policy &rhs)
  : value_ (rhs.value_)
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
  // Commit suicide: must have been dynamically allocated
  delete this;
}

TAO_Servant_Retention_Policy::TAO_Servant_Retention_Policy (PortableServer::ServantRetentionPolicyValue value)
  : value_ (value)
{
}

TAO_Servant_Retention_Policy::TAO_Servant_Retention_Policy (const TAO_Servant_Retention_Policy &rhs)
  : value_ (rhs.value_)
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
  // Commit suicide: must have been dynamically allocated
  delete this;
}

TAO_Request_Processing_Policy::TAO_Request_Processing_Policy (PortableServer::RequestProcessingPolicyValue value)
  : value_ (value)
{
}

TAO_Request_Processing_Policy::TAO_Request_Processing_Policy (const TAO_Request_Processing_Policy &rhs)
  : value_ (rhs.value_)
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
  // Commit suicide: must have been dynamically allocated
  delete this;
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
TAO_POA_Policies::parse_policies (const PortableServer::PolicyList& policies,
                                  CORBA::Environment &env)
{
  for (CORBA::ULong i = 0;
       i < policies.length () && env.exception () == 0;
       i++)
    {
      this->parse_policy (policies[i].in (), env);
    }
}

void
TAO_POA_Policies::parse_policy (const PortableServer::Policy_ptr policy,
                                CORBA::Environment &env)
{
  PortableServer::ThreadPolicy_var thread
    = PortableServer::ThreadPolicy::_narrow (policy, env);
  if (!CORBA::is_nil (thread))
    {
      this->thread_ = thread->value (env);
      return;
    }

  PortableServer::LifespanPolicy_var lifespan
    = PortableServer::LifespanPolicy::_narrow (policy, env);
  if (!CORBA::is_nil (lifespan))
    {
      this->lifespan_ = lifespan->value (env);
      return;
    }

  PortableServer::IdUniquenessPolicy_var id_uniqueness
    = PortableServer::IdUniquenessPolicy::_narrow (policy, env);
  if (!CORBA::is_nil (id_uniqueness))
    {
      this->id_uniqueness_ = id_uniqueness->value (env);
      return;
    }

  PortableServer::IdAssignmentPolicy_var id_assignment
    = PortableServer::IdAssignmentPolicy::_narrow (policy, env);
  if (!CORBA::is_nil (id_assignment))
    {
      this->id_assignment_ = id_assignment->value (env);
      return;
    }

  PortableServer::ImplicitActivationPolicy_var implicit_activation
    = PortableServer::ImplicitActivationPolicy::_narrow (policy, env);
  if (!CORBA::is_nil (implicit_activation))
    {
      this->implicit_activation_ = implicit_activation->value (env);
      return;
    }

  PortableServer::ServantRetentionPolicy_var servant_retention
    = PortableServer::ServantRetentionPolicy::_narrow (policy, env);
  if (!CORBA::is_nil (servant_retention))
    {
      this->servant_retention_ = servant_retention->value (env);
      return;
    }

  PortableServer::RequestProcessingPolicy_var request_processing
    = PortableServer::RequestProcessingPolicy::_narrow (policy, env);
  if (!CORBA::is_nil (request_processing))
    {
      this->request_processing_ = request_processing->value (env);
      return;
    }

  CORBA::Exception *exception = new PortableServer::POA::InvalidPolicy;
  env.exception (exception);
  return;
}

PortableServer::ThreadPolicyValue
TAO_POA_Policies::thread (void)
{
  return this->thread_;
}

PortableServer::LifespanPolicyValue
TAO_POA_Policies::lifespan (void)
{
  return this->lifespan_;
}

PortableServer::IdUniquenessPolicyValue
TAO_POA_Policies::id_uniqueness (void)
{
  return this->id_uniqueness_;
}

PortableServer::IdAssignmentPolicyValue
TAO_POA_Policies::id_assignment (void)
{
  return this->id_assignment_;
}

PortableServer::ImplicitActivationPolicyValue
TAO_POA_Policies::implicit_activation (void)
{
  return this->implicit_activation_;
}

PortableServer::ServantRetentionPolicyValue
TAO_POA_Policies::servant_retention (void)
{
  return this->servant_retention_;
}

PortableServer::RequestProcessingPolicyValue
TAO_POA_Policies::request_processing (void)
{
  return this->request_processing_;
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
    active_object_table_ (new TAO_Object_Table),
    delete_active_object_table_ (1),
    adapter_activator_ (),
    servant_activator_ (),
    servant_locator_ (),
    default_servant_ (0),
    children_ (),
    lock_ (),
    closing_down_ (0),
    counter_ (0)
{
  this->set_complete_name ();
}

TAO_POA::TAO_POA (const TAO_POA::String &adapter_name,
                  TAO_POA_Manager &poa_manager,
                  const TAO_POA_Policies &policies,
                  TAO_POA *parent,
                  TAO_Object_Table &active_object_table,
                  CORBA::Environment &env)
  : name_ (adapter_name),
    poa_manager_ (poa_manager),
    policies_ (policies),
    parent_ (parent),
    active_object_table_ (&active_object_table),
    delete_active_object_table_ (0),
    adapter_activator_ (),
    servant_activator_ (),
    servant_locator_ (),
    default_servant_ (0),
    children_ (),
    lock_ (),
    closing_down_ (0),
    counter_ (0)
{
  this->set_complete_name ();
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
                TAO_Object_Table &active_object_table,
                CORBA::Environment &env)
{
  return new TAO_POA (adapter_name,
                      poa_manager,
                      policies,
                      parent,
                      active_object_table,
                      env);
}

TAO_POA::~TAO_POA (void)
{
  if (this->delete_active_object_table_)
    delete active_object_table_;
}

ACE_Lock &
TAO_POA::lock (void)
{
  return this->lock_;
}

PortableServer::POA_ptr
TAO_POA::create_POA (const char *adapter_name,
                     PortableServer::POAManager_ptr poa_manager,
                     const PortableServer::PolicyList& policies,
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
      poa_manager_impl = stub_to_impl<PortableServer::POAManager_ptr, 
                                      POA_PortableServer::_tao_collocated_POAManager, 
                                      TAO_POA_Manager> (poa_manager);
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
      if (result == 0)
        {
          CORBA::Exception *exception = new PortableServer::POA::AdapterAlreadyExists;
          env.exception (exception);
          return 0;
        }

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

      this->poa_manager_.register_poa (new_poa.get (), env);
      if (env.exception () != 0)
        {
          // @@ Remove new POA from parent list
          return 0;
        }

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

      // Try to find the topmost child
      TAO_POA *child_poa = 0;
      int result = this->children_.find (topmost_poa_name, child_poa);

      // Child was not found
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

      // Child was not found
      if (result != 0)
        {
          // If a child POA with the specified name does not exist and
          // the value of the activate_it parameter is TRUE, the
          // target POA's AdapterActivator, if one exists, is invoked,
          // and, if it successfully activates the child POA, that
          // child POA is returned.
          if (activate_it && !CORBA::is_nil (this->the_activator (env)))
            {
              CORBA::Boolean success =
                this->the_activator (env)->unknown_adapter (this->_this (env),
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
                  if (result == 0)
                    return child_poa;
                }
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

      // Try to find the topmost child
      TAO_POA *child_poa;
      int result = this->children_.find (adapter_name, child_poa);      

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
  this->parent_->delete_child (this->name_, env);

  if (env.exception () != 0)
    return;

  // Remove POA from the POAManager
  this->poa_manager_.remove_poa (this, env);

  if (env.exception () != 0)
    return;

  // Remove all children POAs
  for (CHILDREN::iterator iterator = this->children_.begin ();
       iterator != this->children_.end () && env.exception () != 0;
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
          !CORBA::is_nil (this->servant_activator_))
        {
          for (TAO_Object_Table::iterator iterator = this->active_object_table ().begin ();
               iterator != this->active_object_table ().end () && env.exception () != 0;
               iterator++)
            {
              this->servant_activator_->etherealize ((*iterator).ext_id_,
                                                     this->_this (env),
                                                     (*iterator).int_id_,
                                                     CORBA::B_TRUE,
                                                     CORBA::B_FALSE,
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
  // No locks are necessary if we are closing down
  if (this->closing_down_)
    {
      this->delete_child_i (child,
                            env);
    }
  else
    {
      // Lock access to the POA for the duration of this transaction
      TAO_POA_WRITE_GUARD (ACE_Lock, monitor, this->lock (), env);
      
      this->delete_child_i (child,
                            env);
    }
}

void
TAO_POA::delete_child_i (const TAO_POA::String &child,
                         CORBA::Environment &env)
{
  if (this->children_.unbind (child) == 0)
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
TAO_POA::active_object_table (void)
{
  return *this->active_object_table_;
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
    return PortableServer::ServantManager::_duplicate (this->servant_activator_);
  else
    return PortableServer::ServantManager::_duplicate (this->servant_locator_);
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
      if (CORBA::is_nil (this->servant_activator_))
        {
          CORBA::Exception *exception = new PortableServer::POA::WrongPolicy;
          env.exception (exception);
          return;
        }
    }
  else
    {
      this->servant_locator_ = PortableServer::ServantLocator::_narrow (imgr, env);
      if (CORBA::is_nil (this->servant_locator_))
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
      this->active_object_table ().find (servant) != -1)
    {
      CORBA::Exception *exception = new PortableServer::POA::ServantAlreadyActive;
      env.exception (exception);
      return 0;
    }

  // Otherwise, the activate_object operation generates an Object Id
  // and enters the Object Id and the specified servant in the Active
  // Object Map. The Object Id is returned.
  PortableServer::ObjectId_var new_id = this->create_object_id ();

  if (this->active_object_table ().bind (new_id.in (), servant) == -1)
    {
      CORBA::Exception *exception = new CORBA::OBJ_ADAPTER (CORBA::COMPLETED_NO);
      env.exception (exception);
      return 0;
    }

  // Everything is finally ok
  return new_id._retn ();
}

void
TAO_POA::activate_object_with_id (const PortableServer::ObjectId& id,
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
TAO_POA::activate_object_with_id_i (const PortableServer::ObjectId& id,
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
  if (this->active_object_table ().find (id) != -1)
    {
      CORBA::Exception *exception = new PortableServer::POA::ObjectAlreadyActive;
      env.exception (exception);
      return;
    }

  // If the POA has the UNIQUE_ID policy and the servant is already in
  // the Active Object Map, the ServantAlreadyActive exception is
  // raised.
  if (this->policies ().id_uniqueness () == PortableServer::UNIQUE_ID &&
      this->active_object_table ().find (servant) != -1)
    {
      CORBA::Exception *exception = new PortableServer::POA::ServantAlreadyActive;
      env.exception (exception);
      return;
    }

  // Otherwise, the activate_object_with_id operation enters an
  // association between the specified Object Id and the specified
  // servant in the Active Object Map.
  if (this->active_object_table ().bind (id, servant) == -1)
    {
      CORBA::Exception *exception = new CORBA::OBJ_ADAPTER (CORBA::COMPLETED_NO);
      env.exception (exception);
      return;
    }

  // Everything is finally ok
}

void
TAO_POA::deactivate_object (const PortableServer::ObjectId& oid,
                            CORBA::Environment &env)
{
  // Lock access to the POA for the duration of this transaction
  TAO_POA_WRITE_GUARD (ACE_Lock, monitor, this->lock (), env);

  this->deactivate_object_i (oid,
                             env);
}

void
TAO_POA::deactivate_object_i (const PortableServer::ObjectId& oid,
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
  int result = this->active_object_table ().unbind (oid, servant);

  // If there is no active object associated with the specified Object
  // Id, the operation raises an ObjectNotActive exception.
  if (result == -1)
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
  if (!CORBA::is_nil (this->servant_activator_))
    this->servant_activator_->etherealize (oid,
                                           this->_this (env),
                                           servant,
                                           0,
                                           CORBA::B_FALSE,
                                           env);
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
  TAO::ObjectKey_var new_key = this->create_object_key (new_id.in ());

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
  TAO::ObjectKey_var new_key = this->create_object_key (oid);

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
  if (this->policies ().id_uniqueness () == PortableServer::UNIQUE_ID &&
      this->active_object_table ().find (servant, id.out ()) != -1)
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
          this->active_object_table ().find (servant) != 0)
        {
          PortableServer::ObjectId_var new_id = this->create_object_id ();

          if (this->active_object_table ().bind (new_id.in (), servant) == -1)
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
      TAO::ObjectKey_var key = reference->key (env);

      // If the object reference was not created by this POA, the
      // WrongAdapter exception is raised.
      PortableServer::ObjectId_var id;
      TAO_POA::String poa_name;

      int result = this->parse_key (key.in (), poa_name, id.out ());
      if (result != 0 || poa_name != this->complete_name ())
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
  TAO::ObjectKey_var key = reference->key (env);
  PortableServer::ObjectId_var id;
  TAO_POA::String poa_name;

  int result = this->parse_key (key.in (), poa_name, id.out ());
  if (result != 0 || poa_name != this->complete_name ())
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
  if (this->active_object_table ().find (oid, servant) != -1)
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
  PortableServer::Servant servant = this->id_to_servant (oid,
                                                         env);

  if (env.exception () != 0)
    return CORBA::Object::_nil ();
  else
    return this->servant_to_reference (servant,
                                       env);
}

PortableServer::POA_ptr
TAO_POA::the_parent (CORBA::Environment &env)
{
  return this->parent_->_this (env);
}

PortableServer::POAManager_ptr
TAO_POA::the_POAManager (CORBA::Environment &env)
{
  return this->poa_manager_._this (env);
}

PortableServer::AdapterActivator_ptr
TAO_POA::the_activator (CORBA::Environment &env)
{
  return PortableServer::AdapterActivator::_duplicate (this->adapter_activator_);
}

void
TAO_POA::the_activator (PortableServer::AdapterActivator_ptr adapter_activator, 
                        CORBA::Environment &env)
{
  this->adapter_activator_ = PortableServer::AdapterActivator::_duplicate (adapter_activator);
}

void
TAO_POA::dispatch_servant (const TAO::ObjectKey &key,
                           CORBA::ServerRequest &req,
                           void *context,
                           CORBA::Environment &env)
{
  // Lock access to the POAManager for the duration of this transaction
  TAO_POA_READ_GUARD (ACE_Lock, monitor, this->lock (), env);

  PortableServer::Servant servant = this->locate_servant_i (key, env);
  
  if (env.exception () != 0 || servant == 0)
    return;

  servant->dispatch (req, context, env);
}

PortableServer::Servant
TAO_POA::locate_servant (const TAO::ObjectKey &key,
                         CORBA::Environment &env)
{
  // Lock access to the POAManager for the duration of this transaction
  TAO_POA_READ_GUARD_RETURN (ACE_Lock, monitor, this->lock (), ERROR, env);

  return this->locate_servant_i (key, env);
}

PortableServer::Servant
TAO_POA::locate_servant_i (const TAO::ObjectKey &key,
                           CORBA::Environment &env)
{
  PortableServer::ObjectId_var id;
  TAO_POA::String poa_name;

  int result = this->parse_key (key, poa_name, id.out ());
  if (result != 0)
    {
      CORBA::Exception *exception = new CORBA::OBJ_ADAPTER (CORBA::COMPLETED_NO);
      env.exception (exception);
      return 0;
 }

  TAO_POA *poa = this->find_POA_i (poa_name,
                                   CORBA::B_FALSE,
                                   env);

  if (env.exception () != 0)
    return 0;

  PortableServer::Servant servant = poa->id_to_servant_i (id.in (), env);

  if (env.exception () != 0)
    return 0;

  return servant;
}

int
TAO_POA::parse_key (const TAO::ObjectKey &key, 
                    TAO_POA::String &poa_name,
                    PortableServer::ObjectId_out id)
{
  // Grab the id buffer
  TAO_POA::String object_key (TAO_POA::ObjectKey_to_const_string (key));
  
  // Try to find the name separator
  int token_position = object_key.rfind (TAO_POA::name_separator ());

  // If not found, the name is not correct
  if (token_position == TAO_POA::String::npos)
    {
      return -1;
    }
  else
    {
      // If found, take the substring from 0 to token_position for the
      // poa_name and the substring from (token_position +
      // separator_length) to length for the objectId
      poa_name = object_key.substr (0, token_position);

      TAO_POA::String objectId = object_key.substr (token_position + TAO_POA::name_separator_length (), 
                                                    object_key.length ());
      
      id = TAO_POA::string_to_ObjectId (objectId.c_str ());

      // Success
      return 0;
    }
}

PortableServer::ObjectId *
TAO_POA::create_object_id (void)
{
  // Note:  This method assumes two things:
  // 1. Locks are held when this method is called
  // 2. Size of octet == size of string element

  // This is the maximum space require to convert the counter into a
  // string
  const int max_space_required_for_counter = 24;

  // Buffer for counter 
  char counter[max_space_required_for_counter];

  // Convert counter into string
  ACE_OS::sprintf (counter,
                   "%d",
                   this->counter_);

  // Calculate the required buffer size.
  // Note: 1 is for the null terminator
  TAO_POA::String id = 
    this->name_ + 
    TAO_POA::id_separator () +  
    counter;

  // Increment counter
  this->counter_++;

  // Create the sequence
  return TAO_POA::string_to_ObjectId (id.c_str ());
}

TAO::ObjectKey *
TAO_POA::create_object_key (const PortableServer::ObjectId &id)
{
  // Calculate the required buffer size.
  // Note: 1 is for the null terminator
  int buffer_size =
    this->complete_name_.length () +
    TAO_POA::name_separator_length () +
    id.length () +
    1;

  // Create the buffer for the key
  CORBA::Octet *buffer = TAO::ObjectKey::allocbuf (buffer_size);

  // Grab the id buffer
  const char *id_buffer = TAO_POA::ObjectId_to_const_string (id);

  // Make an object key
  ACE_OS::sprintf ((CORBA::String) buffer,
                   "%s%c%s",
                   this->complete_name_.c_str (),
                   TAO_POA::name_separator (),
                   id_buffer);

  // Create the key, giving the ownership of the buffer to the
  // sequence.
  return new TAO::ObjectKey (buffer_size,
                             buffer_size,
                             buffer,
                             CORBA::B_TRUE);
}

PortableServer::POA_ptr
TAO_POA::_this (CORBA_Environment &env)
{
  STUB_Object *stub = new IIOP_Object (CORBA::string_copy (this->_interface_repository_id ()),
                                       TAO_ORB_Core_instance ()->orb_params ()->addr (),
                                       0);
  if (env.exception () != 0)
    return 0;

  return new POA_PortableServer::_tao_collocated_POA (this, stub);
}

int
TAO_POA::is_poa_generated_id (const PortableServer::ObjectId& id)
{
  // Grab the buffer
  const char *id_buffer = TAO_POA::ObjectId_to_const_string (id);

  // Check to see if the POA name is the first part of the id
  if (ACE_OS::strncmp (id_buffer,
                       this->name_.c_str (),
                       this->name_.length ()) == 0)
    return 1;
  else
    return 0;
}

int
TAO_POA::is_poa_generated_key (const TAO::ObjectKey& key)
{
  // Grab the buffer
  const char *id_buffer = TAO_POA::ObjectKey_to_const_string (key);

  // Check to see if the POA name is the first part of the id
  if (ACE_OS::strncmp (id_buffer,
                       this->complete_name_.c_str (),
                       this->complete_name_.length ()) == 0)
    return 1;
  else
    return 0;
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
      topmost_poa_name = adapter_name.substr (0, token_position);
    }

  // Try to find the name separator
  token_position = adapter_name.rfind (TAO_POA::name_separator ());
  
  // If not found, the name was a leaf, throw exception
  if (token_position == TAO_POA::String::npos)
    {
      CORBA::Exception *exception = new CORBA::OBJ_ADAPTER (CORBA::COMPLETED_NO);
      env.exception (exception);
      return;
    }
  else
    {
      // If found, take the substring from (token_position +
      // separator_length) to length
      tail_poa_name = adapter_name.substr (token_position + TAO_POA::name_separator_length (), 
                                           adapter_name.length ());
    }
}

CORBA::String
TAO_POA::the_name (CORBA::Environment &env)
{
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

char *
TAO_POA::ObjectId_to_string (const PortableServer::ObjectId &id)
{
  // This method assumes that the id was initially placed in the octet
  // sequence as a char string

  // Grab the id buffer
  CORBA::String id_buffer = (CORBA::String) &id[0];
  
  // Create space and copy the contents
  return CORBA::string_dup (id_buffer);
}

const char *
TAO_POA::ObjectId_to_const_string (const PortableServer::ObjectId &id)
{
  // This method assumes that the id was initially placed in the octet
  // sequence as a char string

  // Grab the id buffer
  return (CORBA::String) &id[0];
}

const char *
TAO_POA::ObjectKey_to_const_string (const TAO::ObjectKey &key)
{
  // Grab the id buffer
  return (CORBA::String) &key[0];
}

PortableServer::ObjectId *
TAO_POA::string_to_ObjectId (const char *id)
{
  // Size of Id
  CORBA::ULong id_length = ACE_OS::strlen (id) + 1;

  // Create the buffer for the Id
  CORBA::Octet *buffer = PortableServer::ObjectId::allocbuf (id_length);
  
  // Copy contents
  ACE_OS::strcpy ((CORBA::String) buffer, id);

  // Create a new ID
  return new PortableServer::ObjectId (id_length,
                                       id_length,
                                       buffer,
                                       CORBA::B_TRUE);
}

PortableServer::ObjectId *
TAO_POA::wstring_to_ObjectId (const wchar_t *id)
{
  // Size of Id
  CORBA::ULong id_length = ACE_OS::strlen (id) + 1;

  // Create the buffer for the Id
  CORBA::Octet *buffer = PortableServer::ObjectId::allocbuf (id_length * sizeof CORBA::WChar);
  
  // Copy contents
  ACE_OS::strcpy ((CORBA::WString) buffer, id);

  // Create a new ID
  return new PortableServer::ObjectId (id_length,
                                       id_length,
                                       buffer,
                                       CORBA::B_TRUE);
}

wchar_t *
TAO_POA::ObjectId_to_wstring (const PortableServer::ObjectId &id)
{
  // This method assumes that the id was initially placed in the octet
  // sequence as a wchar string

  // Grab the id buffer
  CORBA::WString id_buffer = (CORBA::WString) &id[0];

  // Create space and copy the contents
  return CORBA::wstring_dup (id_buffer);
}

const wchar_t *
TAO_POA::ObjectId_to_const_wstring (const PortableServer::ObjectId &id)
{
  // This method assumes that the id was initially placed in the octet
  // sequence as a wchar string

  // Grab the id buffer
  return (CORBA::WString) &id[0];
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

PortableServer::ThreadPolicy_ptr
TAO_POA::create_thread_policy (PortableServer::ThreadPolicyValue value,
                               CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);
  return new TAO_Thread_Policy (value);
}

PortableServer::LifespanPolicy_ptr
TAO_POA::create_lifespan_policy (PortableServer::LifespanPolicyValue value,
                                 CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);
  return new TAO_Lifespan_Policy (value);
}

PortableServer::IdUniquenessPolicy_ptr
TAO_POA::create_id_uniqueness_policy (PortableServer::IdUniquenessPolicyValue value,
                                      CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);
  return new TAO_Id_Uniqueness_Policy (value);
}

PortableServer::IdAssignmentPolicy_ptr
TAO_POA::create_id_assignment_policy (PortableServer::IdAssignmentPolicyValue value,
                                      CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);
  return new TAO_Id_Assignment_Policy (value);
}

PortableServer::ImplicitActivationPolicy_ptr
TAO_POA::create_implicit_activation_policy (PortableServer::ImplicitActivationPolicyValue value,
                                            CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);
  return new TAO_Implicit_Activation_Policy (value);
}

PortableServer::ServantRetentionPolicy_ptr
TAO_POA::create_servant_retention_policy (PortableServer::ServantRetentionPolicyValue value,
                                          CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);
  return new TAO_Servant_Retention_Policy (value);
}

PortableServer::RequestProcessingPolicy_ptr
TAO_POA::create_request_processing_policy (PortableServer::RequestProcessingPolicyValue value,
                                           CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);
  return new TAO_Request_Processing_Policy (value);
}

CORBA::Boolean
TAO_Adapter_Activator::unknown_adapter (PortableServer::POA_ptr parent,
                                        const char *name,
                                        CORBA::Environment &env)
{  
  TAO_POA *parent_impl = stub_to_impl<PortableServer::POA_ptr, 
                                      POA_PortableServer::_tao_collocated_POA, 
                                      TAO_POA> (parent);

  return this->unknown_adapter_i (parent_impl,
                                  name,
                                  env);
}

CORBA::Boolean 
TAO_Adapter_Activator::unknown_adapter_i (TAO_POA *parent,
                                          const TAO_POA::String &name,
                                          CORBA::Environment &env)
{
  // Use default policies
  TAO_POA_Policies default_policies;
  
  // New poa manager
  TAO_POA_Manager *poa_manager = parent->poa_manager_.clone ();

  // This assumes that the lock on the parent is already held
  parent->create_POA_i (name,
                        *poa_manager,
                        default_policies,
                        env);

  if (env.exception () != 0)
    return CORBA::B_FALSE;
  else
    return CORBA::B_TRUE;
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
       iterator != this->poa_collection_.end () && env.exception () != 0;
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
  if (this->closing_down_)
    {      
      this->remove_poa_i (poa, 
                          env);
    }
  else
    {
      // Lock access to the POAManager for the duration of this transaction
      TAO_POA_WRITE_GUARD (ACE_Lock, monitor, this->lock (), env);

      this->remove_poa_i (poa, 
                          env);
    }
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
                                    TAO_Object_Table &active_object_table,
                                    CORBA::Environment &env)
  : TAO_POA (adapter_name,
             poa_manager,
             policies,
             parent,
             active_object_table,
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
                         TAO_Object_Table &active_object_table,
                         CORBA::Environment &env)
{
  return new SELF (adapter_name,
                   poa_manager,
                   policies,
                   parent,
                   active_object_table,
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
    in_upcall_ (0),
    object_key_ (0)
{
}

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
  this->in_upcall_ = 0;
  this->object_key_ = 0;
}

int
TAO_POA_Current::context_is_valid (void)
{
  return 
    this->poa_impl_ != 0 && 
    this->object_id_ != 0 && 
    this->in_upcall_ != 0 && 
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
TAO_POA_Current::object_key (const TAO::ObjectKey &key)
{
  this->object_key_ = &key;
}

const TAO::ObjectKey &
TAO_POA_Current::object_key (void) const
{
  return *this->object_key_;
}

void
TAO_POA_Current::in_upcall (int yesno)
{
  this->in_upcall_ = yesno;
}

int
TAO_POA_Current::in_upcall (void) const
{
  return this->in_upcall_;
}
