// @(#) $Id$

// auto_ptr class
#include "ace/Auto_Ptr.h"

#include "tao/POA.h"
#include "tao/ORB_Core.h"
#include "tao/ORB.h"
#include "tao/Server_Strategy_Factory.h"
#include "tao/Environment.h"
#include "tao/Exception.h"
#include "tao/debug.h"

// Forwarding Servant class
#include "tao/Forwarding_Servant.h"

#if !defined (__ACE_INLINE__)
# include "tao/POA.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, POA, "$Id$")

#if !defined (TAO_NO_IOR_TABLE)
// This is the TAO_Object_key-prefix that is appended to all TAO Object keys.
// Its an array of octets representing ^t^a^o/0 in octal.
CORBA::Octet
TAO_POA::objectkey_prefix [TAO_POA::TAO_OBJECTKEY_PREFIX_SIZE] = {
  024, // octal for ^t
  001, // octal for ^a
  017, // octal for ^o
  000
};
#endif /* TAO_NO_IOR_TABLE */

TAO_POA::TAO_POA (const TAO_POA::String &name,
                  TAO_POA_Manager &poa_manager,
                  const TAO_POA_Policies &policies,
                  TAO_POA *parent,
                  ACE_Lock &lock,
                  ACE_SYNCH_MUTEX &thread_lock,
                  TAO_ORB_Core &orb_core,
                  CORBA::Environment &ACE_TRY_ENV)
  : name_ (name),
    poa_manager_ (poa_manager),
    policies_ (policies),
    parent_ (parent),
    active_object_map_ (0),

#if !defined (TAO_HAS_MINIMUM_CORBA)

    adapter_activator_ (),
    servant_activator_ (),
    servant_locator_ (),
    default_servant_ (),

#endif /* TAO_HAS_MINIMUM_CORBA */

    children_ (),
    lock_ (lock),
    persistent_ (policies.lifespan () == PortableServer::PERSISTENT),
    system_id_ (policies.id_assignment () == PortableServer::SYSTEM_ID),
    creation_time_ (ACE_OS::gettimeofday ()),
    orb_core_ (orb_core),
    cleanup_in_progress_ (0),
    etherealize_objects_ (1),
    outstanding_requests_ (0),
    outstanding_requests_condition_ (thread_lock),
    wait_for_completion_pending_ (0),
    waiting_destruction_ (0),
    servant_deactivation_condition_ (thread_lock),
    waiting_servant_deactivation_ (0)
{
  // Set the folded name of this POA.
  this->set_folded_name ();

  // Create the active object map.
  TAO_Active_Object_Map *active_object_map = 0;
  ACE_NEW_THROW_EX (active_object_map,
                    TAO_Active_Object_Map (!this->system_id (),
                                           this->policies ().id_uniqueness () == PortableServer::UNIQUE_ID,
                                           this->persistent (),
                                           this->orb_core_.server_factory ()->active_object_map_creation_parameters (),
                                           ACE_TRY_ENV),
                    CORBA::NO_MEMORY ());

  // Give ownership of the new map to the auto pointer.  Note, that it
  // is important for the auto pointer to take ownership before
  // checking for exception since we may need to delete the new map.
  auto_ptr<TAO_Active_Object_Map> new_active_object_map (active_object_map);

  // Check for exception in construction of the active object map.
  ACE_CHECK;

  // Register self with manager.
  int result = this->poa_manager_.register_poa (this);
  if (result != 0)
    {
      ACE_THROW (CORBA::OBJ_ADAPTER ());
    }

  // Add self to Object Adapter class.
  result = this->orb_core_.object_adapter ()->bind_poa (this->folded_name_,
                                                        this,
                                                        this->system_name_.out ());
  if (result != 0)
    {
      // Remove from POA Manager in case of errors. No checks of
      // further errors...
      this->poa_manager_.remove_poa (this);

      ACE_THROW (CORBA::OBJ_ADAPTER ());
    }

  // Finally everything is fine.  Make sure to take ownership away
  // from the auto pointer.
  this->active_object_map_ = new_active_object_map.release ();
}

TAO_POA::~TAO_POA (void)
{
  // Delete the active object map.
  delete this->active_object_map_;

  // Note: Errors are ignored here since there is nothing we can do
  // about them.

  // Remove POA from the POAManager.
  this->poa_manager_.remove_poa (this);

  // Remove POA from the Object Adapter.
  this->orb_core_.object_adapter ()->unbind_poa (this,
                                                 this->folded_name_,
                                                 this->system_name_.in ());
}

PortableServer::POA_ptr
TAO_POA::create_POA_i (const char *adapter_name,
                       PortableServer::POAManager_ptr poa_manager,
                       const CORBA::PolicyList &policies,
                       CORBA::Environment &ACE_TRY_ENV)
{
  // If any of the policy objects specified are not valid for the ORB
  // implementation, if conflicting policy objects are specified, or
  // if any of the specified policy objects require prior
  // administrative action that has not been performed, an
  // InvalidPolicy exception is raised containing the index in the
  // policies parameter value of the first offending policy object.
  TAO_POA_Policies tao_policies;
  tao_policies.parse_policies (policies,
                               ACE_TRY_ENV);
  ACE_CHECK_RETURN (PortableServer::POA::_nil ());

  // If the poa_manager parameter is null, a new POAManager object is
  // created and associated with the new POA. Otherwise, the specified
  // POAManager object is associated with the new POA. The POAManager
  // object can be obtained using the attribute name the_POAManager.

  auto_ptr<TAO_POA_Manager> new_poa_manager_impl;
  TAO_POA_Manager *poa_manager_impl = 0;
  if (CORBA::is_nil (poa_manager))
    {
      ACE_NEW_THROW_EX (poa_manager_impl,
                        TAO_POA_Manager (*this->orb_core_.object_adapter ()),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (PortableServer::POA::_nil ());

      // Give ownership to auto pointer.
      auto_ptr<TAO_POA_Manager> auto_new_poa_manager_impl (poa_manager_impl);
      new_poa_manager_impl = auto_new_poa_manager_impl;
    }
  else
    {
      PortableServer::Servant servant = poa_manager->_servant ();
      if (servant == 0)
        {
          ACE_THROW_RETURN (CORBA::OBJ_ADAPTER (),
                            PortableServer::POA::_nil ());
        }

      void *ptr = servant->_downcast (servant->_interface_repository_id ());
      POA_PortableServer::POAManager *mgr = (POA_PortableServer::POAManager *) ptr;
      poa_manager_impl = ACE_dynamic_cast (TAO_POA_Manager *, mgr);
    }

  TAO_POA *poa = this->create_POA_i (adapter_name,
                                     *poa_manager_impl,
                                     tao_policies,
                                     ACE_TRY_ENV);
  ACE_CHECK_RETURN (PortableServer::POA::_nil ());

  // Give ownership of the new poa to the auto pointer.
  auto_ptr<TAO_POA> new_poa (poa);

  PortableServer::POA_var result = new_poa->_this (ACE_TRY_ENV);
  ACE_CHECK_RETURN (PortableServer::POA::_nil ());

  // Finally everything is fine.  Make sure to take ownership away
  // from the auto pointers.
  new_poa.release ();
  new_poa_manager_impl.release ();

  return result._retn ();
}

TAO_POA *
TAO_POA::create_POA_i (const TAO_POA::String &adapter_name,
                       TAO_POA_Manager &poa_manager,
                       const TAO_POA_Policies &policies,
                       CORBA::Environment &ACE_TRY_ENV)
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
      ACE_THROW_RETURN (PortableServer::POA::AdapterAlreadyExists (),
                        0);
    }

  //
  // Child was not found
  //

  // The specified policy objects are associated with the POA and used
  // to control its behavior. The policy objects are effectively
  // copied before this operation returns, so the application is free
  // to destroy them while the POA is in use. Policies are not
  // inherited from the parent POA.
  TAO_POA *poa = 0;
  ACE_NEW_THROW_EX (poa,
                    TAO_POA (adapter_name,
                             poa_manager,
                             policies,
                             this,
                             this->orb_core_.object_adapter ()->lock (),
                             this->orb_core_.object_adapter ()->thread_lock (),
                             this->orb_core_,
                             ACE_TRY_ENV),
                    CORBA::NO_MEMORY ());

  // Give ownership of the new map to the auto pointer.  Note, that it
  // is important for the auto pointer to take ownership before
  // checking for exception since we may need to delete the new map.
  auto_ptr<TAO_POA> new_poa (poa);

  // Check for exception in construction of the POA.
  ACE_CHECK_RETURN (0);

  // Add to children map
  result = this->children_.bind (adapter_name,
                                 new_poa.get ());
  if (result != 0)
    {
      ACE_THROW_RETURN (CORBA::OBJ_ADAPTER (),
                        0);
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

  // Everything is fine. Don't let the auto_ptr delete the
  // implementation.
  return new_poa.release ();
}

PortableServer::POA_ptr
TAO_POA::find_POA (const char *adapter_name,
                   CORBA::Boolean activate_it,
                   CORBA::Environment &ACE_TRY_ENV)
{
  // Lock access for the duration of this transaction.
  TAO_POA_GUARD_RETURN (ACE_Lock, monitor, this->lock (), 0);

  // A recursive thread lock without using a recursive thread
  // lock.  Non_Servant_Upcall has a magic constructor and
  // destructor.  We unlock the Object_Adapter lock for the
  // duration of the servant activator upcalls; reacquiring
  // once the upcalls complete.  Even though we are releasing
  // the lock, other threads will not be able to make progress
  // since <Object_Adapter::non_servant_upcall_in_progress_>
  // has been set.
  TAO_Object_Adapter::Non_Servant_Upcall non_servant_upcall (*this->orb_core_.object_adapter ());
  ACE_UNUSED_ARG (non_servant_upcall);

  TAO_POA *child = this->find_POA_i (adapter_name,
                                     activate_it,
                                     ACE_TRY_ENV);
  ACE_CHECK_RETURN (PortableServer::POA::_nil ());

  // Give ownership of the new poa to the auto pointer.
  auto_ptr<TAO_POA> new_child (child);

  PortableServer::POA_var result = new_child->_this (ACE_TRY_ENV);
  ACE_CHECK_RETURN (PortableServer::POA::_nil ());

  // Finally everything is fine.  Make sure to take ownership away
  // from the auto pointer.
  new_child.release ();

  return result._retn ();
}

TAO_POA *
TAO_POA::find_POA_i (const ACE_CString &child_name,
                     CORBA::Boolean activate_it,
                     CORBA::Environment &ACE_TRY_ENV)
{
  TAO_POA *child;
  int result = this->children_.find (child_name,
                                     child);

#if !defined (TAO_HAS_MINIMUM_CORBA)

  if (result != 0)
    {
      if (activate_it)
        {
          if (!CORBA::is_nil (this->adapter_activator_.in ()))
            {
              // Check the state of the POA Manager.
              this->check_poa_manager_state (ACE_TRY_ENV);
              ACE_CHECK_RETURN (0);

              PortableServer::POA_var self = this->_this (ACE_TRY_ENV);
              ACE_CHECK_RETURN (0);

              CORBA::Boolean success =
                this->adapter_activator_->unknown_adapter (self.in (),
                                                           child_name.c_str (),
                                                           ACE_TRY_ENV);
              ACE_CHECK_RETURN (0);

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
#endif /* !TAO_HAS_MINIMUM_CORBA */

  if (result == 0)
    {
      return child;
    }
  else
    {
      // Otherwise, the AdapterNonExistent exception is raised.
      ACE_THROW_RETURN (PortableServer::POA::AdapterNonExistent (),
                        0);
    }
}

void
TAO_POA::destroy_i (CORBA::Boolean etherealize_objects,
                    CORBA::Boolean wait_for_completion,
                    CORBA::Environment &ACE_TRY_ENV)
{
  // Is the <wait_for_completion> semantics for this thread correct?
  TAO_POA::check_for_valid_wait_for_completions (wait_for_completion,
                                                 ACE_TRY_ENV);
  ACE_CHECK;

  this->cleanup_in_progress_ = 1;

  // This operation destroys the POA and all descendant POAs. The POA
  // so destroyed (that is, the POA with its name) may be re-created
  // later in the same process. (This differs from the
  // POAManager::deactivate operation that does not allow a
  // re-creation of its associated POA in the same process.)

  // Remove POA from the parent
  if (this->parent_ != 0)
    {
      int result = this->parent_->delete_child (this->name_);
      if (result != 0)
        {
          ACE_THROW (CORBA::OBJ_ADAPTER ());
        }
    }

  // Remove all children POAs
  for (CHILDREN::iterator iterator = this->children_.begin ();
       iterator != this->children_.end ();
       ++iterator)
    {
      TAO_POA *child_poa = (*iterator).int_id_;
      child_poa->destroy_i (etherealize_objects,
                            wait_for_completion,
                            ACE_TRY_ENV);
      ACE_CHECK;
    }

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
                                  wait_for_completion,
                                  ACE_TRY_ENV);
  ACE_CHECK;

  // If there are no outstanding requests.
  if (this->outstanding_requests_ == 0)
    {
      // Commit suicide
      delete this;
    }
  else
    {
      // Mark that we are ready for destruction.
      this->waiting_destruction_ = 1;
    }
}

int
TAO_POA::delete_child (const TAO_POA::String &child)
{
  int result = 0;

  // If we are not closing down, we must remove this child from our
  // collection.
  if (!this->cleanup_in_progress_)
    {
      result = this->children_.unbind (child);
    }

  // If we are closing down, we are currently iterating over our
  // children and there is not need to remove this child from our
  // collection.

  return result;
}

#if !defined (TAO_HAS_MINIMUM_CORBA)

PortableServer::ServantManager_ptr
TAO_POA::get_servant_manager_i (CORBA::Environment &ACE_TRY_ENV)
{
  // This operation requires the USE_SERVANT_MANAGER policy; if not
  // present, the WrongPolicy exception is raised.
  if (this->policies ().request_processing () != PortableServer::USE_SERVANT_MANAGER)
    {
      ACE_THROW_RETURN (PortableServer::POA::WrongPolicy (),
                        PortableServer::ServantManager::_nil ());
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
                                CORBA::Environment &ACE_TRY_ENV)
{
  // This operation requires the USE_SERVANT_MANAGER policy; if not
  // present, the WrongPolicy exception is raised.
  if (this->policies ().request_processing () != PortableServer::USE_SERVANT_MANAGER)
    {
      ACE_THROW (PortableServer::POA::WrongPolicy ());
    }

  // This operation sets the default servant manager associated with
  // the POA.
  if (this->policies ().servant_retention () == PortableServer::RETAIN)
    {
      this->servant_activator_ = PortableServer::ServantActivator::_narrow (imgr,
                                                                            ACE_TRY_ENV);
      ACE_CHECK;

      if (CORBA::is_nil (this->servant_activator_.in ()))
        {
          ACE_THROW (PortableServer::POA::WrongPolicy ());
        }
    }
  else
    {
      this->servant_locator_ = PortableServer::ServantLocator::_narrow (imgr,
                                                                        ACE_TRY_ENV);
      ACE_CHECK;

      if (CORBA::is_nil (this->servant_locator_.in ()))
        {
          ACE_THROW (PortableServer::POA::WrongPolicy ());
        }
    }
}

PortableServer::Servant
TAO_POA::get_servant_i (CORBA::Environment &ACE_TRY_ENV)
{
  // This operation requires the USE_DEFAULT_SERVANT policy; if not
  // present, the WrongPolicy exception is raised.
  if (this->policies ().request_processing () != PortableServer::USE_DEFAULT_SERVANT)
    {
      ACE_THROW_RETURN (PortableServer::POA::WrongPolicy (),
                        0);
    }

  // This operation returns the default servant associated with the
  // POA.
  PortableServer::Servant result = this->default_servant_.in ();
  if (result != 0)
    {
      // The POA invokes _add_ref once on the Servant before returning
      // it. If the application uses reference counting, the caller of
      // get_servant is responsible for invoking _remove_ref once on
      // the returned Servant when it is finished with it. A
      // conforming caller need not invoke _remove_ref on the returned
      // Servant if the type of the Servant uses the default reference
      // counting inherited from ServantBase.
      result->_add_ref (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      return result;
    }
  else
    // If no servant has been associated with the POA, the NoServant
    // exception is raised.
    {
      ACE_THROW_RETURN (PortableServer::POA::NoServant (),
                        0);
    }
}

void
TAO_POA::set_servant_i (PortableServer::Servant servant,
                        CORBA::Environment &ACE_TRY_ENV)
{
  // This operation requires the USE_DEFAULT_SERVANT policy; if not
  // present, the WrongPolicy exception is raised.
  if (this->policies ().request_processing () != PortableServer::USE_DEFAULT_SERVANT)
    {
      ACE_THROW (PortableServer::POA::WrongPolicy ());
    }

  // This operation registers the specified servant with the POA as
  // the default servant. This servant will be used for all requests
  // for which no servant is found in the Active Object Map.
  this->default_servant_ = servant;

  // The implementation of set_servant will invoke _add_ref at least
  // once on the Servant argument before returning. When the POA no
  // longer needs the Servant, it will invoke _remove_ref on it the
  // same number of times.
  if (servant != 0)
    {
      servant->_add_ref (ACE_TRY_ENV);
      ACE_CHECK;

      // If we are a single threaded POA, set up the appropriate
      // locking in the servant.
      this->establish_servant_lock (servant);
    }
}

#endif /* TAO_HAS_MINIMUM_CORBA */

int
TAO_POA::is_servant_in_map (PortableServer::Servant servant)
{
  while (1)
    {
      int deactivated = 0;
      int servant_in_map =
        this->active_object_map ().is_servant_in_map (servant,
                                                      deactivated);

      if (!servant_in_map)
        {
          return 0;
        }
      else
        {
          if (deactivated)
            {
              if (TAO_debug_level > 0)
                ACE_DEBUG ((LM_DEBUG,
                            "(%t) TAO_POA::is_servant_in_map: waiting for servant to deactivate\n"));

              ++this->waiting_servant_deactivation_;

              if (this->orb_core_.object_adapter ()->enable_locking_)
                this->servant_deactivation_condition_.wait ();

              --this->waiting_servant_deactivation_;
            }
          else
            {
              return 1;
            }
        }
    }
}

int
TAO_POA::is_user_id_in_map (const PortableServer::ObjectId &id)
{
  while (1)
    {
      int deactivated = 0;
      int user_id_in_map =
        this->active_object_map ().is_user_id_in_map (id,
                                                      deactivated);

      if (!user_id_in_map)
        {
          return 0;
        }
      else
        {
          if (deactivated)
            {
              if (TAO_debug_level > 0)
                ACE_DEBUG ((LM_DEBUG,
                            "(%t) TAO_POA::is_user_id_in_map: waiting for servant to deactivate\n"));

              ++this->waiting_servant_deactivation_;

              if (this->orb_core_.object_adapter ()->enable_locking_)
                this->servant_deactivation_condition_.wait ();

              --this->waiting_servant_deactivation_;
            }
          else
            {
              return 1;
            }
        }
    }
}

PortableServer::ObjectId *
TAO_POA::activate_object_i (PortableServer::Servant servant,
                            CORBA::Environment &ACE_TRY_ENV)
{
  // This operation requires the SYSTEM_ID and RETAIN policy; if not
  // present, the WrongPolicy exception is raised.
  if (!(this->policies ().id_assignment () == PortableServer::SYSTEM_ID &&
        this->policies ().servant_retention () == PortableServer::RETAIN))
    {
      ACE_THROW_RETURN (PortableServer::POA::WrongPolicy (),
                        0);
    }

  // If the POA has the UNIQUE_ID policy and the specified servant is
  // already in the Active Object Map, the ServantAlreadyActive
  // exception is raised.
  if (this->policies ().id_uniqueness () == PortableServer::UNIQUE_ID &&
      this->is_servant_in_map (servant))
    {
      ACE_THROW_RETURN (PortableServer::POA::ServantAlreadyActive (),
                        0);
    }

  // Otherwise, the activate_object operation generates an Object Id
  // and enters the Object Id and the specified servant in the Active
  // Object Map. The Object Id is returned.
  PortableServer::ObjectId_var user_id;
  if (this->active_object_map ().bind_using_system_id_returning_user_id (servant,
                                                                         user_id.out ()) != 0)
    {
      ACE_THROW_RETURN (CORBA::OBJ_ADAPTER (),
                        0);
    }

  //
  // Everything is finally ok
  //

  // The implementation of activate_object will invoke _add_ref at
  // least once on the Servant argument before returning. When the POA
  // no longer needs the Servant, it will invoke _remove_ref on it the
  // same number of times.
  servant->_add_ref (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  // If we are a single threaded POA, set up the appropriate locking
  // in the servant.
  this->establish_servant_lock (servant);

  return user_id._retn ();
}

void
TAO_POA::activate_object_with_id_i (const PortableServer::ObjectId &id,
                                    PortableServer::Servant servant,
                                    CORBA::Environment &ACE_TRY_ENV)
{
  // This operation requires the RETAIN policy; if not present, the
  // WrongPolicy exception is raised.
  if (this->policies ().servant_retention () != PortableServer::RETAIN)
    {
      ACE_THROW (PortableServer::POA::WrongPolicy ());
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
      ACE_THROW (CORBA::BAD_PARAM ());
    }

  // If the CORBA object denoted by the Object Id value is already
  // active in this POA (there is a servant bound to it in the Active
  // Object Map), the ObjectAlreadyActive exception is raised.
  if (is_user_id_in_map (id))
    {
      ACE_THROW (PortableServer::POA::ObjectAlreadyActive ());
    }

  // If the POA has the UNIQUE_ID policy and the servant is already in
  // the Active Object Map, the ServantAlreadyActive exception is
  // raised.
  if (this->policies ().id_uniqueness () == PortableServer::UNIQUE_ID &&
      this->is_servant_in_map (servant))
    {
      ACE_THROW (PortableServer::POA::ServantAlreadyActive ());
    }

  // Otherwise, the activate_object_with_id operation enters an
  // association between the specified Object Id and the specified
  // servant in the Active Object Map.
  if (this->active_object_map ().bind_using_user_id (servant, id) != 0)
    {
      ACE_THROW (CORBA::OBJ_ADAPTER ());
    }

  //
  // Everything is finally ok
  //

  // The implementation of activate_object_with_id will invoke
  // _add_ref at least once on the Servant argument before
  // returning. When the POA no longer needs the Servant, it will
  // invoke _remove_ref on it the same number of times.
  servant->_add_ref (ACE_TRY_ENV);
  ACE_CHECK;

  // If we are a single threaded POA, set up the appropriate locking
  // in the servant.
  this->establish_servant_lock (servant);
}

void
TAO_POA::deactivate_all_objects_i (CORBA::Boolean etherealize_objects,
                                   CORBA::Boolean wait_for_completion,
                                   CORBA::Environment &ACE_TRY_ENV)
{
  this->deactivate_all_objects_i (etherealize_objects,
                                  ACE_TRY_ENV);
  ACE_CHECK;

  this->wait_for_completions (wait_for_completion,
                              ACE_TRY_ENV);
  ACE_CHECK;
}

void
TAO_POA::wait_for_completions (CORBA::Boolean wait_for_completion,
                               CORBA::Environment &ACE_TRY_ENV)
{
  while (this->orb_core_.object_adapter ()->enable_locking_ &&
         wait_for_completion &&
         this->outstanding_requests_ > 0)
    {
      this->wait_for_completion_pending_ = 1;

      int result = this->outstanding_requests_condition_.wait ();
      if (result == -1)
        {
          ACE_THROW (CORBA::OBJ_ADAPTER ());
        }
    }
}

/* static */
void
TAO_POA::check_for_valid_wait_for_completions (CORBA::Boolean wait_for_completion,
                                               CORBA::Environment &ACE_TRY_ENV)
{
  if (wait_for_completion)
    {
      TAO_POA_Current_Impl *poa_current_impl =
        TAO_TSS_RESOURCES::instance ()->poa_current_impl_;

      // This thread cannot currently be in an upcall.
      if (poa_current_impl != 0)
        {
          // CORBA 2.3 specifies which minor code corresponds to this
          // particular problem.
          ACE_THROW (CORBA::BAD_INV_ORDER (3, CORBA::COMPLETED_NO));
        }
    }
}

void
TAO_POA::deactivate_all_objects_i (CORBA::Boolean etherealize_objects,
                                   CORBA::Environment &ACE_TRY_ENV)
{
  this->etherealize_objects_ = etherealize_objects;

  // This operation is a no-op for the non-RETAIN policy.
  if (this->policies ().servant_retention () != PortableServer::RETAIN)
    {
      return;
    }

  // If the etherealize_objects parameter is TRUE, the POA has the
  // RETAIN policy, and a servant manager is registered with the POA,
  // the etherealize operation on the servant manager will be called
  // for each active object in the Active Object Map. The apparent
  // destruction of the POA occurs before any calls to etherealize are
  // made.  Thus, for example, an etherealize method that attempts to
  // invoke operations on the POA will receive the OBJECT_NOT_EXIST
  // exception.

  // We must copy the user ids into a separate place since we cannot
  // remove entries while iterating through the map.
  ACE_Array<PortableServer::ObjectId> ids (this->active_object_map ().current_size ());

  size_t counter = 0;
  TAO_Active_Object_Map::user_id_map::iterator end
    = this->active_object_map ().user_id_map_->end ();

  for (TAO_Active_Object_Map::user_id_map::iterator iter
         = this->active_object_map ().user_id_map_->begin ();
       iter != end;
       ++iter)
    {
      TAO_Active_Object_Map::user_id_map::value_type map_pair = *iter;
      TAO_Active_Object_Map::Map_Entry *active_object_map_entry = map_pair.second ();

      if (!active_object_map_entry->deactivated_)
        {
          ids[counter] = active_object_map_entry->user_id_;
          ++counter;
        }
    }

  for (size_t i = 0;
       i < counter;
       ++i)
    {
      this->deactivate_object_i (ids[i],
                                 ACE_TRY_ENV);
      ACE_CHECK;
    }
}

void
TAO_POA::deactivate_object_i (const PortableServer::ObjectId &id,
                              CORBA::Environment &ACE_TRY_ENV)
{
  // This operation requires the RETAIN policy; if not present, the
  // WrongPolicy exception is raised.
  if (this->policies ().servant_retention () != PortableServer::RETAIN)
    {
      ACE_THROW (PortableServer::POA::WrongPolicy ());
    }

  TAO_Active_Object_Map::Map_Entry *active_object_map_entry = 0;
  int result = this->active_object_map ().find_servant_and_system_id_using_user_id (id,
                                                                                    active_object_map_entry);

  // If there is no active object associated with the specified Object
  // Id, the operation raises an ObjectNotActive exception.
  if (result != 0)
    {
      ACE_THROW (PortableServer::POA::ObjectNotActive ());
    }

  // Decrement the reference count.
  CORBA::UShort new_count = --active_object_map_entry->reference_count_;

  if (new_count == 0)
    {
      this->cleanup_servant (active_object_map_entry,
                             ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      // It should be noted that there may be a period of time between
      // an object's deactivation and the etherealization (during
      // which outstanding requests are being processed) in which
      // arriving requests on that object should not be passed to its
      // servant. During this period, requests targeted for such an
      // object act as if the POA were in holding state until
      // etherealize completes. If etherealize is called as a
      // consequence of a deactivate call with a etherealize_objects
      // parameter of TRUE, incoming requests are rejected.

      // Else mark entry as closed...
      active_object_map_entry->deactivated_ = 1;
    }
}

void
TAO_POA::cleanup_servant (TAO_Active_Object_Map::Map_Entry *active_object_map_entry,
                          CORBA::Environment &ACE_TRY_ENV)
{
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

  // If the POA has no ServantActivator associated with it, the POA
  // implementation calls _remove_ref when all operation invocations
  // have completed. If there is a ServantActivator, the Servant is
  // consumed by the call to ServantActivator::etherealize instead.

  // First check for a non-zero servant.
  if (active_object_map_entry->servant_)
    {
      // If we are a single threaded POA, teardown the appropriate
      // locking in the servant.
      //
      // Note that teardown of the servant lock must happen before the
      // _remove_ref() or etherealize() calls since they might end up
      // deleting the servant.
      //
      this->teardown_servant_lock (active_object_map_entry->servant_);

#if !defined (TAO_HAS_MINIMUM_CORBA)

      if (this->etherealize_objects_ &&
          this->policies ().request_processing () == PortableServer::USE_SERVANT_MANAGER &&
          !CORBA::is_nil (this->servant_activator_.in ()))
        {
          PortableServer::POA_var self = this->_this (ACE_TRY_ENV);
          ACE_CHECK;

          CORBA::Boolean remaining_activations =
            this->active_object_map ().remaining_activations (active_object_map_entry->servant_);

          // A recursive thread lock without using a recursive thread
          // lock.  Non_Servant_Upcall has a magic constructor and
          // destructor.  We unlock the Object_Adapter lock for the
          // duration of the servant activator upcalls; reacquiring
          // once the upcalls complete.  Even though we are releasing
          // the lock, other threads will not be able to make progress
          // since <Object_Adapter::non_servant_upcall_in_progress_>
          // has been set.
          TAO_Object_Adapter::Non_Servant_Upcall non_servant_upcall (*this->orb_core_.object_adapter ());
          ACE_UNUSED_ARG (non_servant_upcall);

          // If the cleanup_in_progress parameter is TRUE, the reason
          // for the etherealize operation is that either the
          // deactivate or destroy operation was called with an
          // etherealize_objects parameter of TRUE. If the parameter
          // is FALSE, the etherealize operation is called for other
          // reasons.
          this->servant_activator_->etherealize (active_object_map_entry->user_id_,
                                                 self.in (),
                                                 active_object_map_entry->servant_,
                                                 this->cleanup_in_progress_,
                                                 remaining_activations,
                                                 ACE_TRY_ENV);
          ACE_CHECK;
        }
      else

#endif /* TAO_HAS_MINIMUM_CORBA */

        {
          active_object_map_entry->servant_->_remove_ref (ACE_TRY_ENV);
          ACE_CHECK;
        }
    }

  // This operation causes the association of the Object Id specified
  // by the oid parameter and its servant to be removed from the
  // Active Object Map.
  int result = this->active_object_map ().unbind_using_user_id (active_object_map_entry->user_id_);

  if (result != 0)
    {
      ACE_THROW (CORBA::OBJ_ADAPTER ());
    }
}

void
TAO_POA::check_poa_manager_state (CORBA::Environment &ACE_TRY_ENV)
{
  PortableServer::POAManager::State state = this->poa_manager_.get_state_i ();

  if (state == PortableServer::POAManager::ACTIVE)
    {
      // When a POA manager is in the active state, the associated
      // POAs will receive and start processing requests (assuming
      // that appropriate thread resources are available).
      return;
    }

  if (state == PortableServer::POAManager::DISCARDING)
    {
      // When a POA manager is in the discarding state, the associated
      // POAs will discard all incoming requests (whose processing has
      // not yet begun). When a request is discarded, the TRANSIENT
      // system exception must be returned to the client-side to
      // indicate that the request should be re-issued. (Of course, an
      // ORB may always reject a request for other reasons and raise
      // some other system exception.)
      ACE_THROW (CORBA::TRANSIENT (
        CORBA_SystemException::_tao_minor_code (
          TAO_POA_DISCARDING,
          0),
        CORBA::COMPLETED_NO));
    }

  if (state == PortableServer::POAManager::HOLDING)
    {
      // When a POA manager is in the holding state, the associated
      // POAs will queue incoming requests. The number of requests
      // that can be queued is an implementation limit. If this limit
      // is reached, the POAs may discard requests and return the
      // TRANSIENT system exception to the client to indicate that the
      // client should reissue the request. (Of course, an ORB may
      // always reject a request for other reasons and raise some
      // other system exception.)

      // Since there is no queuing in TAO, we immediately raise a
      // TRANSIENT exception.
      ACE_THROW (CORBA::TRANSIENT (
        CORBA_SystemException::_tao_minor_code (
          TAO_POA_HOLDING,
          0),
        CORBA::COMPLETED_NO));
    }

  if (state == PortableServer::POAManager::INACTIVE)
    {
      // The inactive state is entered when the associated POAs are to
      // be shut down. Unlike the discarding state, the inactive state
      // is not a temporary state. When a POA manager is in the
      // inactive state, the associated POAs will reject new
      // requests. The rejection mechanism used is specific to the
      // vendor. The GIOP location forwarding mechanism and
      // CloseConnection message are examples of mechanisms that could
      // be used to indicate the rejection. If the client is
      // co-resident in the same process, the ORB could raise the
      // OBJ_ADAPTER exception to indicate that the object
      // implementation is unavailable.
      ACE_THROW (CORBA::OBJ_ADAPTER ());
    }
}

CORBA::Object_ptr
TAO_POA::create_reference_i (const char *intf,
                             CORBA::Environment &ACE_TRY_ENV)
{
  // This operation requires the SYSTEM_ID policy; if not present, the
  // WrongPolicy exception is raised.
  if (this->policies ().id_assignment () != PortableServer::SYSTEM_ID)
    {
      ACE_THROW_RETURN (PortableServer::POA::WrongPolicy (),
                        CORBA::Object::_nil ());
    }

  // This operation creates an object reference that encapsulates a
  // POA-generated Object Id value and the specified interface
  // repository id. This operation does not cause an activation to
  // take place. The resulting reference may be passed to clients, so
  // that subsequent requests on those references will cause the
  // appropriate servant manager to be invoked, if one is
  // available. The generated Object Id value may be obtained by
  // invoking POA::reference_to_id with the created reference.
  PortableServer::ObjectId_var system_id;
  if (this->active_object_map ().bind_using_system_id_returning_system_id (0,
                                                                           system_id.out ()) != 0)
    {
      ACE_THROW_RETURN (CORBA::OBJ_ADAPTER (),
                        CORBA::Object::_nil ());
    }

  // Create object key.
  TAO_ObjectKey_var key = this->create_object_key (system_id.in ());

  // Ask the ORB to create you a reference
  return this->orb_core_.orb ()->key_to_object (key.in (),
                                                intf,
                                                0,
                                                1,
                                                ACE_TRY_ENV);
}

CORBA::Object_ptr
TAO_POA::create_reference_with_id_i (const PortableServer::ObjectId &user_id,
                                     const char *intf,
                                     CORBA::Environment &ACE_TRY_ENV)
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
      !this->is_poa_generated_id (user_id))
    {
      ACE_THROW_RETURN (CORBA::BAD_PARAM (),
                        CORBA::Object::_nil ());
    }

  // This operation creates an object reference that encapsulates the
  // specified Object Id and interface repository Id values. This
  // operation does not cause an activation to take place.  The
  // resulting reference may be passed to clients, so that subsequent
  // requests on those references will cause the object to be
  // activated if necessary, or the default servant used, depending on
  // the applicable policies.
  PortableServer::Servant servant = 0;
  PortableServer::ObjectId_var system_id;

  // @@ We need something that can find the system id using appropriate strategy,
  // at the same time, return the servant if one is available.  Before we have that
  // function, <create_reference_with_id_i> basically generates broken collocated
  // object when DIRECT collocation strategy is used.

  if (this->active_object_map ().find_system_id_using_user_id (user_id,
                                                               system_id.out ()) != 0)
    {
      ACE_THROW_RETURN (CORBA::OBJ_ADAPTER (),
                        CORBA::Object::_nil ());
    }

  // Create object key.
  TAO_ObjectKey_var key = this->create_object_key (system_id.in ());

  // Ask the ORB to create you a reference
  return this->orb_core_.orb ()->key_to_object (key.in (),
                                                intf,
                                                servant,
                                                1,
                                                ACE_TRY_ENV);
}

PortableServer::ObjectId *
TAO_POA::servant_to_id_i (PortableServer::Servant servant,
                          CORBA::Environment &ACE_TRY_ENV)
{
  // This operation requires the RETAIN and either the UNIQUE_ID or
  // IMPLICIT_ACTIVATION policies; if not present, the WrongPolicy
  // exception is raised.
  if (!(this->policies ().servant_retention () == PortableServer::RETAIN
        && (this->policies ().id_uniqueness () == PortableServer::UNIQUE_ID
            || this->policies ().implicit_activation () == PortableServer::IMPLICIT_ACTIVATION)))
    {
      ACE_THROW_RETURN (PortableServer::POA::WrongPolicy (),
                        0);
    }

  // This operation has three possible behaviors.

  // If the POA has the UNIQUE_ID policy and the specified servant is
  // active, the Object Id associated with that servant is returned.
  PortableServer::ObjectId_var user_id;
  if (this->policies ().id_uniqueness () == PortableServer::UNIQUE_ID &&
      this->active_object_map ().find_user_id_using_servant (servant,
                                                             user_id.out ()) != -1)
    {
      return user_id._retn ();
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
      PortableServer::ObjectId_var user_id;
      if (this->active_object_map ().bind_using_system_id_returning_user_id (servant,
                                                                             user_id.out ()) != 0)
        {
          ACE_THROW_RETURN (CORBA::OBJ_ADAPTER (),
                            0);
        }

      //
      // Everything is finally ok
      //

      // If this operation causes the object to be activated, _add_ref
      // is invoked at least once on the Servant argument before
      // returning. Otherwise, the POA does not increment or decrement
      // the reference count of the Servant passed to this function.
      servant->_add_ref (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      // If we are a single threaded POA, set up the appropriate
      // locking in the servant.
      this->establish_servant_lock (servant);

      return user_id._retn ();
    }

  // Otherwise, the ServantNotActive exception is raised.
  ACE_THROW_RETURN (PortableServer::POA::ServantNotActive (),
                    0);
}

PortableServer::ObjectId *
TAO_POA::servant_to_system_id_i (PortableServer::Servant servant,
                                 CORBA::Environment &ACE_TRY_ENV)
{
  // This operation requires the RETAIN and either the UNIQUE_ID or
  // IMPLICIT_ACTIVATION policies; if not present, the WrongPolicy
  // exception is raised.
  if (!(this->policies ().servant_retention () == PortableServer::RETAIN
        && (this->policies ().id_uniqueness () == PortableServer::UNIQUE_ID
            || this->policies ().implicit_activation () == PortableServer::IMPLICIT_ACTIVATION)))
    {
      ACE_THROW_RETURN (PortableServer::POA::WrongPolicy (),
                        0);
    }

  // This operation has three possible behaviors.

  // If the POA has the UNIQUE_ID policy and the specified servant is
  // active, the Object Id associated with that servant is returned.
  PortableServer::ObjectId_var system_id;
  if (this->policies ().id_uniqueness () == PortableServer::UNIQUE_ID &&
      this->active_object_map ().find_system_id_using_servant (servant,
                                                               system_id.out ()) != -1)
    {
      return system_id._retn ();
    }

  // If the POA has the IMPLICIT_ACTIVATION policy and either the POA
  // has the MULTIPLE_ID policy or the specified servant is not
  // active, the servant is activated using a POA-generated Object Id
  // and the Interface Id associated with the servant, and that Object
  // Id is returned.
  if (this->policies ().implicit_activation () == PortableServer::IMPLICIT_ACTIVATION)
    {
      // If we reach here, then we either have the MULTIPLE_ID policy
      // or we xhave the UNIQUE_ID policy and we are not in the active
      // object map.
      PortableServer::ObjectId_var system_id;
      if (this->active_object_map ().bind_using_system_id_returning_system_id (servant,
                                                                               system_id.out ()) != 0)
        {
          ACE_THROW_RETURN (CORBA::OBJ_ADAPTER (),
                            0);
        }

      //
      // Everything is finally ok
      //

      // If this operation causes the object to be activated, _add_ref
      // is invoked at least once on the Servant argument before
      // returning. Otherwise, the POA does not increment or decrement
      // the reference count of the Servant passed to this function.
      servant->_add_ref (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      // If we are a single threaded POA, set up the appropriate
      // locking in the servant.
      this->establish_servant_lock (servant);

      return system_id._retn ();
    }

  // Otherwise, the ServantNotActive exception is raised.
  ACE_THROW_RETURN (PortableServer::POA::ServantNotActive (),
                    0);
}

CORBA::Object_ptr
TAO_POA::servant_to_reference (PortableServer::Servant servant,
                               CORBA::Environment &ACE_TRY_ENV)
{
  // Note: The allocation of an Object Id value and installation in
  // the Active Object Map caused by implicit activation may actually
  // be deferred until an attempt is made to externalize the
  // reference. The real requirement here is that a reference is
  // produced that will behave appropriately (that is, yield a
  // consistent Object Id value when asked politely).
  PortableServer::ObjectId_var id = this->servant_to_system_id (servant,
                                                                ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  // Create object key.
  TAO_ObjectKey_var key = this->create_object_key (id.in ());

  // Ask the ORB to create you a reference
  return this->orb_core_.orb ()->key_to_object (key.in (),
                                                servant->_interface_repository_id (),
                                                servant,
                                                1,
                                                ACE_TRY_ENV);
}

PortableServer::Servant
TAO_POA::reference_to_servant (CORBA::Object_ptr reference,
                               CORBA::Environment &ACE_TRY_ENV)
{
  // This operation requires the RETAIN policy or the
  // USE_DEFAULT_SERVANT policy. If neither policy is present, the
  // WrongPolicy exception is raised.
  if (!(this->policies ().servant_retention () == PortableServer::RETAIN
        || this->policies ().request_processing () == PortableServer::USE_DEFAULT_SERVANT))
    {
      ACE_THROW_RETURN (PortableServer::POA::WrongPolicy (),
                        0);
    }

  // If the POA has the RETAIN policy and the specified object is
  // present in the Active Object Map, this operation returns the
  // servant associated with that object in the Active Object Map.
  if (this->policies ().servant_retention () == PortableServer::RETAIN)
    {
      TAO_ObjectKey_var key = reference->_key (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      // If the object reference was not created by this POA, the
      // WrongAdapter exception is raised.
      PortableServer::ObjectId system_id;
      TAO_Object_Adapter::poa_name poa_system_name;
      CORBA::Boolean is_root = 0;
      CORBA::Boolean is_persistent = 0;
      CORBA::Boolean is_system_id = 0;
      TAO_Temporary_Creation_Time poa_creation_time;

      int result = this->parse_key (key.in (),
                                    poa_system_name,
                                    system_id,
                                    is_root,
                                    is_persistent,
                                    is_system_id,
                                    poa_creation_time);
      if (result != 0 ||
          !this->root () &&
          poa_system_name != this->system_name () ||
          is_root != this->root () ||
          is_persistent != this->persistent () ||
          is_system_id != this->system_id () ||
          !this->persistent () &&
          poa_creation_time != this->creation_time_)
        {
          ACE_THROW_RETURN (PortableServer::POA::WrongAdapter (),
                            0);
        }

      // Lock access for the duration of this transaction.
      TAO_POA_GUARD_RETURN (ACE_Lock, monitor, this->lock (), 0);

      // Find user id from system id.
      PortableServer::ObjectId user_id;
      if (this->active_object_map ().find_user_id_using_system_id (system_id,
                                                                   user_id) != 0)
        {
          ACE_THROW_RETURN (CORBA::OBJ_ADAPTER (),
                            0);
        }

      // This operation returns the active servant associated with the
      // specified system Object Id value.  If the Object Id value is
      // not active in the POA, an ObjectNotActive exception is
      // raised.
      PortableServer::Servant servant = 0;
      TAO_Active_Object_Map::Map_Entry *entry = 0;

      if (this->active_object_map ().find_servant_using_system_id_and_user_id (system_id,
                                                                               user_id,
                                                                               servant,
                                                                               entry) != -1)
        {
          // The POA invokes _add_ref once on the Servant before
          // returning it. If the application uses reference counting,
          // the caller of reference_to_servant is responsible for
          // invoking _remove_ref once on the returned Servant when it
          // is finished with it.  A conforming caller need not invoke
          // _remove_ref on the returned Servant if the type of the
          // Servant uses the default reference counting inherited
          // from ServantBase.
          servant->_add_ref (ACE_TRY_ENV);
          ACE_CHECK_RETURN (0);

          return servant;
        }
      else
        // Otherwise, the ObjectNotActive exception is raised.
        {
          ACE_THROW_RETURN (PortableServer::POA::ObjectNotActive (),
                            0);
        }
    }

#if !defined (TAO_HAS_MINIMUM_CORBA)

  // Otherwise, if the POA has the USE_DEFAULT_SERVANT policy and a
  // default servant has been registered with the POA, this operation
  // returns the default servant.
  if (this->policies ().request_processing () == PortableServer::USE_DEFAULT_SERVANT)
    {
      // Lock access for the duration of this transaction.
      TAO_POA_GUARD_RETURN (ACE_Lock, monitor, this->lock (), 0);

      PortableServer::Servant result = this->default_servant_.in ();
      if (result != 0)
        {
          // The POA invokes _add_ref once on the Servant before
          // returning it. If the application uses reference counting,
          // the caller of reference_to_servant is responsible for
          // invoking _remove_ref once on the returned Servant when it
          // is finished with it.  A conforming caller need not invoke
          // _remove_ref on the returned Servant if the type of the
          // Servant uses the default reference counting inherited
          // from ServantBase.
          result->_add_ref (ACE_TRY_ENV);
          ACE_CHECK_RETURN (0);

          return result;
        }
      else
        // Otherwise, the ObjectNotActive exception is raised.
        {
          ACE_THROW_RETURN (PortableServer::POA::ObjectNotActive (),
                            0);
        }
    }

#endif /* TAO_HAS_MINIMUM_CORBA */

  // Not reached
  return 0;
}

PortableServer::ObjectId *
TAO_POA::reference_to_id (CORBA::Object_ptr reference,
                          CORBA::Environment &ACE_TRY_ENV)
{
  // The WrongPolicy exception is declared to allow future extensions.

  // This operation returns the Object Id value encapsulated by the
  // specified reference.

  // This operation is valid only if the reference was created by the
  // POA on which the operation is being performed.  If the object
  // reference was not created by this POA, the WrongAdapter exception
  // is raised.
  TAO_ObjectKey_var key = reference->_key (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  PortableServer::ObjectId system_id;
  TAO_Object_Adapter::poa_name poa_system_name;
  CORBA::Boolean is_root = 0;
  CORBA::Boolean is_persistent = 0;
  CORBA::Boolean is_system_id = 0;
  TAO_Temporary_Creation_Time poa_creation_time;

  int result = this->parse_key (key.in (),
                                poa_system_name,
                                system_id,
                                is_root,
                                is_persistent,
                                is_system_id,
                                poa_creation_time);
  if (result != 0 ||
      !this->root () &&
      poa_system_name != this->system_name () ||
      is_root != this->root () ||
      is_persistent != this->persistent () ||
      is_system_id != this->system_id () ||
      !this->persistent () &&
      poa_creation_time != this->creation_time_)
    {
      ACE_THROW_RETURN (PortableServer::POA::WrongAdapter (),
                        0);
    }

  // Lock access for the duration of this transaction.
  TAO_POA_GUARD_RETURN (ACE_Lock, monitor, this->lock (), 0);

  // The object denoted by the reference does not have to be active
  // for this operation to succeed.
  PortableServer::ObjectId_var user_id;
  if (this->active_object_map ().find_user_id_using_system_id (system_id,
                                                               user_id.out ()) != 0)
    {
      ACE_THROW_RETURN (CORBA::OBJ_ADAPTER (),
                        0);
    }

  return user_id._retn ();
}

PortableServer::Servant
TAO_POA::id_to_servant_i (const PortableServer::ObjectId &id,
                          CORBA::Environment &ACE_TRY_ENV)
{
  // This operation requires the RETAIN policy; if not present, the
  // WrongPolicy exception is raised.
  if (this->policies ().servant_retention () != PortableServer::RETAIN)
    {
      ACE_THROW_RETURN (PortableServer::POA::WrongPolicy (),
                        0);
    }

  // This operation returns the active servant associated with the
  // specified Object Id value.  If the Object Id value is not active
  // in the POA, an ObjectNotActive exception is raised.
  PortableServer::Servant servant = 0;
  if (this->active_object_map ().find_servant_using_user_id (id,
                                                             servant) != -1)
    {
      // The POA invokes _add_ref once on the Servant before returning
      // it. If the application uses reference counting, the caller of
      // id_to_servant is responsible for invoking _remove_ref once on
      // the returned Servant when it is finished with it. A
      // conforming caller need not invoke _remove_ref on the returned
      // Servant if the type of the Servant uses the default reference
      // counting inherited from ServantBase.
      servant->_add_ref (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      return servant;
    }
  else
    // Otherwise, the ObjectNotActive exception is raised.
    {
      ACE_THROW_RETURN (PortableServer::POA::ObjectNotActive (),
                        0);
    }
}

CORBA::Object_ptr
TAO_POA::id_to_reference_i (const PortableServer::ObjectId &id,
                            CORBA::Environment &ACE_TRY_ENV)
{
  // This operation requires the RETAIN policy; if not present, the
  // WrongPolicy exception is raised.
  if (this->policies ().servant_retention () != PortableServer::RETAIN)
    {
      ACE_THROW_RETURN (PortableServer::POA::WrongPolicy (),
                        CORBA::Object::_nil ());
    }

  // If an object with the specified Object Id value is currently
  // active, a reference encapsulating the information used to
  // activate the object is returned.
  PortableServer::ObjectId_var system_id;
  PortableServer::Servant servant;
  if (this->active_object_map ().find_servant_and_system_id_using_user_id (id,
                                                                           servant,
                                                                           system_id.out ()) == 0)
    {
      // Create object key.
      TAO_ObjectKey_var key = this->create_object_key (system_id.in ());

      // Ask the ORB to create you a reference
      return this->orb_core_.orb ()->key_to_object (key.in (),
                                                    servant->_interface_repository_id (),
                                                    servant,
                                                    1,
                                                    ACE_TRY_ENV);
    }
  else
    // If the Object Id value is not active in the POA, an
    // ObjectNotActive exception is raised.
    {
      ACE_THROW_RETURN (PortableServer::POA::ObjectNotActive (),
                        CORBA::Object::_nil ());
    }
}

#if !defined (TAO_HAS_MINIMUM_CORBA)

void
TAO_POA::forward_object_i (const PortableServer::ObjectId &oid,
                           CORBA::Object_ptr forward_to,
                           CORBA::Environment &ACE_TRY_ENV)
{
  // First, deactivate the object
  this->deactivate_object_i (oid,
                             ACE_TRY_ENV);
  ACE_CHECK;

  // Create a forwarding servant
  TAO_Forwarding_Servant *forwarding_servant = 0;
  ACE_NEW_THROW_EX (forwarding_servant,
                    TAO_Forwarding_Servant (forward_to,
                                            forward_to->_interface_repository_id ()),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  // Give ownership to the auto pointer.
  auto_ptr<TAO_Forwarding_Servant> new_forwarding_servant (forwarding_servant);

  // Register the forwarding servant with the same object Id.
  this->activate_object_with_id_i (oid,
                                   forwarding_servant,
                                   ACE_TRY_ENV);
  ACE_CHECK;

  // Finally everything is fine.  Make sure to take ownership away
  // from the auto pointer.
  new_forwarding_servant.release ();
}

#endif /* TAO_HAS_MINIMUM_CORBA */

TAO_SERVANT_LOCATION
TAO_POA::locate_servant_i (const PortableServer::ObjectId &system_id,
                           PortableServer::Servant &servant,
                           CORBA::Environment &ACE_TRY_ENV)
{
  // If the POA has the RETAIN policy, the POA looks in the Active
  // Object Map to find if there is a servant associated with the
  // Object Id value from the request. If such a servant exists,
  // return TAO_SERVANT_FOUND.
  if (this->policies ().servant_retention () == PortableServer::RETAIN)
    {
      // Find user id from system id.
      PortableServer::ObjectId user_id;
      if (this->active_object_map ().find_user_id_using_system_id (system_id,
                                                                   user_id) != 0)
        {
          ACE_THROW_RETURN (CORBA::OBJ_ADAPTER (),
                            TAO_SERVANT_NOT_FOUND);
        }

      TAO_Active_Object_Map::Map_Entry *entry = 0;
      int result = this->active_object_map ().find_servant_using_system_id_and_user_id (system_id,
                                                                                        user_id,
                                                                                        servant,
                                                                                        entry);
      if (result == 0)
        {
          // Success
          return TAO_SERVANT_FOUND;
        }
    }

  // If the POA has the NON_RETAIN policy or has the RETAIN policy but
  // didn't find a servant in the Active Object Map, the POA takes the
  // following actions:

  // If the USE_ACTIVE_OBJECT_MAP_ONLY policy is in effect, the POA raises
  // the OBJECT_NOT_EXIST system exception.
  if (this->policies ().request_processing () == PortableServer::USE_ACTIVE_OBJECT_MAP_ONLY)
    {
      return TAO_SERVANT_NOT_FOUND;
    }

#if !defined (TAO_HAS_MINIMUM_CORBA)

  // If the POA has the USE_DEFAULT_SERVANT policy, a default servant
  // has been associated with the POA, return TAO_DEFAULT_SERVANT. If
  // no servant has been associated with the POA, return
  // TAO_SERVANT_NOT_FOUND.
  if (this->policies ().request_processing () == PortableServer::USE_DEFAULT_SERVANT)
    {
      if (this->default_servant_.in () == 0)
        {
          return TAO_SERVANT_NOT_FOUND;
        }
      else
        {
          // Success
          return TAO_DEFAULT_SERVANT;
        }
    }

  // If the POA has the USE_SERVANT_MANAGER policy, a servant manager
  // has been associated with the POA, return
  // TAO_SERVANT_MANAGER. If no servant manager has been
  // associated with the POA, return TAO_SERVANT_NOT_FOUND.
  if (this->policies ().request_processing () == PortableServer::USE_SERVANT_MANAGER)
    {
      if (CORBA::is_nil (this->servant_activator_.in ()) &&
          CORBA::is_nil (this->servant_locator_.in ()))
        {
          return TAO_SERVANT_NOT_FOUND;
        }
      else
        {
          // Success
          return TAO_SERVANT_MANAGER;
        }
    }

#endif /* TAO_HAS_MINIMUM_CORBA */

  // Failure
  return TAO_SERVANT_NOT_FOUND;
}

PortableServer::Servant
TAO_POA::locate_servant_i (const char *operation,
                           const PortableServer::ObjectId &system_id,
                           TAO_Object_Adapter::Servant_Upcall &servant_upcall,
                           TAO_POA_Current_Impl &poa_current_impl,
                           CORBA::Environment &ACE_TRY_ENV)
{
  if (this->active_object_map ().find_user_id_using_system_id (system_id,
                                                               poa_current_impl.object_id_) != 0)
    {
      ACE_THROW_RETURN (CORBA::OBJ_ADAPTER (),
                        0);
    }

  // If the POA has the RETAIN policy, the POA looks in the Active
  // Object Map to find if there is a servant associated with the
  // Object Id value from the request. If such a servant exists, the
  // POA invokes the appropriate method on the servant.
  if (this->policies ().servant_retention () == PortableServer::RETAIN)
    {
      PortableServer::Servant servant = 0;
      int result = this->active_object_map ().find_servant_using_system_id_and_user_id (system_id,
                                                                                        poa_current_impl.object_id (),
                                                                                        servant,
                                                                                        servant_upcall.active_object_map_entry_);

      if (result == 0)
        {
          // Increment the reference count.
          ++servant_upcall.active_object_map_entry ()->reference_count_;

          // Success
          return servant;
        }
    }

  // If the POA has the NON_RETAIN policy or has the RETAIN policy but
  // didn't find a servant in the Active Object Map, the POA takes the
  // following actions:

  // If the USE_ACTIVE_OBJECT_MAP_ONLY policy is in effect, the POA raises
  // the OBJECT_NOT_EXIST system exception.
  if (this->policies ().request_processing () == PortableServer::USE_ACTIVE_OBJECT_MAP_ONLY)
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        0);
    }

#if !defined (TAO_HAS_MINIMUM_CORBA)

  // If the POA has the USE_DEFAULT_SERVANT policy, a default servant
  // has been associated with the POA so the POA will invoke the
  // appropriate method on that servant. If no servant has been
  // associated with the POA, the POA raises the OBJ_ADAPTER system
  // exception.
  if (this->policies ().request_processing () == PortableServer::USE_DEFAULT_SERVANT)
    {
      PortableServer::Servant result = this->default_servant_.in ();
      if (result == 0)
        {
          ACE_THROW_RETURN (CORBA::OBJ_ADAPTER (),
                            0);
        }
      else
        {
          // Success
          return result;
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
  if (this->policies ().request_processing () == PortableServer::USE_SERVANT_MANAGER)
    {
      if (CORBA::is_nil (this->servant_activator_.in ()) &&
          CORBA::is_nil (this->servant_locator_.in ()))
        {
          ACE_THROW_RETURN (CORBA::OBJ_ADAPTER (),
                            0);
        }

      PortableServer::POA_var poa = this->_this (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      PortableServer::Servant servant = 0;
      if (this->policies ().servant_retention () == PortableServer::RETAIN)
        {
          {
            // A recursive thread lock without using a recursive
            // thread lock.  Non_Servant_Upcall has a magic
            // constructor and destructor.  We unlock the
            // Object_Adapter lock for the duration of the servant
            // activator upcalls; reacquiring once the upcalls
            // complete.  Even though we are releasing the lock, other
            // threads will not be able to make progress since
            // <Object_Adapter::non_servant_upcall_in_progress_> has
            // been set.
            TAO_Object_Adapter::Non_Servant_Upcall non_servant_upcall (*this->orb_core_.object_adapter ());
            ACE_UNUSED_ARG (non_servant_upcall);

            // @@
            // Invocations of incarnate on the servant manager are serialized.
            // Invocations of etherealize on the servant manager are serialized.
            // Invocations of incarnate and etherealize on the servant manager are mutually exclusive.
            servant = this->servant_activator_->incarnate (poa_current_impl.object_id (),
                                                           poa.in (),
                                                           ACE_TRY_ENV);
            ACE_CHECK_RETURN (0);

            if (servant == 0)
              {
                ACE_THROW_RETURN (CORBA::OBJ_ADAPTER (),
                                  0);
              }
          }

          // If the incarnate operation returns a servant that is
          // already active for a different Object Id and if the POA
          // also has the UNIQUE_ID policy, the incarnate has violated
          // the POA policy and is considered to be in error. The POA
          // will raise an OBJ_ADAPTER system exception for the
          // request.
          if (this->policies ().id_uniqueness () == PortableServer::UNIQUE_ID &&
              this->is_servant_in_map (servant))
            {
              ACE_THROW_RETURN (CORBA::OBJ_ADAPTER (),
                                0);
            }

          // The POA enters the returned Servant value into the Active
          // Object Map so that subsequent requests with the same
          // ObjectId value will be delivered directly to that servant
          // without invoking the servant manager.
          int result = this->active_object_map ().rebind_using_user_id_and_system_id (servant,
                                                                                      poa_current_impl.object_id (),
                                                                                      system_id,
                                                                                      servant_upcall.active_object_map_entry_);
          if (result != 0)
            {
              ACE_THROW_RETURN (CORBA::OBJ_ADAPTER (),
                                0);
            }
          else
            {
              // Increment the reference count.
              ++servant_upcall.active_object_map_entry ()->reference_count_;

              // If this operation causes the object to be activated,
              // _add_ref is invoked at least once on the Servant
              // argument before returning. Otherwise, the POA does
              // not increment or decrement the reference count of the
              // Servant passed to this function.
              servant->_add_ref (ACE_TRY_ENV);
              ACE_CHECK_RETURN (0);

              // If we are a single threaded POA, set up the
              // appropriate locking in the servant.
              this->establish_servant_lock (servant);

              // Success
              return servant;
            }
        }
      else
        //
        // Don't retain servant
        //
        {
          // A recursive thread lock without using a recursive thread
          // lock.  Non_Servant_Upcall has a magic constructor and
          // destructor.  We unlock the Object_Adapter lock for the
          // duration of the servant activator upcalls; reacquiring
          // once the upcalls complete.  Even though we are releasing
          // the lock, other threads will not be able to make progress
          // since <Object_Adapter::non_servant_upcall_in_progress_>
          // has been set.
          TAO_Object_Adapter::Non_Servant_Upcall non_servant_upcall (*this->orb_core_.object_adapter ());
          ACE_UNUSED_ARG (non_servant_upcall);

          // No serialization of invocations of preinvoke or
          // postinvoke may be assumed; there may be multiple
          // concurrent invocations of preinvoke for the same
          // ObjectId.
          //
          // The same thread will be used to preinvoke the object,
          // process the request, and postinvoke the object.
          //
          PortableServer::ServantLocator::Cookie cookie;
          PortableServer::Servant servant = this->servant_locator_->preinvoke (poa_current_impl.object_id (),
                                                                               poa.in (),
                                                                               operation,
                                                                               cookie,
                                                                               ACE_TRY_ENV);
          ACE_CHECK_RETURN (0);

          if (servant == 0)
            {
              ACE_THROW_RETURN (CORBA::OBJ_ADAPTER (),
                                0);
            }

          // If we are a single threaded POA, set up the
          // appropriate locking in the servant.
          this->establish_servant_lock (servant);

          // Remember to invoke <postinvoke>
          servant_upcall.using_servant_locator ();

          // Remember the cookie
          servant_upcall.locator_cookie (cookie);

          // Remember operation name.
          servant_upcall.operation (operation);

          // Success
          return servant;
        }
    }
#else
  ACE_UNUSED_ARG (operation);
#endif /* !TAO_HAS_MINIMUM_CORBA */

  // Failure
  ACE_THROW_RETURN (CORBA::OBJ_ADAPTER (),
                    0);
}

/* static */
int
TAO_POA::parse_key (const TAO_ObjectKey &key,
                    TAO_Object_Adapter::poa_name &poa_system_name,
                    PortableServer::ObjectId &system_id,
                    CORBA::Boolean &is_root,
                    CORBA::Boolean &is_persistent,
                    CORBA::Boolean &is_system_id,
                    TAO_Temporary_Creation_Time &poa_creation_time)
{
  // Start at zero.
  CORBA::ULong starting_at = 0;

  // Get the object key octets.
  const CORBA::Octet *key_data = key.get_buffer ();

#if !defined (TAO_NO_IOR_TABLE)
  // Skip the object key prefix since we have already checked for
  // this.
  starting_at += TAO_OBJECTKEY_PREFIX_SIZE;
#endif /* TAO_NO_IOR_TABLE */

  // Check the root indicator.
  char root_key_type = key_data[starting_at];
  if (root_key_type == TAO_POA::root_key_char ())
    {
      is_root = 1;
    }
  else if (root_key_type == TAO_POA::non_root_key_char ())
    {
      is_root = 0;
    }
  else
    {
      // Incorrect key
      return -1;
    }

  // Skip past the system id indicator
  starting_at += TAO_POA::root_key_type_length ();

  // Check the system id indicator.
  char system_id_key_type = key_data[starting_at];
  if (system_id_key_type == TAO_POA::system_id_key_char ())
    {
      is_system_id = 1;
    }
  else if (system_id_key_type == TAO_POA::user_id_key_char ())
    {
      is_system_id = 0;
    }
  else
    {
      // Incorrect key
      return -1;
    }

  // Skip past the system id indicator
  starting_at += TAO_POA::system_id_key_type_length ();

  // Check the persistence indicator
  char persistent_key_type = key_data[starting_at];
  if (persistent_key_type == TAO_POA::persistent_key_char ())
    {
      is_persistent = 1;
    }
  else if (persistent_key_type == TAO_POA::transient_key_char ())
    {
      is_persistent = 0;
    }
  else
    {
      // Incorrect key
      return -1;
    }

  // Skip past the persistent indicator
  starting_at += TAO_POA::persistent_key_type_length ();

#if !defined (POA_NO_TIMESTAMP)
  // Grab the timestamp for transient POAs.
  if (!is_persistent)
    {
      // Take the creation time for the timestamp
      poa_creation_time.creation_time (key_data + starting_at);

      // Skip past the timestamp
      starting_at += TAO_Creation_Time::creation_time_length ();
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
      poa_name_size = key.length () - starting_at - TAO_Active_Object_Map::system_id_size ();
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

  // For non-root POAs, grab their name.
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
  system_id.replace (system_id_size,
                     system_id_size,
                     (CORBA::Octet *) key_data + starting_at,
                     0);

  // Success
  return 0;
}

TAO_ObjectKey *
TAO_POA::create_object_key (const PortableServer::ObjectId &id)
{
  // Calculate the prefix size.
  CORBA::ULong prefix_size = 0;
#if !defined (TAO_NO_IOR_TABLE)
  prefix_size += TAO_OBJECTKEY_PREFIX_SIZE;
#endif /* TAO_NO_IOR_TABLE */

  // If we are dealing with a persistent POA and user ids are being
  // used, then we need to add the POA name length field to the object
  // key. Otherwise, the POA name length can be calculated by looking
  // at the remainder after extracting other parts of the key.
  int add_poa_name_length =
    this->persistent_ &&
    !this->system_id_;

  // Size required by the POA name.
  CORBA::ULong poa_name = 0;

  // Calculate the space required for the POA name.
  CORBA::ULong poa_name_length = this->system_name_->length ();
  if (!this->root ())
    {
      poa_name += poa_name_length;
    }

  // Check if we need to added the length of the POA name.
  if (add_poa_name_length)
    {
      poa_name += sizeof (poa_name_length);
    }

  // Calculate the space required for the timestamp and the persistent
  // byte.
  CORBA::ULong creation_time = this->persistent_key_type_length ();
#if !defined (POA_NO_TIMESTAMP)
  // Calculate the space required for the timestamp.
  CORBA::ULong creation_time_length = TAO_Creation_Time::creation_time_length ();
  if (!this->persistent_)
    {
      creation_time += creation_time_length;
    }
#endif /* POA_NO_TIMESTAMP */

  // Calculate the space required for the key.
  CORBA::ULong buffer_size =
    prefix_size +
    this->root_key_type_length () +
    this->system_id_key_type_length () +
    creation_time +
    poa_name +
    id.length ();

  // Create the buffer for the key.
  CORBA::Octet *buffer = TAO_ObjectKey::allocbuf (buffer_size);

  // Keeps track of where the next infomation goes; start at 0 byte.
  CORBA::ULong starting_at = 0;

#if !defined (TAO_NO_IOR_TABLE)
  // Add the object key prefix.
  ACE_OS::memcpy (&buffer[starting_at],
                  &objectkey_prefix[0],
                  TAO_OBJECTKEY_PREFIX_SIZE);

  starting_at += TAO_OBJECTKEY_PREFIX_SIZE;
#endif /* TAO_NO_IOR_TABLE */

  // Copy the root byte.
  buffer[starting_at] = (CORBA::Octet) this->root_key_type ();
  starting_at += this->root_key_type_length ();

  // Copy the system id byte.
  buffer[starting_at] = (CORBA::Octet) this->system_id_key_type ();
  starting_at += this->system_id_key_type_length ();

  // Copy the persistence byte.
  buffer[starting_at] = (CORBA::Octet) this->persistent_key_type ();
  starting_at += this->persistent_key_type_length ();

#if !defined (POA_NO_TIMESTAMP)
  // Then copy the timestamp for transient POAs.
  if (!this->persistent ())
    {
      ACE_OS::memcpy (&buffer[starting_at],
                      this->creation_time_.creation_time (),
                      creation_time_length);
      starting_at += creation_time_length;
    }
#endif /* POA_NO_TIMESTAMP */

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
  if (!this->root ())
    {
      ACE_OS::memcpy (&buffer[starting_at],
                      this->system_name_->get_buffer (),
                      this->system_name_->length ());
      starting_at += this->system_name_->length ();
    }

  // Then copy the object id into the key.
  ACE_OS::memcpy (&buffer[starting_at],
                  id.get_buffer (),
                  id.length ());

  // Create the key, giving the ownership of the buffer to the
  // sequence.
  TAO_ObjectKey *key = 0;
  ACE_NEW_RETURN (key,
                  TAO_ObjectKey (buffer_size,
                                 buffer_size,
                                 buffer,
                                 1),
                  0);

  return key;
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

void
TAO_POA::set_folded_name (void)
{
  CORBA::ULong length = 0;
  CORBA::ULong parent_length = 0;

  if (this->parent_ != 0)
    {
      parent_length = this->parent_->folded_name ().length ();
      length += parent_length;
    }

  length += this->name_.length ();
  length += TAO_POA::name_separator_length ();

  this->folded_name_.length (length);
  CORBA::Octet *folded_name_buffer = this->folded_name_.get_buffer ();

  if (this->parent_ != 0)
    {
      ACE_OS::memcpy (folded_name_buffer,
                      this->parent_->folded_name ().get_buffer (),
                      parent_length);
    }

  ACE_OS::memcpy (&folded_name_buffer[parent_length],
                  this->name_.c_str (),
                  this->name_.length ());

  folded_name_buffer[length - TAO_POA::name_separator_length ()] = TAO_POA::name_separator ();
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
  PortableServer::ObjectId *id = 0;
  ACE_NEW_RETURN (id,
                  PortableServer::ObjectId (buffer_size,
                                            buffer_size,
                                            buffer,
                                            1),
                  0);

  return id;
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
  PortableServer::ObjectId *id = 0;
  ACE_NEW_RETURN (id,
                  PortableServer::ObjectId (size,
                                            size,
                                            buffer,
                                            1),
                  0);

  return id;
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
  PortableServer::ObjectId *id = 0;
  ACE_NEW_RETURN (id,
                  PortableServer::ObjectId (buffer_size,
                                            buffer_size,
                                            buffer,
                                            1),
                  0);

  return id;
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
  CORBA::WChar* string = CORBA::wstring_alloc (id.length ());

  // Copy the data
  ACE_OS::memcpy (string, id.get_buffer (), id.length () * sizeof (CORBA::WChar));

  // Null terminate the string
  string[id.length ()] = '\0';

  // Return string
  return string;
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
  u_int len = 3 * seq.length (); /* space for zero termination not needed */;
  str = CORBA::string_alloc (len);

  char *cp = str;

  for (u_int i = 0;
       cp < (cp + len) && i < seq.length();
       ++i)
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

#if !defined (TAO_HAS_MINIMUM_CORBA)

PortableServer::ThreadPolicy_ptr
TAO_POA::create_thread_policy (PortableServer::ThreadPolicyValue value,
                               CORBA::Environment &ACE_TRY_ENV)
{
  PortableServer::POA_var rootPOA = this->orb_core_.root_poa_reference (ACE_TRY_ENV);
  ACE_CHECK_RETURN (PortableServer::ThreadPolicy::_nil ());

  TAO_Thread_Policy *thread_policy = 0;
  ACE_NEW_THROW_EX (thread_policy,
                    TAO_Thread_Policy (value,
                                       rootPOA.in ()),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (PortableServer::ThreadPolicy::_nil ());

  // Give ownership of the copy to the auto pointer.
  auto_ptr<TAO_Thread_Policy> new_thread_policy (thread_policy);

  PortableServer::ThreadPolicy_var result = new_thread_policy->_this (ACE_TRY_ENV);
  ACE_CHECK_RETURN (PortableServer::ThreadPolicy::_nil ());

  // Give ownership of this servant to the POA.
  new_thread_policy->_remove_ref (ACE_TRY_ENV);
  ACE_CHECK_RETURN (PortableServer::ThreadPolicy::_nil ());

  // Make sure that the auto_ptr does not delete the implementation
  // object.
  new_thread_policy.release ();
  return result._retn ();
}

#endif /* TAO_HAS_MINIMUM_CORBA */

PortableServer::LifespanPolicy_ptr
TAO_POA::create_lifespan_policy (PortableServer::LifespanPolicyValue value,
                                 CORBA::Environment &ACE_TRY_ENV)
{
  PortableServer::POA_var rootPOA = this->orb_core_.root_poa_reference (ACE_TRY_ENV);
  ACE_CHECK_RETURN (PortableServer::LifespanPolicy::_nil ());

  TAO_Lifespan_Policy *lifespan_policy = 0;
  ACE_NEW_THROW_EX (lifespan_policy,
                    TAO_Lifespan_Policy (value,
                                         rootPOA.in ()),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (PortableServer::LifespanPolicy::_nil ());

  // Give ownership of the copy to the auto pointer.
  auto_ptr<TAO_Lifespan_Policy> new_lifespan_policy (lifespan_policy);

  PortableServer::LifespanPolicy_var result = new_lifespan_policy->_this (ACE_TRY_ENV);
  ACE_CHECK_RETURN (PortableServer::LifespanPolicy::_nil ());

  // Give ownership of this servant to the POA.
  new_lifespan_policy->_remove_ref (ACE_TRY_ENV);
  ACE_CHECK_RETURN (PortableServer::LifespanPolicy::_nil ());

  // Make sure that the auto_ptr does not delete the implementation
  // object.
  new_lifespan_policy.release ();
  return result._retn ();
}

PortableServer::IdUniquenessPolicy_ptr
TAO_POA::create_id_uniqueness_policy (PortableServer::IdUniquenessPolicyValue value,
                                      CORBA::Environment &ACE_TRY_ENV)
{
  PortableServer::POA_var rootPOA = this->orb_core_.root_poa_reference (ACE_TRY_ENV);
  ACE_CHECK_RETURN (PortableServer::IdUniquenessPolicy::_nil ());

  TAO_Id_Uniqueness_Policy *id_uniqueness_policy = 0;
  ACE_NEW_THROW_EX (id_uniqueness_policy,
                    TAO_Id_Uniqueness_Policy (value,
                                              rootPOA.in ()),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (PortableServer::IdUniquenessPolicy::_nil ());

  // Give ownership of the copy to the auto pointer.
  auto_ptr<TAO_Id_Uniqueness_Policy> new_id_uniqueness_policy (id_uniqueness_policy);

  PortableServer::IdUniquenessPolicy_var result = new_id_uniqueness_policy->_this (ACE_TRY_ENV);
  ACE_CHECK_RETURN (PortableServer::IdUniquenessPolicy::_nil ());

  // Give ownership of this servant to the POA.
  new_id_uniqueness_policy->_remove_ref (ACE_TRY_ENV);
  ACE_CHECK_RETURN (PortableServer::IdUniquenessPolicy::_nil ());

  // Make sure that the auto_ptr does not delete the implementation
  // object.
  new_id_uniqueness_policy.release ();
  return result._retn ();
}

PortableServer::IdAssignmentPolicy_ptr
TAO_POA::create_id_assignment_policy (PortableServer::IdAssignmentPolicyValue value,
                                      CORBA::Environment &ACE_TRY_ENV)
{
  PortableServer::POA_var rootPOA = this->orb_core_.root_poa_reference (ACE_TRY_ENV);
  ACE_CHECK_RETURN (PortableServer::IdAssignmentPolicy::_nil ());

  TAO_Id_Assignment_Policy *id_assignment_policy = 0;
  ACE_NEW_THROW_EX (id_assignment_policy,
                    TAO_Id_Assignment_Policy (value,
                                              rootPOA.in ()),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (PortableServer::IdAssignmentPolicy::_nil ());

  // Give ownership of the copy to the auto pointer.
  auto_ptr<TAO_Id_Assignment_Policy> new_id_assignment_policy (id_assignment_policy);

  PortableServer::IdAssignmentPolicy_var result = new_id_assignment_policy->_this (ACE_TRY_ENV);
  ACE_CHECK_RETURN (PortableServer::IdAssignmentPolicy::_nil ());

  // Give ownership of this servant to the POA.
  new_id_assignment_policy->_remove_ref (ACE_TRY_ENV);
  ACE_CHECK_RETURN (PortableServer::IdAssignmentPolicy::_nil ());

  // Make sure that the auto_ptr does not delete the implementation
  // object.
  new_id_assignment_policy.release ();
  return result._retn ();
}

#if !defined (TAO_HAS_MINIMUM_CORBA)

PortableServer::ImplicitActivationPolicy_ptr
TAO_POA::create_implicit_activation_policy (PortableServer::ImplicitActivationPolicyValue value,
                                            CORBA::Environment &ACE_TRY_ENV)
{
  PortableServer::POA_var rootPOA = this->orb_core_.root_poa_reference (ACE_TRY_ENV);
  ACE_CHECK_RETURN (PortableServer::ImplicitActivationPolicy::_nil ());

  TAO_Implicit_Activation_Policy *implicit_activation_policy = 0;
  ACE_NEW_THROW_EX (implicit_activation_policy,
                    TAO_Implicit_Activation_Policy (value,
                                                    rootPOA.in ()),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (PortableServer::ImplicitActivationPolicy::_nil ());

  // Give ownership of the copy to the auto pointer.
  auto_ptr<TAO_Implicit_Activation_Policy> new_implicit_activation_policy (implicit_activation_policy);

  PortableServer::ImplicitActivationPolicy_var result = new_implicit_activation_policy->_this (ACE_TRY_ENV);
  ACE_CHECK_RETURN (PortableServer::ImplicitActivationPolicy::_nil ());

  // Give ownership of this servant to the POA.
  new_implicit_activation_policy->_remove_ref (ACE_TRY_ENV);
  ACE_CHECK_RETURN (PortableServer::ImplicitActivationPolicy::_nil ());

  // Make sure that the auto_ptr does not delete the implementation
  // object.
  new_implicit_activation_policy.release ();
  return result._retn ();
}

PortableServer::ServantRetentionPolicy_ptr
TAO_POA::create_servant_retention_policy (PortableServer::ServantRetentionPolicyValue value,
                                          CORBA::Environment &ACE_TRY_ENV)
{
  PortableServer::POA_var rootPOA = this->orb_core_.root_poa_reference (ACE_TRY_ENV);
  ACE_CHECK_RETURN (PortableServer::ServantRetentionPolicy::_nil ());

  TAO_Servant_Retention_Policy *servant_retention_policy = 0;
  ACE_NEW_THROW_EX (servant_retention_policy,
                    TAO_Servant_Retention_Policy (value,
                                                  rootPOA.in ()),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (PortableServer::ServantRetentionPolicy::_nil ());

  // Give ownership of the copy to the auto pointer.
  auto_ptr<TAO_Servant_Retention_Policy> new_servant_retention_policy (servant_retention_policy);

  PortableServer::ServantRetentionPolicy_var result = new_servant_retention_policy->_this (ACE_TRY_ENV);
  ACE_CHECK_RETURN (PortableServer::ServantRetentionPolicy::_nil ());

  // Give ownership of this servant to the POA.
  new_servant_retention_policy->_remove_ref (ACE_TRY_ENV);
  ACE_CHECK_RETURN (PortableServer::ServantRetentionPolicy::_nil ());

  // Make sure that the auto_ptr does not delete the implementation
  // object.
  new_servant_retention_policy.release ();
  return result._retn ();
}

PortableServer::RequestProcessingPolicy_ptr
TAO_POA::create_request_processing_policy (PortableServer::RequestProcessingPolicyValue value,
                                           CORBA::Environment &ACE_TRY_ENV)
{
  PortableServer::POA_var rootPOA = this->orb_core_.root_poa_reference (ACE_TRY_ENV);
  ACE_CHECK_RETURN (PortableServer::RequestProcessingPolicy::_nil ());

  TAO_Request_Processing_Policy *request_processing_policy = 0;
  ACE_NEW_THROW_EX (request_processing_policy,
                    TAO_Request_Processing_Policy (value,
                                                   rootPOA.in ()),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (PortableServer::RequestProcessingPolicy::_nil ());

  // Give ownership of the copy to the auto pointer.
  auto_ptr<TAO_Request_Processing_Policy> new_request_processing_policy (request_processing_policy);

  PortableServer::RequestProcessingPolicy_var result = new_request_processing_policy->_this (ACE_TRY_ENV);
  ACE_CHECK_RETURN (PortableServer::RequestProcessingPolicy::_nil ());

  // Give ownership of this servant to the POA.
  new_request_processing_policy->_remove_ref (ACE_TRY_ENV);
  ACE_CHECK_RETURN (PortableServer::RequestProcessingPolicy::_nil ());

  // Make sure that the auto_ptr does not delete the implementation
  // object.
  new_request_processing_policy.release ();
  return result._retn ();
}

#endif /* TAO_HAS_MINIMUM_CORBA */

#if !defined (TAO_HAS_MINIMUM_CORBA)

TAO_Thread_Policy::TAO_Thread_Policy (PortableServer::ThreadPolicyValue value,
                                      PortableServer::POA_ptr poa)
  : value_ (value),
    poa_ (PortableServer::POA::_duplicate (poa))
{
}

PortableServer::ThreadPolicyValue
TAO_Thread_Policy::value (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  return this->value_;
}

CORBA::Policy_ptr
TAO_Thread_Policy::copy (CORBA::Environment &ACE_TRY_ENV)
{
  TAO_Thread_Policy *thread_policy_copy = 0;
  ACE_NEW_THROW_EX (thread_policy_copy,
                    TAO_Thread_Policy (*this),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  // Give ownership of the copy to the auto pointer.
  auto_ptr<TAO_Thread_Policy> new_thread_policy (thread_policy_copy);

  CORBA::Policy_var result = new_thread_policy->_this (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  // Give ownership of this servant to the POA.
  new_thread_policy->_remove_ref (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  // Make sure that the auto_ptr does not delete the implementation
  // object.
  new_thread_policy.release ();
  return result._retn ();
}

void
TAO_Thread_Policy::destroy (CORBA::Environment &ACE_TRY_ENV)
{
  //
  // Remove self from POA.  Because of reference counting, the POA
  // will automatically delete the servant when all pending requests
  // on this servant are complete.
  //

  PortableServer::POA_var poa = this->_default_POA (ACE_TRY_ENV);
  ACE_CHECK;

  PortableServer::ObjectId_var id = poa->servant_to_id (this,
                                                        ACE_TRY_ENV);
  ACE_CHECK;

  poa->deactivate_object (id.in (),
                          ACE_TRY_ENV);
  ACE_CHECK;
}

CORBA::PolicyType
TAO_Thread_Policy::policy_type (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  return PortableServer::THREAD_POLICY_ID;
}

PortableServer::POA_ptr
TAO_Thread_Policy::_default_POA (CORBA::Environment & /* ACE_TRY_ENV */)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

#endif /* TAO_HAS_MINIMUM_CORBA */

TAO_Lifespan_Policy::TAO_Lifespan_Policy (PortableServer::LifespanPolicyValue value,
                                          PortableServer::POA_ptr poa)
  : value_ (value),
    poa_ (PortableServer::POA::_duplicate (poa))
{
}

PortableServer::LifespanPolicyValue
TAO_Lifespan_Policy::value (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  return this->value_;
}

CORBA::Policy_ptr
TAO_Lifespan_Policy::copy (CORBA::Environment &ACE_TRY_ENV)
{
  TAO_Lifespan_Policy *lifespan_policy_copy = 0;
  ACE_NEW_THROW_EX (lifespan_policy_copy,
                    TAO_Lifespan_Policy (*this),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  // Give ownership of the copy to the auto pointer.
  auto_ptr<TAO_Lifespan_Policy> new_lifespan_policy (lifespan_policy_copy);

  CORBA::Policy_var result = new_lifespan_policy->_this (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  // Give ownership of this servant to the POA.
  new_lifespan_policy->_remove_ref (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  // Make sure that the auto_ptr does not delete the implementation
  // object.
  new_lifespan_policy.release ();
  return result._retn ();
}

void
TAO_Lifespan_Policy::destroy (CORBA::Environment &ACE_TRY_ENV)
{
  //
  // Remove self from POA.  Because of reference counting, the POA
  // will automatically delete the servant when all pending requests
  // on this servant are complete.
  //

  PortableServer::POA_var poa = this->_default_POA (ACE_TRY_ENV);
  ACE_CHECK;

  PortableServer::ObjectId_var id = poa->servant_to_id (this,
                                                        ACE_TRY_ENV);
  ACE_CHECK;

  poa->deactivate_object (id.in (),
                          ACE_TRY_ENV);
  ACE_CHECK;
}

CORBA::PolicyType
TAO_Lifespan_Policy::policy_type (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  return PortableServer::LIFESPAN_POLICY_ID;
}

PortableServer::POA_ptr
TAO_Lifespan_Policy::_default_POA (CORBA::Environment & /* ACE_TRY_ENV */)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

TAO_Id_Uniqueness_Policy::TAO_Id_Uniqueness_Policy (PortableServer::IdUniquenessPolicyValue value,
                                                    PortableServer::POA_ptr poa)
  : value_ (value),
    poa_ (PortableServer::POA::_duplicate (poa))
{
}

PortableServer::IdUniquenessPolicyValue
TAO_Id_Uniqueness_Policy::value (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  return this->value_;
}

CORBA::Policy_ptr
TAO_Id_Uniqueness_Policy::copy (CORBA::Environment &ACE_TRY_ENV)
{
  TAO_Id_Uniqueness_Policy *id_uniqueness_policy_copy = 0;
  ACE_NEW_THROW_EX (id_uniqueness_policy_copy,
                    TAO_Id_Uniqueness_Policy (*this),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  // Give ownership of the copy to the auto pointer.
  auto_ptr<TAO_Id_Uniqueness_Policy> new_id_uniqueness_policy (id_uniqueness_policy_copy);

  CORBA::Policy_var result = new_id_uniqueness_policy->_this (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  // Give ownership of this servant to the POA.
  new_id_uniqueness_policy->_remove_ref (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  // Make sure that the auto_ptr does not delete the implementation
  // object.
  new_id_uniqueness_policy.release ();
  return result._retn ();
}

void
TAO_Id_Uniqueness_Policy::destroy (CORBA::Environment &ACE_TRY_ENV)
{
  //
  // Remove self from POA.  Because of reference counting, the POA
  // will automatically delete the servant when all pending requests
  // on this servant are complete.
  //

  PortableServer::POA_var poa = this->_default_POA (ACE_TRY_ENV);
  ACE_CHECK;

  PortableServer::ObjectId_var id = poa->servant_to_id (this,
                                                        ACE_TRY_ENV);
  ACE_CHECK;

  poa->deactivate_object (id.in (),
                          ACE_TRY_ENV);
  ACE_CHECK;
}

CORBA::PolicyType
TAO_Id_Uniqueness_Policy::policy_type (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  return PortableServer::ID_UNIQUENESS_POLICY_ID;
}

PortableServer::POA_ptr
TAO_Id_Uniqueness_Policy::_default_POA (CORBA::Environment & /* ACE_TRY_ENV */)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

TAO_Id_Assignment_Policy::TAO_Id_Assignment_Policy (PortableServer::IdAssignmentPolicyValue value,
                                                    PortableServer::POA_ptr poa)
  : value_ (value),
    poa_ (PortableServer::POA::_duplicate (poa))
{
}

PortableServer::IdAssignmentPolicyValue
TAO_Id_Assignment_Policy::value (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  return this->value_;
}

CORBA::Policy_ptr
TAO_Id_Assignment_Policy::copy (CORBA::Environment &ACE_TRY_ENV)
{
  TAO_Id_Assignment_Policy *id_assignment_policy_copy = 0;
  ACE_NEW_THROW_EX (id_assignment_policy_copy,
                    TAO_Id_Assignment_Policy (*this),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  // Give ownership of the copy to the auto pointer.
  auto_ptr<TAO_Id_Assignment_Policy> new_id_assignment_policy (id_assignment_policy_copy);

  CORBA::Policy_var result = new_id_assignment_policy->_this (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  // Give ownership of this servant to the POA.
  new_id_assignment_policy->_remove_ref (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  // Make sure that the auto_ptr does not delete the implementation
  // object.
  new_id_assignment_policy.release ();
  return result._retn ();
}

void
TAO_Id_Assignment_Policy::destroy (CORBA::Environment &ACE_TRY_ENV)
{
  //
  // Remove self from POA.  Because of reference counting, the POA
  // will automatically delete the servant when all pending requests
  // on this servant are complete.
  //

  PortableServer::POA_var poa = this->_default_POA (ACE_TRY_ENV);
  ACE_CHECK;

  PortableServer::ObjectId_var id = poa->servant_to_id (this,
                                                        ACE_TRY_ENV);
  ACE_CHECK;

  poa->deactivate_object (id.in (),
                          ACE_TRY_ENV);
  ACE_CHECK;
}

CORBA::PolicyType
TAO_Id_Assignment_Policy::policy_type (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  return PortableServer::ID_ASSIGNMENT_POLICY_ID;
}

PortableServer::POA_ptr
TAO_Id_Assignment_Policy::_default_POA (CORBA::Environment & /* ACE_TRY_ENV */)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

#if !defined (TAO_HAS_MINIMUM_CORBA)

TAO_Implicit_Activation_Policy::TAO_Implicit_Activation_Policy (PortableServer::ImplicitActivationPolicyValue value,
                                                                PortableServer::POA_ptr poa)
  : value_ (value),
    poa_ (PortableServer::POA::_duplicate (poa))
{
}

PortableServer::ImplicitActivationPolicyValue
TAO_Implicit_Activation_Policy::value (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  return this->value_;
}

CORBA::Policy_ptr
TAO_Implicit_Activation_Policy::copy (CORBA::Environment &ACE_TRY_ENV)
{
  TAO_Implicit_Activation_Policy *implicit_activation_policy_copy = 0;
  ACE_NEW_THROW_EX (implicit_activation_policy_copy,
                    TAO_Implicit_Activation_Policy (*this),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  // Give ownership of the copy to the auto pointer.
  auto_ptr<TAO_Implicit_Activation_Policy> new_implicit_activation_policy (implicit_activation_policy_copy);

  CORBA::Policy_var result = new_implicit_activation_policy->_this (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  // Give ownership of this servant to the POA.
  new_implicit_activation_policy->_remove_ref (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  // Make sure that the auto_ptr does not delete the implementation
  // object.
  new_implicit_activation_policy.release ();
  return result._retn ();
}

void
TAO_Implicit_Activation_Policy::destroy (CORBA::Environment &ACE_TRY_ENV)
{
  //
  // Remove self from POA.  Because of reference counting, the POA
  // will automatically delete the servant when all pending requests
  // on this servant are complete.
  //

  PortableServer::POA_var poa = this->_default_POA (ACE_TRY_ENV);
  ACE_CHECK;

  PortableServer::ObjectId_var id = poa->servant_to_id (this,
                                                        ACE_TRY_ENV);
  ACE_CHECK;

  poa->deactivate_object (id.in (),
                          ACE_TRY_ENV);
  ACE_CHECK;
}

CORBA::PolicyType
TAO_Implicit_Activation_Policy::policy_type (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  return PortableServer::IMPLICIT_ACTIVATION_POLICY_ID;
}

PortableServer::POA_ptr
TAO_Implicit_Activation_Policy::_default_POA (CORBA::Environment & /* ACE_TRY_ENV */)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

TAO_Servant_Retention_Policy::TAO_Servant_Retention_Policy (PortableServer::ServantRetentionPolicyValue value,
                                                            PortableServer::POA_ptr poa)
  : value_ (value),
    poa_ (PortableServer::POA::_duplicate (poa))
{
}

PortableServer::ServantRetentionPolicyValue
TAO_Servant_Retention_Policy::value (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  return this->value_;
}

CORBA::Policy_ptr
TAO_Servant_Retention_Policy::copy (CORBA::Environment &ACE_TRY_ENV)
{
  TAO_Servant_Retention_Policy *servant_retention_policy_copy = 0;
  ACE_NEW_THROW_EX (servant_retention_policy_copy,
                    TAO_Servant_Retention_Policy (*this),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  // Give ownership of the copy to the auto pointer.
  auto_ptr<TAO_Servant_Retention_Policy> new_servant_retention_policy (servant_retention_policy_copy);

  CORBA::Policy_var result = new_servant_retention_policy->_this (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  // Give ownership of this servant to the POA.
  new_servant_retention_policy->_remove_ref (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  // Make sure that the auto_ptr does not delete the implementation
  // object.
  new_servant_retention_policy.release ();
  return result._retn ();
}

void
TAO_Servant_Retention_Policy::destroy (CORBA::Environment &ACE_TRY_ENV)
{
  //
  // Remove self from POA.  Because of reference counting, the POA
  // will automatically delete the servant when all pending requests
  // on this servant are complete.
  //

  PortableServer::POA_var poa = this->_default_POA (ACE_TRY_ENV);
  ACE_CHECK;

  PortableServer::ObjectId_var id = poa->servant_to_id (this,
                                                        ACE_TRY_ENV);
  ACE_CHECK;

  poa->deactivate_object (id.in (),
                          ACE_TRY_ENV);
  ACE_CHECK;
}

CORBA::PolicyType
TAO_Servant_Retention_Policy::policy_type (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  return PortableServer::SERVANT_RETENTION_POLICY_ID;
}

PortableServer::POA_ptr
TAO_Servant_Retention_Policy::_default_POA (CORBA::Environment & /* ACE_TRY_ENV */)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

TAO_Request_Processing_Policy::TAO_Request_Processing_Policy (PortableServer::RequestProcessingPolicyValue value,
                                                              PortableServer::POA_ptr poa)
  : value_ (value),
    poa_ (PortableServer::POA::_duplicate (poa))
{
}

PortableServer::RequestProcessingPolicyValue
TAO_Request_Processing_Policy::value (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  return this->value_;
}

CORBA::Policy_ptr
TAO_Request_Processing_Policy::copy (CORBA::Environment &ACE_TRY_ENV)
{
  TAO_Request_Processing_Policy *request_processing_policy_copy = 0;
  ACE_NEW_THROW_EX (request_processing_policy_copy,
                    TAO_Request_Processing_Policy (*this),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  // Give ownership of the copy to the auto pointer.
  auto_ptr<TAO_Request_Processing_Policy> new_request_processing_policy (request_processing_policy_copy);

  CORBA::Policy_var result = new_request_processing_policy->_this (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  // Give ownership of this servant to the POA.
  new_request_processing_policy->_remove_ref (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  // Make sure that the auto_ptr does not delete the implementation
  // object.
  new_request_processing_policy.release ();
  return result._retn ();
}

void
TAO_Request_Processing_Policy::destroy (CORBA::Environment &ACE_TRY_ENV)
{
  //
  // Remove self from POA.  Because of reference counting, the POA
  // will automatically delete the servant when all pending requests
  // on this servant are complete.
  //

  PortableServer::POA_var poa = this->_default_POA (ACE_TRY_ENV);
  ACE_CHECK;

  PortableServer::ObjectId_var id = poa->servant_to_id (this,
                                                        ACE_TRY_ENV);
  ACE_CHECK;

  poa->deactivate_object (id.in (),
                          ACE_TRY_ENV);
  ACE_CHECK;
}

CORBA::PolicyType
TAO_Request_Processing_Policy::policy_type (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  return PortableServer::REQUEST_PROCESSING_POLICY_ID;
}

PortableServer::POA_ptr
TAO_Request_Processing_Policy::_default_POA (CORBA::Environment & /* ACE_TRY_ENV */)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

#endif /* TAO_HAS_MINIMUM_CORBA */

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
TAO_POA_Policies::parse_policies (const CORBA::PolicyList &policies,
                                  CORBA::Environment &ACE_TRY_ENV)
{
  for (CORBA::ULong i = 0;
       i < policies.length ();
       i++)
    {
      this->parse_policy (policies[i],
                          ACE_TRY_ENV);
      ACE_CHECK;
    }

  if (this->validity_check () == -1)
    {
      ACE_THROW (PortableServer::POA::InvalidPolicy ());
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
                                CORBA::Environment &ACE_TRY_ENV)
{

#if !defined (TAO_HAS_MINIMUM_CORBA)

  PortableServer::ThreadPolicy_var thread
    = PortableServer::ThreadPolicy::_narrow (policy,
                                             ACE_TRY_ENV);
  ACE_CHECK;

  if (!CORBA::is_nil (thread.in ()))
    {
      this->thread_ = thread->value (ACE_TRY_ENV);
      ACE_CHECK;

      return;
    }

#endif /* TAO_HAS_MINIMUM_CORBA */

  PortableServer::LifespanPolicy_var lifespan
    = PortableServer::LifespanPolicy::_narrow (policy,
                                               ACE_TRY_ENV);
  ACE_CHECK;

  if (!CORBA::is_nil (lifespan.in ()))
    {
      this->lifespan_ = lifespan->value (ACE_TRY_ENV);
      ACE_CHECK;

      return;
    }

  PortableServer::IdUniquenessPolicy_var id_uniqueness
    = PortableServer::IdUniquenessPolicy::_narrow (policy,
                                                   ACE_TRY_ENV);
  ACE_CHECK;

  if (!CORBA::is_nil (id_uniqueness.in ()))
    {
      this->id_uniqueness_ = id_uniqueness->value (ACE_TRY_ENV);
      ACE_CHECK;

      return;
    }

  PortableServer::IdAssignmentPolicy_var id_assignment
    = PortableServer::IdAssignmentPolicy::_narrow (policy,
                                                   ACE_TRY_ENV);
  ACE_CHECK;

  if (!CORBA::is_nil (id_assignment.in ()))
    {
      this->id_assignment_ = id_assignment->value (ACE_TRY_ENV);
      ACE_CHECK;

      return;
    }

#if !defined (TAO_HAS_MINIMUM_CORBA)

  PortableServer::ImplicitActivationPolicy_var implicit_activation
    = PortableServer::ImplicitActivationPolicy::_narrow (policy,
                                                         ACE_TRY_ENV);
  ACE_CHECK;

  if (!CORBA::is_nil (implicit_activation.in ()))
    {
      this->implicit_activation_ = implicit_activation->value (ACE_TRY_ENV);
      ACE_CHECK;

      return;
    }

  PortableServer::ServantRetentionPolicy_var servant_retention
    = PortableServer::ServantRetentionPolicy::_narrow (policy,
                                                       ACE_TRY_ENV);
  ACE_CHECK;

  if (!CORBA::is_nil (servant_retention.in ()))
    {
      this->servant_retention_ = servant_retention->value (ACE_TRY_ENV);
      ACE_CHECK;

      return;
    }

  PortableServer::RequestProcessingPolicy_var request_processing
    = PortableServer::RequestProcessingPolicy::_narrow (policy,
                                                        ACE_TRY_ENV);
  ACE_CHECK;

  if (!CORBA::is_nil (request_processing.in ()))
    {
      this->request_processing_ = request_processing->value (ACE_TRY_ENV);
      ACE_CHECK;

      return;
    }

#endif /* TAO_HAS_MINIMUM_CORBA */

  ACE_THROW (PortableServer::POA::InvalidPolicy ());
}

#if !defined (TAO_HAS_MINIMUM_CORBA)

TAO_Adapter_Activator::TAO_Adapter_Activator (PortableServer::POAManager_ptr poa_manager)
  : poa_manager_ (PortableServer::POAManager::_duplicate (poa_manager))
{
}

CORBA::Boolean
TAO_Adapter_Activator::unknown_adapter (PortableServer::POA_ptr parent,
                                        const char *name,
                                        CORBA::Environment &ACE_TRY_ENV)
{
  // Default policies
  CORBA::PolicyList default_policies;

  // This assumes that the lock on the parent is recursive
  PortableServer::POA_var child = parent->create_POA (name,
                                                      this->poa_manager_.in (),
                                                      default_policies,
                                                      ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  ACE_TRY
    {
      PortableServer::AdapterActivator_var activator = this->_this (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      child->the_activator (activator.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      child->destroy (1,
                      1,
                      ACE_TRY_ENV);

      return 0;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);

  // Finally everything is fine
  return 1;
}

#endif /* TAO_HAS_MINIMUM_CORBA */

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Array<PortableServer::ObjectId>;
template class ACE_Array_Base<PortableServer::ObjectId>;

template class ACE_Auto_Basic_Ptr<TAO_Id_Assignment_Policy>;
template class ACE_Auto_Basic_Ptr<TAO_Id_Uniqueness_Policy>;
template class ACE_Auto_Basic_Ptr<TAO_Lifespan_Policy>;

#if !defined (TAO_HAS_MINIMUM_CORBA)

template class ACE_Auto_Basic_Ptr<TAO_Thread_Policy>;
template class ACE_Auto_Basic_Ptr<TAO_Implicit_Activation_Policy>;
template class ACE_Auto_Basic_Ptr<TAO_Servant_Retention_Policy>;
template class ACE_Auto_Basic_Ptr<TAO_Request_Processing_Policy>;

template class ACE_Auto_Basic_Ptr<TAO_Forwarding_Servant>;

#endif /* TAO_HAS_MINIMUM_CORBA */

//template class ACE_Auto_Basic_Ptr<TAO_Active_Object_Map_Iterator_Impl>;
template class ACE_Auto_Basic_Ptr<TAO_POA>;
template class ACE_Auto_Basic_Ptr<TAO_Active_Object_Map>;
template class ACE_Auto_Basic_Ptr<TAO_POA_Manager>;
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
template class auto_ptr<TAO_Id_Assignment_Policy>;
template class auto_ptr<TAO_Id_Uniqueness_Policy>;
template class auto_ptr<TAO_Lifespan_Policy>;

#if !defined (TAO_HAS_MINIMUM_CORBA)

template class auto_ptr<TAO_Thread_Policy>;
template class auto_ptr<TAO_Implicit_Activation_Policy>;
template class auto_ptr<TAO_Servant_Retention_Policy>;
template class auto_ptr<TAO_Request_Processing_Policy>;

template class auto_ptr<TAO_Forwarding_Servant>;

#endif /* TAO_HAS_MINIMUM_CORBA */

//template class auto_ptr<TAO_Active_Object_Map_Iterator_Impl>;
template class auto_ptr<TAO_POA>;
template class auto_ptr<TAO_Active_Object_Map>;
template class auto_ptr<TAO_POA_Manager>;
template class ACE_Node<TAO_POA *>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Array<PortableServer::ObjectId>
#pragma instantiate ACE_Array_Base<PortableServer::ObjectId>

#pragma instantiate ACE_Auto_Basic_Ptr<TAO_Id_Assignment_Policy>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_Id_Uniqueness_Policy>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_Lifespan_Policy>

#if !defined (TAO_HAS_MINIMUM_CORBA)

#pragma instantiate ACE_Auto_Basic_Ptr<TAO_Thread_Policy>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_Implicit_Activation_Policy>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_Servant_Retention_Policy>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_Request_Processing_Policy>

#pragma instantiate ACE_Auto_Basic_Ptr<TAO_Forwarding_Servant>

#endif /* TAO_HAS_MINIMUM_CORBA */

//#pragma instantiate ACE_Auto_Basic_Ptr<TAO_Active_Object_Map_Iterator_Impl>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_POA>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_Active_Object_Map>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_POA_Manager>
#pragma instantiate ACE_Lock_Adapter<ACE_Null_Mutex>
#if defined (ACE_HAS_THREADS)
#pragma instantiate ACE_Lock_Adapter<ACE_Recursive_Thread_Mutex>
#pragma instantiate ACE_Lock_Adapter<ACE_Thread_Mutex>
#endif /* ACE_HAS_THREADS */
#pragma instantiate ACE_Map_Entry<TAO_Unbounded_Sequence<unsigned char>, TAO_ServantBase *>
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
#pragma instantiate auto_ptr<TAO_Id_Assignment_Policy>
#pragma instantiate auto_ptr<TAO_Id_Uniqueness_Policy>
#pragma instantiate auto_ptr<TAO_Lifespan_Policy>

#if !defined (TAO_HAS_MINIMUM_CORBA)

#pragma instantiate auto_ptr<TAO_Thread_Policy>
#pragma instantiate auto_ptr<TAO_Implicit_Activation_Policy>
#pragma instantiate auto_ptr<TAO_Servant_Retention_Policy>
#pragma instantiate auto_ptr<TAO_Request_Processing_Policy>

#pragma instantiate auto_ptr<TAO_Forwarding_Servant>

#endif /* TAO_HAS_MINIMUM_CORBA */

//#pragma instantiate auto_ptr<TAO_Active_Object_Map_Iterator_Impl>
#pragma instantiate auto_ptr<TAO_POA>
#pragma instantiate auto_ptr<TAO_Active_Object_Map>
#pragma instantiate auto_ptr<TAO_POA_Manager>
#pragma instantiate ACE_Node<TAO_POA *>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
