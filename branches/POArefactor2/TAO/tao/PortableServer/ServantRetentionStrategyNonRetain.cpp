// -*- C++ -*-

//=============================================================================
/**
 *  @file    ServantRetentionStrategyNonRetain.cpp
 *
 *  $Id$
 *
 */
//=============================================================================

#include "tao/PortableServer/ServantRetentionStrategyNonRetain.h"
#include "tao/PortableServer/RequestProcessingStrategy.h"
#include "tao/PortableServer/ImplicitActivationStrategy.h"
#include "tao/PortableServer/IdUniquenessStrategy.h"
#include "tao/PortableServer/LifespanStrategy.h"
#include "tao/PortableServer/Non_Servant_Upcall.h"
#include "tao/PortableServer/Servant_Upcall.h"
#include "tao/PortableServer/POA_Current_Impl.h"
#include "tao/PortableServer/POA.h"
#include "tao/debug.h"
#include "tao/TSS_Resources.h"

// @todo Check the usage of non_retain and request_processing (default servant especially)

ACE_RCSID (PortableServer,
           Servant_Retention_Strategy,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    Non_Retain_Servant_Retention_Strategy::~Non_Retain_Servant_Retention_Strategy (void)
    {
    }

    void
    Non_Retain_Servant_Retention_Strategy::deactivate_object (
      const PortableServer::ObjectId &id
      ACE_ENV_ARG_DECL)
    {
      // When using Non_Retain we don't have an active object map and we just
      // can't deactivate any object
      ACE_UNUSED_ARG (id);

      ACE_THROW (PortableServer::POA::WrongPolicy ());
    }

    TAO_Active_Object_Map*
    Non_Retain_Servant_Retention_Strategy::get_aom() const
    {
      return 0;
    }

    PortableServer::Servant
    Non_Retain_Servant_Retention_Strategy::reference_to_servant (
      CORBA::Object_ptr reference
      ACE_ENV_ARG_DECL)
    {
      ACE_UNUSED_ARG (reference);

      // @Johnny, shouldn't we check here whether reference belongs to this POA?

     // Always try the request processing strategy
     PortableServer::Servant servant = this->poa_->active_policy_strategies().request_processing_strategy()->get_servant (ACE_ENV_SINGLE_ARG_PARAMETER);
     ACE_CHECK_RETURN (0);

      if (servant != 0)
        {
          // A recursive thread lock without using a recursive thread
          // lock.  Non_Servant_Upcall has a magic constructor and
          // destructor.  We unlock the Object_Adapter lock for the
          // duration of the servant activator upcalls; reacquiring once
          // the upcalls complete.  Even though we are releasing the lock,
          // other threads will not be able to make progress since
          // <Object_Adapter::non_servant_upcall_in_progress_> has been
          // set.
          TAO::Portable_Server::Non_Servant_Upcall non_servant_upcall (*this->poa_);
          ACE_UNUSED_ARG (non_servant_upcall);

          // The POA invokes _add_ref once on the Servant before returning
          // it. If the application uses reference counting, the caller of
          // id_to_servant is responsible for invoking _remove_ref once on
          // the returned Servant when it is finished with it. A
          // conforming caller need not invoke _remove_ref on the returned
          // Servant if the type of the Servant uses the default reference
          // counting inherited from ServantBase.
          servant->_add_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);

          return servant;
        }
      else
        {
          // Otherwise the ObjectNotActive exception is raised.
          // @todo, is this the correct exception, the spec is not clear
          ACE_THROW_RETURN (PortableServer::POA::ObjectNotActive (),
                            0);
        }
    }

    PortableServer::ObjectId *
    Non_Retain_Servant_Retention_Strategy::reference_to_id (
      CORBA::Object_ptr reference,
      PortableServer::ObjectId system_id
      ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongAdapter,
                         PortableServer::POA::WrongPolicy))
    {
      ACE_UNUSED_ARG (reference);

      // The system id is the id (and no conversion/transformation is
      // needed).
      CORBA::OctetSeq *id = 0;
      ACE_NEW_THROW_EX (id,
                        PortableServer::ObjectId (system_id),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (0);

      return id;
    }

    PortableServer::Servant
    Non_Retain_Servant_Retention_Strategy::id_to_servant (
      const PortableServer::ObjectId &id
      ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::ObjectNotActive,
                         PortableServer::POA::WrongPolicy))
    {
      ACE_UNUSED_ARG (id);

      // Get the default servant, in case we have a not correct request_processing
      // strategy we will get an exception
      PortableServer::Servant servant = 0;
      servant = this->poa_->active_policy_strategies().request_processing_strategy()->get_servant (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      if (servant != 0)
        {
          // A recursive thread lock without using a recursive thread
          // lock.  Non_Servant_Upcall has a magic constructor and
          // destructor.  We unlock the Object_Adapter lock for the
          // duration of the servant activator upcalls; reacquiring once
          // the upcalls complete.  Even though we are releasing the lock,
          // other threads will not be able to make progress since
          // <Object_Adapter::non_servant_upcall_in_progress_> has been
          // set.
          TAO::Portable_Server::Non_Servant_Upcall non_servant_upcall (*this->poa_);
          ACE_UNUSED_ARG (non_servant_upcall);

          // The POA invokes _add_ref once on the Servant before returning
          // it. If the application uses reference counting, the caller of
          // id_to_servant is responsible for invoking _remove_ref once on
          // the returned Servant when it is finished with it. A
          // conforming caller need not invoke _remove_ref on the returned
          // Servant if the type of the Servant uses the default reference
          // counting inherited from ServantBase.
          servant->_add_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);

          return servant;
        }
      else
        {
          /*
           * If using default servant request processing strategy but
           * no default servant is available, we will raise the
           * OBJ_ADAPTER system exception.
           */
          ACE_THROW_RETURN (CORBA::OBJ_ADAPTER (),
                            0);
        }
    }

    CORBA::Object_ptr
    Non_Retain_Servant_Retention_Strategy::id_to_reference (
      const PortableServer::ObjectId &id
      ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::ObjectNotActive,
                         PortableServer::POA::WrongPolicy))
    {
      ACE_UNUSED_ARG (id);

      ACE_THROW_RETURN (PortableServer::POA::WrongPolicy (),
                        CORBA::Object::_nil ());
    }

    TAO_SERVANT_LOCATION
    Non_Retain_Servant_Retention_Strategy::locate_servant (
      const PortableServer::ObjectId &system_id,
      PortableServer::Servant &servant
      ACE_ENV_ARG_DECL)
    {
      return this->poa_->active_policy_strategies().request_processing_strategy()->locate_servant (system_id, servant);
    }

    PortableServer::Servant
    Non_Retain_Servant_Retention_Strategy::locate_servant (const char *operation,
                      const PortableServer::ObjectId &system_id,
                       TAO::Portable_Server::Servant_Upcall &servant_upcall,
                         TAO::Portable_Server::POA_Current_Impl &poa_current_impl,
                         int &wait_occurred_restart_call
                         ACE_ENV_ARG_DECL)
    {
      // We have the NON_RETAIN policy, user id is the system id.

      // Smartly copy all the data; <poa_current_impl.object_id_> does
      // not own the data.
      poa_current_impl.replace_object_id (system_id);

      servant_upcall.user_id (&system_id);

      PortableServer::Servant servant = 0;

      // Not found a servant, try the request processing strategy
      servant = this->poa_->active_policy_strategies().request_processing_strategy()->locate_servant (operation, system_id, servant_upcall, poa_current_impl, wait_occurred_restart_call);

      if (servant == 0)
      {
        // Failure
        ACE_THROW_RETURN (CORBA::OBJ_ADAPTER (),
                        0);
      }

      return servant;
    }

    int
    Non_Retain_Servant_Retention_Strategy::is_servant_in_map (
      PortableServer::Servant servant,
      int &wait_occurred_restart_call)
    {
      ACE_UNUSED_ARG (servant);
      ACE_UNUSED_ARG (wait_occurred_restart_call);

      return 0;
    }

    CORBA::ULong
    Non_Retain_Servant_Retention_Strategy::waiting_servant_deactivation (void) const
    {
      return 0;
    }

    void
    Non_Retain_Servant_Retention_Strategy::deactivate_all_objects (
      CORBA::Boolean etherealize_objects
      ACE_ENV_ARG_DECL_NOT_USED)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy))
    {
      ACE_UNUSED_ARG (etherealize_objects);
    }

    void
    Non_Retain_Servant_Retention_Strategy::cleanup_servant (
      TAO_Active_Object_Map::Map_Entry *active_object_map_entry
      ACE_ENV_ARG_DECL_NOT_USED)
    {
      ACE_UNUSED_ARG (active_object_map_entry);
    }

    PortableServer::ObjectId *
    Non_Retain_Servant_Retention_Strategy::servant_to_id (
      PortableServer::Servant servant
      ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::ServantNotActive,
                         PortableServer::POA::WrongPolicy))
    {
      /*
       * If the POA has the USE_DEFAULT_SERVANT policy, the servant specified
       * is the default servant, and the operation is being invoked in the
       * context of executing a request on the default servant, then the
       * ObjectId associated with the current invocation is returned.
       */

      PortableServer::Servant default_servant = 0;
      default_servant = this->poa_->active_policy_strategies().request_processing_strategy()->get_servant (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      if (default_servant != 0 &&
          default_servant == servant)
        {
          // If they are the same servant, then check if we are in an
          // upcall.
          TAO::Portable_Server::POA_Current_Impl *poa_current_impl =
            static_cast <TAO::Portable_Server::POA_Current_Impl *>
                        (TAO_TSS_RESOURCES::instance ()->poa_current_impl_);
          // If we are in an upcall on the default servant, return the
          // ObjectId associated with the current invocation.
          if (poa_current_impl != 0 &&
              servant == poa_current_impl->servant ())
            {
              return poa_current_impl->get_object_id (ACE_ENV_SINGLE_ARG_PARAMETER);
            }
        }
      else
        {
          /*
           * If no default servant is available, the POA will raise the
            * OBJ_ADAPTER system exception.
            */
          ACE_THROW_RETURN (CORBA::OBJ_ADAPTER (),
                            0);
        }
    }

    CORBA::Object_ptr
    Non_Retain_Servant_Retention_Strategy::servant_to_reference (
      PortableServer::Servant servant
      ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                      PortableServer::POA::ServantNotActive,
                      PortableServer::POA::WrongPolicy))
    {
      ACE_UNUSED_ARG (servant);

      ACE_THROW_RETURN (PortableServer::POA::WrongPolicy (),
                        CORBA::Object::_nil ());
    }

    PortableServer::ObjectId *
    Non_Retain_Servant_Retention_Strategy::activate_object (
      PortableServer::Servant servant,
      CORBA::Short priority,
      int &wait_occurred_restart_call
      ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::ServantAlreadyActive,
                         PortableServer::POA::WrongPolicy))
    {
      ACE_UNUSED_ARG (servant);
      ACE_UNUSED_ARG (priority);
      ACE_UNUSED_ARG (wait_occurred_restart_call);

      ACE_THROW_RETURN (PortableServer::POA::WrongPolicy (),
                        0);
    }

    void
    Non_Retain_Servant_Retention_Strategy::activate_object_with_id (
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
      ACE_UNUSED_ARG (id);
      ACE_UNUSED_ARG (servant);
      ACE_UNUSED_ARG (priority);
      ACE_UNUSED_ARG (wait_occurred_restart_call);

      ACE_THROW (PortableServer::POA::WrongPolicy ());
    }

    CORBA::Object_ptr
    Non_Retain_Servant_Retention_Strategy::create_reference (
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

      // Otherwise, it is the NON_RETAIN policy.  Therefore, any ol'
      // object id will do (even an empty one).
      PortableServer::ObjectId *sys_id;
      ACE_NEW_THROW_EX (sys_id,
                        PortableServer::ObjectId,
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (CORBA::Object::_nil ());

      system_id = sys_id;

      // User id is the same as system id.
      user_id = system_id.in ();

      // Remember params for potentially invoking <key_to_object> later.
      this->poa_->key_to_object_params_.set (system_id,
                                             intf,
                                             0,
                                             1,
                                             priority);

      return this->poa_->invoke_key_to_object_helper_i (intf,
                                                  user_id
                                                  ACE_ENV_ARG_PARAMETER);
    }

    CORBA::Object_ptr
    Non_Retain_Servant_Retention_Strategy::create_reference_with_id (
      const PortableServer::ObjectId &oid,
      const char *intf,
      CORBA::Short priority
      ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         PortableServer::POA::WrongPolicy))
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

      // Otherwise, it is the NON_RETAIN policy.  Therefore, user id
      // is the same as system id.
      PortableServer::ObjectId *sys_id;
      ACE_NEW_THROW_EX (sys_id,
                        PortableServer::ObjectId (oid),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (CORBA::Object::_nil ());

      system_id = sys_id;

      // Remember params for potentially invoking <key_to_object> later.
      this->poa_->key_to_object_params_.set (system_id,
                                       intf,
                                       servant,
                                       1,
                                       priority);

      return this->poa_->invoke_key_to_object_helper_i (intf,
                                                  oid
                                                  ACE_ENV_ARG_PARAMETER);
    }

  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

