// -*- C++ -*-

//=============================================================================
/**
 *  @file    ServantRetentionStrategyRetain.cpp
 *
 *  $Id$
 *
 */
//=============================================================================

#include "tao/ORB_Core.h"
#include "tao/debug.h"
#include "tao/PortableServer/ServantRetentionStrategyRetain.h"
#include "tao/PortableServer/Non_Servant_Upcall.h"
#include "tao/PortableServer/Servant_Upcall.h"
#include "tao/PortableServer/POA_Current_Impl.h"
#include "tao/PortableServer/Root_POA.h"
#include "tao/PortableServer/Active_Object_Map.h"
#include "tao/PortableServer/Active_Object_Map_Entry.h"
#include "ace/Auto_Ptr.h"

ACE_RCSID (PortableServer,
           Servant_Retention_Strategy,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    ServantRetentionStrategyRetain::ServantRetentionStrategyRetain (void) :
      ServantRetentionStrategyNonRetain (),
      active_object_map_ (0),
      waiting_servant_deactivation_ (0)
    {
    }

    void
    ServantRetentionStrategyRetain::strategy_init (
      TAO_Root_POA *poa
      ACE_ENV_ARG_DECL)
    {
      poa_ = poa;

      // Create the active object map to be used
      TAO_Active_Object_Map *active_object_map = 0;
      ACE_NEW_THROW_EX (active_object_map,
                        TAO_Active_Object_Map (!poa->system_id (),
                                               !poa->allow_multiple_activations (),
                                               poa->is_persistent (),
                                               poa->orb_core().server_factory ()->active_object_map_creation_parameters ()
                                               ACE_ENV_ARG_PARAMETER), CORBA::NO_MEMORY ());
      ACE_CHECK;

      // Give ownership of the new map to the auto pointer.  Note, that it
      // is important for the auto pointer to take ownership before
      // checking for exception since we may need to delete the new map.
      auto_ptr<TAO_Active_Object_Map> new_active_object_map (active_object_map);

      // Check for exception in construction of the active object map.
      ACE_CHECK;

      // Finally everything is fine.  Make sure to take ownership away
      // from the auto pointer.
      this->active_object_map_ = new_active_object_map.release ();
    }

    void
    ServantRetentionStrategyRetain::strategy_cleanup(
      ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    {
      // Delete the active object map.
      delete this->active_object_map_;
      active_object_map_ = 0;
    }

    void
    ServantRetentionStrategyRetain::deactivate_object (
      const PortableServer::ObjectId &id
      ACE_ENV_ARG_DECL)
    {

      TAO_Active_Object_Map_Entry *active_object_map_entry = 0;
      int result = this->active_object_map_->
        find_entry_using_user_id (id,
                                  active_object_map_entry);

      // If there is no active object associated with the specified Object
      // Id, the operation raises an ObjectNotActive exception.
      if (result != 0)
        {
          ACE_THROW (PortableServer::POA::ObjectNotActive ());
        }

      this->deactivate_map_entry (active_object_map_entry
                                  ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

    void
    ServantRetentionStrategyRetain::deactivate_map_entry (
      TAO_Active_Object_Map_Entry *active_object_map_entry
      ACE_ENV_ARG_DECL)
    {
      // Decrement the reference count.
      CORBA::UShort new_count = --active_object_map_entry->reference_count_;

      // Inform the custom servant dispatching (CSD) strategy that the
      // servant is deactivated. This would be called just once when the
      // servant is deactivated the first time.
      if (active_object_map_entry->deactivated_ == 0)
        {
          this->poa_->servant_deactivated_hook (
            active_object_map_entry->servant_,
            active_object_map_entry->user_id_
            ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }

      if (new_count == 0)
        {
          this->poa_->cleanup_servant (active_object_map_entry->servant_,
                                       active_object_map_entry->user_id_
                                       ACE_ENV_ARG_PARAMETER);
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

    int
    ServantRetentionStrategyRetain::unbind_using_user_id (
      const PortableServer::ObjectId &user_id)
    {
      return this->active_object_map_->
        unbind_using_user_id (user_id);
    }

    PortableServer::Servant
    ServantRetentionStrategyRetain::find_servant (
      const PortableServer::ObjectId &system_id
      ACE_ENV_ARG_DECL)
    {
      // Find user id from system id.
      PortableServer::ObjectId_var user_id;
      if (active_object_map_->
          find_user_id_using_system_id (system_id,
                                        user_id.out()) != 0)
        {
          ACE_THROW_RETURN (CORBA::OBJ_ADAPTER (),
                            0);
        }

      // This operation returns the active servant associated with the
      // specified system Object Id value.  If the Object Id value is
      // not active in the POA, an ObjectNotActive exception is
      // raised.
      TAO_Active_Object_Map_Entry *entry = 0;
      PortableServer::Servant servant = 0;

      int result =
        active_object_map_->
          find_servant_using_system_id_and_user_id (system_id,
                                                    user_id.in(),
                                                    servant,
                                                    entry);

      if (result == -1)
        {
          ACE_THROW_RETURN (PortableServer::POA::ObjectNotActive (),
                            0);
        }

      return servant;
    }

    PortableServer::ObjectId *
    ServantRetentionStrategyRetain::system_id_to_object_id (
      const PortableServer::ObjectId &system_id
      ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongAdapter,
                         PortableServer::POA::WrongPolicy))
    {
      // The object denoted by the reference does not have to be
      // active for this operation to succeed.
      PortableServer::ObjectId_var user_id;
      if (this->active_object_map_->
          find_user_id_using_system_id (system_id,
                                        user_id.out ()) != 0)
        {
          ACE_THROW_RETURN (CORBA::OBJ_ADAPTER (),
                            0);
        }

      return user_id._retn ();
    }

    PortableServer::Servant
    ServantRetentionStrategyRetain::user_id_to_servant (
      const PortableServer::ObjectId &id
      ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::ObjectNotActive,
                         PortableServer::POA::WrongPolicy))
    {
      // If the POA has the RETAIN policy and the specified ObjectId is in
      // the Active Object Map, this operation returns the servant
      // associated with that object in the Active Object Map.
      PortableServer::Servant servant = 0;

      int result =
        this->active_object_map_->find_servant_using_user_id (id,
                                                              servant);

      if (result == -1)
        {
          ACE_THROW_RETURN (PortableServer::POA::ObjectNotActive (),
                            0);
        }

      return servant;
    }

    CORBA::Object_ptr
    ServantRetentionStrategyRetain::id_to_reference (
      const PortableServer::ObjectId &id,
      bool indirect
      ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::ObjectNotActive,
                         PortableServer::POA::WrongPolicy))
    {
      // If an object with the specified Object Id value is currently
      // active, a reference encapsulating the information used to
      // activate the object is returned.
      PortableServer::ObjectId_var system_id;
      PortableServer::Servant servant;
      CORBA::Short priority;

      if (this->active_object_map_->
          find_servant_and_system_id_using_user_id (id,
                                                    servant,
                                                    system_id.out (),
                                                    priority) == 0)
        {
          // Remember params for potentially invoking <key_to_object> later.
          this->poa_->key_to_object_params_.set (system_id,
                                                 servant->_interface_repository_id (),
                                                 servant,
                                                 1,
                                                 priority,
                                                 indirect);

          return this->poa_->invoke_key_to_object_helper_i (servant->_interface_repository_id (),
                                                            id
                                                            ACE_ENV_ARG_PARAMETER);
        }
      else
        {
          // If the Object Id value is not active in the POA, an
          // ObjectNotActive exception is raised.
          ACE_THROW_RETURN (PortableServer::POA::ObjectNotActive (),
                            CORBA::Object::_nil ());
        }
    }

    TAO_SERVANT_LOCATION
    ServantRetentionStrategyRetain::servant_present (
      const PortableServer::ObjectId &system_id,
      PortableServer::Servant &servant
      ACE_ENV_ARG_DECL)
    {
      // Find user id from system id.
      PortableServer::ObjectId_var user_id;
      if (this->active_object_map_->
          find_user_id_using_system_id (system_id,
                                        user_id.out()) != 0)
        {
          ACE_THROW_RETURN (CORBA::OBJ_ADAPTER (),
                            TAO_SERVANT_NOT_FOUND);
        }

      TAO_Active_Object_Map_Entry *entry = 0;
      int result = this->active_object_map_->
        find_servant_using_system_id_and_user_id (system_id,
                                                  user_id.in(),
                                                  servant,
                                                  entry);
      if (result == 0)
        {
          // Success
          return TAO_SERVANT_FOUND;
        }
      else
        {
          return TAO_SERVANT_NOT_FOUND;
        }
    }

    PortableServer::Servant
    ServantRetentionStrategyRetain::find_servant (
      const PortableServer::ObjectId &system_id,
      TAO::Portable_Server::Servant_Upcall &servant_upcall,
      TAO::Portable_Server::POA_Current_Impl &poa_current_impl
      ACE_ENV_ARG_DECL)
    {
      PortableServer::ObjectId user_id;
      // If we have the RETAIN policy, convert/transform from system id to
      // user id.
      if (this->active_object_map_->
          find_user_id_using_system_id (system_id,
                                        user_id) != 0)
        {
          ACE_THROW_RETURN (CORBA::OBJ_ADAPTER (),
                            0);
        }

      poa_current_impl.object_id(user_id);
      servant_upcall.user_id (&poa_current_impl.object_id());

      // If the POA has the RETAIN policy, the POA looks in the Active
      // Object Map to find if there is a servant associated with the
      // Object Id value from the request. If such a servant exists, the
      // POA invokes the appropriate method on the servant.
      PortableServer::Servant servant = 0;
      TAO_Active_Object_Map_Entry *active_object_map_entry = 0;
      int result = this->active_object_map_->
        find_servant_using_system_id_and_user_id (system_id,
                                                  user_id,
                                                  servant,
                                                  active_object_map_entry);


      if (result == 0)
        {
          servant_upcall.active_object_map_entry (active_object_map_entry);

          // Increment the reference count.
          servant_upcall.increment_servant_refcount ();
        }

      return servant;
    }

    int
    ServantRetentionStrategyRetain::find_servant_priority (
        const PortableServer::ObjectId &system_id,
        CORBA::Short &priority
        ACE_ENV_ARG_DECL)
    {
      PortableServer::ObjectId user_id;
      // If we have the RETAIN policy, convert/transform from system id to
      // user id.
      if (this->active_object_map_->
          find_user_id_using_system_id (system_id,
                                        user_id) != 0)
        {
          ACE_THROW_RETURN (CORBA::OBJ_ADAPTER (),
                            -1);
        }

      // If the POA has the RETAIN policy, the POA looks in the Active
      // Object Map to find if there is a servant associated with the
      // Object Id value from the request. If such a servant exists, the
      // POA invokes the appropriate method on the servant.
      PortableServer::Servant servant = 0;
      TAO_Active_Object_Map_Entry *active_object_map_entry = 0;
      int result = this->active_object_map_->
        find_servant_using_system_id_and_user_id (system_id,
                                                  user_id,
                                                  servant,
                                                  active_object_map_entry);

      if (result == 0)
        {
          priority = active_object_map_entry->priority_;
          return 0;
        }

      return -1;
    }

    int
    ServantRetentionStrategyRetain::is_servant_in_map (
      PortableServer::Servant servant,
      int &wait_occurred_restart_call)
    {
      int deactivated = 0;
      int servant_in_map =
        this->active_object_map_->is_servant_in_map (servant,
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
                            ACE_TEXT ("(%t) TAO_Root_POA::is_servant_in_map: waiting for servant to deactivate\n")));

              // We are going to wait on this condition variable; the POA
              // state may change by the time we get the lock again.
              // Therefore, indicate to the caller that all conditions
              // need to be checked again.
              wait_occurred_restart_call = 1;

              ++this->waiting_servant_deactivation_;

              if (this->poa_->object_adapter ().enable_locking ())
                this->poa_->servant_deactivation_condition ().wait ();

              --this->waiting_servant_deactivation_;

              return 0;
            }
          else
            {
              return 1;
            }
        }
    }

    int
    ServantRetentionStrategyRetain::is_user_id_in_map (
      const PortableServer::ObjectId &id,
      CORBA::Short priority,
      int &priorities_match,
      int &wait_occurred_restart_call)
    {
      int deactivated = 0;
      int user_id_in_map =
        this->active_object_map_->is_user_id_in_map (id,
                                                      priority,
                                                      priorities_match,
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
                            ACE_TEXT ("(%t) TAO_Root_POA::is_user_id_in_map: waiting for servant to deactivate\n")));

              // We are going to wait on this condition variable; the POA
              // state may change by the time we get the lock again.
              // Therefore, indicate to the caller that all conditions
              // need to be checked again.
              wait_occurred_restart_call = 1;

              ++this->waiting_servant_deactivation_;

              if (this->poa_->object_adapter ().enable_locking ())
                this->poa_->servant_deactivation_condition ().wait ();

              --this->waiting_servant_deactivation_;

              return 0;
            }
          else
            {
              return 1;
            }
        }
    }

    CORBA::ULong
    ServantRetentionStrategyRetain::waiting_servant_deactivation (void) const
    {
      return waiting_servant_deactivation_;
    }

    void
    ServantRetentionStrategyRetain::deactivate_all_objects (
      ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy))
    {
      // If the etherealize_objects parameter is TRUE, the POA has the
      // RETAIN policy, and a servant manager is registered with the POA,
      // the etherealize operation on the servant manager will be called
      // for each active object in the Active Object Map. The apparent
      // destruction of the POA occurs before any calls to etherealize are
      // made.  Thus, for example, an etherealize method that attempts to
      // invoke operations on the POA will receive the OBJECT_NOT_EXIST
      // exception.

      // We must copy the map entries into a separate place since we
      // cannot remove entries while iterating through the map.
      ACE_Array_Base<TAO_Active_Object_Map_Entry *> map_entries
        (this->active_object_map_->current_size ());

      size_t counter = 0;
      TAO_Active_Object_Map::user_id_map::iterator end
        = this->active_object_map_->user_id_map_->end ();

      for (TAO_Active_Object_Map::user_id_map::iterator iter
             = this->active_object_map_->user_id_map_->begin ();
           iter != end;
           ++iter)
        {
          TAO_Active_Object_Map::user_id_map::value_type map_pair = *iter;
          TAO_Active_Object_Map_Entry *active_object_map_entry = map_pair.second ();

          if (!active_object_map_entry->deactivated_)
            {
              map_entries[counter] = active_object_map_entry;
              ++counter;
            }
        }

      for (size_t i = 0;
           i < counter;
           ++i)
        {
          this->deactivate_map_entry (map_entries[i]
                                      ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }
    }

    PortableServer::ObjectId *
    ServantRetentionStrategyRetain::servant_to_user_id (
      PortableServer::Servant servant
      ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::ServantNotActive,
                         PortableServer::POA::WrongPolicy))
    {
      // This operation requires the RETAIN and either the UNIQUE_ID or
      // IMPLICIT_ACTIVATION policies; if not present, the WrongPolicy
      // exception is raised.
      if (!((!this->poa_->allow_multiple_activations ()
                || this->poa_->allow_implicit_activation ())))
        {
          ACE_THROW_RETURN (PortableServer::POA::WrongPolicy (),
                            0);
        }

      /**
       * If the POA has both the RETAIN and the UNIQUE_ID policy and the
       * specified servant is active, the Object Id associated with that
       * servant is returned.
       *
       * If the POA has both the RETAIN and the IMPLICIT_ACTIVATION policy and
       * either the POA has the MULTIPLE_ID policy or the specified servant is
       * not active, the servant is activated using a POA-generated Object Id
       * and the Interface Id associated with the servant, and that Object Id
       * is returned.
       *
       */

      // If the POA has the UNIQUE_ID policy and the specified servant is
      // active, the Object Id associated with that servant is returned.
      PortableServer::ObjectId_var user_id;
      if (!this->poa_->allow_multiple_activations () &&
          this->active_object_map_->
          find_user_id_using_servant (servant,
                                      user_id.out ()) != -1)
        {
          return user_id._retn ();
        }

      // If the POA has the IMPLICIT_ACTIVATION policy and either the POA
      // has the MULTIPLE_ID policy or the specified servant is not
      // active, the servant is activated using a POA-generated Object Id
      // and the Interface Id associated with the servant, and that Object
      // Id is returned.
      if (this->poa_->allow_implicit_activation ())
        {
          // If we reach here, then we either have the MULTIPLE_ID policy
          // or we have the UNIQUE_ID policy and we are not in the active
          // object map.
          PortableServer::ObjectId_var user_id;
          if (this->active_object_map_->
              bind_using_system_id_returning_user_id (servant,
                                                      this->poa_->server_priority (),
                                                      user_id.out ()) != 0)
            {
              ACE_THROW_RETURN (CORBA::OBJ_ADAPTER (),
                                0);
            }

          //
          // Everything is finally ok
          //

          // Inform the custom servant dispatching (CSD) strategy that the
          // sevant is activated.
          this->poa_->servant_activated_hook (servant,
                                              user_id.in ()
                                              ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);

          // ATTENTION: Trick locking here, see class header for details
          Non_Servant_Upcall non_servant_upcall (*this->poa_);
          ACE_UNUSED_ARG (non_servant_upcall);

          // If this operation causes the object to be activated, _add_ref
          // is invoked at least once on the Servant argument before
          // returning. Otherwise, the POA does not increment or decrement
          // the reference count of the Servant passed to this function.
          servant->_add_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);

          return user_id._retn ();
        }

      /*
       * Otherwise, the ServantNotActive exception is raised.
       */
      ACE_THROW_RETURN (PortableServer::POA::ServantNotActive (),
                        0);
    }

    PortableServer::ObjectId *
    ServantRetentionStrategyRetain::servant_to_system_id_i (
      PortableServer::Servant servant,
      CORBA::Short &priority
      ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::ServantNotActive,
                         PortableServer::POA::WrongPolicy))
    {
      // This operation requires the RETAIN and either the UNIQUE_ID or
      // IMPLICIT_ACTIVATION policies; if not present, the WrongPolicy
      // exception is raised.
      if (!((!this->poa_->allow_multiple_activations ()
                || this->poa_->allow_implicit_activation ())))
        {
          ACE_THROW_RETURN (PortableServer::POA::WrongPolicy (),
                            0);
        }

      // This operation has three possible behaviors.

      // If the POA has the UNIQUE_ID policy and the specified servant is
      // active, the Object Id associated with that servant is returned.
      PortableServer::ObjectId_var system_id;
      if (!this->poa_->allow_multiple_activations () &&
          this->active_object_map_->
          find_system_id_using_servant (servant,
                                        system_id.out (),
                                        priority) != -1)
        {
          return system_id._retn ();
        }

      // If the POA has the IMPLICIT_ACTIVATION policy and either the POA
      // has the MULTIPLE_ID policy or the specified servant is not
      // active, the servant is activated using a POA-generated Object Id
      // and the Interface Id associated with the servant, and that Object
      // Id is returned.
      if (this->poa_->allow_implicit_activation ())
        {
          // If we reach here, then we either have the MULTIPLE_ID policy
          // or we have the UNIQUE_ID policy and we are not in the active
          // object map.
          PortableServer::ObjectId_var system_id;
          if (this->active_object_map_->
              bind_using_system_id_returning_system_id (servant,
                                                        priority,
                                                        system_id.out ()) != 0)
            {
              ACE_THROW_RETURN (CORBA::OBJ_ADAPTER (),
                                0);
            }

          //
          // Everything is finally ok
          //

          // Inform the custom servant dispatching (CSD) strategy that the
          // sevant is activated.
          this->poa_->servant_activated_hook (servant,
                                              system_id.in ()
                                              ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);

          // ATTENTION: Trick locking here, see class header for details
          Non_Servant_Upcall non_servant_upcall (*this->poa_);
          ACE_UNUSED_ARG (non_servant_upcall);

          // If this operation causes the object to be activated, _add_ref
          // is invoked at least once on the Servant argument before
          // returning. Otherwise, the POA does not increment or decrement
          // the reference count of the Servant passed to this function.
          servant->_add_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);

          return system_id._retn ();
        }

      // Otherwise, the ServantNotActive exception is raised.
      ACE_THROW_RETURN (PortableServer::POA::ServantNotActive (),
                        0);
    }

    CORBA::Object_ptr
    ServantRetentionStrategyRetain::servant_to_reference (
      PortableServer::Servant servant
      ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                      PortableServer::POA::ServantNotActive,
                      PortableServer::POA::WrongPolicy))
    {
      // Note: The allocation of an Object Id value and installation in
      // the Active Object Map caused by implicit activation may actually
      // be deferred until an attempt is made to externalize the
      // reference. The real requirement here is that a reference is
      // produced that will behave appropriately (that is, yield a
      // consistent Object Id value when asked politely).
      CORBA::Short priority =
        this->poa_->server_priority ();

      PortableServer::ObjectId_var system_id =
        this->servant_to_system_id_i (servant,
                                      priority
                                      ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::Object::_nil ());

      PortableServer::ObjectId user_id;

      // This operation requires the RETAIN, therefore don't worry about
      // the NON_RETAIN case.
      if (this->active_object_map_->
          find_user_id_using_system_id (system_id.in (),
                                        user_id) != 0)
        {
          ACE_THROW_RETURN (CORBA::OBJ_ADAPTER (),
                            CORBA::Object::_nil ());
        }

      // Remember params for potentially invoking <key_to_object> later.
      this->poa_->key_to_object_params_.set (
        system_id,
        servant->_interface_repository_id (),
        servant,
        1,
        priority,
        true);

      // Ask the ORT to create the object.
      // @@NOTE:There is a possible deadlock lurking here. We held the
      // lock, and we are possibly trying to make a call into the
      // application code. Think what would happen if the app calls us
      // back. We need to get to this at some point.
      return this->poa_->invoke_key_to_object_helper_i (
              servant->_interface_repository_id (),
              user_id
              ACE_ENV_ARG_PARAMETER);
    }

    PortableServer::ObjectId *
    ServantRetentionStrategyRetain::activate_object (
      PortableServer::Servant servant,
      CORBA::Short priority,
      int &wait_occurred_restart_call
      ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::ServantAlreadyActive,
                         PortableServer::POA::WrongPolicy))
    {
      if (!this->poa_->has_system_id ())
        {
          ACE_THROW_RETURN (PortableServer::POA::WrongPolicy (),
                            0);
        }

      bool may_activate =
        this->poa_->is_servant_activation_allowed (servant, wait_occurred_restart_call);

      if (!may_activate)
        {
          if (wait_occurred_restart_call)
            {
              return 0;
            }
          else
            {
              ACE_THROW_RETURN (PortableServer::POA::ServantAlreadyActive (),
                                0);
            }
        }

      // Otherwise, the activate_object operation generates an Object Id
      // and enters the Object Id and the specified servant in the Active
      // Object Map. The Object Id is returned.
      PortableServer::ObjectId_var user_id;
      if (this->active_object_map_->
          bind_using_system_id_returning_user_id (servant,
                                                  priority,
                                                  user_id.out ()) != 0)
        {
          ACE_THROW_RETURN (CORBA::OBJ_ADAPTER (),
                            0);
        }

      //
      // Everything is finally ok
      //

      // Inform the custom servant dispatching (CSD) strategy that the
      // sevant is activated.
      this->poa_->servant_activated_hook (servant,
                                          user_id.in ()
                                          ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      // ATTENTION: Trick locking here, see class header for details
      Non_Servant_Upcall non_servant_upcall (*this->poa_);
      ACE_UNUSED_ARG (non_servant_upcall);

      // The implementation of activate_object will invoke _add_ref at
      // least once on the Servant argument before returning. When the POA
      // no longer needs the Servant, it will invoke _remove_ref on it the
      // same number of times.
      servant->_add_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      return user_id._retn ();
    }

#if !defined (CORBA_E_MICRO)
    void
    ServantRetentionStrategyRetain::activate_object_with_id (
      const PortableServer::ObjectId &id,
      PortableServer::Servant servant,
      CORBA::Short priority,
      int &wait_occurred_restart_call
      ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::ServantAlreadyActive,
                         PortableServer::POA::ObjectAlreadyActive,
                         PortableServer::POA::WrongPolicy))
    {
      // If the POA has the SYSTEM_ID policy and it detects that the
      // Object Id value was not generated by the system or for this POA,
      // the activate_object_with_id operation may raise the BAD_PARAM
      // system exception.  An ORB is not required to detect all such
      // invalid Object Id values, but a portable application must not
      // invoke activate_object_with_id on a POA that has the SYSTEM_ID
      // policy with an Object Id value that was not previously generated
      // by the system for that POA, or, if the POA also has the
      // PERSISTENT policy, for a previous instantiation of the same POA.
      if (this->poa_->has_system_id () &&
          !this->poa_->is_poa_generated_id (id))
        {
          ACE_THROW (CORBA::BAD_PARAM ());
        }

      // If the CORBA object denoted by the Object Id value is already
      // active in this POA (there is a servant bound to it in the Active
      // Object Map), the ObjectAlreadyActive exception is raised.
      int priorities_match = 1;
      int result =
        this->is_user_id_in_map (id,
                                 priority,
                                 priorities_match,
                                 wait_occurred_restart_call);

    // @johnny the implementation is not complete, this does the spec also say
    // If the POA has the UNIQUE_ID policy and the servant is already
    // in the Active Object Map, the ServantAlreadyActive exception is raised.
      if (result)
        {
          ACE_THROW (PortableServer::POA::ObjectAlreadyActive ());
        }
      else if (wait_occurred_restart_call)
        {
          // We ended up waiting on a condition variable, the POA state
          // may have changed while we are waiting.  Therefore, we need to
          // restart this call.
          return;
        }

      // If the activate_object_with_id_and_priority operation is invoked
      // with a different priority to an earlier invocation of one of the
      // create reference with priority operations, for the same object,
      // then the ORB shall raise a BAD_INV_ORDER system exception (with a
      // Standard Minor Exception Code of 1). If the priority value is the
      // same then the ORB shall return SUCCESS.
      if (!priorities_match)
        {
          ACE_THROW (CORBA::BAD_INV_ORDER (CORBA::OMGVMCID | 1,
                                           CORBA::COMPLETED_NO));
        }

      bool may_activate =
        this->poa_->is_servant_activation_allowed (servant, wait_occurred_restart_call);

      if (!may_activate)
        {
          if (wait_occurred_restart_call)
            {
              return;
            }
          else
            {
              ACE_THROW (PortableServer::POA::ServantAlreadyActive ());
            }
        }

      // Otherwise, the activate_object_with_id operation enters an
      // association between the specified Object Id and the specified
      // servant in the Active Object Map.
      if (this->active_object_map_->bind_using_user_id (servant,
                                                         id,
                                                         priority) != 0)
        {
          ACE_THROW (CORBA::OBJ_ADAPTER ());
        }

      //
      // Everything is finally ok
      //

      // Inform the custom servant dispatching (CSD) strategy that the
      // sevant is activated.
      this->poa_->servant_activated_hook (servant,
                                          id
                                          ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      // ATTENTION: Trick locking here, see class header for details
      Non_Servant_Upcall non_servant_upcall (*this->poa_);
      ACE_UNUSED_ARG (non_servant_upcall);

      // The implementation of activate_object_with_id will invoke
      // _add_ref at least once on the Servant argument before
      // returning. When the POA no longer needs the Servant, it will
      // invoke _remove_ref on it the same number of times.
      servant->_add_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }
#endif

    CORBA::Object_ptr
    ServantRetentionStrategyRetain::create_reference (
      const char *intf,
      CORBA::Short priority
      ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy))
    {
      // This operation creates an object reference that encapsulates a
      // POA-generated Object Id value and the specified interface
      // repository id. This operation does not cause an activation to
      // take place. The resulting reference may be passed to clients, so
      // that subsequent requests on those references will cause the
      // appropriate servant manager to be invoked, if one is
      // available. The generated Object Id value may be obtained by
      // invoking POA::reference_to_id with the created reference.

      PortableServer::ObjectId_var system_id;
      PortableServer::ObjectId user_id;

      if (this->active_object_map_->
          bind_using_system_id_returning_system_id (0,
                                                    priority,
                                                    system_id.out ()) != 0)
        {
          ACE_THROW_RETURN (CORBA::OBJ_ADAPTER (),
                            CORBA::Object::_nil ());
        }

      // Find user id from system id.
      if (this->active_object_map_->
          find_user_id_using_system_id (system_id.in (),
                                        user_id) != 0)
        {
          ACE_THROW_RETURN (CORBA::OBJ_ADAPTER (),
                            CORBA::Object::_nil ());
        }

      // Remember params for potentially invoking <key_to_object> later.
      this->poa_->key_to_object_params_.set (system_id,
                                       intf,
                                       0,
                                       1,
                                       priority,
                                       true);

      return this->poa_->invoke_key_to_object_helper_i (intf,
                                                        user_id
                                                        ACE_ENV_ARG_PARAMETER);

    }

    CORBA::Object_ptr
    ServantRetentionStrategyRetain::create_reference_with_id (
      const PortableServer::ObjectId &oid,
      const char *intf,
      CORBA::Short priority
      ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException))
    {
      // This operation creates an object reference that encapsulates the
      // specified Object Id and interface repository Id values. This
      // operation does not cause an activation to take place.  The
      // resulting reference may be passed to clients, so that subsequent
      // requests on those references will cause the object to be
      // activated if necessary, or the default servant used, depending on
      // the applicable policies.

      PortableServer::Servant servant = 0;
      PortableServer::ObjectId_var system_id;

      // @@ We need something that can find the system id using
      // appropriate strategy, at the same time, return the servant if
      // one is available.  Before we have that function,
      // <create_reference_with_id_i> basically generates broken
      // collocated object when DIRECT collocation strategy is used.

      if (this->active_object_map_->
          find_system_id_using_user_id (oid,
                                        priority,
                                        system_id.out ()) != 0)
        {
          ACE_THROW_RETURN (CORBA::OBJ_ADAPTER (),
                            CORBA::Object::_nil ());
        }

      // Remember params for potentially invoking <key_to_object> later.
      this->poa_->key_to_object_params_.set (system_id,
                                       intf,
                                       servant,
                                       1,
                                       priority,
                                       true);

      return this->poa_->invoke_key_to_object_helper_i (intf,
                                                  oid
                                                  ACE_ENV_ARG_PARAMETER);
    }

    int
    ServantRetentionStrategyRetain::rebind_using_user_id_and_system_id (
      PortableServer::Servant servant,
      const PortableServer::ObjectId &user_id,
      const PortableServer::ObjectId &system_id,
      TAO::Portable_Server::Servant_Upcall &servant_upcall)
    {
      TAO_Active_Object_Map_Entry *entry = 0;
      int result = this->active_object_map_->
        rebind_using_user_id_and_system_id (servant,
                                            user_id,
                                            system_id,
                                            entry);
      servant_upcall.active_object_map_entry(entry);

      return result;
    }

    CORBA::Boolean
    ServantRetentionStrategyRetain::servant_has_remaining_activations (
      PortableServer::Servant servant)
    {
      return this->active_object_map_->remaining_activations (servant);
    }


    ::PortableServer::ServantRetentionPolicyValue
    ServantRetentionStrategyRetain::type() const
    {
      return ::PortableServer::RETAIN;
    }

  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
