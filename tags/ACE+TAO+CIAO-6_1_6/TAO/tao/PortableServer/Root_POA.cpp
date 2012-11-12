// $Id$

#include "tao/PortableServer/Root_POA.h"
#include "tao/PortableServer/Regular_POA.h"

#include "tao/PortableServer/ThreadPolicy.h"
#include "tao/PortableServer/LifespanPolicy.h"
#include "tao/PortableServer/IdAssignmentPolicy.h"
#include "tao/PortableServer/IdUniquenessPolicy.h"
#include "tao/PortableServer/ImplicitActivationPolicy.h"
#include "tao/PortableServer/RequestProcessingPolicy.h"
#include "tao/PortableServer/ServantRetentionPolicy.h"
#include "tao/PortableServer/Active_Object_Map.h"
#include "tao/PortableServer/Default_Acceptor_Filter.h"
#include "tao/PortableServer/ORT_Adapter.h"
#include "tao/PortableServer/ORT_Adapter_Factory.h"
#include "tao/PortableServer/POA_Current_Impl.h"
#include "tao/PortableServer/Servant_Upcall.h"
#include "tao/PortableServer/AdapterActivatorC.h"
#include "tao/PortableServer/Non_Servant_Upcall.h"
#include "tao/PortableServer/POAManager.h"
#include "tao/PortableServer/POAManagerFactory.h"
#include "tao/PortableServer/ServantManagerC.h"
#include "tao/PortableServer/poa_macros.h"
#include "tao/PortableServer/POA_Guard.h"
#include "tao/PortableServer/Creation_Time.h"
#include "tao/PortableServer/RequestProcessingStrategy.h"
#include "tao/PortableServer/LifespanStrategy.h"
#include "tao/PortableServer/IdUniquenessStrategy.h"
#include "tao/PortableServer/IdAssignmentStrategy.h"
#include "tao/PortableServer/ServantRetentionStrategy.h"
#include "tao/PortableServer/ImplicitActivationStrategy.h"
#include "tao/PortableServer/ThreadStrategy.h"
#include "tao/PortableServer/Acceptor_Filter_Factory.h"
#include "tao/PortableServer/Network_Priority_Hook.h"

#include "tao/StringSeqC.h"
#include "tao/PortableInterceptorC.h"
#include "tao/PolicyC.h"
#include "tao/ORB_Core.h"
#include "tao/ORB.h"
#include "tao/Server_Strategy_Factory.h"
#include "tao/Acceptor_Registry.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Exception.h"
#include "tao/Stub.h"
#include "tao/Profile.h"
#include "tao/TSS_Resources.h"
#include "tao/IORInterceptor_Adapter.h"
#include "tao/debug.h"

// auto_ptr class
#include "ace/Auto_Ptr.h"
#include "ace/Dynamic_Service.h"
#include "ace/OS_NS_netdb.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Log_Msg.h"

#if !defined (__ACE_INLINE__)
# include "tao/PortableServer/Root_POA.inl"
#endif /* ! __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// This is the TAO_Object_key-prefix that is appended to all TAO Object keys.
// It's an array of octets representing ^t^a^o/0 in octal.
CORBA::Octet const
TAO_Root_POA::objectkey_prefix [TAO_Root_POA::TAO_OBJECTKEY_PREFIX_SIZE] = {
  024, // octal for ^t
  001, // octal for ^a
  017, // octal for ^o
  000
};

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

PortableServer::ThreadPolicy_ptr
TAO_Root_POA::create_thread_policy (PortableServer::ThreadPolicyValue value)
{
  TAO::Portable_Server::ThreadPolicy *policy = 0;
  ACE_NEW_THROW_EX (policy,
                    TAO::Portable_Server::ThreadPolicy (value),
                    CORBA::NO_MEMORY ());

  return policy;
}

#endif /* TAO_HAS_MINIMUM_POA == 0 && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO) */

#if !defined (CORBA_E_MICRO)

PortableServer::LifespanPolicy_ptr
TAO_Root_POA::create_lifespan_policy (PortableServer::LifespanPolicyValue value)
{
  TAO::Portable_Server::LifespanPolicy *policy = 0;
  ACE_NEW_THROW_EX (policy,
                    TAO::Portable_Server::LifespanPolicy (value),
                    CORBA::NO_MEMORY ());

  return policy;
}

#endif

#if !defined (CORBA_E_MICRO)
PortableServer::IdUniquenessPolicy_ptr
TAO_Root_POA::create_id_uniqueness_policy (PortableServer::IdUniquenessPolicyValue value)
{
  TAO::Portable_Server::IdUniquenessPolicy *policy = 0;
  ACE_NEW_THROW_EX (policy,
                    TAO::Portable_Server::IdUniquenessPolicy (value),
                    CORBA::NO_MEMORY ());

  return policy;
}
#endif

#if !defined (CORBA_E_MICRO)
PortableServer::IdAssignmentPolicy_ptr
TAO_Root_POA::create_id_assignment_policy (PortableServer::IdAssignmentPolicyValue value)
{
  TAO::Portable_Server::IdAssignmentPolicy *policy = 0;
  ACE_NEW_THROW_EX (policy,
                    TAO::Portable_Server::IdAssignmentPolicy (value),
                    CORBA::NO_MEMORY ());

  return policy;
}
#endif

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

PortableServer::ImplicitActivationPolicy_ptr
TAO_Root_POA::create_implicit_activation_policy (PortableServer::ImplicitActivationPolicyValue value)
{
  TAO::Portable_Server::ImplicitActivationPolicy *policy = 0;
  ACE_NEW_THROW_EX (policy,
                    TAO::Portable_Server::ImplicitActivationPolicy (value),
                    CORBA::NO_MEMORY ());

  return policy;
}

PortableServer::ServantRetentionPolicy_ptr
TAO_Root_POA::create_servant_retention_policy (PortableServer::ServantRetentionPolicyValue value)
{
  TAO::Portable_Server::ServantRetentionPolicy *policy = 0;
  ACE_NEW_THROW_EX (policy,
                    TAO::Portable_Server::ServantRetentionPolicy (value),
                    CORBA::NO_MEMORY ());

  return policy;
}

PortableServer::RequestProcessingPolicy_ptr
TAO_Root_POA::create_request_processing_policy (PortableServer::RequestProcessingPolicyValue value)
{
  TAO::Portable_Server::RequestProcessingPolicy *policy = 0;
  ACE_NEW_THROW_EX (policy,
                    TAO::Portable_Server::RequestProcessingPolicy (value),
                    CORBA::NO_MEMORY ());

  return policy;
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

void
TAO_Root_POA::set_obj_ref_factory (
  PortableInterceptor::ObjectReferenceFactory *current_factory)
{
  TAO::ORT_Adapter *adapter = this->ORT_adapter ();

  if (adapter)
    {
      // Activate a different factory
      this->ort_adapter_->set_obj_ref_factory (current_factory);
    }
}

TAO_Root_POA::TAO_Root_POA (const TAO_Root_POA::String &name,
                            PortableServer::POAManager_ptr poa_manager,
                            const TAO_POA_Policy_Set &policies,
                            TAO_Root_POA *parent,
                            ACE_Lock &lock,
                            TAO_SYNCH_MUTEX &thread_lock,
                            TAO_ORB_Core &orb_core,
                            TAO_Object_Adapter *object_adapter)
  : name_ (name),
    poa_manager_ (* (dynamic_cast <TAO_POA_Manager*> (poa_manager))),

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
    poa_manager_factory_ (* (object_adapter->poa_manager_factory_)),
#endif

    tagged_component_ (),
    tagged_component_id_ (),
    profile_id_array_ (0),
    policies_ (policies),
    ort_adapter_ (0),
    ort_adapter_factory_ (0),
    adapter_state_ (PortableInterceptor::HOLDING),
    network_priority_hook_ (0),
#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
    adapter_activator_ (),
#endif /* TAO_HAS_MINIMUM_POA == 0 */
    children_ (),
    lock_ (lock),
    orb_core_ (orb_core),
    object_adapter_ (object_adapter),
    cleanup_in_progress_ (false),
    outstanding_requests_ (0),
    outstanding_requests_condition_ (thread_lock),
    wait_for_completion_pending_ (0),
    waiting_destruction_ (false),
    servant_deactivation_condition_ (thread_lock),
#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
    filter_factory_ (0),
#endif
    caller_key_to_object_ (0),
    servant_for_key_to_object_ (0)
{
  // Since we are keeping a reference to a POAManager, we need to
  // increment the reference count but we do this safely.
  PortableServer::POAManager_var pm_guard (
    PortableServer::POAManager::_duplicate(&this->poa_manager_));

  // Parse the policies that are used in the critical path in
  // a cache.
  this->cached_policies_.update (this->policies_);

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
  this->filter_factory_
    = ACE_Dynamic_Service<TAO_Acceptor_Filter_Factory>::instance (
        "TAO_Acceptor_Filter_Factory");
#endif

  this->network_priority_hook_
    = ACE_Dynamic_Service<TAO_Network_Priority_Hook>::instance (
        "TAO_Network_Priority_Hook");

  if (this->network_priority_hook_ != 0)
    {
      this->network_priority_hook_->update_network_priority (
        *this, this->policies_);
    }

  // Cache ort adapter factory
  this->ort_adapter_factory_
   = ACE_Dynamic_Service<TAO::ORT_Adapter_Factory>::instance
      (orb_core_.configuration (), TAO_Root_POA::ort_adapter_factory_name ());

#if (TAO_HAS_MINIMUM_POA == 1)
  // If this is the RootPOA, set the value of the ImplicitActivationPolicy
  // to IMPLICIT_ACTIVATION since it is impossible to pass the policy
  // as it is not compiled into the library.
  //
  // If the ImplicitActivationPolicy policy is ever compiled in the
  // minimum POA builds, remove this code and remove the guards
  // in Object_Adapter.cpp when changing the default policy for the
  // RootPOA.
  if (ACE_OS::strcmp (this->name_.c_str (),
                      TAO_DEFAULT_ROOTPOA_NAME) == 0)
    {
      this->cached_policies_.implicit_activation
        (PortableServer::IMPLICIT_ACTIVATION);
    }
#endif /* TAO_HAS_MINIMUM_POA == 1 */

  // Set the active strategies to be used by this POA
  this->active_policy_strategies_.update (this->cached_policies_,
                                          this);
  TAO::Portable_Server::Active_Policy_Strategies_Cleanup_Guard aps_cleanup_guard (
    &this->active_policy_strategies_);

  // Set the folded name of this POA.
  this->set_folded_name (parent);

  // Register self with manager.
  int result = this->poa_manager_.register_poa (this);
  if (result != 0)
    {
      throw ::CORBA::OBJ_ADAPTER ();
    }

  // Add self to Object Adapter class.
  result =
    this->object_adapter ().bind_poa (this->folded_name_,
                                      this,
                                      this->system_name_.out ());
  if (result != 0)
    {
      // Remove from POA Manager in case of errors. No checks of
      // further errors...
      this->poa_manager_.remove_poa (this);

      throw ::CORBA::OBJ_ADAPTER ();
    }

  // Set the id for this POA.
  this->set_id (parent);

  // Notify the Lifespan strategy of our startup
  try
    {
      this->active_policy_strategies_.lifespan_strategy()->notify_startup ();
    }
  catch (const ::CORBA::Exception&)
    {
      this->poa_manager_.remove_poa (this);
      this->object_adapter ().unbind_poa (this,
                                          this->folded_name_,
                                          this->system_name_.in ());
      throw;
    }

  // Now when everything is fine we can release the quards.
  pm_guard._retn ();
  aps_cleanup_guard._retn ();
}

TAO_Root_POA::~TAO_Root_POA (void)
{
  this->poa_manager_._remove_ref();
}

void
TAO_Root_POA::complete_destruction_i (void)
{
  bool doing_complete_destruction =
    this->waiting_destruction_ != false;

  // No longer awaiting destruction.
  this->waiting_destruction_ = false;

  PortableServer::POA_var poa;
  TAO::ORT_Array my_array_obj_ref_template;
  TAO::ORT_Adapter *ort_adapter = 0;
  if (doing_complete_destruction)
    {
      ort_adapter =
        this->ORT_adapter_i ();

      // In case no ORT library is linked we get zero.
      if (ort_adapter != 0)
        {
          // Get the ObjectReferenceTemplate.
          PortableInterceptor::ObjectReferenceTemplate * const ort =
            ort_adapter->get_adapter_template ();

          // Add it to the sequence of object reference templates, we
          // just notify for ourselves that we are now non_existent,
          // our childs will do it for themselves.
          my_array_obj_ref_template.size (1);
          my_array_obj_ref_template[0] = ort;
        }

      poa = PortableServer::POA::_duplicate (this);
    }

  // Remove POA from the POAManager.
  if (this->poa_manager_.remove_poa (this) != 0)
    throw ::CORBA::OBJ_ADAPTER ();

  // Remove POA from the Object Adapter.
  int result = this->object_adapter ().unbind_poa (this,
                                                   this->folded_name_,
                                                   this->system_name_.in ());
  if (result != 0)
    throw ::CORBA::OBJ_ADAPTER ();

  // Cleanup all strategies
  this->active_policy_strategies_.cleanup ();

  // Forced cleanup.  The new memory management scheme is evil and can
  // lead to reference deadlock, i.e., POA holds object A, but POA
  // cannot die because object A hold POA.
  {
    //
    // If new things are added to this cleanup code, make sure to move
    // the minimum CORBA #define after the declaration of
    // <non_servant_upcall>.
    //

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

    // ATTENTION: Trick locking here, see class header for details
    TAO::Portable_Server::Non_Servant_Upcall non_servant_upcall (*this);
    ACE_UNUSED_ARG (non_servant_upcall);

    this->adapter_activator_ = PortableServer::AdapterActivator::_nil ();

#endif /* TAO_HAS_MINIMUM_POA == 0 */

  }

  ::CORBA::release (this);

  if (doing_complete_destruction)
    {
      this->adapter_state_ = PortableInterceptor::NON_EXISTENT;

      this->adapter_state_changed (my_array_obj_ref_template,
                                   this->adapter_state_);

      if (ort_adapter != 0)
        {
          ort_adapter->release (my_array_obj_ref_template[0]);

          if (this->ort_adapter_factory_)
            {
              this->ort_adapter_factory_->destroy (ort_adapter);
            }

          this->ort_adapter_ = 0;
        }
    }
}

#if ! defined (CORBA_E_MICRO)
PortableServer::POA_ptr
TAO_Root_POA::create_POA_i (const char *adapter_name,
                            PortableServer::POAManager_ptr poa_manager,
                            const CORBA::PolicyList &policies)
{
  // Initialize a TAO_POA_Policy_Set instance so that it contains the
  // default POA policies.
  TAO_POA_Policy_Set tao_policies (this->object_adapter ().default_poa_policies ());

  // Merge policies from the ORB level.
  this->object_adapter ().validator ().merge_policies (tao_policies.policies ());

  // Merge in any policies that the user may have specified.
  tao_policies.merge_policies (policies);

  // If any of the policy objects specified are not valid for the ORB
  // implementation, if conflicting policy objects are specified, or
  // if any of the specified policy objects require prior
  // administrative action that has not been performed, an
  // InvalidPolicy exception is raised containing the index in the
  // policies parameter value of the first offending policy object.
  tao_policies.validate_policies (this->object_adapter ().validator (),
                                  this->orb_core_);

  // If the poa_manager parameter is null, a new POAManager object is
  // created and associated with the new POA. Otherwise, the specified
  // POAManager object is associated with the new POA. The POAManager
  // object can be obtained using the attribute name the_POAManager.

  PortableServer::POAManager_var the_poa_manager;

  if (CORBA::is_nil (poa_manager))
    {
#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT)

      PortableServer::POA_var poa = PortableServer::POA::_duplicate (this);
      PortableServer::POA_var root_poa;

      // Find the RootPOA by traversing the POA hierarchy until the
      // RootPOA is reached.  The RootPOA has no parent.
      while (!CORBA::is_nil (poa.in ()))
      {
        root_poa = poa;
        poa = poa->the_parent ();
      }

      // Get the POAManagerFactory instance owned by RootPOA.
      PortableServer::POAManagerFactory_var tao_poa_manager_factory
        = root_poa->the_POAManagerFactory ();

      CORBA::PolicyList empty_policies;

      // The POAManager name will be generated when the POAManager instance
      // is created.
      the_poa_manager
        = tao_poa_manager_factory->create_POAManager (0, empty_policies);
#else

      PortableServer::POAManager_ptr the_poa_manager_ptr;
      ACE_NEW_THROW_EX (the_poa_manager_ptr,
                        TAO_POA_Manager (this->object_adapter (), 0),
                        CORBA::NO_MEMORY ());
      the_poa_manager = the_poa_manager_ptr;
#endif /* TAO_HAS_MINIMUM_POA == 0 && ! CORBA_E_COMPACT) */

    }
  else
    {
      the_poa_manager = PortableServer::POAManager::_duplicate (poa_manager);
    }

  PortableServer::POA_var poa = this->create_POA_i (adapter_name,
                                                    the_poa_manager.in (),
                                                    tao_policies);

  return poa._retn ();
}
#endif /* !CORBA_E_MICRO */

#if ! defined (CORBA_E_MICRO)
TAO_Root_POA *
TAO_Root_POA::new_POA (const String &name,
                       PortableServer::POAManager_ptr poa_manager,
                       const TAO_POA_Policy_Set &policies,
                       TAO_Root_POA *parent,
                       ACE_Lock &lock,
                       TAO_SYNCH_MUTEX &thread_lock,
                       TAO_ORB_Core &orb_core,
                       TAO_Object_Adapter *object_adapter)
{
  TAO_Regular_POA *poa = 0;

  ACE_NEW_THROW_EX (poa,
                    TAO_Regular_POA (name,
                             poa_manager,
                             policies,
                             parent,
                             lock,
                             thread_lock,
                             orb_core,
                             object_adapter),
                    CORBA::NO_MEMORY ());

  return poa;
}

PortableServer::POA_ptr
TAO_Root_POA::create_POA_i (const TAO_Root_POA::String &adapter_name,
                            PortableServer::POAManager_ptr poa_manager,
                            const TAO_POA_Policy_Set &policies)
{
  // This operaton creates a new POA as a child of the target POA. The
  // specified name identifies the new POA with respect to other POAs
  // with the same parent POA. If the target POA already has a child
  // POA with the specified name, the AdapterAlreadyExists exception
  // is raised.
  // Child was found
  if (this->children_.find (adapter_name) != -1)
    {
      throw PortableServer::POA::AdapterAlreadyExists ();
    }

  //
  // Child was not found.  Create one.
  //

  // The specified policy objects are associated with the POA and used
  // to control its behavior. The policy objects are effectively
  // copied before this operation returns, so the application is free
  // to destroy them while the POA is in use. Policies are not
  // inherited from the parent POA.
  TAO_Root_POA * poa = this->new_POA (adapter_name,
                                      poa_manager,
                                      policies,
                                      this,
                                      this->object_adapter ().lock (),
                                      this->object_adapter ().thread_lock (),
                                      this->orb_core_,
                                      this->object_adapter_);

  // Give ownership of the new map to the POA_var.  Note, that it
  // is important for the POA_var to take ownership before
  // checking for exception since we may need to delete the new map.
  PortableServer::POA_var new_poa = poa;

  // Check for exception in construction of the POA.

  // Add to children map
  if (this->children_.bind (adapter_name, poa) != 0)
    {
      throw ::CORBA::OBJ_ADAPTER ();
    }

  // Increment the reference count on the child POA since the children
  // map must retain ownership.  Do so immediately before any other
  // operations to prevent memory cleanup problems induced from
  // errors below.
  poa->_add_ref ();

  // Iterate over the registered IOR interceptors so that they may be
  // given the opportunity to add tagged components to the profiles
  // for this servant.
  poa->establish_components ();

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

  // Everything is fine. Don't let the POA_var release the
  // implementation.
  return new_poa._retn ();
}
#endif

#if ! defined (CORBA_E_MICRO)
PortableServer::POA_ptr
TAO_Root_POA::find_POA (const char *adapter_name,
                        CORBA::Boolean activate_it)
{
  // Lock access for the duration of this transaction.
  TAO_POA_GUARD_RETURN (0);

  TAO_Root_POA *poa = this->find_POA_i (adapter_name, activate_it);

  return PortableServer::POA::_duplicate (poa);
}
#endif

#if ! defined (CORBA_E_MICRO)
TAO_Root_POA *
TAO_Root_POA::find_POA_i (const ACE_CString &child_name,
                          CORBA::Boolean activate_it)
{
  TAO_Root_POA *child = 0;
  int result = this->children_.find (child_name, child);

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT)

  if (result != 0)
    {
      if (activate_it)
        {
          if (!CORBA::is_nil (this->adapter_activator_.in ()))
            {
              // Check our state
              this->check_state ();

              CORBA::Boolean success = false;

              {
                // ATTENTION: Trick locking here, see class header for details
                TAO::Portable_Server::Non_Servant_Upcall non_servant_upcall (
                  *this);
                ACE_UNUSED_ARG (non_servant_upcall);

                // When unknown_adapter gives a system exception, the POA
                // should just pass the system exception through
                // See 15.3.9.2 of the 3.1 CORBA spec
                success =
                  this->adapter_activator_->unknown_adapter (
                    this,
                    child_name.c_str ());
              }

              if (success)
                {
                  result = this->children_.find (child_name,
                                                 child);
                }
              else
                {
                  result = -1;
                }
            }
          else
            {
              result = -1;
            }
        }
      else
        {
          result = -1;
        }
    }
#else
  ACE_UNUSED_ARG (activate_it);
#endif /* TAO_HAS_MINIMUM_POA == 0 */

  if (result == 0)
    {
      return child;
    }
  else
    {
      // Otherwise, the AdapterNonExistent exception is raised.
      throw PortableServer::POA::AdapterNonExistent ();
    }
}
#endif

TAO_POA_Manager &
TAO_Root_POA::tao_poa_manager ()
{
  return poa_manager_;
}

#if ! defined (CORBA_E_MICRO)
PortableServer::POA_ptr
TAO_Root_POA::create_POA (const char *adapter_name,
                          PortableServer::POAManager_ptr poa_manager,
                          const CORBA::PolicyList &policies)
{
  // Lock access for the duration of this transaction.
  TAO_POA_GUARD_RETURN (0);

  return this->create_POA_i (adapter_name, poa_manager, policies);
}
#endif

PortableServer::ObjectId *
TAO_Root_POA::servant_to_id (PortableServer::Servant servant)
{
  // If we had upgradeable locks, this would initially be a read lock
  //
  // Lock access for the duration of this transaction.
  TAO_POA_GUARD_RETURN (0);

  return this->servant_to_id_i (servant);
}

PortableServer::ObjectId *
TAO_Root_POA::servant_to_user_id (PortableServer::Servant servant)
{
  return this->active_policy_strategies_.servant_retention_strategy()->
    servant_to_user_id (servant);
}

PortableServer::Servant
TAO_Root_POA::reference_to_servant (CORBA::Object_ptr reference)
{
  // Lock access for the duration of this transaction.
  TAO_POA_GUARD_RETURN (0);

  return this->reference_to_servant_i (reference);
}

CORBA::Object_ptr
TAO_Root_POA::servant_to_reference (PortableServer::Servant servant)
{
  TAO_POA_GUARD_RETURN (CORBA::Object::_nil ());

  return this->servant_to_reference_i (servant);
}

PortableServer::POAList *
TAO_Root_POA::the_children (void)
{
  // Lock access for the duration of this transaction.
  TAO_POA_GUARD_RETURN (0);

  return this->the_children_i ();
}


PortableServer::Servant
TAO_Root_POA::id_to_servant (const PortableServer::ObjectId &oid)
{
  // Lock access for the duration of this transaction.
  TAO_POA_GUARD_RETURN (0);

  return this->id_to_servant_i (oid);
}

CORBA::Object_ptr
TAO_Root_POA::id_to_reference (const PortableServer::ObjectId &oid)
{
  // Lock access for the duration of this transaction.
  TAO_POA_GUARD_RETURN (0);

  return this->id_to_reference_i (oid, true);
}


#if ! defined (CORBA_E_MICRO)
CORBA::Object_ptr
TAO_Root_POA::create_reference_with_id (const PortableServer::ObjectId &id,
                                        const char *intf)
{
  // Lock access for the duration of this transaction.
  TAO_POA_GUARD_RETURN (CORBA::Object::_nil ());

  return this->create_reference_with_id_i (id,
                                           intf,
                                           this->server_priority ());
}
#endif

void
TAO_Root_POA::destroy (CORBA::Boolean etherealize_objects,
                       CORBA::Boolean wait_for_completion)
{
  // Lock access for the duration of this transaction.
  TAO::Portable_Server::POA_Guard poa_guard (*this , 0);
  ACE_UNUSED_ARG (poa_guard);

  this->destroy_i (etherealize_objects, wait_for_completion);
}

void
TAO_Root_POA::remove_from_parent_i (void)
{
  // The root poa has no parent, so this is a noop
}

void
TAO_Root_POA::destroy_i (CORBA::Boolean etherealize_objects,
                         CORBA::Boolean wait_for_completion)
{
  if (this->cleanup_in_progress_)
    return;

  // Is the <wait_for_completion> semantics for this thread correct?
  TAO_Root_POA::check_for_valid_wait_for_completions (this->orb_core (),
                                                      wait_for_completion);

  this->cleanup_in_progress_ = true;

  // Inform the custom servant dispatching strategy to stop the working
  // threads when the poa is destroyed.
  this->poa_deactivated_hook ();

  // This operation destroys the POA and all descendant POAs. The POA
  // so destroyed (that is, the POA with its name) may be re-created
  // later in the same process. (This differs from the
  // POAManager::deactivate operation that does not allow a
  // re-creation of its associated POA in the same process.)

  // Remove POA from the parent
  this->remove_from_parent_i ();

  TAO::ORT_Array array_obj_ref_template (1);

  CORBA::ULong i = 0;

  // Gather all ObjectReferenceTemplates and change all adapter states
  // to INACTIVE.
  for (CHILDREN::iterator iterator = this->children_.begin ();
       iterator != this->children_.end ();
       ++iterator)
    {
      TAO_Root_POA * const child_poa = (*iterator).int_id_;

      TAO::ORT_Adapter * const adapter = child_poa->ORT_adapter_i ();

      // In case no ORT library is linked we get zero.
      if (adapter != 0)
        {
          // Get the ObjectReferenceTemplate for the child POA.
          PortableInterceptor::ObjectReferenceTemplate * const ort =
            adapter->get_adapter_template ();

          // Add it to the sequence of object reference templates that
          // will be destroyed.
          array_obj_ref_template.size (1);

          array_obj_ref_template[0] = ort;
        }

      child_poa->adapter_state_ =
        PortableInterceptor::INACTIVE;

      // Notify the state changes to the IORInterceptors
      this->adapter_state_changed (array_obj_ref_template,
                                   PortableInterceptor::INACTIVE);

      if (adapter != 0)
        adapter->release (array_obj_ref_template[0]);

      ++i;
    }

  // Destroy all child POA's now.
  for (CHILDREN::iterator destroy_iterator = this->children_.begin ();
       destroy_iterator != this->children_.end ();
       ++destroy_iterator)
    {
      TAO_Root_POA *destroy_child_poa = (*destroy_iterator).int_id_;

      destroy_child_poa->destroy_i (etherealize_objects,
                                    wait_for_completion);
    }

  // Notify the lifespan strategy of our shutdown
  this->active_policy_strategies_.lifespan_strategy()->notify_shutdown ();

// @todo, is the exception handling above correct, should we just fail when
// the notify above fails

  // When a POA is destroyed, any requests that have started execution
  // continue to completion. Any requests that have not started
  // execution are processed as if they were newly arrived, that is,
  // the POA will attempt to cause recreation of the POA by invoking
  // one or more adapter activators as described in Section 3.3.3.
  // If the wait_for_completion parameter is TRUE, the destroy
  // operation will return only after all requests in process have
  // completed and all invocations of etherealize have
  // completed. Otherwise, the destroy operation returns after
  // destroying the POAs.

  this->deactivate_all_objects_i (etherealize_objects,
                                  wait_for_completion);

  // If there are no outstanding requests and that we are not in a
  // non-servant upcall or if we are in a non-servant upcall, make
  // sure we are the POA related to the non-servant upcall.
  TAO::Portable_Server::Non_Servant_Upcall *non_servant_upcall_in_progress =
    this->object_adapter ().non_servant_upcall_in_progress ();
  if (this->outstanding_requests_ == 0 &&
      (non_servant_upcall_in_progress == 0 ||
       &non_servant_upcall_in_progress->poa () != this))
    {
      TAO::ORT_Array my_array_obj_ref_template;

      TAO::ORT_Adapter * const ort_adapter =
        this->ORT_adapter_i ();

      // In case no ORT library is linked we get zero.
      if (ort_adapter != 0)
        {
          // Get the ObjectReferenceTemplate.
          PortableInterceptor::ObjectReferenceTemplate * const ort =
            ort_adapter->get_adapter_template ();

          // Add it to the sequence of object reference templates, we
          // just notify for ourselves that we are now non_existent,
          // our childs will do it for themselves.
          my_array_obj_ref_template.size (1);
          my_array_obj_ref_template[0] = ort;
        }

      // According to the ORT spec, after a POA is destroyed, its state
      // has to be changed to NON_EXISTENT and all the registered
      // interceptors are to be informed. Since, the POA is destroyed
      // and is released in the complete_destruction_i method, we are
      // trying to keep the poa still around by doing a duplicate of
      // it. (a hack).
      PortableServer::POA_var poa = PortableServer::POA::_duplicate (this);

      this->complete_destruction_i ();

      this->adapter_state_ = PortableInterceptor::NON_EXISTENT;

      this->adapter_state_changed (my_array_obj_ref_template,
                                   this->adapter_state_);

      if (ort_adapter != 0)
        {
          ort_adapter->release (my_array_obj_ref_template[0]);

          if (this->ort_adapter_factory_)
            {
              this->ort_adapter_factory_->destroy (ort_adapter);
            }

          this->ort_adapter_ = 0;
        }
    }
  else
    {
      // Mark that we are ready for destruction.
      this->waiting_destruction_ = true;
    }
}

int
TAO_Root_POA::delete_child (const TAO_Root_POA::String &child)
{
  int result = 0;

  // If we are not closing down, we must remove this child from our
  // collection.
  if (!this->cleanup_in_progress_)
    result = this->children_.unbind (child);

  // Otherwise, if we are closing down, we are currently iterating
  // over our children and there is not need to remove this child from
  // our collection.

  return result;
}

PortableServer::POAList *
TAO_Root_POA::the_children_i (void)
{
  PortableServer::POAList_var children;
  CORBA::ULong child_current = static_cast <CORBA::ULong>
                                           (this->children_.current_size ());
  ACE_NEW_THROW_EX (children,
                    PortableServer::POAList (child_current),
                    CORBA::NO_MEMORY ());

  children->length (child_current);

  CORBA::ULong index = 0;
  for (CHILDREN::iterator iterator = this->children_.begin ();
       iterator != this->children_.end ();
       ++iterator, ++index)
    {
      TAO_Root_POA *child_poa = (*iterator).int_id_;
      children[index] = PortableServer::POA::_duplicate (child_poa);
    }

  return children._retn ();
}

PortableInterceptor::AdapterName *
TAO_Root_POA::adapter_name_i (void)
{
  // The adapter name is the sequence of names starting from the
  // RootPOA to the one whose name is requested.  The name of the
  // RootPOA is "RootPOA".

  PortableServer::POA_var poa = PortableServer::POA::_duplicate (this);

  CORBA::ULong len = 0;

  // Find the length of the adapter name sequence by traversing the
  // POA hierarchy until the RootPOA is reached.  The RootPOA has no
  // parent.
  while (!CORBA::is_nil (poa.in ()))
    {
      poa = poa->the_parent ();
      ++len;
    }

  // Empty adapter name sequence.
  PortableInterceptor::AdapterName *names = 0;
  ACE_NEW_THROW_EX (names,
                    PortableInterceptor::AdapterName (len),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  PortableInterceptor::AdapterName_var safe_names (names);

  names->length (len);

  poa = PortableServer::POA::_duplicate (this);

  (*names)[0] = CORBA::string_dup ("RootPOA");

  // Fill in the AdapterName sequence as the POA hierarchy is
  // traversed.
  CORBA::ULong ilen = len;
  for (CORBA::ULong i = 1; i < len; ++i)
    {
      (*names)[--ilen] = poa->the_name ();

      poa = poa->the_parent ();

      // If this condition asserts, the POA hierarchy was modified
      // (i.e. reduced in size) by another thread!
      ACE_ASSERT ((ilen > 0 ? !CORBA::is_nil (poa.in ()) : 1));
    }

  return safe_names._retn ();
}

void
TAO_Root_POA::add_ior_component (TAO_MProfile & mprofile,
                                 const IOP::TaggedComponent &component)
{
  // Add the given tagged component to all profiles.
  const CORBA::ULong profile_count = mprofile.profile_count ();

  for (CORBA::ULong i = 0; i < profile_count; ++i)
    {
      TAO_Profile *profile = mprofile.get_profile (i);

      profile->add_tagged_component (component);
    }
}

void
TAO_Root_POA::add_ior_component_to_profile (
    TAO_MProfile & mprofile,
    const IOP::TaggedComponent &component,
    IOP::ProfileId profile_id)
{
  // Add the given tagged component to all profiles matching the given
  // ProfileId.
  bool found_profile = false;

  CORBA::ULong const profile_count = mprofile.profile_count ();

  for (CORBA::ULong i = 0; i < profile_count; ++i)
    {
      TAO_Profile *profile = mprofile.get_profile (i);

      if (profile->tag () == profile_id)
        {
          profile->add_tagged_component (component);

          found_profile = true;
        }
    }

  // According to the Portable Interceptor specification, we're
  // supposed to throw a CORBA::BAD_PARAM exception if no profile
  // matched the given ProfileId.
  if (found_profile == false)
    throw ::CORBA::BAD_PARAM (CORBA::OMGVMCID | 29, CORBA::COMPLETED_NO);
}

void
TAO_Root_POA::adapter_state_changed (
   const TAO::ORT_Array &array_obj_ref_template,
   PortableInterceptor::AdapterState state)
{
  TAO_IORInterceptor_Adapter *ior_adapter =
    this->orb_core_.ior_interceptor_adapter ();

  if (ior_adapter)
    {
      ior_adapter->adapter_state_changed (array_obj_ref_template, state);
    }
}

PortableServer::ObjectId *
TAO_Root_POA::activate_object_i (PortableServer::Servant servant,
                                 CORBA::Short priority,
                                 bool &wait_occurred_restart_call)
{
  return this->active_policy_strategies_.servant_retention_strategy()->
    activate_object (servant,
                     priority,
                     wait_occurred_restart_call);
}

PortableServer::ObjectId *
TAO_Root_POA::activate_object (PortableServer::Servant servant)
{
  while (1)
    {
      bool wait_occurred_restart_call = false;

      // Lock access for the duration of this transaction.
      TAO_POA_GUARD_RETURN (0);

      PortableServer::ObjectId *result =
        this->activate_object_i (servant,
                                 this->server_priority (),
                                 wait_occurred_restart_call);

      // If we ended up waiting on a condition variable, the POA state
      // may have changed while we are waiting.  Therefore, we need to
      // restart this call.
      if (wait_occurred_restart_call)
        continue;
      else
        return result;
    }
}

void
TAO_Root_POA::activate_object_with_id (const PortableServer::ObjectId &id,
                                       PortableServer::Servant servant)
{
  while (1)
    {
      bool wait_occurred_restart_call = false;

      // Lock access for the duration of this transaction.
      TAO_POA_GUARD;

      this->activate_object_with_id_i (id,
                                       servant,
                                       this->server_priority (),
                                       wait_occurred_restart_call);

      // If we ended up waiting on a condition variable, the POA state
      // may have changed while we are waiting.  Therefore, we need to
      // restart this call.
      if (wait_occurred_restart_call)
        continue;
      else
        return;
    }
}

void
TAO_Root_POA::activate_object_with_id_i (const PortableServer::ObjectId &id,
                                         PortableServer::Servant servant,
                                         CORBA::Short priority,
                                         bool &wait_occurred_restart_call)
{
  this->active_policy_strategies_.servant_retention_strategy()->
    activate_object_with_id (id,
                             servant,
                             priority,
                             wait_occurred_restart_call);
}

void
TAO_Root_POA::deactivate_all_objects_i (CORBA::Boolean etherealize_objects,
                                        CORBA::Boolean wait_for_completion)
{
  this->deactivate_all_objects_i (etherealize_objects);

  this->wait_for_completions (wait_for_completion);
}

void
TAO_Root_POA::wait_for_completions (CORBA::Boolean wait_for_completion)
{
  while (wait_for_completion &&
         this->outstanding_requests_ > 0)
    {
      this->wait_for_completion_pending_ = true;

      int const result = this->outstanding_requests_condition_.wait ();
      if (result == -1)
        {
          throw ::CORBA::OBJ_ADAPTER ();
        }
    }
}

/* static */
void
TAO_Root_POA::check_for_valid_wait_for_completions (const TAO_ORB_Core &orb_core,
                                                    CORBA::Boolean wait_for_completion)
{
  if (wait_for_completion)
    {
      TAO::Portable_Server::POA_Current_Impl *poa_current_impl =
        static_cast <TAO::Portable_Server::POA_Current_Impl *>
                    (TAO_TSS_Resources::instance ()->poa_current_impl_);

      while (1)
        {
          // If wait_for_completion is TRUE and the current thread is
          // in an invocation context dispatched from some POA
          // belonging to the same ORB as this POA, the BAD_INV_ORDER
          // system exception with standard minor code 3 is raised and
          // POA destruction does not occur.
          if ((poa_current_impl != 0) && (poa_current_impl->poa () != 0))
            {
              if (&orb_core == &poa_current_impl->orb_core ())
                {
                  // CORBA 2.3 specifies which minor code corresponds
                  // to this particular problem.
                  throw ::CORBA::BAD_INV_ORDER (CORBA::OMGVMCID | 3,
                                                CORBA::COMPLETED_NO);
                }
            }
          else
            break;

          poa_current_impl =
            poa_current_impl->previous_current_impl_;
        }
    }
}

void
TAO_Root_POA::deactivate_all_objects_i (CORBA::Boolean etherealize_objects)
{
  this->active_policy_strategies_.request_processing_strategy ()->
    etherealize_objects (etherealize_objects);

  this->active_policy_strategies_.servant_retention_strategy ()->
    deactivate_all_objects ();
}

void
TAO_Root_POA::deactivate_object (const PortableServer::ObjectId &oid)
{
  // Lock access for the duration of this transaction.
  TAO_POA_GUARD;

  this->deactivate_object_i (oid);
}

void
TAO_Root_POA::deactivate_object_i (const PortableServer::ObjectId &id)
{
  this->active_policy_strategies_.servant_retention_strategy()->
    deactivate_object (id);
}

CORBA::Boolean
TAO_Root_POA::is_persistent (void) const
{
  return active_policy_strategies_.lifespan_strategy()->is_persistent ();
}

CORBA::Object_ptr
TAO_Root_POA::create_reference (const char *intf)
{
  // Lock access for the duration of this transaction.
  TAO_POA_GUARD_RETURN (CORBA::Object::_nil ());

  return this->create_reference_i (intf,
                                   this->server_priority ());
}

CORBA::Object_ptr
TAO_Root_POA::create_reference_i (const char *intf,
                                  CORBA::Short priority)
{
  if (!this->has_system_id ())
    {
      throw PortableServer::POA::WrongPolicy ();
    }

  return this->active_policy_strategies_.servant_retention_strategy()->
    create_reference (intf, priority);
}

CORBA::Object_ptr
TAO_Root_POA::invoke_key_to_object_helper_i (const char * repository_id,
                                             const PortableServer::ObjectId & id)
{
  const PortableInterceptor::ObjectId &user_oid =
    reinterpret_cast <const PortableInterceptor::ObjectId &>(id);

  // Ask the ORT to create the object.
  if (this->ORT_adapter_i ())
    {
      // Ask the ORT to create the object.
      return this->ort_adapter_->make_object (repository_id, user_oid);
    }
  else
    {
      return this->invoke_key_to_object ();
    }
}

#if ! defined (CORBA_E_MICRO)
CORBA::Object_ptr
TAO_Root_POA::create_reference_with_id_i (const PortableServer::ObjectId &user_id,
                                          const char *intf,
                                          CORBA::Short priority)
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
  if (this->has_system_id () &&
      !this->is_poa_generated_id (user_id))
    {
      throw ::CORBA::BAD_PARAM (CORBA::OMGVMCID | 14, CORBA::COMPLETED_NO);
    }

  return this->active_policy_strategies_.servant_retention_strategy()->
    create_reference_with_id (user_id, intf, priority);
}
#endif

PortableServer::ObjectId *
TAO_Root_POA::servant_to_id_i (PortableServer::Servant servant)
{
  return this->active_policy_strategies_.request_processing_strategy()->
    servant_to_id (servant);
}

CORBA::Object_ptr
TAO_Root_POA::servant_to_reference_i (PortableServer::Servant servant)
{
  return this->active_policy_strategies_.servant_retention_strategy()->
    servant_to_reference (servant);
}

PortableServer::Servant
TAO_Root_POA::reference_to_servant_i (CORBA::Object_ptr reference)
{
  // Make sure that the reference is valid.
  if (CORBA::is_nil (reference))
    {
      throw ::CORBA::BAD_PARAM ();
    }

  PortableServer::ObjectId system_id;
  bool const is_generated =
    this->is_poa_generated (reference, system_id);

  if (!is_generated)
    {
      // In case this object reference is not generated by this POA throw
      // an exception
      throw PortableServer::POA::WrongAdapter ();
    }

  PortableServer::Servant servant =
    this->active_policy_strategies_.request_processing_strategy()->
      system_id_to_servant (system_id);

  if (servant != 0)
    {
      // ATTENTION: Trick locking here, see class header for details
      TAO::Portable_Server::Non_Servant_Upcall non_servant_upcall (*this);
      ACE_UNUSED_ARG (non_servant_upcall);

      // The POA invokes _add_ref once on the Servant before returning
      // it. If the application uses reference counting, the caller of
      // id_to_servant is responsible for invoking _remove_ref once on
      // the returned Servant when it is finished with it. A
      // conforming caller need not invoke _remove_ref on the returned
      // Servant if the type of the Servant uses the default reference
      // counting inherited from ServantBase.
      servant->_add_ref ();
    }

  return servant;
}

bool
TAO_Root_POA::is_poa_generated (CORBA::Object_ptr reference,
                                PortableServer::ObjectId &system_id)
{
  TAO::ObjectKey_var key = reference->_key ();

  TAO_Object_Adapter::poa_name poa_system_name;
  CORBA::Boolean is_root = false;
  CORBA::Boolean is_persistent = false;
  CORBA::Boolean is_system_id = false;
  TAO::Portable_Server::Temporary_Creation_Time poa_creation_time;

  int const result = this->parse_key (key.in (),
                                      poa_system_name,
                                      system_id,
                                      is_root,
                                      is_persistent,
                                      is_system_id,
                                      poa_creation_time);
  if (result != 0
      || (this->root () == 0 && poa_system_name != this->system_name ())
      || is_root != this->root ()
      || is_system_id != this->system_id ()
      || !this->validate_lifespan (is_persistent, poa_creation_time))
    {
      // The passed reference is NOT generated by this POA.
      return false;
    }
  else
    {
      // The passed reference is generated by this POA.
      return true;
    }
}

PortableServer::ObjectId *
TAO_Root_POA::reference_to_id (CORBA::Object_ptr reference)
{
  // Make sure that the reference is valid.
  if (CORBA::is_nil (reference))
    {
      throw ::CORBA::BAD_PARAM ();
    }

  // The WrongPolicy exception is declared to allow future extensions.

  // This operation is valid only if the reference was created by the
  // POA on which the operation is being performed.  If the object
  // reference was not created by this POA, the WrongAdapter exception
  // is raised.
  PortableServer::ObjectId system_id;
  bool const is_generated = this->is_poa_generated (reference, system_id);

  if (!is_generated)
    {
      throw PortableServer::POA::WrongAdapter ();
    }

  // Lock access for the duration of this transaction.
  TAO_POA_GUARD_RETURN (0);

  return this->active_policy_strategies_.servant_retention_strategy()->
    system_id_to_object_id (system_id);
}

PortableServer::Servant
TAO_Root_POA::find_servant (const PortableServer::ObjectId &system_id)
{
  return this->active_policy_strategies_.servant_retention_strategy()->
    find_servant (system_id);
}

int
TAO_Root_POA::unbind_using_user_id (const PortableServer::ObjectId &user_id)
{
  return this->active_policy_strategies_.servant_retention_strategy()->
    unbind_using_user_id  (user_id);
}

void
TAO_Root_POA::cleanup_servant (
  PortableServer::Servant servant,
  const PortableServer::ObjectId &user_id)
{
  this->active_policy_strategies_.request_processing_strategy()->
    cleanup_servant (servant, user_id);
}

PortableServer::Servant
TAO_Root_POA::id_to_servant_i (const PortableServer::ObjectId &id)
{

  PortableServer::Servant servant =
    this->active_policy_strategies_.request_processing_strategy()->
      id_to_servant (id);

  if (servant != 0)
    {
      // ATTENTION: Trick locking here, see class header for details
      TAO::Portable_Server::Non_Servant_Upcall non_servant_upcall (*this);
      ACE_UNUSED_ARG (non_servant_upcall);

      // The POA invokes _add_ref once on the Servant before returning
      // it. If the application uses reference counting, the caller of
      // id_to_servant is responsible for invoking _remove_ref once on
      // the returned Servant when it is finished with it. A
      // conforming caller need not invoke _remove_ref on the returned
      // Servant if the type of the Servant uses the default reference
      // counting inherited from ServantBase.
      servant->_add_ref ();
    }

  return servant;
}

PortableServer::Servant
TAO_Root_POA::user_id_to_servant_i (const PortableServer::ObjectId &id)
{
  return this->active_policy_strategies_.servant_retention_strategy()->
    user_id_to_servant (id);
}

CORBA::Object_ptr
TAO_Root_POA::id_to_reference_i (const PortableServer::ObjectId &id,
                                 bool indirect)
{
  return this->active_policy_strategies_.servant_retention_strategy()->
    id_to_reference (id, indirect);
}

CORBA::OctetSeq *
TAO_Root_POA::id (void)
{
  CORBA::OctetSeq *id = 0;
  ACE_NEW_THROW_EX (id,
                    CORBA::OctetSeq (this->id_),
                    CORBA::NO_MEMORY ());

  return id;
}

PortableServer::Servant
TAO_Root_POA::locate_servant_i (const char *operation,
                                const PortableServer::ObjectId &system_id,
                                TAO::Portable_Server::Servant_Upcall &servant_upcall,
                                TAO::Portable_Server::POA_Current_Impl &poa_current_impl,
                                bool &wait_occurred_restart_call)
{
  return this->active_policy_strategies_.request_processing_strategy()->
    locate_servant (operation,
                    system_id,
                    servant_upcall,
                    poa_current_impl,
                    wait_occurred_restart_call);
}

/* static */
int
TAO_Root_POA::parse_key (const TAO::ObjectKey &key,
                         TAO_Object_Adapter::poa_name &poa_system_name,
                         PortableServer::ObjectId &system_id,
                         CORBA::Boolean &is_root,
                         CORBA::Boolean &is_persistent,
                         CORBA::Boolean &is_system_id,
                         TAO::Portable_Server::Temporary_Creation_Time &poa_creation_time)
{
  // Get the object key octets.
  const CORBA::Octet *key_data = key.get_buffer ();

  // Skip the object key prefix since we have already checked for this.
  CORBA::ULong starting_at = TAO_OBJECTKEY_PREFIX_SIZE;

  // Check the root indicator.
  char root_key_type = key_data[starting_at];
  if (root_key_type == TAO_Root_POA::root_key_char ())
    {
      is_root = true;
    }
  else if (root_key_type == TAO_Root_POA::non_root_key_char ())
    {
      is_root = false;
    }
  else
    {
      // Incorrect key
      return -1;
    }

  // Skip past the system id indicator
  starting_at += TAO_Root_POA::root_key_type_length ();

  // Check the system id indicator.
  char system_id_key_type = key_data[starting_at];
  if (system_id_key_type == TAO_Root_POA::system_id_key_char ())
    {
      is_system_id = true;
    }
  else if (system_id_key_type == TAO_Root_POA::user_id_key_char ())
    {
      is_system_id = false;
    }
  else
    {
      // Incorrect key
      return -1;
    }

  // Skip past the system id indicator
  starting_at += TAO_Root_POA::system_id_key_type_length ();

  // Check the persistence indicator
  char persistent_key_type = key_data[starting_at];
  if (persistent_key_type == TAO_Root_POA::persistent_key_char ())
    {
      is_persistent = true;
    }
  else if (persistent_key_type == TAO_Root_POA::transient_key_char ())
    {
      is_persistent = false;
    }
  else
    {
      // Incorrect key
      return -1;
    }

  // Skip past the persistent indicator
  starting_at += TAO_Root_POA::persistent_key_type_length ();

#if (POA_NO_TIMESTAMP == 0)
  // Grab the timestamp for transient POAs.
  if (!is_persistent)
    {
      // Take the creation time for the timestamp
      poa_creation_time.creation_time (key_data + starting_at);

      // Skip past the timestamp
      starting_at += TAO::Portable_Server::Creation_Time::creation_time_length ();
    }
#else
  ACE_UNUSED_ARG (poa_creation_time);
#endif /* POA_NO_TIMESTAMP */

  // Calculate the size of the POA name.
  CORBA::ULong poa_name_size = 0;
  if (!is_persistent)
    {
      // Transient POAs have fixed size.
      poa_name_size = TAO_Object_Adapter::transient_poa_name_size ();
    }
  else if (is_system_id)
    {
      // System ids have fixed size.
      poa_name_size = static_cast <CORBA::ULong>
                                  (key.length () - starting_at -
                                   TAO_Active_Object_Map::system_id_size ());
    }
  else
    {
      // Get the size from the object key.
      ACE_OS::memcpy (&poa_name_size,
                      key_data + starting_at,
                      sizeof (poa_name_size));
      poa_name_size = ACE_NTOHL (poa_name_size);

      starting_at += sizeof (poa_name_size);
    }

  // Grep the name if there is a name
  if (!is_root)
    {
      poa_system_name.replace (poa_name_size,
                               poa_name_size,
                               (CORBA::Octet *) key_data + starting_at,
                               0);

      starting_at += poa_name_size;
    }

  // The rest is the system id.
  CORBA::ULong system_id_size = key.length () - starting_at;

  // Reset <system_id>.
  system_id.length (system_id_size);
  CORBA::Octet * buf = system_id.get_buffer ();
  ACE_OS::memcpy (buf, key_data + starting_at, system_id_size);

  // Success
  return 0;
}

TAO::ObjectKey *
TAO_Root_POA::create_object_key (const PortableServer::ObjectId &id)
{
  // Calculate the space required for the key.
  CORBA::ULong buffer_size =
    this->id_.length () +
    id.length ();

  // Create the buffer for the key.
  CORBA::Octet *buffer = TAO::ObjectKey::allocbuf (buffer_size);

  // First copy the POA id into the key.
  ACE_OS::memcpy (&buffer[0],
                  this->id_.get_buffer (),
                  this->id_.length ());

  // Then copy the object id into the key.
  ACE_OS::memcpy (&buffer[this->id_.length ()],
                  id.get_buffer (),
                  id.length ());

  // Create the key, giving the ownership of the buffer to the
  // sequence.
  TAO::ObjectKey *key = 0;
  ACE_NEW_RETURN (key,
                  TAO::ObjectKey (buffer_size,
                                  buffer_size,
                                  buffer,
                                  1),
                  0);

  return key;
}

void
TAO_Root_POA::set_id (TAO_Root_POA *parent)
{
  // Calculate the prefix size.
  CORBA::ULong prefix_size = 0;
  prefix_size += TAO_OBJECTKEY_PREFIX_SIZE;

  // If we are dealing with a persistent POA and user ids are being
  // used, then we need to add the POA name length field to the object
  // key. Otherwise, the POA name length can be calculated by looking
  // at the remainder after extracting other parts of the key.
  bool const add_poa_name_length =
    this->is_persistent () &&
    !this->system_id ();

  // Size required by the POA name.
  CORBA::ULong poa_name = 0;

  // Calculate the space required for the POA name.
  CORBA::ULong poa_name_length = this->system_name_->length ();
  if (parent != 0)
    {
      poa_name += poa_name_length;
    }

  // Check if we need to added the length of the POA name.
  if (add_poa_name_length)
    {
      poa_name += sizeof (poa_name_length);
    }

  // Get the space needed for the lifespan length
  // byte.
  CORBA::ULong const lifespan_key_length =
    this->active_policy_strategies_.lifespan_strategy()->key_length ();

  CORBA::ULong const id_assignment_key_length =
    this->active_policy_strategies_.id_assignment_strategy()->key_type_length ();

  // Calculate the space required for the POA id.
  CORBA::ULong const buffer_size =
    prefix_size +
    this->root_key_type_length () +
    id_assignment_key_length +
    lifespan_key_length +
    poa_name;

  // Create the buffer for the POA id.
  this->id_.length (buffer_size);
  CORBA::Octet *buffer = &this->id_[0];

  // Keeps track of where the next infomation goes; start at 0 byte.
  CORBA::ULong starting_at = 0;

  // Add the object key prefix.
  ACE_OS::memcpy (&buffer[starting_at],
                  &objectkey_prefix[0],
                  TAO_OBJECTKEY_PREFIX_SIZE);

  starting_at += TAO_OBJECTKEY_PREFIX_SIZE;

  // Copy the root byte.
  if (parent != 0)
    {
      buffer[starting_at] = (CORBA::Octet) TAO_Root_POA::non_root_key_char ();
    }
  else
    {
      buffer[starting_at] = (CORBA::Octet) TAO_Root_POA::root_key_char ();
    }
  starting_at += this->root_key_type_length ();

  // Add the id_assignment part
  this->active_policy_strategies_.id_assignment_strategy()->create_key (buffer, starting_at);

  // Add the lifespan part
  this->active_policy_strategies_.lifespan_strategy()->create_key (buffer, starting_at);

  // Check if we need to added the length of the POA name.
  if (add_poa_name_length)
    {
      poa_name_length = ACE_HTONL (poa_name_length);
      ACE_OS::memcpy (&buffer[starting_at],
                      &poa_name_length,
                      sizeof (poa_name_length));
      starting_at += sizeof (poa_name_length);
    }

  // Put the POA name into the key (for non-root POAs).
  if (parent != 0)
    {
      ACE_OS::memcpy (&buffer[starting_at],
                      this->system_name_->get_buffer (),
                      this->system_name_->length ());
      starting_at += this->system_name_->length ();
    }
}

int
TAO_Root_POA::is_poa_generated_id (const PortableServer::ObjectId &id)
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

void
TAO_Root_POA::set_folded_name (TAO_Root_POA *parent)
{
  size_t length = 0;
  size_t parent_length = 0;

  if (parent != 0)
    {
      parent_length = parent->folded_name ().length ();
      length += parent_length;
    }

  length += this->name_.length ();
  length += TAO_Root_POA::name_separator_length ();

  this->folded_name_.length (static_cast <CORBA::ULong> (length));
  CORBA::Octet *folded_name_buffer = this->folded_name_.get_buffer ();

  if (parent != 0)
    {
      ACE_OS::memcpy (folded_name_buffer,
                      parent->folded_name ().get_buffer (),
                      parent_length);
    }

  ACE_OS::memcpy (&folded_name_buffer[parent_length],
                  this->name_.c_str (),
                  this->name_.length ());

  folded_name_buffer[length - TAO_Root_POA::name_separator_length ()] = TAO_Root_POA::name_separator ();
}

int
TAO_Root_POA::parse_ir_object_key (const TAO::ObjectKey &object_key,
                                   PortableServer::ObjectId &user_id)
{
  TAO_Object_Adapter::poa_name poa_system_name;
  CORBA::Boolean is_root = false;
  CORBA::Boolean is_persistent = false;
  CORBA::Boolean is_system_id = false;
  TAO::Portable_Server::Temporary_Creation_Time poa_creation_time;

  return TAO_Root_POA::parse_key (object_key,
                                  poa_system_name,
                                  user_id,
                                  is_root,
                                  is_persistent,
                                  is_system_id,
                                  poa_creation_time);
}

TAO_Object_Adapter &
TAO_Root_POA::object_adapter (void)
{
  return *this->object_adapter_;
}

CORBA::Object_ptr
TAO_Root_POA::invoke_key_to_object (void)
{
  PortableServer::ObjectId_var &system_id =
    *this->key_to_object_params_.system_id_;

  // Create object key.
  TAO::ObjectKey_var key =
    this->create_object_key (system_id.in ());

  return this->key_to_object (key.in (),
                              this->key_to_object_params_.type_id_,
                              this->key_to_object_params_.servant_,
                              this->key_to_object_params_.collocated_,
                              this->key_to_object_params_.priority_,
                              this->key_to_object_params_.indirect_);
}

CORBA::Object_ptr
TAO_Root_POA::key_to_object (const TAO::ObjectKey &key,
                             const char *type_id,
                             TAO_ServantBase *servant,
                             CORBA::Boolean collocated,
                             CORBA::Short priority,
                             bool indirect)
{
  // Check if the ORB is still running, otherwise throw an exception.
  // @@ What if the ORB was destroyed?  In that case we shouldn't even
  //    get here!
  this->orb_core_.check_shutdown ();

  //
  // ImplRepo related.
  //
#if (TAO_HAS_MINIMUM_CORBA == 0)

  CORBA::Object_ptr obj = CORBA::Object::_nil ();

  if (indirect && this->active_policy_strategies_.lifespan_strategy()->use_imr ()
         && this->orb_core ().imr_endpoints_in_ior ())
    {
      // Check to see if we alter the IOR.
      CORBA::Object_var imr = this->orb_core ().implrepo_service ();

      if (CORBA::is_nil (imr.in ())
          || !imr->_stubobj ()
          || !imr->_stubobj ()->profile_in_use ())
        {
          if (TAO_debug_level > 1)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "Missing ImR IOR, will not use the ImR\n"));
            }
          goto orbkey;
        }

      CORBA::String_var imr_str =
        imr->_stubobj ()->profile_in_use ()->to_string ();

      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "IMR IOR =\n%C\n",
                    imr_str.in ()));

      // Search for "corbaloc:" alone, without the protocol.  This code
      // should be protocol neutral.
      const char corbaloc[] = "corbaloc:";
      char *pos = ACE_OS::strstr (imr_str.inout (), corbaloc);
      pos = ACE_OS::strchr (pos + sizeof (corbaloc), ':');

      pos = ACE_OS::strchr (pos + 1,
                            imr->_stubobj ()->profile_in_use ()->object_key_delimiter ());

      if (pos)
        pos[1] = 0;  // Crop the string.
      else
        {
          if (TAO_debug_level > 0)
            ACE_ERROR ((LM_ERROR,
                        "Could not parse ImR IOR, skipping ImRification\n"));
          goto orbkey;
        }

      ACE_CString ior (imr_str.in ());

      // Add the key.

      CORBA::String_var key_str;
      TAO::ObjectKey::encode_sequence_to_string (key_str.inout (), key);

      ior += key_str.in ();

      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "ImR-ified IOR =\n%C\n",
                    ior.c_str ()));

      obj = this->orb_core_.orb ()->string_to_object (ior.c_str ());

      // type_id info is not in the corbaloc, so set it here
      obj->_stubobj()->type_id = type_id;

      return obj;
    }

orbkey:

#else
  ACE_UNUSED_ARG (indirect);
#endif /* TAO_HAS_MINIMUM_CORBA */

  TAO_Stub *data = this->key_to_stub_i (key, type_id, priority);

  TAO_Stub_Auto_Ptr safe_data (data);

  CORBA::Object_ptr tmp;

  if (this->orb_core_.optimize_collocation_objects ())
    {
      ACE_NEW_THROW_EX (tmp, CORBA::Object (data,
                                            collocated,
                                            servant),
                        CORBA::INTERNAL ());

    }
  else
    {
      ACE_NEW_THROW_EX (tmp,
                        CORBA::Object (data,
                                       collocated),
                        CORBA::INTERNAL ());
    }

  data->servant_orb (this->orb_core_.orb ());

  // Transfer ownership to the Object.
  (void) safe_data.release ();

  return tmp;
}

TAO_Stub *
TAO_Root_POA::key_to_stub (const TAO::ObjectKey &key,
                           const char *type_id,
                           CORBA::Short priority)
{
  // Check if the ORB is still running, otherwise throw an exception.
  // @@ What if the ORB was destroyed?  In that case we shouldn't even
  //    get here!
  this->orb_core_.check_shutdown ();

  return this->key_to_stub_i (key, type_id, priority);
}

TAO_Stub *
TAO_Root_POA::key_to_stub_i (const TAO::ObjectKey &key,
                             const char *type_id,
                             CORBA::Short priority)
{
  CORBA::PolicyList_var client_exposed_policies =
    this->client_exposed_policies (priority);

  TAO_Acceptor_Filter* filter = 0;

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
  if (this->filter_factory_)
    {
      filter = this->filter_factory_->create_object (this->poa_manager_);
    }
  else
#endif
    {
      ACE_NEW_RETURN (filter,
                      TAO_Default_Acceptor_Filter (),
                      0);
    }

  // Give ownership to the auto pointer.
  auto_ptr<TAO_Acceptor_Filter> new_filter (filter);

  TAO_Stub *data =
    this->create_stub_object (
      key,
      type_id,
      client_exposed_policies._retn (),
      filter,
      this->orb_core_.lane_resources ().acceptor_registry ());

  return data;
}

void
TAO_Root_POA::establish_components (void)
{
  TAO_IORInterceptor_Adapter *ior_adapter =
    this->orb_core_.ior_interceptor_adapter ();

  if (ior_adapter)
    {
      ior_adapter->establish_components (this);
    }
}

void
TAO_Root_POA::components_established (PortableInterceptor::IORInfo_ptr info)
{
  TAO_IORInterceptor_Adapter *ior_adapter =
    this->orb_core_.ior_interceptor_adapter ();

  if (ior_adapter)
    {
      ior_adapter->components_established (info);
    }
}

void
TAO_Root_POA::save_ior_component (const IOP::TaggedComponent &component)
{
  CORBA::ULong const old_len = this->tagged_component_.length ();

  this->tagged_component_.length (old_len + 1);
  this->tagged_component_[old_len] = component;
}

void
TAO_Root_POA::
save_ior_component_and_profile_id (const IOP::TaggedComponent &component,
                                   IOP::ProfileId profile_id)
{
  // The length of this->tagged_component_id_ is the same as the
  // length of the profile_id_array_ since we are trying to make a
  // one-to-one link between these two arrays. So, whenever
  // this->tagged_component_id_ is increased, we need to increase the
  // size of this->profile_id_array_ also.

  CORBA::ULong const old_len = this->tagged_component_id_.length ();

  CORBA::ULong const new_len = old_len + 1;

  this->tagged_component_id_.length (new_len);
  this->tagged_component_id_[old_len] = component;

  this->profile_id_array_.size (new_len);
  this->profile_id_array_[old_len] = profile_id;
}

TAO_Stub *
TAO_Root_POA::create_stub_object (const TAO::ObjectKey &object_key,
                                  const char *type_id,
                                  CORBA::PolicyList *policy_list,
                                  TAO_Acceptor_Filter *filter,
                                  TAO_Acceptor_Registry &acceptor_registry)
{
  bool error = false;

  // Count the number of endpoints.
  size_t const profile_count = acceptor_registry.endpoint_count ();

  // Create a profile container and have acceptor registries populate
  // it with profiles as appropriate.
  TAO_MProfile mprofile (0);

  // Allocate space for storing the profiles.  There can never be more
  // profiles than there are endpoints.  In some cases, there can be
  // less profiles than endpoints.
  int result = mprofile.set (static_cast <CORBA::ULong> (profile_count));
  if (result == -1)
    error = true;

  if (!error)
    {
      result =
        filter->fill_profile (object_key,
                              mprofile,
                              acceptor_registry.begin (),
                              acceptor_registry.end ());
      if (result == -1)
        error = true;
    }

  if (!error)
    result = filter->encode_endpoints (mprofile);

  if (result == -1)
    error = true;

  if (error)
    throw ::CORBA::INTERNAL (
      CORBA::SystemException::_tao_minor_code (
        TAO_MPROFILE_CREATION_ERROR,
        0),
      CORBA::COMPLETED_NO);

  // Make sure we have at least one profile.  <mp> may end up being
  // empty if none of the acceptor endpoints have the right priority
  // for this object, for example.
  if (mprofile.profile_count () == 0)
    throw ::CORBA::BAD_PARAM (
      CORBA::SystemException::_tao_minor_code (
        TAO_MPROFILE_CREATION_ERROR,
        0),
      CORBA::COMPLETED_NO);

  TAO_Stub *stub =
    this->orb_core_.create_stub_object (mprofile, type_id, policy_list);

  // Add the saved tagged components methods to the profiles.
  CORBA::ULong len = this->tagged_component_.length ();
  for (CORBA::ULong i = 0; i != len; ++i)
    {
      this->add_ior_component (mprofile, this->tagged_component_[i]);
    }

  len = this->tagged_component_id_.length ();

  for (CORBA::ULong k = 0; k != len; ++k)
    {
      this->add_ior_component_to_profile (mprofile,
                                          this->tagged_component_id_[k],
                                          this->profile_id_array_[k]);
    }

  return stub;
}

CORBA::PolicyList *
TAO_Root_POA::client_exposed_policies (CORBA::Short /* object_priority */)
{
  CORBA::PolicyList *client_exposed_policies = 0;
  ACE_NEW_THROW_EX (client_exposed_policies,
                    CORBA::PolicyList (),
                    CORBA::NO_MEMORY (TAO::VMCID,
                                      CORBA::COMPLETED_NO));

  CORBA::PolicyList_var policies = client_exposed_policies;

  // Add in all of the client exposed policies.
  this->policies_.add_client_exposed_fixed_policies (client_exposed_policies);

  return policies._retn ();
}

TAO_SERVANT_LOCATION
TAO_Root_POA::locate_servant_i (const PortableServer::ObjectId &system_id,
                                PortableServer::Servant &servant)
{
  return this->active_policy_strategies_.request_processing_strategy()->
          locate_servant (system_id, servant);
}

TAO_SERVANT_LOCATION
TAO_Root_POA::servant_present (const PortableServer::ObjectId &system_id,
                               PortableServer::Servant &servant)
{
  return this->active_policy_strategies_.servant_retention_strategy()->
          servant_present (system_id, servant);
}

PortableServer::Servant
TAO_Root_POA::find_servant (
        const PortableServer::ObjectId &system_id,
        TAO::Portable_Server::Servant_Upcall &servant_upcall,
        TAO::Portable_Server::POA_Current_Impl &poa_current_impl)
{
  return this->active_policy_strategies_.servant_retention_strategy()->
          find_servant (system_id,
                        servant_upcall,
                        poa_current_impl);
}

int
TAO_Root_POA::find_servant_priority (
         const PortableServer::ObjectId &system_id,
         CORBA::Short &priority)
{
  return this->active_policy_strategies_.servant_retention_strategy()->
          find_servant_priority (system_id, priority);
}

TAO::ORT_Adapter *
TAO_Root_POA::ORT_adapter_i (void)
{
  if ((this->ort_adapter_factory_) && (this->ort_adapter_ == 0))
    {
      try
        {
          // Get the full adapter name of this POA, do this before we
          // create the adapter so that in case this fails, we just
          // return 0 and not a not activated adapter
          PortableInterceptor::AdapterName *adapter_name = this->adapter_name_i ();

          this->ort_adapter_ = this->ort_adapter_factory_->create ();

          if (this->ort_adapter_)
            {
              // @todo We have to look at this, we activate it but hold the POA lock,
              // in case we are called by ORT_adapter, we shouldn't keep the lock
              // here, but then the ort_adapter should be guarded against multiple
              // activations.
              this->ort_adapter_->activate (this->orb_core_.server_id (),
                                            this->orb_core_.orbid (),
                                            adapter_name,
                                            this);
            }
        }
      catch (const ::CORBA::Exception& ex)
        {
          ex._tao_print_exception (
            "(%P|%t) Cannot initialize the "
            "object_reference_template_adapter\n");
        }
    }

  return this->ort_adapter_;
}

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

PortableServer::AdapterActivator_ptr
TAO_Root_POA::the_activator (void)
{
  // Lock access for the duration of this transaction.
  TAO_POA_GUARD_RETURN (PortableServer::AdapterActivator::_nil ());

  return PortableServer::AdapterActivator::_duplicate (this->adapter_activator_.in ());
}

void
TAO_Root_POA::the_activator (PortableServer::AdapterActivator_ptr adapter_activator)
{
  // Lock access for the duration of this transaction.
  TAO_POA_GUARD;

  this->adapter_activator_ = PortableServer::AdapterActivator::_duplicate (adapter_activator);
}

#endif /* TAO_HAS_MINIMUM_POA == 0 && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)*/

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

PortableServer::ServantManager_ptr
TAO_Root_POA::get_servant_manager (void)
{
  // Lock access for the duration of this transaction.
  TAO_POA_GUARD_RETURN (PortableServer::ServantManager::_nil ());

  return this->active_policy_strategies_.request_processing_strategy()->
    get_servant_manager ();
}

void
TAO_Root_POA::set_servant_manager (PortableServer::ServantManager_ptr imgr)
{
  // Lock access for the duration of this transaction.
  TAO_POA_GUARD;

  this->active_policy_strategies_.request_processing_strategy()->
    set_servant_manager (imgr);
}

PortableServer::Servant
TAO_Root_POA::get_servant_i (void)
{
  return this->active_policy_strategies_.request_processing_strategy()->
    get_servant ();
}

PortableServer::Servant
TAO_Root_POA::get_servant (void)
{
  // Lock access for the duration of this transaction.
  TAO_POA_GUARD_RETURN (0);

  PortableServer::Servant servant = this->get_servant_i ();

  if (servant != 0)
    {
      // ATTENTION: Trick locking here, see class header for details
      TAO::Portable_Server::Non_Servant_Upcall non_servant_upcall (*this);
      ACE_UNUSED_ARG (non_servant_upcall);

      // The POA invokes _add_ref once on the Servant before returning
      // it. If the application uses reference counting, the caller of
      // get_servant is responsible for invoking _remove_ref once on
      // the returned Servant when it is finished with it. A
      // conforming caller need not invoke _remove_ref on the returned
      // Servant if the type of the Servant uses the default reference
      // counting inherited from ServantBase.
      servant->_add_ref ();

      return servant;
    }
  else
    {
      // If no servant has been associated with the POA, the NoServant
      // exception is raised.
      throw PortableServer::POA::NoServant ();
    }
}

void
TAO_Root_POA::set_servant (PortableServer::Servant servant)
{
  // Lock access for the duration of this transaction.
  TAO_POA_GUARD;

  this->active_policy_strategies_.request_processing_strategy()->
    set_servant (servant);
}

#endif /* TAO_HAS_MINIMUM_POA == 0 && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO) */

bool
TAO_Root_POA::is_servant_activation_allowed (PortableServer::Servant servant,
                                             bool &wait_occurred_restart_call)
{
  return this->active_policy_strategies_.id_uniqueness_strategy ()->
    is_servant_activation_allowed (servant, wait_occurred_restart_call);
}

bool
TAO_Root_POA::has_system_id (void) const
{
  return this->active_policy_strategies_.id_assignment_strategy ()->
    has_system_id ();
}

int
TAO_Root_POA::rebind_using_user_id_and_system_id (
  PortableServer::Servant servant,
  const PortableServer::ObjectId &user_id,
  const PortableServer::ObjectId &system_id,
  TAO::Portable_Server::Servant_Upcall &servant_upcall)
{
  return this->active_policy_strategies_.servant_retention_strategy ()->
    rebind_using_user_id_and_system_id (servant,
                                        user_id,
                                        system_id,
                                        servant_upcall);
}

CORBA::Boolean
TAO_Root_POA::servant_has_remaining_activations (PortableServer::Servant servant)
{
  return this->active_policy_strategies_.servant_retention_strategy ()->
    servant_has_remaining_activations (servant);
}

bool
TAO_Root_POA::allow_implicit_activation (void) const
{
  return this->active_policy_strategies_.implicit_activation_strategy ()->
    allow_implicit_activation ();
}

bool
TAO_Root_POA::allow_multiple_activations (void) const
{
  return this->active_policy_strategies_.id_uniqueness_strategy ()->
    allow_multiple_activations ();
}

void
TAO_Root_POA::post_invoke_servant_cleanup(
  const PortableServer::ObjectId &system_id,
  const TAO::Portable_Server::Servant_Upcall &servant_upcall)
{
  this->active_policy_strategies_.request_processing_strategy ()->
    post_invoke_servant_cleanup (system_id, servant_upcall);
}

CORBA::Short
TAO_Root_POA::server_priority (void) const
{
  return this->cached_policies_.server_priority ();
}

int
TAO_Root_POA::is_servant_active (
  PortableServer::Servant servant,
  bool &wait_occurred_restart_call)
{
  return this->active_policy_strategies_.servant_retention_strategy ()->
    is_servant_in_map (servant, wait_occurred_restart_call);
}

TAO::Portable_Server::Cached_Policies&
TAO_Root_POA::cached_policies (void)
{
  return this->cached_policies_;
}

TAO_Network_Priority_Hook*
TAO_Root_POA::network_priority_hook (void)
{
  return this->network_priority_hook_;
}

TAO::Portable_Server::Cached_Policies::PriorityModel
TAO_Root_POA::priority_model (void) const
{
  return cached_policies_.priority_model ();
}

#if (TAO_HAS_MINIMUM_POA == 0)
int
TAO_Root_POA::enter ()
{
  return this->active_policy_strategies_.thread_strategy ()->enter();
}
#endif /* TAO_HAS_MINIMUM_POA == 0 */

#if (TAO_HAS_MINIMUM_POA == 0)
int
TAO_Root_POA::exit ()
{
  return this->active_policy_strategies_.thread_strategy ()->exit();
}
#endif /* TAO_HAS_MINIMUM_POA == 0 */

bool
TAO_Root_POA::validate_lifespan (
  CORBA::Boolean is_persistent,
  const TAO::Portable_Server::Temporary_Creation_Time& creation_time) const
{
  return this->active_policy_strategies_.lifespan_strategy()->
    validate (is_persistent, creation_time);
}

CORBA::Boolean
TAO_Root_POA::root (void) const
{
  return true;
}

TAO::ORT_Adapter *
TAO_Root_POA::ORT_adapter (void)
{
  if (this->ort_adapter_ != 0)
    return this->ort_adapter_;

  // Lock access for the duration of this transaction.
  TAO_POA_GUARD_RETURN (0);

  // DCL ..
  if (this->ort_adapter_ != 0)
    {
      return this->ort_adapter_;
    }

  return this->ORT_adapter_i ();
}

CORBA::Policy *
TAO_Root_POA::server_protocol (void)
{
  return 0;
}

void
TAO_Root_POA::Key_To_Object_Params::set (PortableServer::ObjectId_var &system_id,
                                         const char *type_id,
                                         TAO_ServantBase *servant,
                                         CORBA::Boolean collocated,
                                         CORBA::Short priority,
                                         bool indirect)
{
  this->system_id_ = &system_id;
  this->type_id_ = type_id;
  this->servant_ = servant;
  this->collocated_ = collocated;
  this->priority_ = priority;
  this->indirect_ = indirect;
}

CORBA::ULong
TAO_Root_POA::waiting_servant_deactivation (void) const
{
  return this->active_policy_strategies_.servant_retention_strategy ()->
          waiting_servant_deactivation ();
}

void
TAO_Root_POA::ort_adapter_factory_name (const char *name)
{
  TAO_POA_Static_Resources::instance ()->ort_adapter_factory_name_ =
    name;
}

CORBA::Policy_ptr
TAO_Root_POA::get_policy (CORBA::PolicyType policy)
{
  return this->policies_.get_policy (policy);
}

void
TAO_Root_POA::check_state (void)
{
  this->active_policy_strategies_.lifespan_strategy ()->check_state ();
}

const char *
TAO_Root_POA::ort_adapter_factory_name (void)
{
  return TAO_POA_Static_Resources::instance ()->ort_adapter_factory_name_.c_str();
}

void
TAO_Root_POA::imr_client_adapter_name (const char *name)
{
  TAO_POA_Static_Resources::instance ()->imr_client_adapter_name_ = name;
}

const char *
TAO_Root_POA::imr_client_adapter_name (void)
{
  return TAO_POA_Static_Resources::instance ()->imr_client_adapter_name_.c_str();
}

PortableServer::POAManager_ptr
TAO_Root_POA::the_POAManager (void)
{
  return PortableServer::POAManager::_duplicate (&this->poa_manager_);
}

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
PortableServer::POAManagerFactory_ptr
TAO_Root_POA::the_POAManagerFactory (void)
{
  return PortableServer::POAManagerFactory::_duplicate (&this->poa_manager_factory_);
}
#endif

CORBA::ORB_ptr
TAO_Root_POA::_get_orb (void)
{
  return CORBA::ORB::_duplicate (this->orb_core_.orb ());
}

// Initialize instance_ to 0, since this is what we test for in the call
// to instance ().  Note that this does not require a constructor call, so
// it is always initialized by the time that instance () can be called.
TAO_POA_Static_Resources* TAO_POA_Static_Resources::instance_ = 0;

// Force an instance to be created at module initialization time,
// since we do not want to worry about double checked locking and
// the race condition to initialize the lock.
TAO_POA_Static_Resources* TAO_POA_Static_Resources::initialization_reference_ =
  TAO_POA_Static_Resources::instance ();

void
TAO_POA_Static_Resources::fini (void)
{
  delete TAO_POA_Static_Resources::instance_;
  TAO_POA_Static_Resources::instance_ = 0;
}

TAO_POA_Static_Resources*
TAO_POA_Static_Resources::instance (void)
{
  if (TAO_POA_Static_Resources::instance_ == 0)
    {
      // This new is never freed on purpose.  The data specified by
      // it needs to be around for the last shared library that references
      // this class.  This could occur in a destructor in a shared library
      // that is unloaded after this one.  One solution to avoid this
      // harmless memory leak would be to use reference counting.
      ACE_NEW_RETURN (TAO_POA_Static_Resources::instance_,
                      TAO_POA_Static_Resources (),
                      0);
    }

  return TAO_POA_Static_Resources::instance_;
}

TAO_POA_Static_Resources::TAO_POA_Static_Resources (void)
  : ort_adapter_factory_name_ ("ORT_Adapter_Factory"),
    imr_client_adapter_name_ ("ImR_Client_Adapter")
{
}

void
TAO_Root_POA::poa_activated_hook ()
{
}

void
TAO_Root_POA::poa_deactivated_hook ()
{
}

void
TAO_Root_POA::servant_activated_hook (PortableServer::Servant,
                                      const PortableServer::ObjectId&)
{
}

void
TAO_Root_POA::servant_deactivated_hook (PortableServer::Servant,
                                        const PortableServer::ObjectId&)
{
}

TAO_Active_Object_Map *
TAO_Root_POA::get_active_object_map() const
{
  return this->active_policy_strategies_.servant_retention_strategy()->
    get_active_object_map();
}


TAO_END_VERSIONED_NAMESPACE_DECL
