// -*- C++ -*-

//=============================================================================
/**
 *  @file    Request_Processing_Strategy.cpp
 *
 *  $Id$
 *
 */
//=============================================================================

#include "Request_Processing_Strategy.h"
#include "Lifespan_Strategy.h"
#include "ServantActivatorC.h"
#include "ServantLocatorC.h"
#include "tao/Server_Strategy_Factory.h"
#include "tao/ORB_Core.h"
#include "Non_Servant_Upcall.h"
#include "POA.h"
#include "POA_Current_Impl.h"
#include "Servant_Upcall.h"
//#include "Servant_Retention_Strategy.h"
#include "tao/TSS_Resources.h"

ACE_RCSID (PortableServer,
           Request_Processing,
           "$Id$")


#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    Request_Processing_Strategy::~Request_Processing_Strategy()
    {
    }

    void
    Request_Processing_Strategy::strategy_init(TAO_POA *poa)
    {
      poa_ = poa;
      // dependent on type create the correct strategy.
    }

    AOM_Only_Request_Processing_Strategy::AOM_Only_Request_Processing_Strategy()
    {
    }

    AOM_Only_Request_Processing_Strategy::~AOM_Only_Request_Processing_Strategy()
    {
    }

    void
    AOM_Only_Request_Processing_Strategy::strategy_init(TAO_POA *poa)
    {
      Request_Processing_Strategy::strategy_init (poa);
    }

    PortableServer::Servant
    AOM_Only_Request_Processing_Strategy::reference_to_servant (
      CORBA::Object_ptr reference
      ACE_ENV_ARG_DECL)
    {
      return 0;
    }

    TAO_Active_Object_Map*
    AOM_Only_Request_Processing_Strategy::get_aom() const
    {
      return 0;
      //todo
      //return this->poa_->active_policy_strategies().servant_retention_strategy()->get_aom ();
    }

    void
    AOM_Only_Request_Processing_Strategy::set_user_id (
      const PortableServer::ObjectId &system_id,
      TAO::Portable_Server::Servant_Upcall &servant_upcall,
      TAO::Portable_Server::POA_Current_Impl &poa_current_impl
      ACE_ENV_ARG_DECL)
    {
      PortableServer::ObjectId object_id;
      if (this->get_aom ()->
          find_user_id_using_system_id (system_id,
                                        object_id) != 0)
        {
          ACE_THROW (CORBA::OBJ_ADAPTER ());
        }

      poa_current_impl.object_id (object_id);
      servant_upcall.user_id (&object_id);
    }

    PortableServer::ServantManager_ptr
    AOM_Only_Request_Processing_Strategy::get_servant_manager (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableServer::POA::WrongPolicy))
    {
      ACE_THROW_RETURN (PortableServer::POA::WrongPolicy (),
                        PortableServer::ServantManager::_nil ());
    }

    void
    AOM_Only_Request_Processing_Strategy::set_servant_manager (PortableServer::ServantManager_ptr imgr
                         ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableServer::POA::WrongPolicy))
    {
      ACE_UNUSED_ARG (imgr);

      ACE_THROW (PortableServer::POA::WrongPolicy ());
    }

    PortableServer::Servant
    AOM_Only_Request_Processing_Strategy::get_servant (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableServer::POA::WrongPolicy))
    {
      ACE_THROW_RETURN (PortableServer::POA::WrongPolicy (),
                        0);
    }

    void
    AOM_Only_Request_Processing_Strategy::set_servant (PortableServer::Servant servant
                 ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableServer::POA::WrongPolicy))
    {
      ACE_UNUSED_ARG (servant);

      ACE_THROW (PortableServer::POA::WrongPolicy ());
    }

    TAO_SERVANT_LOCATION
    AOM_Only_Request_Processing_Strategy::locate_servant (
      const PortableServer::ObjectId &system_id,
      PortableServer::Servant &servant
      ACE_ENV_ARG_DECL)
    {
        return TAO_SERVANT_NOT_FOUND;
    }

    PortableServer::Servant
    AOM_Only_Request_Processing_Strategy::locate_servant (const char *operation,
                        const PortableServer::ObjectId &system_id,
                        TAO::Portable_Server::Servant_Upcall &servant_upcall,
                        TAO::Portable_Server::POA_Current_Impl &poa_current_impl,
                        int &wait_occurred_restart_call
                        ACE_ENV_ARG_DECL)
    {
      // If the USE_ACTIVE_OBJECT_MAP_ONLY policy is in effect, the POA raises
      // the OBJECT_NOT_EXIST system exception.
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        0);
    }

    Default_Servant_Request_Processing_Strategy::~Default_Servant_Request_Processing_Strategy (void)
    {
      this->default_servant_ = 0;
    }

    PortableServer::Servant
    Default_Servant_Request_Processing_Strategy::reference_to_servant (
      CORBA::Object_ptr reference
      ACE_ENV_ARG_DECL)
    {
      return this->default_servant_.in ();
    }

    void
    Default_Servant_Request_Processing_Strategy::set_user_id (
      const PortableServer::ObjectId &system_id,
      TAO::Portable_Server::Servant_Upcall &servant_upcall,
      TAO::Portable_Server::POA_Current_Impl &poa_current_impl
      ACE_ENV_ARG_DECL)
    {
      // Just replace the object id in the POA Current with the system id
      poa_current_impl.replace_object_id (system_id);

      servant_upcall.user_id (&system_id);
    }

    PortableServer::ServantManager_ptr
    Default_Servant_Request_Processing_Strategy::get_servant_manager (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableServer::POA::WrongPolicy))
    {
      ACE_THROW_RETURN (PortableServer::POA::WrongPolicy (),
                        PortableServer::ServantManager::_nil ());
    }

    void
    Default_Servant_Request_Processing_Strategy::set_servant_manager (PortableServer::ServantManager_ptr imgr
                         ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableServer::POA::WrongPolicy))
    {
      ACE_UNUSED_ARG (imgr);

      ACE_THROW (PortableServer::POA::WrongPolicy ());
    }

    PortableServer::Servant
    Default_Servant_Request_Processing_Strategy::get_servant (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableServer::POA::WrongPolicy))
    {
      // This operation returns the default servant associated with the
      // POA.
      return this->default_servant_.in ();
    }

    void
    Default_Servant_Request_Processing_Strategy::set_servant (PortableServer::Servant servant
                 ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableServer::POA::WrongPolicy))
    {
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
          // A recursive thread lock without using a recursive thread
          // lock.  Non_Servant_Upcall has a magic constructor and
          // destructor.  We unlock the Object_Adapter lock for the
          // duration of the servant activator upcalls; reacquiring once
          // the upcalls complete.  Even though we are releasing the lock,
          // other threads will not be able to make progress since
          // <Object_Adapter::non_servant_upcall_in_progress_> has been
          // set.
          Non_Servant_Upcall non_servant_upcall (*this->poa_);
          ACE_UNUSED_ARG (non_servant_upcall);

          servant->_add_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK;
        }
    }

    TAO_SERVANT_LOCATION
    Default_Servant_Request_Processing_Strategy::locate_servant (
      const PortableServer::ObjectId &system_id,
      PortableServer::Servant &servant
      ACE_ENV_ARG_DECL)
    {
      ACE_UNUSED_ARG (system_id);
      ACE_UNUSED_ARG (servant);

      if (this->default_servant_.in () == 0)
        {
          return TAO_SERVANT_NOT_FOUND;
        }
      else
        {
          return TAO_DEFAULT_SERVANT;
        }
    }

    PortableServer::Servant
    Default_Servant_Request_Processing_Strategy::locate_servant (const char *operation,
                        const PortableServer::ObjectId &system_id,
                        TAO::Portable_Server::Servant_Upcall &servant_upcall,
                        TAO::Portable_Server::POA_Current_Impl &poa_current_impl,
                        int &wait_occurred_restart_call
                        ACE_ENV_ARG_DECL)
    {
      // If the POA has the USE_DEFAULT_SERVANT policy, a default servant
      // has been associated with the POA so the POA will invoke the
      // appropriate method on that servant. If no servant has been
      // associated with the POA, the POA raises the OBJ_ADAPTER system
      // exception.
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

    Servant_Manager_Request_Processing_Strategy::~Servant_Manager_Request_Processing_Strategy (void)
    {
    }

    PortableServer::Servant
    Servant_Manager_Request_Processing_Strategy::get_servant (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableServer::POA::WrongPolicy))
    {
      ACE_THROW_RETURN (PortableServer::POA::WrongPolicy (),
                        0);
    }

    void
    Servant_Manager_Request_Processing_Strategy::set_servant (PortableServer::Servant servant
                 ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableServer::POA::WrongPolicy))
    {
      ACE_UNUSED_ARG (servant);

      ACE_THROW (PortableServer::POA::WrongPolicy ());
    }

    Servant_Activator_Request_Processing_Strategy::Servant_Activator_Request_Processing_Strategy (void)
    {
    }

    Servant_Activator_Request_Processing_Strategy::~Servant_Activator_Request_Processing_Strategy (void)
    {
    }

    void
    Servant_Activator_Request_Processing_Strategy::strategy_init(TAO_POA *poa)
    {
      Request_Processing_Strategy::strategy_init (poa);
    }

    void
    Servant_Activator_Request_Processing_Strategy::set_user_id (
      const PortableServer::ObjectId &system_id,
      TAO::Portable_Server::Servant_Upcall &servant_upcall,
      TAO::Portable_Server::POA_Current_Impl &poa_current_impl
      ACE_ENV_ARG_DECL)
    {
      PortableServer::ObjectId object_id;
      if (this->get_aom ()->
          find_user_id_using_system_id (system_id,
                                        object_id) != 0)
        {
          ACE_THROW (CORBA::OBJ_ADAPTER ());
        }

      poa_current_impl.object_id (object_id);
      servant_upcall.user_id (&object_id);
    }

    PortableServer::Servant
    Servant_Activator_Request_Processing_Strategy::reference_to_servant (
      CORBA::Object_ptr reference
      ACE_ENV_ARG_DECL)
    {
      // Get the servant out of the active object map
      return 0;
    }

    TAO_Active_Object_Map*
    Servant_Activator_Request_Processing_Strategy::get_aom() const
    {
      return 0;
      // todo
      //return this->poa_->active_policy_strategies().servant_retention_strategy()->get_aom ();
    }


    PortableServer::ServantManager_ptr
    Servant_Activator_Request_Processing_Strategy::get_servant_manager (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableServer::POA::WrongPolicy))
    {
      return PortableServer::ServantManager::_duplicate (this->servant_activator_.in ());
    }

    void
    Servant_Activator_Request_Processing_Strategy::set_servant_manager (PortableServer::ServantManager_ptr imgr
                         ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableServer::POA::WrongPolicy))
    {
      // This operation sets the default servant manager associated with the
      // POA. This operation may only be invoked once after a POA has been
      // created. Attempting to set the servant manager after one has already
      // been set will result in the BAD_INV_ORDER system exception with
      // standard minor code 6 being raised (see 11.3.9.12 of the corba spec)
      if (!CORBA::is_nil (this->servant_activator_.in ()))
        {
          ACE_THROW (CORBA::BAD_INV_ORDER (CORBA::OMGVMCID | 6,
                                           CORBA::COMPLETED_NO));
        }

      this->servant_activator_ = PortableServer::ServantActivator::_narrow (imgr
                                                                            ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      // If the argument is nil, or does not support the required interface,
      // then the OBJ_ADAPTER system exception with standard minor code 4 is
      // raised.
      if (CORBA::is_nil (this->servant_activator_.in ()))
        {
          ACE_THROW (CORBA::OBJ_ADAPTER (CORBA::OMGVMCID | 4,
                                         CORBA::COMPLETED_NO));
        }
    }

    TAO_SERVANT_LOCATION
    Servant_Activator_Request_Processing_Strategy::locate_servant (
      const PortableServer::ObjectId &system_id,
      PortableServer::Servant &servant
      ACE_ENV_ARG_DECL)
    {
      ACE_UNUSED_ARG (system_id);
      ACE_UNUSED_ARG (servant);

      if (CORBA::is_nil (this->servant_activator_.in ()))
        {
          return TAO_SERVANT_NOT_FOUND;
        }
      else
        {
          // Success
          return TAO_SERVANT_MANAGER;
        }
    }

    PortableServer::Servant
    Servant_Activator_Request_Processing_Strategy::locate_servant (const char *operation,
                        const PortableServer::ObjectId &system_id,
                        TAO::Portable_Server::Servant_Upcall &servant_upcall,
                        TAO::Portable_Server::POA_Current_Impl &poa_current_impl,
                        int &wait_occurred_restart_call
                        ACE_ENV_ARG_DECL)
    {
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
      if (CORBA::is_nil (this->servant_activator_.in ()))
        {
          ACE_THROW_RETURN (CORBA::OBJ_ADAPTER (),
                            0);
        }

      PortableServer::Servant servant = 0;

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
        TAO::Portable_Server::Non_Servant_Upcall non_servant_upcall (*this->poa_);
        ACE_UNUSED_ARG (non_servant_upcall);

        // @@
        // Invocations of incarnate on the servant manager are serialized.
        // Invocations of etherealize on the servant manager are serialized.
        // Invocations of incarnate and etherealize on the servant manager are mutually exclusive.
        servant = this->servant_activator_->incarnate (poa_current_impl.object_id (),
                                                       this->poa_
                                                       ACE_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN (0);

        if (servant == 0)
          {
            ACE_THROW_RETURN (CORBA::OBJ_ADAPTER (),
                              0);
          }
      }

      int error = 0;

      // If the incarnate operation returns a servant that is
      // already active for a different Object Id and if the POA
      // also has the UNIQUE_ID policy, the incarnate has violated
      // the POA policy and is considered to be in error. The POA
      // will raise an OBJ_ADAPTER system exception for the
      // request.
      if (this->poa_->cached_policies().id_uniqueness () == PortableServer::UNIQUE_ID)
        {
          int result =
            this->poa_->is_servant_in_map (servant,
                                     wait_occurred_restart_call);
          if (result)
            error = 1;
        }

      // The POA enters the returned Servant value into the Active
      // Object Map so that subsequent requests with the same
      // ObjectId value will be delivered directly to that servant
      // without invoking the servant manager.  Only run if there
      // are no errors or if a restart is not required.
      if (!error && !wait_occurred_restart_call)
        {
          TAO_Active_Object_Map::Map_Entry *entry = 0;
          int result = this->poa_->active_object_map ().
            rebind_using_user_id_and_system_id (servant,
                                                poa_current_impl.object_id (),
                                                system_id,
                                                entry);
          servant_upcall.active_object_map_entry(entry);
          if (result != 0)
            error = 1;
        }

      // If error occurred or a restart is required, etherealize
      // the incarnated servant.
      if (error || wait_occurred_restart_call)
        {
          CORBA::Boolean remaining_activations =
            this->poa_->active_object_map ().remaining_activations (servant);

          // A recursive thread lock without using a recursive
          // thread lock.  Non_Servant_Upcall has a magic
          // constructor and destructor.  We unlock the
          // Object_Adapter lock for the duration of the servant
          // activator upcalls; reacquiring once the upcalls
          // complete.  Even though we are releasing the lock,
          // other threads will not be able to make progress since
          // <Object_Adapter::non_servant_upcall_in_progress_> has
          // been set.
          TAO::Portable_Server::Non_Servant_Upcall non_servant_upcall (*this->poa_);
          ACE_UNUSED_ARG (non_servant_upcall);

          CORBA::Boolean cleanup_in_progress = 0;
          this->servant_activator_->etherealize (poa_current_impl.object_id (),
                                                 this->poa_,
                                                 servant,
                                                 cleanup_in_progress,
                                                 remaining_activations
                                                 ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);

          // If error, throw exception.
          if (error)
            {
              ACE_THROW_RETURN (CORBA::OBJ_ADAPTER (),
                                0);
            }
          else
            {
              // We ended up waiting on a condition variable, the
              // POA state may have changed while we are waiting.
              // Therefore, we need to restart this call.
              return 0;
            }
        }
      else
        {
          // Increment the reference count.
          ++servant_upcall.active_object_map_entry ()->reference_count_;

          // Success
          return servant;
        }

      // todo
    }

    Servant_Locator_Request_Processing_Strategy::~Servant_Locator_Request_Processing_Strategy (void)
    {
    }

    void
    Servant_Locator_Request_Processing_Strategy::set_user_id (
      const PortableServer::ObjectId &system_id,
      TAO::Portable_Server::Servant_Upcall &servant_upcall,
      TAO::Portable_Server::POA_Current_Impl &poa_current_impl
      ACE_ENV_ARG_DECL)
    {
      // Just replace the object id in the POA Current with the system id
      poa_current_impl.replace_object_id (system_id);

      servant_upcall.user_id (&system_id);
    }

    PortableServer::ServantManager_ptr
    Servant_Locator_Request_Processing_Strategy::get_servant_manager (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableServer::POA::WrongPolicy))
    {
      return PortableServer::ServantManager::_duplicate (this->servant_locator_.in ());
    }

    void
    Servant_Locator_Request_Processing_Strategy::set_servant_manager (PortableServer::ServantManager_ptr imgr
                         ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableServer::POA::WrongPolicy))
    {
      // This operation sets the default servant manager associated with the
      // POA. This operation may only be invoked once after a POA has been
      // created. Attempting to set the servant manager after one has already
      // been set will result in the BAD_INV_ORDER system exception with
      // standard minor code 6 being raised (see 11.3.9.12 of the corba spec)
      if (!CORBA::is_nil (this->servant_locator_.in ()))
        {
          ACE_THROW (CORBA::BAD_INV_ORDER (CORBA::OMGVMCID | 6,
                                           CORBA::COMPLETED_NO));
        }

      this->servant_locator_ = PortableServer::ServantLocator::_narrow (imgr
                                                                        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      // If the argument is nil, or does not support the required interface,
      // then the OBJ_ADAPTER system exception with standard minor code 4 is
      // raised.
      if (CORBA::is_nil (this->servant_locator_.in ()))
        {
          ACE_THROW (CORBA::OBJ_ADAPTER (CORBA::OMGVMCID | 4,
                                         CORBA::COMPLETED_NO));
        }
    }

    PortableServer::Servant
    Servant_Locator_Request_Processing_Strategy::reference_to_servant (
      CORBA::Object_ptr reference
      ACE_ENV_ARG_DECL)
    {
      ACE_THROW_RETURN (PortableServer::POA::WrongPolicy (),
                        0);
    }

    TAO_SERVANT_LOCATION
    Servant_Locator_Request_Processing_Strategy::locate_servant (
      const PortableServer::ObjectId &system_id,
      PortableServer::Servant &servant
      ACE_ENV_ARG_DECL)
    {
      ACE_UNUSED_ARG (system_id);
      ACE_UNUSED_ARG (servant);

      if (CORBA::is_nil (this->servant_locator_.in ()))
        {
          return TAO_SERVANT_NOT_FOUND;
        }
      else
        {
          // Success
          return TAO_SERVANT_MANAGER;
        }
    }

    PortableServer::Servant
    Servant_Locator_Request_Processing_Strategy::locate_servant (const char *operation,
                        const PortableServer::ObjectId &system_id,
                        TAO::Portable_Server::Servant_Upcall &servant_upcall,
                        TAO::Portable_Server::POA_Current_Impl &poa_current_impl,
                        int &wait_occurred_restart_call
                        ACE_ENV_ARG_DECL)
    {
      ACE_UNUSED_ARG (wait_occurred_restart_call);
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

      if (CORBA::is_nil (this->servant_locator_.in ()))
        {
          ACE_THROW_RETURN (CORBA::OBJ_ADAPTER (),
                            0);
        }

      // No serialization of invocations of preinvoke or
      // postinvoke may be assumed; there may be multiple
      // concurrent invocations of preinvoke for the same
      // ObjectId.
      //
      // The same thread will be used to preinvoke the object,
      // process the request, and postinvoke the object.

      // @@ Note that it is possible for some other thread to
      // reset the servant locator once the lock is released.
      // However, this possiblility also exists for postinvoke()
      // which is also called outside the lock.

      // Release the object adapter lock.
      this->poa_->object_adapter().lock ().release ();

      // We have release the object adapter lock.  Record this
      // for later use.
      servant_upcall.state (TAO::Portable_Server::Servant_Upcall::OBJECT_ADAPTER_LOCK_RELEASED);

      PortableServer::ServantLocator::Cookie cookie = 0;
      PortableServer::Servant servant =
        this->servant_locator_->preinvoke (poa_current_impl.object_id (),
                                           this->poa_,
                                           operation,
                                           cookie
                                           ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      if (servant == 0)
        {
          ACE_THROW_RETURN (CORBA::OBJ_ADAPTER (),
                            0);
        }

      // Remember to invoke <postinvoke> on the given locator
      servant_upcall.servant_locator (servant_locator_.in());

      // Remember the cookie
      servant_upcall.locator_cookie (cookie);

      // Remember operation name.
      servant_upcall.operation (operation);

      // Success
      return servant;

    }

  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

