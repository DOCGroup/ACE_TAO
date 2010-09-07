// $Id$
#include "tao/orbconf.h"

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

#include "tao/ORB_Constants.h"
#include "tao/PortableServer/ServantActivatorC.h"
#include "tao/PortableServer/RequestProcessingStrategyServantActivator.h"
#include "tao/PortableServer/Servant_Base.h"
#include "tao/PortableServer/Non_Servant_Upcall.h"
#include "tao/PortableServer/Root_POA.h"
#include "tao/PortableServer/POA_Current_Impl.h"
#include "tao/PortableServer/Servant_Upcall.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    RequestProcessingStrategyServantActivator::RequestProcessingStrategyServantActivator (void) :
      etherealize_objects_ (true)
    {
    }

    void
    RequestProcessingStrategyServantActivator::strategy_cleanup (void)
    {
      {
        Non_Servant_Upcall non_servant_upcall (*this->poa_);
        ACE_UNUSED_ARG (non_servant_upcall);

        this->servant_activator_ = PortableServer::ServantActivator::_nil ();
      }

      RequestProcessingStrategy::strategy_cleanup ();
    }

    PortableServer::ServantManager_ptr
    RequestProcessingStrategyServantActivator::get_servant_manager (void)
    {
      return PortableServer::ServantManager::_duplicate (this->servant_activator_.in ());
    }

    void
    RequestProcessingStrategyServantActivator::set_servant_manager (
      PortableServer::ServantManager_ptr imgr)
    {
      // This operation sets the default servant manager associated with the
      // POA. This operation may only be invoked once after a POA has been
      // created. Attempting to set the servant manager after one has already
      // been set will result in the BAD_INV_ORDER system exception with
      // standard minor code 6 being raised (see 11.3.9.12 of the corba spec)
      if (!CORBA::is_nil (this->servant_activator_.in ()))
        {
          throw ::CORBA::BAD_INV_ORDER (CORBA::OMGVMCID | 6, CORBA::COMPLETED_NO);
        }

      this->servant_activator_ =
        PortableServer::ServantActivator::_narrow (imgr);

      this->validate_servant_manager (this->servant_activator_.in ());
    }

    TAO_SERVANT_LOCATION
    RequestProcessingStrategyServantActivator::locate_servant (
      const PortableServer::ObjectId &system_id,
      PortableServer::Servant &servant)
    {
      TAO_SERVANT_LOCATION location = TAO_SERVANT_NOT_FOUND;

      location = this->poa_->servant_present (system_id, servant);

      if (location == TAO_SERVANT_NOT_FOUND)
        {
          if (!CORBA::is_nil (this->servant_activator_.in ()))
            {
              location = TAO_SERVANT_MANAGER;
            }
        }

      return location;
    }

    PortableServer::Servant
    RequestProcessingStrategyServantActivator::locate_servant (
      const char * /*operation*/,
      const PortableServer::ObjectId &system_id,
      TAO::Portable_Server::Servant_Upcall &servant_upcall,
      TAO::Portable_Server::POA_Current_Impl &poa_current_impl,
      bool &wait_occurred_restart_call
      )
    {
      PortableServer::Servant servant = 0;

      servant = this->poa_->find_servant (system_id,
                                          servant_upcall,
                                          poa_current_impl);

      if (servant != 0)
        {
          return servant;
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

      this->validate_servant_manager (this->servant_activator_.in ());

      servant = this->incarnate_servant (poa_current_impl.object_id ());

      // If the incarnate operation returns a servant that is
      // already active for a different Object Id and if the POA
      // also has the UNIQUE_ID policy, the incarnate has violated
      // the POA policy and is considered to be in error. The POA
      // will raise an OBJ_ADAPTER system exception for the
      // request.
      bool may_activate =
        this->poa_->is_servant_activation_allowed (servant,
                                                   wait_occurred_restart_call);

      if (!may_activate)
        {
          // If we are not allowed to activate the servant, throw an exception
          // etherealize is not called because the servant is never added to
          // the active object map
          throw ::CORBA::OBJ_ADAPTER ();
        }

      // The POA enters the returned Servant value into the Active
      // Object Map so that subsequent requests with the same
      // ObjectId value will be delivered directly to that servant
      // without invoking the servant manager.  Only run if there
      // are no errors or if a restart is not required.
      if (!wait_occurred_restart_call)
        {
          int const result =
            this->poa_->
              rebind_using_user_id_and_system_id (servant,
                                                  poa_current_impl.object_id (),
                                                  system_id,
                                                  servant_upcall);
          if (result != 0)
            {
              // Throw an exception, etherealize is not called because servant
              // is not added to the active object map
              throw ::CORBA::OBJ_ADAPTER ();
            }

          // Increment the reference count on the servant upcall.
          servant_upcall.increment_servant_refcount ();
        }
      else
        {
          CORBA::Boolean cleanup_in_progress = 0;
          this->etherealize_servant (poa_current_impl.object_id (),
                                     servant,
                                     cleanup_in_progress);

          // We ended up waiting on a condition variable, the
          // POA state may have changed while we are waiting.
          // Therefore, we need to restart this call.
          return 0;
        }

      // Success
      return servant;
    }

    void
    RequestProcessingStrategyServantActivator::etherealize_servant (
      const PortableServer::ObjectId& object_id,
      PortableServer::Servant servant,
      CORBA::Boolean cleanup_in_progress
      )
    {
      CORBA::Boolean remaining_activations =
        this->poa_->servant_has_remaining_activations (servant);

      // ATTENTION: Trick locking here, see class header for details
      Non_Servant_Upcall non_servant_upcall (*this->poa_);
      ACE_UNUSED_ARG (non_servant_upcall);

      // @todo This is not according to the spec. According to 11.3.6.2 at the
      // end when etherealize returns a system exception the POA ignores the
      // exception
      this->servant_activator_->etherealize (object_id,
                                             this->poa_,
                                             servant,
                                             cleanup_in_progress,
                                             remaining_activations);
    }

    PortableServer::Servant
    RequestProcessingStrategyServantActivator::incarnate_servant (
      const PortableServer::ObjectId& object_id)
    {
      PortableServer::Servant servant = 0;

      // ATTENTION: Trick locking here, see class header for details
      Non_Servant_Upcall non_servant_upcall (*this->poa_);
      ACE_UNUSED_ARG (non_servant_upcall);

      // @@
      // Invocations of incarnate on the servant manager are serialized.
      // Invocations of etherealize on the servant manager are serialized.
      // Invocations of incarnate and etherealize on the servant manager are mutually exclusive.
      servant = this->servant_activator_->incarnate (object_id, this->poa_);

      if (servant == 0)
        {
          throw ::CORBA::OBJ_ADAPTER (CORBA::OMGVMCID | 7, CORBA::COMPLETED_NO);
        }
      else
        {
          return servant;
        }
    }

    void
    RequestProcessingStrategyServantActivator::cleanup_servant (
      PortableServer::Servant servant,
      const PortableServer::ObjectId &user_id
      )
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

      // @bala, is this order correct, see 11.3.9.17 of the spec, it
      // says first remove from the map, then etherealize. not the
      // other way around
      // @@ Johnny, you are right! This will not show up until a
      // thread is trying to activate the object in another thread
      // using activate_object_with_id (). A test case is a must for
      // atleast this issue.
      if (servant)
        {
          if (this->etherealize_objects_ &&
              !CORBA::is_nil (this->servant_activator_.in ()))
            {
              this->etherealize_servant (user_id,
                                         servant,
                                         this->poa_->cleanup_in_progress ());
            }
          else
            {
              // ATTENTION: Trick locking here, see class header for details
              Non_Servant_Upcall non_servant_upcall (*this->poa_);
              ACE_UNUSED_ARG (non_servant_upcall);

              servant->_remove_ref ();
            }
        }

      // This operation causes the association of the Object Id specified
      // by the oid parameter and its servant to be removed from the
      // Active Object Map.
      if (this->poa_->unbind_using_user_id (user_id) != 0)
        throw ::CORBA::OBJ_ADAPTER ();
    }

    void
    RequestProcessingStrategyServantActivator::etherealize_objects (
      CORBA::Boolean etherealize_objects)
    {
      this->etherealize_objects_ = etherealize_objects;
    }

    void
    RequestProcessingStrategyServantActivator::post_invoke_servant_cleanup(
      const PortableServer::ObjectId &/*system_id*/,
      const TAO::Portable_Server::Servant_Upcall &/*servant_upcall*/)
    {
    }
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_MINIMUM_POA == 0 */
